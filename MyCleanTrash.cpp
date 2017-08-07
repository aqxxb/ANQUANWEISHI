// MyCleanTrash.cpp : 实现文件
//

#include "stdafx.h"
#include "安全卫士项目.h"
#include "MyCleanTrash.h"
#include "afxdialogex.h"


// CMyCleanTrash 对话框

IMPLEMENT_DYNAMIC(CMyCleanTrash, CDialogEx)

CMyCleanTrash::CMyCleanTrash(CWnd* pParent /*=NULL*/)
: CDialogEx(CMyCleanTrash::IDD, pParent)
, m_OpenFile(_T(""))
, m_Suffix(_T(".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log"))
{

}

CMyCleanTrash::~CMyCleanTrash()
{
}

void CMyCleanTrash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_OpenFile);
	DDX_Text(pDX, IDC_EDIT2, m_Suffix);
	DDX_Control(pDX, IDC_LIST1, m_PathList);
	DDX_Control(pDX, IDC_LIST2, m_FileList);
}


BEGIN_MESSAGE_MAP(CMyCleanTrash, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyCleanTrash::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyCleanTrash::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyCleanTrash::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyCleanTrash::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMyCleanTrash::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyCleanTrash::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyCleanTrash 消息处理程序



//打开文件
void CMyCleanTrash::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szPath1[MAX_PATH] = {};            //定义一个TCHAR类型数组szPath1
	BROWSEINFO bi = {};

	//以带有颜色区分的格式查看复制到剪贴板

	typedef  struct  _browseinfo {           //包含本程序功能参数和接收用户选择的文件夹的信息。
		HWND                  hwndOwner;         //对话框的所有者窗口的句柄
		PCIDLIST_ABSOLUTE     pidlRoot;			 //初始根目录
		LPTSTR                pszDisplayName;	 //用来保存用户选中的目录字符串的内存地址
		LPCTSTR               lpszTitle;		 //该浏览文件夹对话框的显示文本
		UINT                  ulFlags;			 //指定对话框选项的标志
		BFFCALLBACK           lpfn;				 //该标志位描述了对话框的选项。
		LPARAM                lParam;			 //对话框传递给回调函数的一个参数指针
		int                   iImage;			 //与选中目录相关的图像。该图像将被指定为系统图像列表中的索引值
	} BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO;

	// 文件夹选择对话框所属窗口句柄
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath1;
	// 窗口说明信息
	bi.lpszTitle = L"打开要遍历的文件夹";
	// 对话框带编辑框
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// 返回标识文件夹路径的“PIDL”，用它找到文件夹路径
	pid = SHBrowseForFolder(&bi);       //显示一个对话框，该对话框允许用户选择shell文件夹。
	SHGetPathFromIDList(pid, szPath1);  //
	// 把路径更新到路径显示编辑框内
	m_OpenFile = szPath1;
	// 把路径更新到路径显示m_PathList内
	if (LB_ERR == m_PathList.FindString(0, m_OpenFile))
	{// 如果m_PathList里面没有这个新的文件夹路径，再添加进m_PathList
		m_PathList.AddString(m_OpenFile);
	}
	// 把变量内容更新到对话框
	UpdateData(FALSE);
	// 用完释放掉之前的“PID”
	CoTaskMemFree(pid);
}

//后缀名更新
void CMyCleanTrash::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	//将在编辑框中更改的文件后缀更新到变量中
	UpdateData(TRUE);
}

//遍历路径列表的文件到文件列表显示
void CMyCleanTrash::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	// 遍历之前先清空之前遍历的文件信息和列表框内容
	m_vecFileDelete.clear();
	m_FileList.ResetContent();
	// 获取要遍历的文件夹数目
	int nCount = m_PathList.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString test;
		// 获取ListBox控件内的文件夹路径
		m_PathList.GetText(i, test);
		// 遍历文件夹内的所有文件和子文件夹
		TraverseFile(test.GetBuffer(), m_Suffix, m_vecFileDelete);
	}
	// 将遍历到的要删除文件添加到ListBox内显示出来
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		m_FileList.AddString(m_vecFileDelete[i]);
		m_FileList.SelectString(i, m_vecFileDelete[i]);

	}
}

//删除文件列表中的文件
void CMyCleanTrash::OnBnClickedButton5()
{
	// TODO:  在此添加控件通知处理程序代码
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		DeleteFile(m_vecFileDelete[i].GetBuffer());
		m_FileList.DeleteString(0);
	}
	m_vecFileDelete.swap(vector<CString>());
}

//遍历文件路径，将遍历到的文件在文件列表中显示
void CMyCleanTrash::TraverseFile(
	TCHAR  szPath[MAX_PATH], // 要遍历的文件路径
	const CString &strFilter,// 过滤的文件后缀
	vector<CString> &vecPath)// 保存遍历到的文件
{
	// 向文件夹末尾添加\\*，遍历所有文件
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// 找到的文件信息放到fi里面
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// 判断是否当前目录或上级目录
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// 判断是否是文件夹，若是，递归遍历
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
		// 获取文件扩展名，用于判断是否垃圾文件后缀
		TCHAR *p = PathFindExtension(fi.cFileName);
		// Find返回查找的子串p在m_Delete中的索引，若找不到返回-1
		if (-1 == strFilter.Find(p))
		{
			continue;
		}
		// 经过重重筛选，终于找到要删除的文件名了，和之前的路径构造成完整路径
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// 保存进路径向量里
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}

//浏览器垃圾
void CMyCleanTrash::OnBnClickedButton6()
{
	// TODO:  在此添加控件通知处理程序代码
	m_OpenFile = L"C:\\Users\\北辰\\AppData\\Local\\Mozilla\\Firefox\\Profiles\\safebrowsing";
	if (LB_ERR == m_PathList.FindString(0, m_OpenFile))
	{// 如果m_PathList里面没有这个新的文件夹路径，再添加进m_PathList
		m_PathList.AddString(m_OpenFile);
	}
	UpdateData(FALSE);
}
//系统垃圾
void CMyCleanTrash::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	HRESULT rec = SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI);
	Sleep(1000);
	m_OpenFile = L"C:\\Users\\北辰\\AppData\\Local\\Temp";
	if (LB_ERR == m_PathList.FindString(0, m_OpenFile))
	{// 如果m_PathList里面没有这个新的文件夹路径，再添加进m_PathList
		m_PathList.AddString(m_OpenFile);
	}
	UpdateData(FALSE);
}
