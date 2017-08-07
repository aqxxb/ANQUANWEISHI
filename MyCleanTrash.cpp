// MyCleanTrash.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "��ȫ��ʿ��Ŀ.h"
#include "MyCleanTrash.h"
#include "afxdialogex.h"


// CMyCleanTrash �Ի���

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


// CMyCleanTrash ��Ϣ�������



//���ļ�
void CMyCleanTrash::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath1[MAX_PATH] = {};            //����һ��TCHAR��������szPath1
	BROWSEINFO bi = {};

	//�Դ�����ɫ���ֵĸ�ʽ�鿴���Ƶ�������

	typedef  struct  _browseinfo {           //�����������ܲ����ͽ����û�ѡ����ļ��е���Ϣ��
		HWND                  hwndOwner;         //�Ի���������ߴ��ڵľ��
		PCIDLIST_ABSOLUTE     pidlRoot;			 //��ʼ��Ŀ¼
		LPTSTR                pszDisplayName;	 //���������û�ѡ�е�Ŀ¼�ַ������ڴ��ַ
		LPCTSTR               lpszTitle;		 //������ļ��жԻ������ʾ�ı�
		UINT                  ulFlags;			 //ָ���Ի���ѡ��ı�־
		BFFCALLBACK           lpfn;				 //�ñ�־λ�����˶Ի����ѡ�
		LPARAM                lParam;			 //�Ի��򴫵ݸ��ص�������һ������ָ��
		int                   iImage;			 //��ѡ��Ŀ¼��ص�ͼ�񡣸�ͼ�񽫱�ָ��Ϊϵͳͼ���б��е�����ֵ
	} BROWSEINFO, *PBROWSEINFO, *LPBROWSEINFO;

	// �ļ���ѡ��Ի����������ھ��
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath1;
	// ����˵����Ϣ
	bi.lpszTitle = L"��Ҫ�������ļ���";
	// �Ի�����༭��
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// ���ر�ʶ�ļ���·���ġ�PIDL���������ҵ��ļ���·��
	pid = SHBrowseForFolder(&bi);       //��ʾһ���Ի��򣬸öԻ��������û�ѡ��shell�ļ��С�
	SHGetPathFromIDList(pid, szPath1);  //
	// ��·�����µ�·����ʾ�༭����
	m_OpenFile = szPath1;
	// ��·�����µ�·����ʾm_PathList��
	if (LB_ERR == m_PathList.FindString(0, m_OpenFile))
	{// ���m_PathList����û������µ��ļ���·��������ӽ�m_PathList
		m_PathList.AddString(m_OpenFile);
	}
	// �ѱ������ݸ��µ��Ի���
	UpdateData(FALSE);
	// �����ͷŵ�֮ǰ�ġ�PID��
	CoTaskMemFree(pid);
}

//��׺������
void CMyCleanTrash::OnBnClickedButton3()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//���ڱ༭���и��ĵ��ļ���׺���µ�������
	UpdateData(TRUE);
}

//����·���б���ļ����ļ��б���ʾ
void CMyCleanTrash::OnBnClickedButton4()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ����֮ǰ�����֮ǰ�������ļ���Ϣ���б������
	m_vecFileDelete.clear();
	m_FileList.ResetContent();
	// ��ȡҪ�������ļ�����Ŀ
	int nCount = m_PathList.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString test;
		// ��ȡListBox�ؼ��ڵ��ļ���·��
		m_PathList.GetText(i, test);
		// �����ļ����ڵ������ļ������ļ���
		TraverseFile(test.GetBuffer(), m_Suffix, m_vecFileDelete);
	}
	// ����������Ҫɾ���ļ���ӵ�ListBox����ʾ����
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		m_FileList.AddString(m_vecFileDelete[i]);
		m_FileList.SelectString(i, m_vecFileDelete[i]);

	}
}

//ɾ���ļ��б��е��ļ�
void CMyCleanTrash::OnBnClickedButton5()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		DeleteFile(m_vecFileDelete[i].GetBuffer());
		m_FileList.DeleteString(0);
	}
	m_vecFileDelete.swap(vector<CString>());
}

//�����ļ�·���������������ļ����ļ��б�����ʾ
void CMyCleanTrash::TraverseFile(
	TCHAR  szPath[MAX_PATH], // Ҫ�������ļ�·��
	const CString &strFilter,// ���˵��ļ���׺
	vector<CString> &vecPath)// ������������ļ�
{
	// ���ļ���ĩβ���\\*�����������ļ�
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// �ҵ����ļ���Ϣ�ŵ�fi����
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// �ж��Ƿ�ǰĿ¼���ϼ�Ŀ¼
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// �ж��Ƿ����ļ��У����ǣ��ݹ����
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
		// ��ȡ�ļ���չ���������ж��Ƿ������ļ���׺
		TCHAR *p = PathFindExtension(fi.cFileName);
		// Find���ز��ҵ��Ӵ�p��m_Delete�е����������Ҳ�������-1
		if (-1 == strFilter.Find(p))
		{
			continue;
		}
		// ��������ɸѡ�������ҵ�Ҫɾ�����ļ����ˣ���֮ǰ��·�����������·��
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// �����·��������
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}

//���������
void CMyCleanTrash::OnBnClickedButton6()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_OpenFile = L"C:\\Users\\����\\AppData\\Local\\Mozilla\\Firefox\\Profiles\\safebrowsing";
	if (LB_ERR == m_PathList.FindString(0, m_OpenFile))
	{// ���m_PathList����û������µ��ļ���·��������ӽ�m_PathList
		m_PathList.AddString(m_OpenFile);
	}
	UpdateData(FALSE);
}
//ϵͳ����
void CMyCleanTrash::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	HRESULT rec = SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI);
	Sleep(1000);
	m_OpenFile = L"C:\\Users\\����\\AppData\\Local\\Temp";
	if (LB_ERR == m_PathList.FindString(0, m_OpenFile))
	{// ���m_PathList����û������µ��ļ���·��������ӽ�m_PathList
		m_PathList.AddString(m_OpenFile);
	}
	UpdateData(FALSE);
}
