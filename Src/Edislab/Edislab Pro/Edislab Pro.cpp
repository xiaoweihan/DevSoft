
// Edislab Pro.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include <string>
#include <boost/filesystem.hpp>
#include "MainFrm.h"
#include "Edislab ProDoc.h"
#include "Edislab ProView.h"
#include "DumpFileSwitch.h"
#include "Utility.h"
#include "BaseDialog.h"
#include "ComImple.h"
#include "Log.h"
#include "SensorConfig.h"
using std::string;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEdislabProApp

BEGIN_MESSAGE_MAP(CEdislabProApp, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CEdislabProApp::OnAppAbout)
	// 基于文件的标准文档命令
	//ON_COMMAND(ID_FILE_NEW, &CBCGPWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CBCGPWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CBCGPWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEdislabProApp 构造

CEdislabProApp::CEdislabProApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Edislab Pro.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CEdislabProApp 对象

CEdislabProApp theApp;


// CEdislabProApp 初始化

BOOL CEdislabProApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CBCGPWinApp::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);
	Init();
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4); 
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CEdislabProDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CEdislabProView));
	if (!pDocTemplate)
	{
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);
	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
	{
		return FALSE;
	}
	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->SetWindowText(_T("Edislab Pro"));
	m_pMainWnd->UpdateWindow();
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	SetVisualTheme(BCGP_VISUAL_THEME_OFFICE_2007_BLUE);

	//启动串口通信
	//CComImple::CreateInstance().StartCom(NULL);

	return TRUE;
}

int CEdislabProApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);
	::BCGCBProCleanUp();
	CleanState();
	return CWinApp::ExitInstance();
}

// CEdislabProApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CBaseDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBaseDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBaseDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CEdislabProApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEdislabProApp 自定义加载/保存方法

void CEdislabProApp::PreLoadState()
{
}

void CEdislabProApp::LoadCustomState()
{
}

void CEdislabProApp::SaveCustomState()
{
}

void CEdislabProApp::Init( void )
{

	//初始化打印日志
	std::string strLogDir = Utility::GetExeDirecory() + std::string("\\log\\");

	boost::filesystem::create_directories(strLogDir);


	CLog::CreateInstance().SetLogPath(strLogDir.c_str());
	CLog::CreateInstance().SetLogNamePrefix("Edislab");
#ifdef _DEBUG
	CLog::CreateInstance().SetLogLevel(LOG_DEBUG);
#else
	CLog::CreateInstance().SetLogLevel(LOG_ERROR);
#endif
	//设置生成的dump文件路径
	std::string strDumpFilePath = Utility::GetExeDirecory() + std::string("\\Edislab.dmp");
	CDumpFileSwitch::CreateInstance().SetDumpFilePath(strDumpFilePath.c_str());
	CDumpFileSwitch::CreateInstance().OpenSwitch();

	//加载传感器配置文件
	if (!CSensorConfig::CreateInstance().LoadSensorConfig())
	{
		ERROR_LOG("LoadSensorConfig failed!");
	}
}

// CEdislabProApp 消息处理程序


