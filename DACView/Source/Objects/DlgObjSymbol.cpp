// DlgSymbol.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjSymbol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetSymbolProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate, bool transparent ) {
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_eScanRate = eScanRate / 1000;
  m_fTransparent = transparent;
}

void CDlgSetSymbolProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate, bool& transparent ) {
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	ulScanRate = (INT32)(m_eScanRate * 1000);
  transparent = m_fTransparent;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetSymbolProperty dialog

CDlgSetSymbolProperty::CDlgSetSymbolProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetSymbolProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetSymbolProperty)
	m_ObjectName = "";
	m_eScanRate = 0.0;
	m_fTransparent = FALSE;
	//}}AFX_DATA_INIT
}

void CDlgSetSymbolProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetSymbolProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	DDX_Check(pDX, IDC_CHECK_TRANSPARENT, m_fTransparent);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetSymbolProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetSymbolProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetSymbolProperty message handlers

BOOL CDlgSetSymbolProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	return( TRUE );
}

void CDlgSetSymbolProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
{
	// TODO: Add your message handler code here and/or call default
	switch ( lpDraw->CtlType ) {
	case ODT_BUTTON :
		switch ( nIDCtl ) {
		case IDC_BUTTONFORECOLOR :
			DrawColor( lpDraw, m_clrForeGrd );
			break;
		case IDC_BUTTONBACKCOLOR : 
  		DrawColor( lpDraw, m_clrBkGrd );
			break;
		default :
			break;
		} // switch CtrlID
		break;
	default :
	  break;
	} // 
	
	CDialog::OnDrawItem(nIDCtl, lpDraw);
}

void CDlgSetSymbolProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetSymbolProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

