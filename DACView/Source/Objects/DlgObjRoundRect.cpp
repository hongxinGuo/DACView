// setprope.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjRoundRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetRoundRectProperty::SetData( COLORREF colorFore, COLORREF colorBk, 
																				CString Name, INT32 Round, double eScanRate ) {
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_ulRound = Round;
	m_eScanRate = eScanRate / 1000;
}

void CDlgSetRoundRectProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, 
																				CString &Name, INT32& Round, INT32& ulScanRate) {
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	Round = m_ulRound;
	ulScanRate = (INT32)(m_eScanRate * 1000);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetRoundRectProperty dialog

CDlgSetRoundRectProperty::CDlgSetRoundRectProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetRoundRectProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetRoundRectProperty)
	m_ObjectName = "";
	m_ulRound = 0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetRoundRectProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetRoundRectProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_EDIT_ROUND_LENGTH, m_ulRound);
	DDV_MinMaxDWord(pDX, m_ulRound, 0, 500);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetRoundRectProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetRoundRectProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetRoundRectProperty message handlers

BOOL CDlgSetRoundRectProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	return( TRUE );
}

void CDlgSetRoundRectProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetRoundRectProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetRoundRectProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

