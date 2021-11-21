
// obs-jasmineDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "obs-jasmine.h"
#include "obs-jasmineDlg.h"
#include "afxdialogex.h"
#include "interface.h"
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
#include "WebView2.h"
#include <wrl.h>
#include <string>
#include <wrl.h>
#include <wil/com.h>


// CobsjasmineDlg 消息处理程序
using namespace Microsoft::WRL;
// Pointer to WebViewController
static wil::com_ptr<ICoreWebView2Controller> webviewController;
// Pointer to WebView window
static wil::com_ptr<ICoreWebView2> webviewWindow;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CobsjasmineDlg 对话框



CobsjasmineDlg::CobsjasmineDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBSJASMINE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CobsjasmineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_wnd);
	DDX_Control(pDX, IDC_STATIC_WEB, m_web);
}

BEGIN_MESSAGE_MAP(CobsjasmineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CobsjasmineDlg::OnBnClickedButton1)
	ON_WM_NCDESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


CString GetLocalAppdataPath()
{
	wchar_t m_lpszDefaultDir[MAX_PATH] = { 0 };
	wchar_t szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}
	return m_lpszDefaultDir;
}

BOOL CobsjasmineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CHAR exeFullPath[MAX_PATH];
	CString strPath;
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	strPath = (CString)exeFullPath;
	int position = strPath.ReverseFind('\\');
	strPath = strPath.Left(position + 1);
	CHAR FilePath[MAX_PATH];
	GetModuleFileNameA(NULL, FilePath, MAX_PATH);
	(strrchr(FilePath, '\\'))[1] = 0;
	lstrcatA(FilePath, ("..\\..\\nginx"));



	ShellExecuteA(NULL, "open", "nginxxa.exe", NULL, FilePath, SW_SHOWNORMAL);



	jasmine_obs_startup();
	jasmine_obs_capture_monitor(m_wnd.GetSafeHwnd());
	//jasmine_obs_enable_audio_input();
	jasmine_obs_enable_audio_output();
	jasmine_obs_start_streaming();

#if 0
	if (webviewController != nullptr) {
		RECT bounds;
		m_web.GetClientRect(&bounds);
		webviewController->put_Bounds(bounds);
	};
#endif

	HWND hWnd = m_web.GetSafeHwnd();

	// Step 3 - Create a single WebView within the parent window
// Locate the browser and set up the environment for WebView
	CreateCoreWebView2EnvironmentWithOptions(_T("../../WebView2"), GetLocalAppdataPath(), nullptr,
		Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
			[hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {

		// Create a CoreWebView2Controller and get the associated CoreWebView2 whose parent is the main window hWnd
		env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
			[hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
			if (controller != nullptr) {
				webviewController = controller;
				webviewController->get_CoreWebView2(&webviewWindow);
			}

			// Add a few settings for the webview
			// The demo step is redundant since the values are the default settings
			ICoreWebView2Settings* Settings;
			webviewWindow->get_Settings(&Settings);
			Settings->put_IsScriptEnabled(TRUE);
			Settings->put_AreDefaultScriptDialogsEnabled(TRUE);
			Settings->put_IsWebMessageEnabled(TRUE);

			// Resize WebView to fit the bounds of the parent window
			RECT bounds;
			::GetClientRect(hWnd, &bounds);
			webviewController->put_Bounds(bounds);

			// Schedule an async task to navigate to Bing
			webviewWindow->Navigate(L"https://www.bing.com/");

			// Step 4 - Navigation events

			// Step 5 - Scripting

			// Step 6 - Communication between host and web content

			return S_OK;
		}).Get());
		return S_OK;
	}).Get());


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CobsjasmineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CobsjasmineDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CobsjasmineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//#include "libevent_http.h"
void CobsjasmineDlg::OnBnClickedButton1()
{
#if 1
	// TODO: 在此添加控件通知处理程序代码
	//curl_global_init(CURL_GLOBAL_ALL);
	//jasminecore::instance().init();

	//libevent::http::instance().start();




#endif


#if 0
	windows_process::controller * p = new windows_process::controller();

	TCHAR exeFullPath[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	strPath = (CString)exeFullPath;
	int position = strPath.ReverseFind('\\');
	strPath = strPath.Left(position + 1);
	TCHAR FilePath[MAX_PATH];
	GetModuleFileName(NULL, FilePath, MAX_PATH);
	(_tcsrchr(FilePath, '\\'))[1] = 0;//// 删除文件名，只获得路径字串   
    lstrcat(FilePath,_T("..\\..\\nginx\\nginxxa.exe")); 


	p->start(FilePath);
#endif


}
BOOL CobsjasmineDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;

		case VK_ESCAPE:
			return TRUE;

		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CobsjasmineDlg::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// TODO: 在此处添加消息处理程序代码
	//
	jasmine_obs_stop_streaming();
	jasmine_obs_shutdown();
	//
	CHAR exeFullPath[MAX_PATH];
	CString strPath;
	GetModuleFileNameA(NULL, exeFullPath, MAX_PATH);
	strPath = (CString)exeFullPath;
	int position = strPath.ReverseFind('\\');
	strPath = strPath.Left(position + 1);
	CHAR FilePath[MAX_PATH];
	GetModuleFileNameA(NULL, FilePath, MAX_PATH);
	(strrchr(FilePath, '\\'))[1] = 0;
	lstrcatA(FilePath, ("..\\..\\nginx"));
	ShellExecuteA(NULL, "open", "nginxxa.exe", "-s quit", FilePath, SW_SHOWNORMAL);
	//
}


void CobsjasmineDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (webviewController != nullptr) {
		RECT bounds;
		m_web.GetClientRect(&bounds);
		webviewController->put_Bounds(bounds);
	};

}
