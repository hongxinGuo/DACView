// setButtonProperty.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetButtonProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, CString strOn, CString strOff, double ulScanRate ) {
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_strOn = strOn;
	m_strOff = strOff;
	m_eScanRate = ulScanRate / 1000;
}

void CDlgSetButtonProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, CString& strOn, CString& strOff, INT32& ulScanRate) {
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	strOn = m_strOn;
	strOff = m_strOff;
	ulScanRate = (INT32)(m_eScanRate * 1000);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetButtonProperty dialog

CDlgSetButtonProperty::CDlgSetButtonProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetButtonProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetButtonProperty)
	m_ObjectName = "";
	m_strOff = "";
	m_strOn = "";
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetButtonProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetButtonProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_EDIT_STR_OFF, m_strOff);
	DDX_Text(pDX, IDC_EDIT_STR_ON, m_strOn);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetButtonProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetButtonProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetButtonProperty message handlers

BOOL CDlgSetButtonProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	return( TRUE );
}

void CDlgSetButtonProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetButtonProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetButtonProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

