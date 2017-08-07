// MyTool.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "MyTool.h"
#include "afxdialogex.h"
#include <locale.h>
#include "MyRocket.h"

// CMyTool �Ի���

IMPLEMENT_DYNAMIC(CMyTool, CDialogEx)

CMyTool::CMyTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyTool::IDD, pParent)
	, m_ToolOpenFileEdit(_T("C:\\Users\\����\\Desktop\\��ͼС��.exe"))
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


// CMyTool ��Ϣ�������

DWORD   RvaToOffset(const void* pFileData, DWORD dwRva)
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	IMAGE_NT_HEADERS *pNtHeader =   /*����*/
		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	// �õ����θ���
	DWORD   dwSectionNumber = pNtHeader->FileHeader.NumberOfSections;

	// �õ���һ������
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);

	// �������α��ҵ�RVA���ڵ�����
	/*
	* ÿ��ƫ�ƣ����������ļ��У��������ڴ��У����Ǿ������ο�ʼλ�õľ���
	* ������ȵġ�
	* ���ң����α��У�������������ʼƫ�ƣ�
	*  1. �ļ��еĿ�ʼƫ��
	*  2. �ڴ��еĿ�ʼƫ��
	* ������̣�
	*  �ҵ�RVA�������Σ� Ȼ���������RVA���������ڴ��еĿ�ʼλ�õľ��롣
	*  �������������������ļ��еĿ�ʼλ�þ͵õ��ļ�ƫ����
	*/

	for (int i = 0; i < dwSectionNumber; ++i) {

		// �ж�RVA�Ƿ��ڵ�ǰ��������

		DWORD dwSectionEndRva =   /*����*/
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if (dwRva >= pSectionHeader[i].VirtualAddress
			&& dwRva <= dwSectionEndRva) {

			// �����RVA��Ӧ���ļ�ƫ��
			// ��ʽ��
			// �ļ�ƫ��  =  RVA - ���ε���ʼRVA + ���ε���ʼ�ļ�ƫ��
			DWORD dwTemp = dwRva - pSectionHeader[i].VirtualAddress;
			DWORD dwOffset = dwTemp + pSectionHeader[i].PointerToRawData;

			return dwOffset;
		}
	}

	return -1;
}

typedef struct TypeOffset
{
	WORD Offset : 12;  // (1) ��СΪ12Bit���ض�λƫ��
	WORD Type : 4;    // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
}TypeOffset;
//���ļ�
void CMyTool::OnBnClickedButton41()
{
	UpdateData(TRUE);
	m_ToolQdMessage.DeleteAllItems();
	m_ToolMlMessage.DeleteAllItems();
	MessageOftable();
}
//�������Ϣ��Ӧ
void CMyTool::OnBnClickedButton42()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(L"�ļ�������,���߱�ռ��!");
		return ;
	}
	// ��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// ���뻺���������ļ�����
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// ���ļ���ȡ��������
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// ʹ��DOSͷ�ṹָ�򻺳���
	IMAGE_DOS_HEADER* pDosHeader =  /*����*/
		(IMAGE_DOS_HEADER*)pFileData;

	// ��ȡNtͷ
	IMAGE_NT_HEADERS* pNtHeader = /*����*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// ��ȡ��չͷ
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*����*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// ��ȡ������Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;


	//1. ��ȡ�����
	// 1.1 ��ȡ������RVA
	DWORD   dwImportTableRva = pDataDirectory[1].VirtualAddress;
	// 1.2 ��������RVAת�����ļ�ƫ��
	DWORD   dwImportTableOffset = RvaToOffset(pFileData, dwImportTableRva);
	// 1.3 �������ṹ��ָ��ָ�򻺳����еĵ����
	IMAGE_IMPORT_DESCRIPTOR* pImportTable =
		(IMAGE_IMPORT_DESCRIPTOR*)((DWORD)pFileData + dwImportTableOffset);

	//2. ����������
	//  ��Ϊһ��exe���ܻᵼ����DLL����ÿһ��Dll��Ӧ��һ�������
	//  ����������γ�һ��������
	//  �������������ȫ0��β(ȫ0��βָ���������ṹ�嶼��0)

	//2.1�ж��Ƿ�����������һ���ṹ�塣
	while (pImportTable->Name != 0) {

		//3. �����������Dll��ģ����
		//  3.1 ��Dllģ������RVAת�����ļ�ƫ��
		DWORD dwNameOffset = RvaToOffset(pFileData, pImportTable->Name);
		//  3.2 ��һ��char*ָ��ָ��dll�����ڴ��е�λ��
		char* pDllName = (char*)(dwNameOffset + (DWORD)pFileData);
		//////////////////���뺯�����ڵ�DLL///////////////////////////

		m_vecTableMessage.push_back(L"DLL����");
		CStringA STR2 = pDllName;
		m_vecTableMessage.push_back((CString)STR2);
		m_vecTableMessage.push_back(L"��������");
		//////////////////////////////////////////////////////////////
		
		// 4. ������ǰdll�ĵ��뺯������
		// 4.1 �õ��������Ʊ�ĵ�ַ
		//pImportTable->OriginalFirstThunk; // �������Ʊ�
		//pImportTable->FirstThunk; // �����ַ��
		// ����˵�����������ļ��б������������ȫ��ͬ�ġ�
		DWORD dwIATOffset = RvaToOffset(pFileData, pImportTable->FirstThunk);
		IMAGE_THUNK_DATA *pIAT = (IMAGE_THUNK_DATA *)((DWORD)pFileData + dwIATOffset);

		// 4.2 ����IAT
		//  IAT��һ��IMAGE_THUNK_DATA�ṹ�����飬
		//  ������һ��ȫ0��β��
		
		while (pIAT->u1.AddressOfData != 0) {

			// 5. �жϵ���ĺ���������ʲô��ʽ�����
			//    ���������ŵ���ģ�������ṹ�����κ�һ���ֶ���߾���1��
			//    ����������Ʒ�ʽ����ģ�������ṹ���е��κ�һ���ֶα���
			//    ����һ��RVA �����RVA��һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ�塣
			//    IMAGE_IMPORT_BY_NAME�У�������ǵ��뺯������ź����ơ�

			// 5.1 �ж����λ�Ƿ���1

			if ((pIAT->u1.Ordinal & 0x80000000) == 1) {
				// ����������ŵ���ġ�
				// ��Ŷ���WORD���ͣ����ԣ�ֻȡ����16��ֵ
				//m_ToolTableMessage.AddString(L"������ţ�");
				m_vecTableMessage.push_back(L"������ţ�");
				CString STR;
				STR.Format(_T("%d"), (pIAT->u1.Ordinal & 0xFFFF));
				m_vecTableMessage.push_back(STR);
			}
			else {

				// �����������Ʒ�ʽ�����
				// �ֶα�����һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ���RVA 
				DWORD dwFunctionNameOffset = /*����*/
					RvaToOffset(pFileData, pIAT->u1.AddressOfData);

				IMAGE_IMPORT_BY_NAME* pImportName = /*����*/
					(IMAGE_IMPORT_BY_NAME*)((DWORD)pFileData + dwFunctionNameOffset);

				//m_ToolTableMessage.AddString(L"�������ƣ�");
				CStringA STR1 = pImportName->Name;

				m_vecTableMessage.push_back((CString)STR1);
			}

			// ��������һ���ṹ��
			++pIAT;
		}
		// ������һ���ṹ��
		++pImportTable;
	}
	//������Ϣ���
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
	}
}
//��������Ϣ��Ӧ
void CMyTool::OnBnClickedButton43()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���ListBox�ؼ��е�����
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();
	/////////////������/////////////////
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
		MessageBox(L"�ļ�������,���߱�ռ��!");
		return;
	}
	// ��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// ���뻺���������ļ�����
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// ���ļ���ȡ��������
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// ʹ��DOSͷ�ṹָ�򻺳���
	IMAGE_DOS_HEADER* pDosHeader =  /*����*/
		(IMAGE_DOS_HEADER*)pFileData;

	// ��ȡNtͷ
	IMAGE_NT_HEADERS* pNtHeader = /*����*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// ��ȡ��չͷ
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*����*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// ��ȡ������Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// ��ȡ������
	DWORD dwExportTableRva = pDataDirectory[0].VirtualAddress;
	// ���������RVAת�����ļ�ƫ��
	DWORD dwExportTableOffset = RvaToOffset(pFileData, dwExportTableRva);

	// ��ȡ������
	IMAGE_EXPORT_DIRECTORY* pExportTable = /*����*/
		(IMAGE_EXPORT_DIRECTORY*)(dwExportTableOffset + (DWORD)pFileData);

	// �õ�����DLL��ģ����
	// Name�ֶα������һ���ַ�����RVA����Ҫת�����ļ�ƫ��
	DWORD dwNameOffset = RvaToOffset(pFileData, pExportTable->Name);
	char* pDllName = (char*)(dwNameOffset + (DWORD)pFileData);
	CStringA str = pDllName;
	m_vecTableMessage.push_back((CString)str);
	m_vecTableMessage.push_back(L"��������");

	// �����еĵ����ĺ�����ַ��ӡ������
	// ���ң�����������Ƶ����������������
	// ���������ŵ��������������š�

	// ����ַ���RVAת���ļ�ƫ��
	DWORD dwAddressTableOffset = RvaToOffset(pFileData,
		pExportTable->AddressOfFunctions);
	// �õ�������ַ��
	DWORD* pAddressTable = /*����*/
		(DWORD*)(dwAddressTableOffset + (DWORD)pFileData);


	// �õ���ű�ĵ�ַ
	WORD* pOrdinalTable =
		(WORD*)((DWORD)pFileData + RvaToOffset(pFileData, pExportTable->AddressOfNameOrdinals));

	DWORD* pNameTable =
		(DWORD*)((DWORD)pFileData + RvaToOffset(pFileData, pExportTable->AddressOfNames));

	BOOL bIndexIsExist = FALSE; // 

	for (int i = 0; i < pExportTable->NumberOfFunctions; ++i) {

		
		// ��ӡ��ַ
		/*m_vecTableMessage.push_back(L"����ţ�");
		CString STR2,str3;
		STR2.Format(L"%d", i);
		m_vecTableMessage.push_back(STR2);
		m_vecTableMessage.push_back(L"RVA��");
		str3.Format(L"%d", pAddressTable[i]);
		m_vecTableMessage.push_back(str3);*/


		// �жϵ�ǰ�������ַ�Ƿ��������Ʒ�ʽ������
		// �ж����ݣ�
		//   ��ű�����ǵ�ַ���һ���±꣬����±��¼��
		//   ��ַ�����ĸ���ַ�������Ʒ�ʽ�����ġ�
		//   �����ǰ������±걣������ű��У���˵�������ַ
		//   ��һ�����Ʒ�ʽ�������������±�����ű��в����ڣ�
		//   ��˵���������ַ����һ�����Ʒ�ʽ����
		bIndexIsExist = FALSE;
		int nNameIndex = 0;
		for (; nNameIndex < pExportTable->NumberOfNames; ++nNameIndex) {

			// �жϵ�ַ����±��Ƿ��������ű���
			if (i == pOrdinalTable[nNameIndex]) {
				bIndexIsExist = TRUE;
				break;
			}
		}

		if (bIndexIsExist == TRUE) {
			// �õ����Ʊ��е�RVA
			DWORD dwNameRva = pNameTable[nNameIndex];

			// ������Rvaת�����ļ�ƫ��
			char* pFunName =
				(char*)((DWORD)pFileData + RvaToOffset(pFileData, dwNameRva));

			CStringA STR3 = pFunName;
			
			m_vecTableMessage.push_back((CString)STR3);
		}
		else {

			// �жϵ�ַ��ǰ��������Ԭ���Ƿ񱣴��ŵ�ַ
			if (pAddressTable[i] != 0) {
				// i �� �ǵ�ַ���е������ţ�Ҳ����һ�������
				// ��������� = ����� + ��Ż���
				
				/*m_vecTableMessage.push_back(L"��ţ�");
				CString STR4;
				STR4.Format(L"%d", i + pExportTable->Base);
				m_vecTableMessage.push_back(STR4);*/
			}
		}
	}
	//������Ϣ���
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
	}
}
//��Դ����Ϣ��Ӧ
void CMyTool::OnBnClickedButton44()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();
	///////////////��Դ��///////////////////////
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
		MessageBox(L"�ļ�������,���߱�ռ��!");
		return;
	}
	// ��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// ���뻺���������ļ�����
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// ���ļ���ȡ��������
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// ʹ��DOSͷ�ṹָ�򻺳���
	IMAGE_DOS_HEADER* pDosHeader =  /*����*/
		(IMAGE_DOS_HEADER*)pFileData;

	// ��ȡNtͷ
	IMAGE_NT_HEADERS* pNtHeader = /*����*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// ��ȡ��չͷ
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*����*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// ��ȡ������Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// ��ȡ��Դ��
	DWORD resourcesTableOffset =
		RvaToOffset(pFileData, pDataDirectory[2].VirtualAddress);

	IMAGE_RESOURCE_DIRECTORY* pRoot =
		(IMAGE_RESOURCE_DIRECTORY*)(((DWORD)pFileData) + resourcesTableOffset);


	IMAGE_RESOURCE_DIRECTORY*    pDir2;// ��ԴĿ¼
	IMAGE_RESOURCE_DIRECTORY*    pDir3;// ��ԴĿ¼

	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry1;//Ŀ¼���
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry2;//Ŀ¼���
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry3;//Ŀ¼���

	IMAGE_RESOURCE_DATA_ENTRY*      pDataEntry;// ��Դ�������
	IMAGE_RESOURCE_DIR_STRING_U*    pIdString; // ����Id���ַ���
	// +----------------+
	// |    Ŀ¼        |
	// +----------------+
	//    +-------------+      |- id(���ַ����͵�ID,�����͵�ID)
	//    |   Ŀ¼���   | ==> |
	//    +-------------+      |- ƫ��(����ƫ�Ƶ�Ŀ¼,����ƫ�Ƶ�����)
	//    +-------------+ 
	//    |   Ŀ¼���   | 
	//    +-------------+ 
	//    +-------------+ 
	//    |   Ŀ¼���   | 
	//    +-------------+ 

	/* �ѵ�һ�����е�Ŀ¼��ڶ��������� */
	// �õ���һ��Ŀ¼��ڵĵ�ַ
	pEntry1 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRoot + 1);
	for (int i = 0;
		i < pRoot->NumberOfIdEntries + pRoot->NumberOfNamedEntries;
		i++, pEntry1++) {

		//��ȡ��һ��Ŀ¼��ڵ�ID��ID������Դ�����ͣ�
		if (pEntry1->NameIsString == 1) {
			// NameOffset���ֶε�ֵ��һ��ƫ��
			// ���ƫ��������Դ���Ŀ¼�ĵ�ַ��Ϊ��ַ��
			pIdString =
				(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry1->NameOffset + (DWORD)pRoot);

			m_vecTableMessage.push_back(L"��Դ����");
			m_vecTableMessage.push_back( pIdString->NameString);
		}
		else {
			char* pType[] =
			{
				"",          // 0
				"���ָ��",   // 1
				"λͼ",   // 2
				"ͼ��",       // 3
				"�˵�",       // 4
				"�Ի���",      //5
				"�ַ����б�",  //6
				"����Ŀ¼",  //7
				"����",      //8
				"��ݼ�",  //9
				"�Ǹ�ʽ����Դ",  //A
				"��Ϣ�б�",  //B
				"���ָ����",  //C
				"",             // D
				"ͼ����",  //E
				"",         // F
				"�汾��Ϣ"//10
			};
			m_vecTableMessage.push_back(L"��Դ����");
			CStringA str = pType[pEntry1->Id];
			m_vecTableMessage.push_back((CString)str);
		}

		// ��ȡ�ڶ���Ŀ¼
		if (pEntry1->DataIsDirectory == 1) {

			pDir2 = /*�õ��ڶ�����ԴĿ¼*/
				(IMAGE_RESOURCE_DIRECTORY*)(pEntry1->OffsetToDirectory + (DWORD)pRoot);
			// +----------------+
			// | ��һ���Ŀ¼���| >---++
			// +----------------+     ||
			//                        \/
			//        +----------------+
			//        |    �ڶ���Ŀ¼   |
			//        +----------------+
			//           +-------------+
			//           |   Ŀ¼���   |
			//           +-------------+
			//           +-------------+
			//           |   Ŀ¼���   |
			//           +-------------+
			//           +-------------+
			//           |   Ŀ¼���   |
			//           +-------------+
			// �����ڶ�����ԴĿ¼������Ŀ¼���
			pEntry2 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir2 + 1);
			for (int j = 0;
				j < pDir2->NumberOfIdEntries + pDir2->NumberOfNamedEntries;
				++j, ++pEntry2) {

				// �õ���Դ��ID
				if (pEntry2->NameIsString == 1) {
					// NameOffset���ֶε�ֵ��һ��ƫ��
					// ���ƫ��������Դ���Ŀ¼�ĵ�ַ��Ϊ��ַ��
					pIdString =
						(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry2->NameOffset + (DWORD)pRoot);


					m_vecTableMessage.push_back(L"��ԴID:");
					CStringA str1 = (char*)pIdString->NameString;
					m_vecTableMessage.push_back((CString)str1);
				}
				else {
					m_vecTableMessage.push_back(L"��ԴID:");
					CString STR4;
					STR4.Format(L"%d", pEntry2->Id);
					m_vecTableMessage.push_back(STR4);
				}

				// �жϵ�������Ŀ¼��������
				if (pEntry2->DataIsDirectory == 1) {
					// +----------------+
					// | �ڶ����Ŀ¼���| >---++
					// +----------------+     ||
					//                        \/
					//        +----------------+
					//        |    ������Ŀ¼   |
					//        +----------------+
					//           +-------------+
					//           |   Ŀ¼���   |  ==> ָ�� IMAGE_RESOUCES_DATA_ENTRY
					//           +-------------+
					// �õ�������Ŀ¼�ĵ�ַ
					pDir3 =
						(IMAGE_RESOURCE_DIRECTORY*)(pEntry2->OffsetToDirectory + (DWORD)pRoot);

					// �õ�������ĵ�һ��Ŀ¼���
					pEntry3 =
						(IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir3 + 1);
					m_vecTableMessage.push_back(L"ID:");
					CString STR5;
					STR5.Format(L"%d", pEntry3->Id);
					m_vecTableMessage.push_back(STR5);

					// �õ��������
					pDataEntry =
						(IMAGE_RESOURCE_DATA_ENTRY*)(pEntry3->OffsetToData + (DWORD)pRoot);
					CString STR6, STR7;
					STR6.Format(L"%d", pDataEntry->OffsetToData);
					STR7.Format(L"%d", pDataEntry->Size);
					m_vecTableMessage.push_back(L"��Դ��ƫ�ƣ�");
					m_vecTableMessage.push_back(STR6);
					m_vecTableMessage.push_back(L"��Դ�Ĵ�С��");
					m_vecTableMessage.push_back(STR7);

				}
			}
		}

	}
	//������Ϣ���
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
	}
}
//�ض�λ����Ϣ��Ӧ
void CMyTool::OnBnClickedButton45()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox(L"�ļ�������,���߱�ռ��!");
		return;
	}

	// ��ȡ�ļ���С
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	// ���뻺���������ļ�����
	BYTE* pFileData = new BYTE[dwFileSize];
	DWORD dwRead = 0;
	// ���ļ���ȡ��������
	ReadFile(hFile, pFileData, dwFileSize, &dwRead, NULL);

	// ʹ��DOSͷ�ṹָ�򻺳���
	IMAGE_DOS_HEADER* pDosHeader =  /*����*/
		(IMAGE_DOS_HEADER*)pFileData;

	// ��ȡNtͷ
	IMAGE_NT_HEADERS* pNtHeader = /*����*/
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pFileData);

	// ��ȡ��չͷ
	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*����*/
		(IMAGE_OPTIONAL_HEADER*)&pNtHeader->OptionalHeader;

	// ��ȡ������Ŀ¼��
	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// ��ȡ�ض�λ��
	/*
	+-----------+
	|  �ض�λ��  |
	+-----------+
	+-----------+
	|  �ض�λ��  |
	+-----------+
	+-----------+
	|  �ض�λ��  |
	+-----------+
	+-----------+
	|00000000000|
	+-----------+
	*/
	// �õ���һ���ض�λ��������׵�ַ(RVA)
	DWORD dwRelcationBlockOffset =
		RvaToOffset(pFileData, pDataDirectory[5].VirtualAddress);

	// �õ���һ���ض�λ��ĵ�ַ
	IMAGE_BASE_RELOCATION* pRelcationBlock =
		(IMAGE_BASE_RELOCATION*)((DWORD)pFileData + dwRelcationBlockOffset);

	while (1)
	{
		// �ж��Ƿ������һ���ض�λ��
		if (pRelcationBlock->VirtualAddress == 0 && pRelcationBlock->SizeOfBlock == 0) {
			break;
		}
		m_vecTableMessage.push_back(L"�鿪ʼ��RVA:");
		CString num1, num2;
		num1.Format(L"%d", pRelcationBlock->VirtualAddress);
		m_vecTableMessage.push_back(num1);
		m_vecTableMessage.push_back(L"�ֽ���:");
		num2.Format(L"%d", pRelcationBlock->SizeOfBlock);
		m_vecTableMessage.push_back(num2);

		// �����ض�λ���ͺ�ƫ�Ƶ����ݿ�
		TypeOffset* pTypeOffset =
			(TypeOffset*)((DWORD)pRelcationBlock + sizeof(IMAGE_BASE_RELOCATION));

		DWORD   dwCount =
			(pRelcationBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);

		for (int i = 0; i < dwCount; ++i) {

			/*
			Type��ֵ��Ӧ���º�:
			IMAGE_REL_BASED_ABSOLUTE (0) ʹ�鰴��32λ���룬λ��Ϊ0��
			IMAGE_REL_BASED_HIGH (1) ��16λ����Ӧ����ƫ������ָ����16λ��
			IMAGE_REL_BASED_LOW (2) ��16λ����Ӧ����ƫ������ָ����16λ��
			IMAGE_REL_BASED_HIGHLOW (3) ȫ��32λӦ��������32λ��
			IMAGE_REL_BASED_HIGHADJ (4) ��Ҫ32λ����16λλ��ƫ��������16λλ����һ��ƫ��������Ԫ�أ����Ϊһ����������������32λ��һ������Ȼ�����8000Ȼ��Ѹ�16λ������ƫ������16λ���ڡ�
			IMAGE_REL_BASED_MIPS_JMPADDR (5) Unknown
			IMAGE_REL_BASED_SECTION (6) Unknown
			IMAGE_REL_BASED_REL32 (7) Unknown
			*/
			if (pTypeOffset[i].Type == 3)
			m_vecTableMessage.push_back(L"+ ƫ�� : ");
			CString num3;
			num3.Format(L"%d", pTypeOffset[i].Offset);
			m_vecTableMessage.push_back(num3);

			// �õ���Ҫ�ض�λ������
			DWORD dwRva = pRelcationBlock->VirtualAddress + pTypeOffset[i].Offset;
			DWORD dwData = RvaToOffset(pFileData, dwRva);
			DWORD* pRelData = (DWORD*)(dwData + pFileData);
			CString num4;
			m_vecTableMessage.push_back(L" + Ҫ�޸ĵĵ�ַ��");
			num4.Format(_T("%d"), (*pRelData));
			m_vecTableMessage.push_back(num4);
		}
		// ��������һ���ض�λ��
		pRelcationBlock =
			(IMAGE_BASE_RELOCATION*)((DWORD)(pRelcationBlock)+pRelcationBlock->SizeOfBlock);
	}
	//������Ϣ���
	for (DWORD i = 0; i < m_vecTableMessage.size(); i++)
	{
		//m_ToolTableMessage.SelectString(i, m_vecTableMessage[i]);
		m_ToolTableMessage.AddString(m_vecTableMessage[i]);
	}

}

//�ӳټ��ر���Ϣ��Ӧ
void CMyTool::OnBnClickedButton46()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();


}

//TLS����Ϣ��Ӧ
void CMyTool::OnBnClickedButton47()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_vecTableMessage.clear();
	m_ToolTableMessage.ResetContent();


}

//С�����ť
void CMyTool::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


	m_Rock = new CMyRocket();
	m_Rock->Create(IDD_DIALOG7);

	//m_Rock.Create(IDD_DIALOG7);    //����һ����ģ̬�Ի���??
	//m_Rock.ShowWindow(SW_SHOWNORMAL);
}


BOOL CMyTool::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	////////////////////////���α�/////////////////////
	//��ȡ��ǰ�ؼ�����չ���///////////////////��ʼ��List�ؼ�
	DWORD dwOldStyle = m_ToolMlMessage.GetExtendedStyle();          //��ȡԭ���
	//���õ�ǰ�ؼ�����չ���
	m_ToolMlMessage.SetExtendedStyle(dwOldStyle |
		LVS_EX_FULLROWSELECT |          //������ͼ��ʽ
		LVS_EX_GRIDLINES          //������,������ֻ������report����listctrl
		);
	//��ȡlist�Ŀ��
	CRect rc;
	m_ToolMlMessage.GetClientRect(rc);
	int nWidth = rc.Width();
	//ΪList��������Ϣ
	m_ToolMlMessage.InsertColumn(0,	           //��ǰ�б��               
		L"   ",       //Ҫ��������Ϣ
		0,	           //���
		nWidth / 4);   //�еĿ��
	m_ToolMlMessage.InsertColumn(1, L"RVA", 0, nWidth / 3);
	m_ToolMlMessage.InsertColumn(2, L"��С", 0, nWidth / 3);
	////////////////////////Ŀ¼��//////////////////////////////////
	//��ȡ��ǰ�ؼ�����չ���///////////////////��ʼ��List�ؼ�
	DWORD dwOldStyle1 = m_ToolQdMessage.GetExtendedStyle();          //��ȡԭ���
	//���õ�ǰ�ؼ�����չ���
	m_ToolQdMessage.SetExtendedStyle(dwOldStyle1 |
		LVS_EX_FULLROWSELECT |          //������ͼ��ʽ
		LVS_EX_GRIDLINES          //������,������ֻ������report����listctrl
		);
	//��ȡlist�Ŀ��
	CRect rc1;
	m_ToolQdMessage.GetClientRect(rc1);
	int nWidth1 = rc1.Width();
	//ΪList��������Ϣ
	m_ToolQdMessage.InsertColumn(0,	           //��ǰ�б��               
		L"����",       //Ҫ��������Ϣ
		0,	           //���
		nWidth1 / 6);   //�еĿ��
	m_ToolQdMessage.InsertColumn(1, L"VOffset", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(2, L"VSize", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(3, L"ROffset", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(4, L"RSize", 0, nWidth1 / 6);
	m_ToolQdMessage.InsertColumn(5, L"��־", 0, nWidth1 / 6);
	MessageOftable();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

VOID CMyTool::MessageOftable()
{
	// 1. ���ļ�,���ļ���ȡ���ڴ�.
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
		MessageBox(L"�ļ������ڣ�");
		return;
	}

	DWORD dwFileSize = 0;
	dwFileSize = GetFileSize(hFile, NULL);

	// 2. �����ڴ�ռ�
	BYTE* pBuf = new BYTE[dwFileSize];

	// 3. ���ļ����ݶ�ȡ���ڴ���
	DWORD dwRead = 0;
	ReadFile(hFile,
		pBuf,
		dwFileSize,
		&dwRead,
		NULL);

	// ������������DOSͷ�ṹ��������
	IMAGE_DOS_HEADER* pDosHdr;// DOSͷ
	pDosHdr = (IMAGE_DOS_HEADER*)pBuf;
	IMAGE_NT_HEADERS* pNtHdr;

	// ntͷ,�������ļ�ͷ����չͷ
	pNtHdr = (IMAGE_NT_HEADERS*)
		(pDosHdr->e_lfanew + (DWORD)pBuf);

	// �ж��Ƿ���һ����Ч��pe�ļ�
	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE
		|| pNtHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		MessageBox(L"������Ч��pe�ļ�!");
		return ;
	}

	IMAGE_FILE_HEADER* pFileHdr; // �ļ�ͷ
	IMAGE_OPTIONAL_HEADER* pOptHdr;// ��չͷ
	pFileHdr = &pNtHdr->FileHeader;
	pOptHdr = &pNtHdr->OptionalHeader;
	//��������
	WORD num = pFileHdr->NumberOfSections;
	m_ToolQdEdit.Format(L"%x", num);
	//ʱ��
	DWORD num1 = pFileHdr->TimeDateStamp;
	m_ToolshijianEdit.Format(L"%x", num1);
	//��ַ
	DWORD num2 = pOptHdr->ImageBase;
	m_ToolJxEdit.Format(L"%x", num2);
	//��ڵ�
	DWORD num3 = pOptHdr->AddressOfEntryPoint;
	m_ToolRuKouEdit.Format(L"%x", num3);
	//ͷ����С
	DWORD num4 = pOptHdr->SizeOfHeaders;
	m_ToolSbEdit.Format(L"%x", num4);
	//����ֵ
	WORD num5 = pFileHdr->Characteristics;
	m_ToolTzEdit.Format(L"%x", num5);

	//////////////////////////////Ŀ¼��Ϣ//////////////////////////////////
	
	m_ToolMlMessage.InsertItem(0, L"  ������");
	m_ToolMlMessage.InsertItem(1, L"  �����");
	m_ToolMlMessage.InsertItem(2, L"  ��Դ��");
	m_ToolMlMessage.InsertItem(3, L"  ����");
	m_ToolMlMessage.InsertItem(4, L"  ��ȫ");
	m_ToolMlMessage.InsertItem(5, L"  �ض�λ");
	m_ToolMlMessage.InsertItem(6, L"  ����");
	m_ToolMlMessage.InsertItem(7, L"  ��Ȩ");
	m_ToolMlMessage.InsertItem(8, L"  ȫ��ָ��");
	m_ToolMlMessage.InsertItem(9, L"  TLS��");
	m_ToolMlMessage.InsertItem(10, L"  ��������");
	m_ToolMlMessage.InsertItem(11, L"  ���뷶Χ");
	m_ToolMlMessage.InsertItem(12, L"  IAT");
	m_ToolMlMessage.InsertItem(13, L"  �ӳٵ���");
	m_ToolMlMessage.InsertItem(14, L"  COM");
	m_ToolMlMessage.InsertItem(15, L"  ����");
	// ���ñ����ַ���
	setlocale(LC_ALL, "chs");
	CString str, str1;
	for (int i = 0; i < 16; ++i)
	{
		str.Format(_T("%x"), pOptHdr->DataDirectory[i].VirtualAddress);
		m_ToolMlMessage.SetItemText(i, 1, str);
		str1.Format(_T("%x"), pOptHdr->DataDirectory[i].Size);
		m_ToolMlMessage.SetItemText(i, 2, str1);
	}
	///////////////////////���α���Ϣ/////////////////////////
	// �ҵ����α�Ĳ���
	// 1. �ҵ�Ntͷ
	// 2. �ҵ���չͷ
	// 3. �õ���չͷ�Ĵ�С
	// 4. ��չͷ���׵�ַ+��չͷ�Ĵ�С => ��һ������ͷ
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

	hProcess = ::GetCurrentProcess(); //��ȡ��ǰӦ�ó�����̾��

	if (!::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken)) //�򿪵�ǰ���̵ķ������ƾ��(OpenProcessToken��������ʧ�ܷ���ֵΪ��)
		return FALSE;

	if (!::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &uID)) //��ȡȨ������Ϊ"SeShutdownPrivilege"��LUID(LookupPrivilegeValue��������ʧ�ܷ���ֵΪ��)
		return FALSE;

	stToken_Privileges.PrivilegeCount = 1; //��������Ȩ�޸���
	stToken_Privileges.Privileges[0].Luid = uID; //Ȩ�޵�LUID
	stToken_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; //Ȩ�޵�����,SE_PRIVILEGE_ENABLEDΪʹ�ܸ�Ȩ��

	if (!::AdjustTokenPrivileges(hToken, FALSE, &stToken_Privileges, sizeof stToken_Privileges, NULL, NULL)) //���������������ָ��Ȩ��(AdjustTokenPrivileges��������ʧ�ܷ���ֵΪ��)
		return FALSE;

	if (::GetLastError() != ERROR_SUCCESS) //�鿴Ȩ���Ƿ�����ɹ�
		return FALSE;

	::CloseHandle(hToken);
	return TRUE;

}
//�ػ�����
void CMyTool::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetToken();
    ::ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0); //ǿ�ƹػ�
	
}

//��������
void CMyTool::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetToken();
	:: ExitWindowsEx(EWX_REBOOT | EWX_FORCE,0);

}

//ע������
void CMyTool::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	::ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
}

//ж�����
void CMyTool::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
