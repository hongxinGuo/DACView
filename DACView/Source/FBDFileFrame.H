#ifndef __FBD_FILE_FRAME_H__
#define __FBD_FILE_FRAME_H__

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFBDFileFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFBDFileFrame)
protected:
	CFBDFileFrame();			// protected constructor used by dynamic creation

// Attributes
protected:
	CSplitterWnd	m_wndSplitter;
public:

// Operations
public:

// Implementation
public:
	virtual ~CFBDFileFrame();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// Generated message map functions
	//{{AFX_MSG(CFBDFileFrame)
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

