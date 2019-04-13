/////////////////////////////////////////////////////////////////////////////
// CSQIFileFrame frame with splitter
#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSQIFileFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CSQIFileFrame)
protected:
	CSQIFileFrame();			// protected constructor used by dynamic creation

// Attributes
protected:
	CSplitterWnd	m_wndSplitter;
public:

// Operations
public:

// Implementation
public:
	virtual ~CSQIFileFrame();
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	// Generated message map functions
	//{{AFX_MSG(CSQIFileFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

