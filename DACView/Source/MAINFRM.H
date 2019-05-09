#ifndef __MAIN_FRAME_H__
#define __MAIN_FRAME_H__

#include "cObjectBase.h"

#include "dacview.h"

extern ULONG                    gl_ulUnitLibrary;
extern ULONG                    gl_ulDrawTool;
extern ULONG                    gl_ulNumberOfShowViewRun;
extern CDacviewApp theApp;

#define MAX_UNIT_CASCADE 4

class CMainFrame : public CMDIFrameWnd
{
  DECLARE_DYNAMIC(CMainFrame)
public:
  CMainFrame();

// Attributes
public:
  BOOL m_bInsert;     // TRUE => insert mode, FALSE => overstrike mode
  
// Operations
public:
  void SetMousePosition(CPoint pt);
  void SetObjectRect( CObjectBase * pobj );
  void SetStatusBarMessage( const CString& str );
       
  void ShowDlgMessage( UINT uID, va_list ap );

  void InShowView( void );

// Implementation
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual ~CMainFrame();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
     
  BOOL CreateToolBar();
  BOOL CreateObjectBar();
  BOOL CreateStatusBar();
  BOOL CreateUnitLibraryBar();

  BOOL ViewUnitLibrary( void );
                         
protected:  // control bar embedded members
	CStatusBar      m_wndStatusBar;
  CDialogBar      m_wndMessageBar;
  CToolBar        m_wndToolBar;
  CToolBar        m_wndObjectBar;
  CToolBar        m_wndUnitLibraryBar;

  int             m_iUnitCurrentCascade; // 当前Unit折叠层选项的位置

// Generated message map functions
protected:
  //{{AFX_MSG(CMainFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnToggleInsert();
  afx_msg void OnUpdateInsert(CCmdUI* pCmdUI);
  afx_msg void OnUpdateObjectBar(CCmdUI* pCmdUI);
  afx_msg void OnObjectBrush();
  afx_msg void OnObjectDynamiclink();
  afx_msg void OnObjectErase();
  afx_msg void OnObjectLine();
  afx_msg void OnObjectOval();
  afx_msg void OnObjectPaint();
  afx_msg void OnObjectPen();
  afx_msg void OnObjectRect();
  afx_msg void OnObjectRoundrect();
  afx_msg void OnObjectSelect();
  afx_msg void OnObjectText();
  afx_msg void OnObjectGraph();
  afx_msg void OnObjectComponent();
  afx_msg void OnObjectPushbutton();
  afx_msg void OnObjectSlider();
  afx_msg void OnObjectGauge();
  afx_msg void OnDestroy();
  afx_msg void OnObjectKnob();
  afx_msg void OnObjectMeter();
  afx_msg void OnObjectFillbar();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateTime(CCmdUI* pCmdUI);
	afx_msg void OnObjectNumberinput();
	afx_msg void OnViewObjectbar();
	afx_msg void OnUpdateViewObjectbar(CCmdUI* pCmdUI);
	afx_msg void OnViewNext();
	afx_msg void OnViewPrev();
	afx_msg void OnViewUnitlibrary();
	afx_msg void OnUpdateViewUnitlibrary(CCmdUI* pCmdUI);
	afx_msg void OnViewMessagebar();
	afx_msg void OnUpdateViewMessagebar(CCmdUI* pCmdUI);
	afx_msg void OnObjectPicture();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif

