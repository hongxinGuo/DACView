// Dacview.h : main header file for the Dacview application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "..\\resource.h"		// main symbols    

#define WM_IDLEACTIVATE WM_USER+1  

class CObjectBase;

void SetStatusBarMessage( const CString& str );
void SetMousePosition( CPoint ptDevice );
void SetObjectRect( CObjectBase * pcobj );

/////////////////////////////////////////////////////////////////////////////
// CDacviewApp:
// See Dacview.cpp for the implementation of this class
//

class CDacviewApp : public CWinApp
{
public:
	CDacviewApp();

// Overrides
	virtual BOOL InitInstance();
  virtual int  ExitInstance();
  virtual BOOL OnIdle(LONG lCount);

  bool CDacviewApp::LoadCursors(HCURSOR * pDL, HCURSOR * pNoDrag, HCURSOR * pCpt);

private:
	static CString sm_strWndClassName;
#ifdef _DEBUG
  CMemoryState   m_CMemOld, m_CMemNew;
#endif

public:
  HCURSOR m_hCursorDynamicLink, m_hCursorNoDrag, m_hCursorComponent;

// Implementation

	//{{AFX_MSG(CDacviewApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// VB-Event extern declarations

//{{AFX_VBX_REGISTER()
//}}AFX_VBX_REGISTER

/////////////////////////////////////////////////////////////////////////////

