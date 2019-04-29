// mainfrm.cpp : implementation of the CMainFrame class
//
// 最先被调用的主框架
//
// CMainFrame()           : 
// ~CMainFrame()          :
// PreCreateWindow()      : Register private window class.
// OnCreate()             : Create tool bar.
// CreateToolBar()
// CreateObjectBar()
// CreateStatusBar()
//
// OnUpdateInsert()       :
// OnToggleInsert()
// OnUpdateObjectBar()
//
// SetMousePosition()     : Set mouse position on status bar.
// SetStatusBarMessage()  : show message on status bar.
// ShowDlgMessage()       : show message on message dialog bar.
// SetObjectRect()        : Show object's rect on status bar.
// ShowMessageBar()       : Toggle show message dialog bar.
// IsShowMessageBar()     : Test weither show message dialog.
//
// OnObjectBrush()
// OnObjectDynamiclink() :
// OnObjectErase()
// OnObjectLibrary()
// OnObjectLine()
// OnObjectOval()
// OnObjectPaint()
// OnObjectPen()
// OnObjectRect()
// OnObjectRoundrect()
// OnObjectSelect()
// OnObjectText()
// OnObjectGraph()
// OnObjectComponent()
// OnObjectPushbutton()
// OnObjectSlider()
// OnObjectGauge()
// OnObjectFillbar()
// OnObjectNumberinput()
//
// OnDestroy()
// OnSysCommand()
// OnUpdateSystemClose()
// OnOptionSystemShowmessagebar()
// OnUpdateOptionSystemShowmessagebar()
//
// OnUpdatetime()
// InitStatusBar()
//
////////////////////////////////////////////////////////////////////// 

#include "stdafx.h"

#include "stdarg.h"

#include "dacview.h"

#include "globedef.h"

#include "mainfrm.h"
#include "cobjectBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIME, OnUpdateTime)
  //{{AFX_MSG_MAP(CMainFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_TOGGLE_INSERT, OnToggleInsert)
  ON_UPDATE_COMMAND_UI(ID_INDICATOR_OVR, OnUpdateInsert)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_BRUSH, OnUpdateObjectBar)
  ON_COMMAND(ID_OBJECT_BRUSH, OnObjectBrush)
  ON_COMMAND(ID_OBJECT_DYNAMICLINK, OnObjectDynamiclink)
  ON_COMMAND(ID_OBJECT_ERASE, OnObjectErase)
  ON_COMMAND(ID_OBJECT_LINE, OnObjectLine)
  ON_COMMAND(ID_OBJECT_OVAL, OnObjectOval)
  ON_COMMAND(ID_OBJECT_PAINT, OnObjectPaint)
  ON_COMMAND(ID_OBJECT_PEN, OnObjectPen)
  ON_COMMAND(ID_OBJECT_RECT, OnObjectRect)
  ON_COMMAND(ID_OBJECT_ROUNDRECT, OnObjectRoundrect)
  ON_COMMAND(ID_OBJECT_SELECT, OnObjectSelect)
  ON_COMMAND(ID_OBJECT_TEXT, OnObjectText)
  ON_COMMAND(ID_OBJECT_GRAPH, OnObjectGraph)
  ON_COMMAND(ID_OBJECT_COMPOUND, OnObjectComponent)
  ON_COMMAND(ID_OBJECT_PUSHBUTTON, OnObjectPushbutton)
  ON_COMMAND(ID_OBJECT_SLIDER, OnObjectSlider)
  ON_COMMAND(ID_OBJECT_GAUGE, OnObjectGauge)
  ON_WM_DESTROY()
  ON_COMMAND(ID_OBJECT_KNOB, OnObjectKnob)
  ON_COMMAND(ID_OBJECT_METER, OnObjectMeter)
  ON_COMMAND(ID_OBJECT_FILLBAR, OnObjectFillbar)
  ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_OBJECT_NUMBERINPUT, OnObjectNumberinput)
	ON_COMMAND(ID_VIEW_OBJECTBAR, OnViewObjectbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECTBAR, OnUpdateViewObjectbar)
	ON_COMMAND(ID_VIEW_NEXT, OnViewNext)
	ON_COMMAND(ID_VIEW_PREV, OnViewPrev)
	ON_COMMAND(ID_VIEW_UNITLIBRARY, OnViewUnitlibrary)
	ON_UPDATE_COMMAND_UI(ID_VIEW_UNITLIBRARY, OnUpdateViewUnitlibrary)
	ON_COMMAND(ID_VIEW_MESSAGEBAR, OnViewMessagebar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MESSAGEBAR, OnUpdateViewMessagebar)
	ON_COMMAND(ID_OBJECT_PICTURE, OnObjectPicture)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_DYNAMICLINK, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_ERASE, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_OVAL, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_PAINT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_PEN, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_RECT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_ROUNDRECT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_SELECT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_TEXT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_LINE, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_GRAPH, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_COMPOUND, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_PUSHBUTTON, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_SLIDER, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_GAUGE, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_KNOB, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_METER, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_FILLBAR, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_NUMBERINPUT, OnUpdateObjectBar)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_PICTURE, OnUpdateObjectBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

ULONG           gl_ulDrawTool;
ULONG           gl_ulUnitLibrary;
ULONG           gl_ulNumberOfShowViewRun = 0;

static UINT BASED_CODE indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_MOUSE_POSITION,
  ID_RECT_POSITION,
  ID_REALTIME_TIME_ALLOCATE,
  ID_SYSTEM_TIME_ALLOCATE,
  ID_INDICATOR_TIME,
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  // TODO: add member initialization code here 
  m_bInsert = TRUE;      // default to over-strke mode

  gl_ulDrawTool = ID_OBJECT_SELECT;

  gl_fontSmall.CreateFont(8, 8, 0, 0, FW_LIGHT, 0, 0, 0, OEM_CHARSET, OUT_TT_PRECIS,
                          CLIP_TT_ALWAYS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, nullptr);
    

}

CMainFrame::~CMainFrame()
{
}  

// We override PreCreateWindow to specify a different window class (WNDCLASS),
//   one with a different background and with the CS_VREDRAW/CS_HREDRAW
//   style so all the frame window's control bars will be repainted
//   when the window is resized.
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  ASSERT(cs.lpszClass == nullptr);       // must not be specified
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)(COLOR_WINDOW+1),
    AfxGetApp()->LoadIcon(IDR_MAINFRAME));
  return TRUE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
    return -1;
  EnableDocking(CBRS_ALIGN_ANY);

  if ( !CreateToolBar() )
    return -1;
  DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_TOP);
  if ( !CreateObjectBar() )
    return -1;
  DockControlBar(&m_wndObjectBar, AFX_IDW_DOCKBAR_LEFT);
  if ( !CreateStatusBar() )
    return -1;

  if ( !CreateUnitLibraryBar() )
    return -1;
  // DockControlBar(&m_wndUnitLibraryBar, AFX_IDW_DOCKBAR_LEFT);

  if (!m_wndMessageBar.Create(this, IDD_VIEW_MESSAGE,
    CBRS_BOTTOM, IDD_VIEW_MESSAGE))
  {
    return -1;      // fail to create
  }

  m_wndStatusBar.SetTimer(0x1000, 1000, nullptr);
  return 0;
} 

BOOL CMainFrame::CreateToolBar()
{
   if (!m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS) ||
    !m_wndToolBar.LoadBitmap(IDR_MAINFRAME))
  {
    return FALSE;       // fail to create
  }
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndToolBar.SetButtons(nullptr, 33);
  m_wndToolBar.SetButtonInfo(0, ID_FILE_NEW,TBBS_BUTTON, 0);
  m_wndToolBar.SetButtonInfo(1, ID_FILE_OPEN,TBBS_BUTTON, 1);
  m_wndToolBar.SetButtonInfo(2, ID_FILE_SAVE,TBBS_BUTTON, 2);
  m_wndToolBar.SetButtonInfo(3, ID_SEPARATOR,TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(4, ID_EDIT_CUT,TBBS_BUTTON, 3);
  m_wndToolBar.SetButtonInfo(5, ID_EDIT_COPY,TBBS_BUTTON, 4);
  m_wndToolBar.SetButtonInfo(6 , ID_EDIT_PASTE,TBBS_BUTTON, 5);
  m_wndToolBar.SetButtonInfo(7, ID_SEPARATOR,TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(8, ID_STYLE_LEFT,TBBS_BUTTON, 11);
  m_wndToolBar.SetButtonInfo(9, ID_STYLE_CENTERED,TBBS_BUTTON, 12);
  m_wndToolBar.SetButtonInfo(10, ID_STYLE_RIGHT,TBBS_BUTTON, 13);
  m_wndToolBar.SetButtonInfo(11, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	m_wndToolBar.SetButtonInfo(12, ID_ARRANGE_TOFRONT, TBBS_BUTTON, 18);
	m_wndToolBar.SetButtonInfo(13, ID_ARRANGE_TOBACK, TBBS_BUTTON, 17);
  m_wndToolBar.SetButtonInfo(14, ID_SEPARATOR,TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(15, ID_ARRANGE_MAKEDYNLINK,TBBS_BUTTON, 16);
  m_wndToolBar.SetButtonInfo(16, ID_ARRANGE_LINK_INTO_COMPONENT,TBBS_BUTTON, 20);
  m_wndToolBar.SetButtonInfo(17, ID_SEPARATOR,TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(18, ID_VIEW_VIEWIN,TBBS_BUTTON, 9);
  m_wndToolBar.SetButtonInfo(19, ID_VIEW_VIEWOUT,TBBS_BUTTON, 10);
  m_wndToolBar.SetButtonInfo(20, ID_SEPARATOR, TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(21, ID_PROJECT_COMPILE, TBBS_BUTTON, 19);
  m_wndToolBar.SetButtonInfo(22, ID_SEPARATOR, TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(23, ID_VIEW_STATUS, TBBS_BUTTON, 15);
  m_wndToolBar.SetButtonInfo(24, ID_SEPARATOR, TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(25, ID_ARRANGE_TOGGLECUTOFF, TBBS_BUTTON, 21);
  m_wndToolBar.SetButtonInfo(26, ID_SEPARATOR, TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(27, ID_RUN_RUN, TBBS_BUTTON, 23);
  m_wndToolBar.SetButtonInfo(28, ID_RUN_STOP, TBBS_BUTTON, 22);
  m_wndToolBar.SetButtonInfo(29, ID_RUN_PAUSE, TBBS_BUTTON, 25);
  m_wndToolBar.SetButtonInfo(30, ID_RUN_RESUME, TBBS_BUTTON, 26);
  m_wndToolBar.SetButtonInfo(31, ID_SEPARATOR,TBBS_SEPARATOR, 6);
  m_wndToolBar.SetButtonInfo(32, ID_APP_ABOUT,TBBS_BUTTON, 7);
  m_wndToolBar.Invalidate();
  return TRUE;
}

BOOL CMainFrame::CreateObjectBar()
{
  if (!m_wndObjectBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS, ID_VIEW_OBJECTBAR) ||
    !m_wndObjectBar.LoadBitmap(IDB_OBJECT))
  {
    return FALSE;       // fail to create
  }
  m_wndObjectBar.EnableDocking(CBRS_ALIGN_ANY);
  m_wndObjectBar.SetButtons(nullptr, 18);
  m_wndObjectBar.SetButtonInfo(0, ID_OBJECT_SELECT,TBBS_BUTTON, 2);
  m_wndObjectBar.SetButtonInfo(1 , ID_OBJECT_TEXT,TBBS_BUTTON, 8 );
  m_wndObjectBar.SetButtonInfo(2, ID_OBJECT_LINE,TBBS_BUTTON, 6 );
  m_wndObjectBar.SetButtonInfo(3, ID_OBJECT_RECT,TBBS_BUTTON, 9);
  m_wndObjectBar.SetButtonInfo(4, ID_OBJECT_ROUNDRECT,TBBS_BUTTON, 10);
  m_wndObjectBar.SetButtonInfo(5, ID_OBJECT_OVAL,TBBS_BUTTON, 11);
  m_wndObjectBar.SetButtonInfo(6, ID_SEPARATOR,TBBS_SEPARATOR, 6);
  m_wndObjectBar.SetButtonInfo(7, ID_OBJECT_PICTURE, TBBS_BUTTON, 21);
  m_wndObjectBar.SetButtonInfo(8, ID_OBJECT_GRAPH, TBBS_BUTTON, 13); 
  m_wndObjectBar.SetButtonInfo(9, ID_OBJECT_GAUGE, TBBS_BUTTON, 16); 
  m_wndObjectBar.SetButtonInfo(10, ID_OBJECT_FILLBAR, TBBS_BUTTON, 19); 
  m_wndObjectBar.SetButtonInfo(11, ID_OBJECT_COMPOUND, TBBS_BUTTON, 12);
  m_wndObjectBar.SetButtonInfo(12, ID_SEPARATOR,TBBS_SEPARATOR, 6);
  m_wndObjectBar.SetButtonInfo(13, ID_OBJECT_KNOB, TBBS_BUTTON, 17); 
  m_wndObjectBar.SetButtonInfo(14, ID_OBJECT_METER, TBBS_BUTTON, 18); 
  m_wndObjectBar.SetButtonInfo(15, ID_OBJECT_SLIDER, TBBS_BUTTON, 15);
  m_wndObjectBar.SetButtonInfo(16, ID_OBJECT_PUSHBUTTON, TBBS_BUTTON, 14);
  m_wndObjectBar.SetButtonInfo(17, ID_OBJECT_NUMBERINPUT, TBBS_BUTTON, 20);
  m_wndObjectBar.Invalidate();
  return TRUE;
}

BOOL CMainFrame::CreateStatusBar()
{
  if (!m_wndStatusBar.Create(this) ||
    !m_wndStatusBar.SetIndicators(indicators,
      sizeof(indicators)/sizeof(UINT)))
  {
    return FALSE;       // fail to create
  }

  UINT nID, nStyle;
  int cxWidth;
  
  m_wndStatusBar.GetPaneInfo( 0, nID, nStyle, cxWidth);
  m_wndStatusBar.SetPaneInfo( 0, nID, SBPS_STRETCH|SBPS_NORMAL, cxWidth);

  return TRUE;
}

BOOL CMainFrame::CreateUnitLibraryBar() {
	if (!m_wndUnitLibraryBar.Create(this,	WS_CHILD | WS_VISIBLE | CBRS_LEFT 
            | CBRS_TOOLTIPS | CBRS_FLYBY, ID_VIEW_UNITLIBRARY) ||
		!m_wndUnitLibraryBar.LoadBitmap(IDB_UNIT_LIBRARY))
	{
		TRACE0("Failed to create unit library bar\n");
		return FALSE;       // fail to create
	}
  m_wndUnitLibraryBar.EnableDocking(CBRS_ALIGN_ANY);
  m_iUnitCurrentCascade = 0;
	ViewUnitLibrary();
//	OnViewUnitlibrary();	// 将UnitLibrary Bar关闭.
	return TRUE;
}

BOOL CMainFrame::ViewUnitLibrary( void ) {
  // Set the UnitLibraryBar to show current command list
  switch ( m_iUnitCurrentCascade ) {
  case 0 :
    m_wndUnitLibraryBar.SetButtons(nullptr, 11);
	  m_wndUnitLibraryBar.SetButtonInfo(0, ID_VIEW_PREV,TBBS_BUTTON, 0);
	  m_wndUnitLibraryBar.SetButtonInfo(1, ID_VIEW_NEXT,TBBS_BUTTON, 1);
    m_wndUnitLibraryBar.SetButtonInfo(2, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	  m_wndUnitLibraryBar.SetButtonInfo(3, ID_BLOCK_BASICLOGIC_AND,TBBS_BUTTON, 2);
	  m_wndUnitLibraryBar.SetButtonInfo(4, ID_BLOCK_BASICLOGIC_OR,TBBS_BUTTON, 3);
	  m_wndUnitLibraryBar.SetButtonInfo(5, ID_BLOCK_BASICLOGIC_XOR,TBBS_BUTTON, 4);
	  m_wndUnitLibraryBar.SetButtonInfo(6, ID_BLOCK_BASICLOGIC_INV,TBBS_BUTTON, 5);
	  m_wndUnitLibraryBar.SetButtonInfo(7, ID_BLOCK_BASICLOGIC_PULS,TBBS_BUTTON, 6);
	  m_wndUnitLibraryBar.SetButtonInfo(8, ID_BLOCK_BASICLOGIC_DLAY,TBBS_BUTTON, 7);
	  m_wndUnitLibraryBar.SetButtonInfo(9, ID_BLOCK_BASICLOGIC_DROT,TBBS_BUTTON, 8);
    m_wndUnitLibraryBar.SetButtonInfo(10, ID_SEPARATOR, TBBS_SEPARATOR, 30);
    break;
  case 1 :
    m_wndUnitLibraryBar.SetButtons(nullptr, 10);
	  m_wndUnitLibraryBar.SetButtonInfo(0, ID_VIEW_PREV,TBBS_BUTTON, 0);
	  m_wndUnitLibraryBar.SetButtonInfo(1, ID_VIEW_NEXT,TBBS_BUTTON, 1);
    m_wndUnitLibraryBar.SetButtonInfo(2, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	  m_wndUnitLibraryBar.SetButtonInfo(3, ID_BLOCK_MATHMATIC_ADD,TBBS_BUTTON, 11);
	  m_wndUnitLibraryBar.SetButtonInfo(4, ID_BLOCK_MATHMATIC_MULTIPLE,TBBS_BUTTON, 12);
	  m_wndUnitLibraryBar.SetButtonInfo(5, ID_BLOCK_MATHMATIC_DIV,TBBS_BUTTON, 13);
	  m_wndUnitLibraryBar.SetButtonInfo(6, ID_BLOCK_MATHMATIC_AVG,TBBS_BUTTON, 14);
	  m_wndUnitLibraryBar.SetButtonInfo(7, ID_BLOCK_MATHMATIC_AVGT,TBBS_BUTTON, 15);
	  m_wndUnitLibraryBar.SetButtonInfo(8, ID_BLOCK_MATHMATIC_EXPR,TBBS_BUTTON, 16);
    m_wndUnitLibraryBar.SetButtonInfo(9, ID_SEPARATOR, TBBS_SEPARATOR, 45);
    break;
  case 2 :
    m_wndUnitLibraryBar.SetButtons(nullptr, 8);
	  m_wndUnitLibraryBar.SetButtonInfo(0, ID_VIEW_PREV,TBBS_BUTTON, 0);
	  m_wndUnitLibraryBar.SetButtonInfo(1, ID_VIEW_NEXT,TBBS_BUTTON, 1);
    m_wndUnitLibraryBar.SetButtonInfo(2, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	  m_wndUnitLibraryBar.SetButtonInfo(3, ID_BLOCK_SELECT_SWCH,TBBS_BUTTON, 21);
	  m_wndUnitLibraryBar.SetButtonInfo(4, ID_BLOCK_SELECT_HSEL,TBBS_BUTTON, 22);
	  m_wndUnitLibraryBar.SetButtonInfo(5, ID_BLOCK_SELECT_MSEL,TBBS_BUTTON, 23);
	  m_wndUnitLibraryBar.SetButtonInfo(6, ID_BLOCK_SELECT_LSEL,TBBS_BUTTON, 24);
    m_wndUnitLibraryBar.SetButtonInfo(7, ID_SEPARATOR, TBBS_SEPARATOR, 75);
	  break;
  case 3 :
    m_wndUnitLibraryBar.SetButtons(nullptr, 9);
	  m_wndUnitLibraryBar.SetButtonInfo(0, ID_VIEW_PREV,TBBS_BUTTON, 0);
    m_wndUnitLibraryBar.SetButtonInfo(1, ID_VIEW_NEXT,TBBS_BUTTON, 1);
    m_wndUnitLibraryBar.SetButtonInfo(2, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	  m_wndUnitLibraryBar.SetButtonInfo(3, ID_BLOCK_BASICCONTROL_PID,TBBS_BUTTON, 31);
	  m_wndUnitLibraryBar.SetButtonInfo(4, ID_BLOCK_BASICCONTROL_INTD,TBBS_BUTTON, 32);
	  m_wndUnitLibraryBar.SetButtonInfo(5, ID_BLOCK_BASICCONTROL_PD,TBBS_BUTTON, 33);
	  m_wndUnitLibraryBar.SetButtonInfo(6, ID_BLOCK_BASICCONTROL_AMB,TBBS_BUTTON, 34);
	  m_wndUnitLibraryBar.SetButtonInfo(7, ID_BLOCK_BASICCONTROL_TIME,TBBS_BUTTON, 35);
    m_wndUnitLibraryBar.SetButtonInfo(8, ID_SEPARATOR, TBBS_SEPARATOR, 60);
    break;
  case 4 :
	  m_wndUnitLibraryBar.SetButtons(nullptr, 6);
	  m_wndUnitLibraryBar.SetButtonInfo(0, ID_VIEW_PREV,TBBS_BUTTON, 0);
	  m_wndUnitLibraryBar.SetButtonInfo(1, ID_VIEW_NEXT,TBBS_BUTTON, 1);
    m_wndUnitLibraryBar.SetButtonInfo(2, ID_SEPARATOR,TBBS_SEPARATOR, 6);
	  m_wndUnitLibraryBar.SetButtonInfo(3, ID_BLOCK_COMPOUND,TBBS_BUTTON, 20);
		m_wndUnitLibraryBar.SetButtonInfo(4, ID_BLOCK_COMPONENT, TBBS_BUTTON, 19);
    m_wndUnitLibraryBar.SetButtonInfo(5, ID_SEPARATOR, TBBS_SEPARATOR, 120);
    break;
  default :
		TRACE("Unit折叠层选项超出范围\n");
		ASSERT(m_iUnitCurrentCascade <= MAX_UNIT_CASCADE);
    break;
  }
	// invalidate the call update handlers before painting
	m_wndUnitLibraryBar.Invalidate();
  return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnUpdateInsert(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!m_bInsert);
}

void CMainFrame::OnToggleInsert()
{
  m_bInsert = !m_bInsert;
}

void CMainFrame::OnUpdateObjectBar(CCmdUI* pCmdUI)
{
  pCmdUI->SetRadio(gl_ulDrawTool == pCmdUI->m_nID);
}

void CMainFrame::SetMousePosition(CPoint pt)
{
  // TODO: Add your message handler code here and/or call default
  char  strTemp[11];                          
  
  sprintf_s(strTemp, "%04d, %04d", pt.x, pt.y);
  m_wndStatusBar.SetPaneText(1, strTemp);
}                                                        
                                        
void CMainFrame::SetStatusBarMessage( const CString& str ) {
  // Can't process empty string, for it produce memory leaks.
  ASSERT( !str.IsEmpty() );
  m_wndStatusBar.SetPaneText( 0, (LPCTSTR)str );
}

void CMainFrame::ShowDlgMessage( UINT uID, va_list ap ) {
  CString str, strFormat;
  char strTemp2[200], strTemp1[200];		// 字符串的总长不能超过200个字。
  LRESULT result, curPos;
  
  ASSERT( (uID >= 1000) && (uID < 10000) );
  VERIFY(strFormat.LoadString( uID ));
  vsprintf_s( strTemp1, (LPCTSTR)strFormat, ap );
  if ( uID < 5000 ) {
    sprintf_s( strTemp2, "Error %d :", uID);
  }
  else if ( uID < 8000 ) {
    sprintf_s( strTemp2, "Warning %d :", uID);
  }
  else {
    sprintf_s( strTemp2, "Message %d :", uID);
  }
  str = strTemp2;
  str += strTemp1;
  curPos = m_wndMessageBar.SendDlgItemMessage( IDC_LIST_MESSAGE, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
  result = m_wndMessageBar.SendDlgItemMessage( IDC_LIST_MESSAGE, LB_ADDSTRING, (WPARAM)0, 
                                              (LPARAM)((LPCTSTR)str));
  if ( result == (curPos + 1) ) {   // is at the bottom line ?
    m_wndMessageBar.SendDlgItemMessage( IDC_LIST_MESSAGE, LB_SETCURSEL, (WPARAM)result, (LPARAM)0);
  }
}

void CMainFrame::SetObjectRect( CObjectBase * pobj ) {
  ASSERT( pobj != nullptr );
  CRect rect;
  CString strNumber;
  static char str[20];
  
  rect = pobj->GetSize();
  sprintf_s(str, "%03d,%03d %03dx%03d", rect.left, rect.top, rect.Width(), rect.Height());
  strNumber.Empty();
  strNumber += str;
  m_wndStatusBar.SetPaneText(2, strNumber);
}

void CMainFrame::InShowView( void ) {
	// do nothing
	/*
  if ( (m_wndObjectBar.GetStyle() & WS_VISIBLE ) != 0 ) {
    ShowControlBar(&m_wndObjectBar, FALSE, FALSE);
  }
  if ( (m_wndUnitLibraryBar.GetStyle() & WS_VISIBLE ) != 0 ) {
    ShowControlBar(&m_wndUnitLibraryBar, FALSE, FALSE);
  }
	*/
}

void CMainFrame::OnObjectBrush() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_BRUSH;
}

void CMainFrame::OnObjectDynamiclink() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_DYNAMICLINK;
}

void CMainFrame::OnObjectErase() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_ERASE;
}

void CMainFrame::OnObjectLine() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_LINE;
}

void CMainFrame::OnObjectOval() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_OVAL;
}

void CMainFrame::OnObjectPaint() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_PAINT;
}

void CMainFrame::OnObjectPen() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_PEN;
}

void CMainFrame::OnObjectRect() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_RECT;
}

void CMainFrame::OnObjectRoundrect() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_ROUNDRECT;
}

void CMainFrame::OnObjectSelect() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_SELECT;
}

void CMainFrame::OnObjectText() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_TEXT;
}


void CMainFrame::OnObjectGraph() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_GRAPH;
}

void CMainFrame::OnObjectComponent() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_COMPOUND;
}

void CMainFrame::OnObjectPushbutton() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_PUSHBUTTON;
}

void CMainFrame::OnObjectSlider() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_SLIDER;
}

void CMainFrame::OnObjectGauge() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_GAUGE;
}

void CMainFrame::OnObjectKnob() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_KNOB;
}

void CMainFrame::OnObjectMeter() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_METER;
}

void CMainFrame::OnObjectFillbar() 
{
  // TODO: Add your command handler code here
  gl_ulDrawTool = ID_OBJECT_FILLBAR;
}

void CMainFrame::OnObjectPicture() 
{
	// TODO: Add your command handler code here
	// gl_ulDrawTool = ID_OBJECT_PICTURE;
}

void CMainFrame::OnObjectNumberinput() 
{
	// TODO: Add your command handler code here
	gl_ulDrawTool = ID_OBJECT_NUMBERINPUT;
}

void CMainFrame::OnDestroy() 
{
  CMDIFrameWnd::OnDestroy();
  
  // TODO: Add your message handler code here

}

/*
  forbidden close file while it is running
*/
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) {
  if ( (nID & 0Xfff0) == SC_CLOSE ) {
    if ( gl_ulNumberOfShowViewRun > 0 ) {
      ShowMessage(ID_ERROR_CLOSE_SYSTEM);
      return;
    }
  }

  CMDIFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnUpdateTime(CCmdUI* pCmdUI)
{
  ULONGLONG ulTick = GetTickCount64();

	// Get current date and format it
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("%X"));

	// BLOCK: compute the width of the date string
	CSize size;
	{
		HGDIOBJ hOldFont = nullptr;
		HFONT hFont = (HFONT)m_wndStatusBar.SendMessage(WM_GETFONT);
		CClientDC dc(nullptr);
		if (hFont != nullptr) 
			hOldFont = dc.SelectObject(hFont);
		size = dc.GetTextExtent(strTime);
		if (hOldFont != nullptr) 
			dc.SelectObject(hOldFont);
	}

	// Update the pane to reflect the current time
	UINT nID, nStyle;
	int nWidth;
	m_wndStatusBar.GetPaneInfo(5, nID, nStyle, nWidth);
	m_wndStatusBar.SetPaneInfo(5, nID, nStyle, size.cx);
	pCmdUI->SetText(strTime);
	pCmdUI->Enable(TRUE);

  static int iTimeMinute;
  char str[20];
                                     
  // show system time allocate, update per minute.
  if ( iTimeMinute != time.GetMinute() ) {
    int iTemp;
    iTemp = gl_ulRealTimeProcessTime / 600;
    iTimeMinute = time.GetMinute();
    sprintf_s(str, "%3u%%", iTemp);
    if ( iTemp > 50 ) ShowMessage(ID_WARN_REALTIME_SYSTEM_OVERFLOW);
    m_wndStatusBar.SetPaneText( 3, str );
    
    iTemp = (gl_ulSystemProcessTime + gl_ulRealTimeProcessTime) / 600;
    sprintf_s(str, "%3u%%", iTemp);
    if ( iTemp > 100 ) ShowMessage(ID_WARN_SYSTEM_OVERFLOW);
    m_wndStatusBar.SetPaneText( 4, str );
    gl_ulRealTimeProcessTime = 0;
    gl_ulSystemProcessTime = 0;
  }
  gl_ulSystemProcessTime += GetTickCount64() - ulTick;
}

void CMainFrame::OnViewObjectbar() 
{
	// TODO: Add your command handler code here
	BOOL bVisible = ((m_wndObjectBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndObjectBar, !bVisible, FALSE);
}

void CMainFrame::OnUpdateViewObjectbar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( (m_wndObjectBar.GetStyle() & WS_VISIBLE) != 0 );
}

void CMainFrame::OnViewNext() 
{
	// TODO: Add your command handler code here
	if ( m_iUnitCurrentCascade < MAX_UNIT_CASCADE ) m_iUnitCurrentCascade++;
  else m_iUnitCurrentCascade = 0;
  ViewUnitLibrary();
}

void CMainFrame::OnViewPrev() 
{
	// TODO: Add your command handler code here
	if ( m_iUnitCurrentCascade == 0 ) m_iUnitCurrentCascade = MAX_UNIT_CASCADE;
  else m_iUnitCurrentCascade--;
  ViewUnitLibrary();
}

void CMainFrame::OnViewUnitlibrary() 
{
	// TODO: Add your command handler code here
	BOOL bVisible = ((m_wndUnitLibraryBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndUnitLibraryBar, !bVisible, FALSE);
}

void CMainFrame::OnUpdateViewUnitlibrary(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( (m_wndUnitLibraryBar.GetStyle() & WS_VISIBLE) != 0 );
}

void CMainFrame::OnViewMessagebar() 
{
	// TODO: Add your command handler code here
	BOOL bVisible = ((m_wndMessageBar.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_wndMessageBar, !bVisible, FALSE);
}

void CMainFrame::OnUpdateViewMessagebar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck( (m_wndMessageBar.GetStyle() & WS_VISIBLE) != 0 );
}

