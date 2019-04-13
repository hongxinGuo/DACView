// showfram.cpp : implementation file
//

#include "stdafx.h"
#include "dacview.h"
#include "FBDFileFrame.h"

#include "globedef.h"

// include the declaration of gl_ulNumberOfShowViewRun
#include "mainfrm.h"

#include "FBDUnitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFBDFileFrame

IMPLEMENT_DYNCREATE(CFBDFileFrame, CMDIChildWnd)

CFBDFileFrame::CFBDFileFrame()
{
}

CFBDFileFrame::~CFBDFileFrame()
{
}

BOOL CFBDFileFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		ShowMessage(ID_ERROR_SHOWVIEW_CANNOT_CREATE_VIEW);
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(800, 1000), pContext))
	{
		ShowMessage(ID_ERROR_SHOWVIEW_CANNOT_CREATE_VIEW);
		return FALSE;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CFBDUnitView), CSize(800, 1000), pContext))
	{
		ShowMessage(ID_ERROR_SHOWVIEW_CANNOT_CREATE_VIEW);
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));	
	return TRUE;
}

BEGIN_MESSAGE_MAP(CFBDFileFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFBDFileFrame)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFBDFileFrame message handlers

void CFBDFileFrame::OnSysCommand(UINT nID, LPARAM lParam) {
	CFBDFileDoc * pDoc = (CFBDFileDoc *)GetActiveDocument();

	ASSERT( pDoc != nullptr );
	if ( pDoc->fInRunShowView() ) {
		if ( (nID & 0xfff0) == SC_CLOSE ) { // 当FBD文件运行时，不允许停止程序。
			ShowMessage(ID_ERROR_CANCEL_SHOWVIEW_FILE);
			return;
		}
	}

	CMDIChildWnd::OnSysCommand(nID, lParam);
}

