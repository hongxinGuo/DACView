// viewtool.h : header file
//

#ifndef _DACVIEW_TOOL_BAR_H_
#define _DACVIEW_TOOL_BAR_H_

/////////////////////////////////////////////////////////////////////////////
// CDacviewToolBar window

class CDacviewToolBar : public CToolBar
{
// Construction
public:
	CDacviewToolBar();
	BOOL Create(CWnd* pOwnerWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT uID = AFX_IDW_TOOLBAR );
	
// Attributes
public:
	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDacviewToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDacviewToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDacviewToolBar)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif
