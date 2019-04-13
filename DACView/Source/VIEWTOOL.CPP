// viewtool.cpp : implementation file
//

#include "stdafx.h"
#include "DacView.h"
#include "viewtool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDacviewToolBar

CDacviewToolBar::CDacviewToolBar()
{
}

CDacviewToolBar::~CDacviewToolBar()
{
}

BOOL CDacviewToolBar::Create(CWnd* pOwnerWnd, DWORD dwStyle, UINT uID)
{
	ASSERT(pOwnerWnd != NULL);
	return(CToolBar::Create(pOwnerWnd, dwStyle, uID));
}


BEGIN_MESSAGE_MAP(CDacviewToolBar, CToolBar)
	//{{AFX_MSG_MAP(CDacviewToolBar)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDacviewToolBar message handlers

void CDacviewToolBar::OnSysCommand(UINT nID, LPARAM lParam) {
	if ( (nID & 0Xfff0) == SC_CLOSE ) {
		return;
	}
	CToolBar::OnSysCommand(nID, lParam);
}
