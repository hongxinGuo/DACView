////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// Viewfram.cpp : implementation file
//
//
//  处理SQI文件时调用的主框架
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "dacview.h"
#include "SQIFileFrame.h"

#include "globedef.h"

#include "SQIUnitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSQIFileFrame

IMPLEMENT_DYNCREATE(CSQIFileFrame, CMDIChildWnd)

CSQIFileFrame::CSQIFileFrame()
{
}

CSQIFileFrame::~CSQIFileFrame()
{
}

BOOL CSQIFileFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		ShowMessage(ID_ERROR_SHOWVIEW_CANNOT_CREATE_VIEW);
		return FALSE;
	}

	// add the first splitter pane - the default view(CSQIObjView) in column 0
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(800, 1000), pContext))
	{
		ShowMessage(ID_ERROR_SHOWVIEW_CANNOT_CREATE_VIEW);
		return FALSE;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CSQIUnitView), CSize(800, 1000), pContext))
	{
		ShowMessage(ID_ERROR_SHOWVIEW_CANNOT_CREATE_VIEW);
		return FALSE;
	}

	// activate the input view
	SetActiveView((CView*)m_wndSplitter.GetPane(0,1));	
	return TRUE;
}

BEGIN_MESSAGE_MAP(CSQIFileFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CSQIFileFrame)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

