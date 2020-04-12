///////////////////////////////////////////////////////////////////////
//
// dacview.cpp : Defines the class behaviors for the application.
// FireBird DACView 的主程序.
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include"afxvisualmanager.h"

#include <afxpriv.h>    // for idle-update windows message

#include "omp.h"

#include "dacview.h"
#include "globedef.h"

#include "mainfrm.h"

#include "SQIFileDoc.h"
#include "SQIFileFrame.h"
#include "SQIObjectView.h"
#include "SQIUnitView.h"

#include "FBDFileFrame.h"
#include "FBDFileDoc.h"
#include "FBDObjView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// end of globe variable define
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// globe function define

//////////////////////////////////////////////////////////////
//
// ShowMessage()
//
// 描述:
//  多功能打印函数.
//
//////////////////////////////////////////////////////////////
BOOL ShowMessage(UINT uID, ...) {
  va_list ap;
  va_start(ap, uID);

  ((CMainFrame*)theApp.m_pMainWnd)->ShowDlgMessage(uID, ap);

  va_end(ap);
  return(TRUE);
}

BOOL ShowMessageIndirect(UINT uID, CString strMessage) {
  gl_fShowMessage = true;
  gl_ulMessageID = uID;
  gl_strMessage = strMessage;
  return(true);
}

void SetStatusBarMessage(const CString& str) {
  ((CMainFrame*)theApp.m_pMainWnd)->SetStatusBarMessage(str);
}

/////////////////////////////////////////////////////////////////////
//
// SetMousePosition()
//
// 显示鼠标位置.
//
/////////////////////////////////////////////////////////////////////
void SetMousePosition(CPoint ptDevice) {
  ((CMainFrame*)theApp.m_pMainWnd)->SetMousePosition(ptDevice);
}

void SetObjectRect(CObjectBase* pcobj) {
  ((CMainFrame*)theApp.m_pMainWnd)->SetObjectRect(pcobj);
}

// end globe functions define
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDacviewApp

BEGIN_MESSAGE_MAP(CDacviewApp, CWinApp)
  //{{AFX_MSG_MAP(CDacviewApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  //}}AFX_MSG_MAP
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
  // Global help commands
  ON_COMMAND(ID_HELP_INDEX, CWinApp::OnHelpIndex)
  ON_COMMAND(ID_HELP_USING, CWinApp::OnHelpUsing)
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
  ON_COMMAND(ID_CONTEXT_HELP, CWinApp::OnContextHelp)
  ON_COMMAND(ID_DEFAULT_HELP, CWinApp::OnHelpIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDacviewApp construction

CDacviewApp::CDacviewApp() {
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance

  gl_fShowMessage = false;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDacviewApp object
CDacviewApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDacviewApp initialization

BOOL CDacviewApp::InitInstance() {
#ifdef _DEBUG
  afxMemDF |= checkAlwaysMemDF;
#endif

  // Only permit one copy running
  if (FindWindow(nullptr, _T("FireBird DACView"))) { // FireBird DACView已经运行?
    MessageBox(nullptr,
               _T("Only one instance can run!"),
               _T("FireBird DACView Warnning:"),
               MB_OK | MB_ICONEXCLAMATION);
    return(FALSE);
  }

  // enable OLE Control
  // AfxEnableControlContainer();

  // Standard initialization
  LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.
  CMultiDocTemplate* pDocTemplate;

  // Register edit type document templates
  pDocTemplate = new CMultiDocTemplate(IDR_VIEWTYPE,
                                       RUNTIME_CLASS(CSQIFileDoc),
                                       RUNTIME_CLASS(CSQIFileFrame),	       // Spite MDI child frame
                                       RUNTIME_CLASS(CSQIObjectView));
  AddDocTemplate(pDocTemplate);

  // Register run type document templates,
  pDocTemplate = new CMultiDocTemplate(IDR_SHOWTYPE,
                                       RUNTIME_CLASS(CFBDFileDoc),
                                       RUNTIME_CLASS(CFBDFileFrame),        // Spite MDI child frame
                                       RUNTIME_CLASS(CFBDObjView));
  AddDocTemplate(pDocTemplate);

  // create main MDI Frame window
  CMainFrame* pMainFrame = new CMainFrame;
  if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    return FALSE;
  pMainFrame->ShowWindow(SW_SHOWMAXIMIZED /* m_nCmdShow */);
  pMainFrame->UpdateWindow();
  m_pMainWnd = pMainFrame;

  if (m_lpCmdLine[0] != '\0')
  {
    // TODO: add command line processing here
  }
  return TRUE;
}

int CDacviewApp::ExitInstance(void) {
  CMFCVisualManager::DestroyInstance(TRUE); // 使用可视化控件后，需要手动释放内存。

  return(CWinApp::ExitInstance());
}

// In this override of OnIdle we are doing UI for our app.
// Since this needs to be as fast as possible to give the user
// the best result we do our updates first when lCount is zero
// then we call the library to do its work.
BOOL CDacviewApp::OnIdle(LONG lCount) {
  BOOL fReturn = CWinApp::OnIdle(lCount);

  if (gl_fShowMessage) {
    ShowMessage(gl_ulMessageID, gl_strMessage);
    gl_fShowMessage = false;
  }

  if (lCount == 0)
  {
    ASSERT(m_pMainWnd != nullptr);

    // look for any top-level windows owned by us
    // we use 'HWND's to avoid generation of too many temporary CWnds
    for (HWND hWnd = ::GetWindow(m_pMainWnd->m_hWnd, GW_HWNDFIRST);
         hWnd != nullptr; hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT))
    {
      if (::GetParent(hWnd) == m_pMainWnd->m_hWnd)
      {
        // if owned window is active, move the activation to the
        //   application window
        if (GetActiveWindow() == hWnd && (::GetCapture() == nullptr))
          m_pMainWnd->SetActiveWindow();

        // also update the buttons for the top-level window
        SendMessage(hWnd, WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0L);
      }
    }
  }
  return(fReturn);
}

bool CDacviewApp::LoadCursors(HCURSOR* pDL, HCURSOR* pNoDrag, HCURSOR* pCpt) {
  VERIFY(*pDL = LoadCursor(_T("CURSOR_DYNAMIC_LINK")));
  VERIFY(*pNoDrag = LoadCursor(_T("NO_DRAG")));
  VERIFY(*pCpt = LoadCursor(_T("COMPOUND")));

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for FireBird DACView's About dialog

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  // Dialog Data
    //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

// Implementation
protected:
  virtual BOOL OnInitDialog();
  virtual void DoDataExchange(CDataExchange* pDX);  // DDX/DDV support
  //{{AFX_MSG(CAboutDlg)
    // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  //  afx_msg void OnBreakLinkOfComponent();
  //  afx_msg void OnUpdateBreakLinkOfComponent(CCmdUI *pCmdUI);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
  //}}AFX_MSG_MAP
//  ON_COMMAND(ID_BREAK_LINK_OF_COMPONENT, &CAboutDlg::OnBreakLinkOfComponent)
//  ON_UPDATE_COMMAND_UI(ID_BREAK_LINK_OF_COMPONENT, &CAboutDlg::OnUpdateBreakLinkOfComponent)
END_MESSAGE_MAP()

// App command to run the dialog
void CDacviewApp::OnAppAbout() {
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// VB-Event registration
// (calls to AfxRegisterVBEvent will be placed here by ClassWizard)

//{{AFX_VBX_REGISTER_MAP()
//}}AFX_VBX_REGISTER_MAP

/////////////////////////////////////////////////////////////////////////////
// CDacviewApp commands

BOOL CAboutDlg::OnInitDialog() {
  CDialog::OnInitDialog();

  return TRUE;
}

//void CAboutDlg::OnBreakLinkOfComponent()
//{
//  // TODO: 在此添加命令处理程序代码
//}

//void CAboutDlg::OnUpdateBreakLinkOfComponent(CCmdUI *pCmdUI)
//{
//  // TODO: 在此添加命令更新用户界面处理程序代码
//}