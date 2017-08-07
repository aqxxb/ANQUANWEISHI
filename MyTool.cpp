// MyTool.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyTool.h"
#include "afxdialogex.h"
#include <locale.h>
#include "MyRocket.h"

// CMyTool 对话框

IMPLEMENT_DYNAMIC(CMyTool, CDialogEx)

CMyTool::CMyTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTool::IDD, pParent)
	, m_ToolOpenFileEdit(_T("C:\\Users\\北辰\\Desktop\\截图小兵.exe"))
	, m_ToolRuKouEdit(_T("00000000"))
	, m_ToolshijianEdit(_T("00000000"))
	, m_ToolJxEdit(_T("00000000"))
	, m_ToolQdEdit(_T("0000"))
	, m_ToolTzEdit(_T("0000"))
	, m_ToolSbEdit(_T("00000000"))
{

}

CMyTool::~CMyTool()
{
}

void CMyTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT42, m_ToolOpenFileEdit);
	DDX_Text(pDX, IDC_EDIT2, m_ToolRuKouEdit);
	DDX_Text(pDX, IDC_EDIT5, m_ToolshijianEdit);
	DDX_Text(pDX, IDC_EDIT3, m_ToolJxEdit);
	DDX_Text(pDX, IDC_EDIT6, m_ToolQdEdit);
	DDX_Text(pDX, IDC_EDIT4, m_ToolTzEdit);
	DDX_Text(pDX, IDC_EDIT7, m_ToolSbEdit);
	DDX_Control(pDX, IDC_LIST1, m_ToolQdMessage);
	DDX_Control(pDX, IDC_LIST3, m_ToolMlMessage);
	DDX_Control(pDX, IDC_LIST1, m_ToolQdMessage);
	DDX_Control(pDX, IDC_LIST3, m_ToolMlMessage);
	DDX_Control(pDX, IDC_LIST2, m_ToolTableMessage);
}


BEGIN_MESSAGE_MAP(CMyTool, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON42, &CMyTool::OnBnClickedButton42)
	ON_BN_CLICKED(IDC_BUTTON41, &CMyTool::OnBnClickedButton41)
	ON_BN_CLICKED(IDC_BUTTON43, &CMyTool::OnBnClickedButton43)
	ON_BN_CLICKED(IDC_BUTTON44, &CMyTool::OnBnClickedButton44)
	ON_BN_CLICKED(IDC_BUTTON45, &CMyTool::OnBnClickedButton45)
	ON_BN_CLICKED(IDC_BUTTON46, &CMyTool::OnBnClickedButton46)
	ON_BN_CLICKED(IDC_BUTTON47, &CMyTool::OnBnClickedButton47)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyTool::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyTool::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyTool::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyTool::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyTool::OnBnClickedButton5)
END_MESSAGE_MAP()


// CMyTool 消息处理程序

DWORD   RvaToOffset(const void* pFileData, DWORD dwRva)
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	IMAGE_NT_HEADERS *pNtHeader =   /*换行*/
		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	// 得到区段个数
	DWORD   dwSectionNumber = pNtHeader->FileHeader.NumberOfSections;

	// 得到第一个区段
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);

	// 遍历区段表，找到RVA所在的区段
	/*
	* 每个偏移，不管是在文件中，还是在内存中，它们距离区段开始位置的距离
	* 总是相等的。
	* 而且，区段表中，保存着两个开始偏移：
	*  1. 文件中的开始偏移
	*  2. 内存中的开始偏移
	* 具体过程：
	*  找到RVA所在区段， 然后计算出这个RVA到区段在内存中的开始位置的距离。
	*  用这个距离加上区段在文件中的开始位置就得到文件偏移了
	*/

	for (int i = 0; i < dwSectionNumber; ++i) {

		// 判断RVA是否在当前的区段中

		DWORD dwSectionEndRva =   /*换行*/
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if (dwRva >= pSectionHeader[i].VirtualAddress
			&& dwRva <= dwSectionEndRva) {

			// 计算出RVA对应的文件偏移
			// 公式：
			// 文件偏移  =  RVA - 区段的起始RVA + 区段的起始文件偏移
			DWORD dwTemp = dwRva - pSectionHeader[i].VirtualAddress;
			DWORD dwOffset = dwTemp + pSectionHeader[i].PointerToRawData;

			return dwOffset;
		}
	}

	return -1;
}

typedef struct TypeOffset
{
	WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
	WORD Type : 4;    // (2) 大小为4Bit的重定位信息类型值
}TypeOffset;
//打开文件
void CMyTool::OnBnClickedButton41()
{
	UpdateData(TRUE);
	m_ToolQdMessage.DeleteAllItems();
	m_ToolMlMessage.DeleteAllItems();
	MessageOftable();
}
//导入表信息响应
void CMyTool::OnBnClickedButton42()
{
	// TODO:  在此添加控件通知处理程序代码
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(m_ToolOpenFileEdit,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(L"文件不存在,或者被占用!");
		return ;
	}
	// 获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 申请缓冲区保存文件内容
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// 将文件读取到缓冲区
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// 使用DOS头结构指向缓冲区
	IMAGE_DOS_HEADER* pDosHeader =  /*换行*/
		(IMAGE_DOS_HEADER*)pFileData;

	// 获取Nt头
	IMAGE_NT_HEADERS* pNtHeader = /*换行*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// 获取扩展头
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*换行*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// 获取到数据目录表
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;


	//1. 获取导入表
	// 1.1 获取导入表的RVA
	DWORD   dwImportTableRva = pDataDirectory[1].VirtualAddress;
	// 1.2 将导入表的RVA转换成文件偏移
	DWORD   dwImportTableOffset = RvaToOffset(pFileData, dwImportTableRva);
	// 1.3 将导入表结构体指针指向缓冲区中的导入表
	IMAGE_IMPORT_DESCRIPTOR* pImportTable =
		(IMAGE_IMPORT_DESCRIPTOR*)((DWORD)pFileData + dwImportTableOffset);

	//2. 遍历导入表块
	//  因为一个exe可能会导入多个DLL，而每一个Dll对应着一个导入表
	//  多个导入表就形成一个导入表块
	//  这个导入表块是以全0结尾(全0结尾指的是整个结构体都是0)

	//2.1判断是否遍历到了最后一个结构体。
	while (pImportTable->Name != 0) {

		//3. 解析出导入的Dll的模块名
		//  3.1 将Dll模块名的RVA转换成文件偏移
		DWORD dwNameOffset = RvaToOffset(pFileData, pImportTable->Name);
		//  3.2 用一个char*指针指向dll名在内存中的位置
		char* pDllName = (char*)(dwNameOffset + (DWORD)pFileData);
		//////////////////插入函数所在的DLL///////////////////////////

		m_vecTableMessage.push_back(L"DLL名：");
		CStringA STR2 = pDllName;
		m_vecTableMessage.push_back((CString)STR2);
		m_vecTableMessage.push_back(L"函数名：");
		//////////////////////////////////////////////////////////////
		
		// 4. 解析当前dll的导入函数名称
		// 4.1 得到导入名称表的地址
		//pImportTable->OriginalFirstThunk; // 导入名称表
		//pImportTable->FirstThunk; // 导入地址表
		// 上面说的两个表，在文件中保存的内容是完全相同的。
		DWORD dwIATOffset = RvaToOffset(pFileData, pImportTable->FirstThunk);
		IMAGE_THUNK_DATA *pIAT = (IMAGE_THUNK_DATA *)((DWORD)pFileData + dwIATOffset);

		// 4.2 遍历IAT
		//  IAT是一个IMAGE_THUNK_DATA结构体数组，
		//  数组是一个全0结尾。
		
		while (pIAT->u1.AddressOfData != 0) {

			// 5. 判断导入的函数，是以什么方式导入的
			//    如果是以序号导入的，则这个结构体中任何一个字段最高就是1。
			//    如果是以名称方式导入的，则这个结构体中的任何一个字段保存
			//    的是一个RVA ，这个RVA是一个指向IMAGE_IMPORT_BY_NAME结构体。
			//    IMAGE_IMPORT_BY_NAME中，保存的是导入函数的序号和名称。

			// 5.1 判断最高位是否是1

			if ((pIAT->u1.Ordinal & 0x80000000) == 1) {
				// 函数是以序号导入的。
				// 序号都是WORD类型，所以，只取它低16的值
				//m_ToolTableMessage.AddString(L"导入序号：");
				m_vecTableMessage.push_back(L"导入序号：");
				CString STR;
				STR.Format(_T("%d"), (pIAT->u1.Ordinal & 0xFFFF));
				m_vecTableMessage.push_back(STR);
			}
			else {

				// 函数是以名称方式导入的
				// 字段保存着一个指向IMAGE_IMPORT_BY_NAME结构体的RVA 
				DWORD dwFunctionNameOffset = /*换行*/
					RvaToOffset(pFileData, pIAT->u1.AddressOfData);

				IMAGE_IMPORT_BY_NAME* pImportName = /*换行*/
					(IMAGE_IMPORT_BY_NAME*)((DWORD)pFileData + dwFunctionNameOffset);

				//m_ToolTableMessage.AddString(L"导入名称：");
				CStringA STR1 = pImportName->Name;

				m_vecTableMessage.push_back((CString)STR1);
			}

			// 递增到下一个结构体
			++pIAT;
		}
		// 递增下一个结构体
		++pImportTable;
	}
	//遍历信息输出
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
	}
}
//导出表信息响应
void CMyTool::OnBnClickedButton43()
{
	// TODO:  在此添加控件通知处理程序代码
	//清空ListBox控件中的内容
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();
	/////////////导出表/////////////////
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(m_ToolOpenFileEdit,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(L"文件不存在,或者被占用!");
		return;
	}
	// 获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 申请缓冲区保存文件内容
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// 将文件读取到缓冲区
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// 使用DOS头结构指向缓冲区
	IMAGE_DOS_HEADER* pDosHeader =  /*换行*/
		(IMAGE_DOS_HEADER*)pFileData;

	// 获取Nt头
	IMAGE_NT_HEADERS* pNtHeader = /*换行*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// 获取扩展头
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*换行*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// 获取到数据目录表
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// 获取导出表
	DWORD dwExportTableRva = pDataDirectory[0].VirtualAddress;
	// 将导出表的RVA转换成文件偏移
	DWORD dwExportTableOffset = RvaToOffset(pFileData, dwExportTableRva);

	// 获取导出表
	IMAGE_EXPORT_DIRECTORY* pExportTable = /*换行*/
		(IMAGE_EXPORT_DIRECTORY*)(dwExportTableOffset + (DWORD)pFileData);

	// 得到导出DLL的模块名
	// Name字段保存的是一个字符串的RVA，需要转换成文件偏移
	DWORD dwNameOffset = RvaToOffset(pFileData, pExportTable->Name);
	char* pDllName = (char*)(dwNameOffset + (DWORD)pFileData);
	CStringA str = pDllName;
	m_vecTableMessage.push_back((CString)str);
	m_vecTableMessage.push_back(L"函数名：");

	// 把所有的导出的函数地址打印出来。
	// 并且，如果是以名称导出，则输出该名称
	// 如果是以序号导出，则输出该序号。

	// 将地址表的RVA转换文件偏移
	DWORD dwAddressTableOffset = RvaToOffset(pFileData,
		pExportTable->AddressOfFunctions);
	// 得到导出地址表
	DWORD* pAddressTable = /*换行*/
		(DWORD*)(dwAddressTableOffset + (DWORD)pFileData);


	// 得到序号表的地址
	WORD* pOrdinalTable =
		(WORD*)((DWORD)pFileData + RvaToOffset(pFileData, pExportTable->AddressOfNameOrdinals));

	DWORD* pNameTable =
		(DWORD*)((DWORD)pFileData + RvaToOffset(pFileData, pExportTable->AddressOfNames));

	BOOL bIndexIsExist = FALSE; // 

	for (int i = 0; i < pExportTable->NumberOfFunctions; ++i) {

		
		// 打印地址
		/*m_vecTableMessage.push_back(L"虚序号：");
		CString STR2,str3;
		STR2.Format(L"%d", i);
		m_vecTableMessage.push_back(STR2);
		m_vecTableMessage.push_back(L"RVA：");
		str3.Format(L"%d", pAddressTable[i]);
		m_vecTableMessage.push_back(str3);*/


		// 判断当前的这个地址是否是以名称方式导出的
		// 判断依据：
		//   序号表保存的是地址表的一个下标，这个下标记录着
		//   地址表中哪个地址是以名称方式导出的。
		//   如果当前的这个下标保存在序号表中，则说明这个地址
		//   是一个名称方式导出，如果这个下标在序号表中不存在，
		//   则说明，这个地址不是一个名称方式导出
		bIndexIsExist = FALSE;
		int nNameIndex = 0;
		for (; nNameIndex < pExportTable->NumberOfNames; ++nNameIndex) {

			// 判断地址表的下标是否存在于序号表中
			if (i == pOrdinalTable[nNameIndex]) {
				bIndexIsExist = TRUE;
				break;
			}
		}

		if (bIndexIsExist == TRUE) {
			// 得到名称表中的RVA
			DWORD dwNameRva = pNameTable[nNameIndex];

			// 将名称Rva转换成文件偏移
			char* pFunName =
				(char*)((DWORD)pFileData + RvaToOffset(pFileData, dwNameRva));

			CStringA STR3 = pFunName;
			
			m_vecTableMessage.push_back((CString)STR3);
		}
		else {

			// 判断地址表当前索引到的袁术是否保存着地址
			if (pAddressTable[i] != 0) {
				// i ： 是地址表中的索引号，也就是一个虚序号
				// 真正的序号 = 虚序号 + 序号基数
				
				/*m_vecTableMessage.push_back(L"序号：");
				CString STR4;
				STR4.Format(L"%d", i + pExportTable->Base);
				m_vecTableMessage.push_back(STR4);*/
			}
		}
	}
	//遍历信息输出
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
	}
}
//资源表信息响应
void CMyTool::OnBnClickedButton44()
{
	// TODO:  在此添加控件通知处理程序代码
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();
	///////////////资源表///////////////////////
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(m_ToolOpenFileEdit,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(L"文件不存在,或者被占用!");
		return;
	}
	// 获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 申请缓冲区保存文件内容
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// 将文件读取到缓冲区
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// 使用DOS头结构指向缓冲区
	IMAGE_DOS_HEADER* pDosHeader =  /*换行*/
		(IMAGE_DOS_HEADER*)pFileData;

	// 获取Nt头
	IMAGE_NT_HEADERS* pNtHeader = /*换行*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// 获取扩展头
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*换行*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// 获取到数据目录表
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// 获取资源表
	DWORD resourcesTableOffset =
		RvaToOffset(pFileData, pDataDirectory[2].VirtualAddress);

	IMAGE_RESOURCE_DIRECTORY* pRoot =
		(IMAGE_RESOURCE_DIRECTORY*)(((DWORD)pFileData) + resourcesTableOffset);


	IMAGE_RESOURCE_DIRECTORY*    pDir2;// 资源目录
	IMAGE_RESOURCE_DIRECTORY*    pDir3;// 资源目录

	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry1;//目录入口
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry2;//目录入口
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry3;//目录入口

	IMAGE_RESOURCE_DATA_ENTRY*      pDataEntry;// 资源数据入口
	IMAGE_RESOURCE_DIR_STRING_U*    pIdString; // 保存Id的字符串
	// +----------------+
	// |    目录        |
	// +----------------+
	//    +-------------+      |- id(有字符串型的ID,和整型的ID)
	//    |   目录入口   | ==> |
	//    +-------------+      |- 偏移(可能偏移到目录,可能偏移到数据)
	//    +-------------+ 
	//    |   目录入口   | 
	//    +-------------+ 
	//    +-------------+ 
	//    |   目录入口   | 
	//    +-------------+ 

	/* 把第一层所有的目录入口都遍历出来 */
	// 得到第一个目录入口的地址
	pEntry1 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRoot + 1);
	for (int i = 0;
		i < pRoot->NumberOfIdEntries + pRoot->NumberOfNamedEntries;
		i++, pEntry1++) {

		//获取第一层目录入口的ID（ID就是资源的类型）
		if (pEntry1->NameIsString == 1) {
			// NameOffset该字段的值是一个偏移
			// 这个偏移是以资源表根目录的地址作为基址。
			pIdString =
				(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry1->NameOffset + (DWORD)pRoot);

			m_vecTableMessage.push_back(L"资源类型");
			m_vecTableMessage.push_back( pIdString->NameString);
		}
		else {
			char* pType[] =
			{
				"",          // 0
				"鼠标指针",   // 1
				"位图",   // 2
				"图标",       // 3
				"菜单",       // 4
				"对话框",      //5
				"字符串列表",  //6
				"字体目录",  //7
				"字体",      //8
				"快捷键",  //9
				"非格式化资源",  //A
				"消息列表",  //B
				"鼠标指针组",  //C
				"",             // D
				"图标组",  //E
				"",         // F
				"版本信息"//10
			};
			m_vecTableMessage.push_back(L"资源类型");
			CStringA str = pType[pEntry1->Id];
			m_vecTableMessage.push_back((CString)str);
		}

		// 获取第二层目录
		if (pEntry1->DataIsDirectory == 1) {

			pDir2 = /*得到第二层资源目录*/
				(IMAGE_RESOURCE_DIRECTORY*)(pEntry1->OffsetToDirectory + (DWORD)pRoot);
			// +----------------+
			// | 第一层的目录入口| >---++
			// +----------------+     ||
			//                        \/
			//        +----------------+
			//        |    第二层目录   |
			//        +----------------+
			//           +-------------+
			//           |   目录入口   |
			//           +-------------+
			//           +-------------+
			//           |   目录入口   |
			//           +-------------+
			//           +-------------+
			//           |   目录入口   |
			//           +-------------+
			// 遍历第二层资源目录的所有目录入口
			pEntry2 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir2 + 1);
			for (int j = 0;
				j < pDir2->NumberOfIdEntries + pDir2->NumberOfNamedEntries;
				++j, ++pEntry2) {

				// 得到资源的ID
				if (pEntry2->NameIsString == 1) {
					// NameOffset该字段的值是一个偏移
					// 这个偏移是以资源表根目录的地址作为基址。
					pIdString =
						(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry2->NameOffset + (DWORD)pRoot);


					m_vecTableMessage.push_back(L"资源ID:");
					CStringA str1 = (char*)pIdString->NameString;
					m_vecTableMessage.push_back((CString)str1);
				}
				else {
					m_vecTableMessage.push_back(L"资源ID:");
					CString STR4;
					STR4.Format(L"%d", pEntry2->Id);
					m_vecTableMessage.push_back(STR4);
				}

				// 判断第三层是目录还是数据
				if (pEntry2->DataIsDirectory == 1) {
					// +----------------+
					// | 第二层的目录入口| >---++
					// +----------------+     ||
					//                        \/
					//        +----------------+
					//        |    第三层目录   |
					//        +----------------+
					//           +-------------+
					//           |   目录入口   |  ==> 指向 IMAGE_RESOUCES_DATA_ENTRY
					//           +-------------+
					// 得到第三层目录的地址
					pDir3 =
						(IMAGE_RESOURCE_DIRECTORY*)(pEntry2->OffsetToDirectory + (DWORD)pRoot);

					// 得到第三层的第一个目录入库
					pEntry3 =
						(IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir3 + 1);
					m_vecTableMessage.push_back(L"ID:");
					CString STR5;
					STR5.Format(L"%d", pEntry3->Id);
					m_vecTableMessage.push_back(STR5);

					// 得到数据入口
					pDataEntry =
						(IMAGE_RESOURCE_DATA_ENTRY*)(pEntry3->OffsetToData + (DWORD)pRoot);
					CString STR6, STR7;
					STR6.Format(L"%d", pDataEntry->OffsetToData);
					STR7.Format(L"%d", pDataEntry->Size);
					m_vecTableMessage.push_back(L"资源的偏移：");
					m_vecTableMessage.push_back(STR6);
					m_vecTableMessage.push_back(L"资源的大小：");
					m_vecTableMessage.push_back(STR7);

				}
			}
		}

	}
	//遍历信息输出
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
	}
}
//重定位表信息响应
void CMyTool::OnBnClickedButton45()
{
	// TODO:  在此添加控件通知处理程序代码
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(m_ToolOpenFileEdit,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(L"文件不存在,或者被占用!");
		return;
	}

	// 获取文件大小
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// 申请缓冲区保存文件内容
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// 将文件读取到缓冲区
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// 使用DOS头结构指向缓冲区
	IMAGE_DOS_HEADER* pDosHeader =  /*换行*/
		(IMAGE_DOS_HEADER*)pFileData;

	// 获取Nt头
	IMAGE_NT_HEADERS* pNtHeader = /*换行*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// 获取扩展头
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*换行*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// 获取到数据目录表
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// 获取重定位表
	/*
	+-----------+
	|  重定位块  |
	+-----------+
	+-----------+
	|  重定位块  |
	+-----------+
	+-----------+
	|  重定位块  |
	+-----------+
	+-----------+
	|00000000000|
	+-----------+
	*/
	// 得到第一个重定位块的数组首地址(RVA)
	DWORD dwRelcationBlockOffset =
		RvaToOffset(pFileData, pDataDirectory[5].VirtualAddress);

	// 得到第一个重定位块的地址
	IMAGE_BASE_RELOCATION* pRelcationBlock =
		(IMAGE_BASE_RELOCATION*)((DWORD)pFileData + dwRelcationBlockOffset);

	while (1)
	{
		// 判断是否是最后一个重定位块
		if (pRelcationBlock->VirtualAddress == 0 && pRelcationBlock->SizeOfBlock == 0) {
			break;
		}
		m_vecTableMessage.push_back(L"块开始的RVA:");
		CString num1, num2;
		num1.Format(L"%d", pRelcationBlock->VirtualAddress);
		m_vecTableMessage.push_back(num1);
		m_vecTableMessage.push_back(L"字节数:");
		num2.Format(L"%d", pRelcationBlock->SizeOfBlock);
		m_vecTableMessage.push_back(num2);

		// 遍历重定位类型和偏移的数据块
		TypeOffset* pTypeOffset =
			(TypeOffset*)((DWORD)pRelcationBlock + sizeof(IMAGE_BASE_RELOCATION));

		DWORD   dwCount =
			(pRelcationBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);

		for (int i = 0; i < dwCount; ++i) {

			/*
			Type的值对应以下宏:
			IMAGE_REL_BASED_ABSOLUTE (0) 使块按照32位对齐，位置为0。
			IMAGE_REL_BASED_HIGH (1) 高16位必须应用于偏移量所指高字16位。
			IMAGE_REL_BASED_LOW (2) 低16位必须应用于偏移量所指低字16位。
			IMAGE_REL_BASED_HIGHLOW (3) 全部32位应用于所有32位。
			IMAGE_REL_BASED_HIGHADJ (4) 需要32位，高16位位于偏移量，低16位位于下一个偏移量数组元素，组合为一个带符号数，加上32位的一个数，然后加上8000然后把高16位保存在偏移量的16位域内。
			IMAGE_REL_BASED_MIPS_JMPADDR (5) Unknown
			IMAGE_REL_BASED_SECTION (6) Unknown
			IMAGE_REL_BASED_REL32 (7) Unknown
			*/
			if (pTypeOffset[i].Type == 3)
			m_vecTableMessage.push_back(L"+ 偏移 : ");
			CString num3;
			num3.Format(L"%d", pTypeOffset[i].Offset);
			m_vecTableMessage.push_back(num3);

			// 得到需要重定位的数据
			DWORD dwRva = pRelcationBlock->VirtualAddress + pTypeOffset[i].Offset;
			DWORD dwData = RvaToOffset(pFileData, dwRva);
			DWORD* pRelData = (DWORD*)(dwData + pFileData);
			CString num4;
			m_vecTableMessage.push_back(L" + 要修改的地址：");
			num4.Format(_T("%d"), (*pRelData));
			m_vecTableMessage.push_back(num4);
		}
		// 递增到下一个重定位块
		pRelcationBlock =
			(IMAGE_BASE_RELOCATION*)((DWORD)(pRelcationBlock)+pRelcationBlock->SizeOfBlock);
	}
	//遍历信息输出
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
	}

}

//延迟加载表信息响应
void CMyTool::OnBnClickedButton46()
{
	// TODO:  在此添加控件通知处理程序代码
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();


}

//TLS表信息响应
void CMyTool::OnBnClickedButton47()
{
	// TODO:  在此添加控件通知处理程序代码
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();


}

//小火箭按钮
void CMyTool::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码


	m_Rock = new CMyRocket();
	m_Rock->Create(IDD_DIALOG7);

	//m_Rock.Create(IDD_DIALOG7);    //创建一个非模态对话框??
	//m_Rock.ShowWindow(SW_SHOWNORMAL);
}


BOOL CMyTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	////////////////////////区段表/////////////////////
	//获取当前控件的扩展风格///////////////////初始化List控件
	DWORD dwOldStyle = m_ToolMlMessage.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_ToolMlMessage.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线,网格线只适用与report风格的listctrl
		);
	//获取list的宽度
	CRect rc;
	m_ToolMlMessage.GetClientRect(rc);
	int nWidth = rc.Width();
	//为List插入列消息
	m_ToolMlMessage.InsertColumn(0,	           //当前列编号               
		L"   ",       //要求插入的信息
		0,	           //风格
		nWidth / 4);   //列的宽度
	m_ToolMlMessage.InsertColumn(1, L"RVA", 0, nWidth / 3);
	m_ToolMlMessage.InsertColumn(2, L"大小", 0, nWidth / 3);
	////////////////////////目录表//////////////////////////////////
	//获取当前控件的扩展风格///////////////////初始化List控件
	DWORD dwOldStyle1 = m_ToolQdMessage.GetExtendedStyle();          //获取原风格
	//设置当前控件的扩展风格
	m_ToolQdMessage.SetExtendedStyle(dwOldStyle1 |
		LVS_EX_FULLROWSELECT |          //报表视图形式
		LVS_EX_GRIDLINES          //网格线,网格线只适用与report风格的listctrl
		);
	//获取list的宽度
	CRect rc1;
	m_ToolQdMessage.GetClientRect(rc1);
	int nWidth1 = rc1.Width();
	//为List插入列消息
	m_ToolQdMessage.InsertColumn(0,	           //当前列编号               
		L"名称",       //要求插入的信息
		0,	           //风格
		nWidth1 / 6);   //列的宽度
	m_ToolQdMessage.InsertColumn(1, L"VOffset", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(2, L"VSize", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(3, L"ROffset", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(4, L"RSize", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(5, L"标志", 0, nWidth1 / 6);
	MessageOftable();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

VOID CMyTool::MessageOftable()
{
	// 1. 打开文件,将文件读取到内存.
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(m_ToolOpenFileEdit,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"文件不存在！");
		return;
	}

	DWORD dwFileSize = 0;
	dwFileSize = GetFileSize(hFile, NULL);

	// 2. 申请内存空间
	BYTE* pBuf = new BYTE[dwFileSize];

	// 3. 将文件内容读取到内存中
	DWORD dwRead = 0;
	ReadFile(hFile,
		pBuf,
		dwFileSize,
		&dwRead,
		NULL);

	// 将缓冲区当成DOS头结构体来解析
	IMAGE_DOS_HEADER* pDosHdr;// DOS头
	pDosHdr = (IMAGE_DOS_HEADER*)pBuf;
	IMAGE_NT_HEADERS* pNtHdr;

	// nt头,包含这文件头和扩展头
	pNtHdr = (IMAGE_NT_HEADERS*)
		(pDosHdr->e_lfanew + (DWORD)pBuf);

	// 判断是否是一个有效的pe文件
	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE
		|| pNtHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"不是有效的pe文件!");
		return ;
	}

	IMAGE_FILE_HEADER* pFileHdr; // 文件头
	IMAGE_OPTIONAL_HEADER* pOptHdr;// 扩展头
	pFileHdr = &pNtHdr->FileHeader;
	pOptHdr = &pNtHdr->OptionalHeader;
	//区段数量
	WORD num = pFileHdr->NumberOfSections;
	m_ToolQdEdit.Format(L"%x", num);
	//时间
	DWORD num1 = pFileHdr->TimeDateStamp;
	m_ToolshijianEdit.Format(L"%x", num1);
	//基址
	DWORD num2 = pOptHdr->ImageBase;
	m_ToolJxEdit.Format(L"%x", num2);
	//入口点
	DWORD num3 = pOptHdr->AddressOfEntryPoint;
	m_ToolRuKouEdit.Format(L"%x", num3);
	//头部大小
	DWORD num4 = pOptHdr->SizeOfHeaders;
	m_ToolSbEdit.Format(L"%x", num4);
	//特征值
	WORD num5 = pFileHdr->Characteristics;
	m_ToolTzEdit.Format(L"%x", num5);

	//////////////////////////////目录信息//////////////////////////////////
	
	m_ToolMlMessage.InsertItem(0, L"  导出表");
	m_ToolMlMessage.InsertItem(1, L"  导入表");
	m_ToolMlMessage.InsertItem(2, L"  资源表");
	m_ToolMlMessage.InsertItem(3, L"  例外");
	m_ToolMlMessage.InsertItem(4, L"  安全");
	m_ToolMlMessage.InsertItem(5, L"  重定位");
	m_ToolMlMessage.InsertItem(6, L"  调试");
	m_ToolMlMessage.InsertItem(7, L"  版权");
	m_ToolMlMessage.InsertItem(8, L"  全局指针");
	m_ToolMlMessage.InsertItem(9, L"  TLS表");
	m_ToolMlMessage.InsertItem(10, L"  载入配置");
	m_ToolMlMessage.InsertItem(11, L"  输入范围");
	m_ToolMlMessage.InsertItem(12, L"  IAT");
	m_ToolMlMessage.InsertItem(13, L"  延迟导入");
	m_ToolMlMessage.InsertItem(14, L"  COM");
	m_ToolMlMessage.InsertItem(15, L"  保留");
	// 设置本地字符集
	setlocale(LC_ALL, "chs");
	CString str, str1;
	for (int i = 0; i < 16; ++i)
	{
		str.Format(_T("%x"), pOptHdr->DataDirectory[i].VirtualAddress);
		m_ToolMlMessage.SetItemText(i, 1, str);
		str1.Format(_T("%x"), pOptHdr->DataDirectory[i].Size);
		m_ToolMlMessage.SetItemText(i, 2, str1);
	}
	///////////////////////区段表信息/////////////////////////
	// 找到区段表的步骤
	// 1. 找到Nt头
	// 2. 找到扩展头
	// 3. 得到扩展头的大小
	// 4. 扩展头的首地址+扩展头的大小 => 第一个区段头
	DWORD dwNtHeaderSize = sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER) + pFileHdr->SizeOfOptionalHeader;
	IMAGE_SECTION_HEADER* pScnHdr = NULL;
	pScnHdr = (IMAGE_SECTION_HEADER*)
		((DWORD)pNtHdr + dwNtHeaderSize);

	
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	CString  str3, str4, str5, str6, str7;
	for (int i = 0; i < pFileHdr->NumberOfSections; ++i)
	{
		CStringA str2 = (char*)pScnHdr[i].Name;
		m_ToolQdMessage.InsertItem(i, (CString)str2);
		str3.Format(_T("%x"), pScnHdr[i].VirtualAddress);
		m_ToolQdMessage.SetItemText(i, 1, str3);
		str4.Format(_T("%x"), pScnHdr[i].Misc.VirtualSize);
		m_ToolQdMessage.SetItemText(i, 2, str4);
		str5.Format(_T("%x"), pScnHdr[i].PointerToRawData);
		m_ToolQdMessage.SetItemText(i, 3, str5);
		str6.Format(_T("%x"), pScnHdr[i].SizeOfRawData);
		m_ToolQdMessage.SetItemText(i, 4, str6);
		str7.Format(_T("%x"), pScnHdr[i].Characteristics);
		m_ToolQdMessage.SetItemText(i, 5, str7);
	}
	UpdateData(FALSE);
}

BOOL CMyTool::GetToken()
{
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	LUID uID = { 0 };
	TOKEN_PRIVILEGES stToken_Privileges = { 0 };

	hProcess = ::GetCurrentProcess(); //获取当前应用程序进程句柄

	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken)) //打开当前进程的访问令牌句柄(OpenProcessToken函数调用失败返回值为零)
		return FALSE;

	if (!::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &uID)) //获取权限名称为"SeShutdownPrivilege"的LUID(LookupPrivilegeValue函数调用失败返回值为零)
		return FALSE;

	stToken_Privileges.PrivilegeCount = 1; //欲调整的权限个数
	stToken_Privileges.Privileges[0].Luid = uID; //权限的LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; //权限的属性,SE_PRIVILEGE_ENABLED为使能该权限

	if (!::AdjustTokenPrivileges(hToken, FALSE, &stToken_Privileges, sizeof stToken_Privileges, NULL, NULL)) //调整访问令牌里的指定权限(AdjustTokenPrivileges函数调用失败返回值为零)
		return FALSE;

	if (::GetLastError() != ERROR_SUCCESS) //查看权限是否调整成功
		return FALSE;

	::CloseHandle(hToken);
	return TRUE;

}
//关机功能
void CMyTool::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetToken();
    ::ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0); //强制关机
	
}

//重启功能
void CMyTool::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	GetToken();
	:: ExitWindowsEx(EWX_REBOOT | EWX_FORCE,0);

}

//注销功能
void CMyTool::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	::ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
}

//卸载软件
void CMyTool::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
}
