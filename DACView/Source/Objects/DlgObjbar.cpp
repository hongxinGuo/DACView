// setFillBar.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetFillBarProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, 
																	COLORREF clrBar, double eScanRate, BOOL fMask,	BOOL fShowRange,
																	double eHiLimit, double eLoLimit, BOOL fVertical ) {
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_clrBar = clrBar;
	m_eScanRate = eScanRate / 1000;
	m_eHiLimit = eHiLimit;
	m_eLoLimit = eLoLimit;
	m_fVertical = fVertical;
	m_fMoveMark = fMask;
	m_fShowRange = fShowRange;
}

void CDlgSetFillBarProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, 
																	 COLORREF& clrBar, INT32& ulScanRate, bool& fMask, bool& fShowRange,
																	 double& eHiLimit, double& eLoLimit, bool& fVertical ) {
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	clrBar = m_clrBar; 
	ulScanRate = (INT32)(m_eScanRate * 1000);
	eHiLimit = m_eHiLimit;
	eLoLimit = m_eLoLimit;
	fVertical = m_fVertical;
	fMask = m_fMoveMark;
	fShowRange = m_fShowRange;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetFillBarProperty dialog

CDlgSetFillBarProperty::CDlgSetFillBarProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetFillBarProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetFillBarProperty)
	m_ObjectName = "";
	m_fVertical = FALSE;
	m_eHiLimit = 0.0;
	m_eLoLimit = 0.0;
	m_fShowRange = FALSE;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetFillBarProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetFillBarProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Check(pDX, IDC_CHECK_VERTICAL, m_fVertical);
	DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
	DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
	DDX_Check(pDX, IDC_CHECK_SHOWRANGE, m_fShowRange);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetFillBarProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetFillBarProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_BAR_COLOR, OnButtonBarColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetFillBarProperty message handlers

BOOL CDlgSetFillBarProperty::OnInitDialog()
{
	CDialog::OnInitDialog();
	
  SendDlgItemMessage(IDC_COMBO_TYPE, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)((LPSTR)"Fill Bar"));
	SendDlgItemMessage(IDC_COMBO_TYPE, CB_INSERTSTRING, (WPARAM)-1, (LPARAM)((LPSTR)"Mask"));
  if ( m_fMoveMark ) {	
  	SendDlgItemMessage(IDC_COMBO_TYPE, CB_SETCURSEL, 1, 0L);
	}
	else {
		SendDlgItemMessage(IDC_COMBO_TYPE, CB_SETCURSEL, 0, 0L);
	}
	return( TRUE );
}

void CDlgSetFillBarProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
{
	// TODO: Add your message handler code here and/or call default
	switch ( lpDraw->CtlType ) {
	case ODT_BUTTON :
		switch ( nIDCtl ) {
		case IDC_BUTTONFORECOLOR :
			DrawColor( lpDraw, m_clrForeGrd);
      break;
		case IDC_BUTTONBACKCOLOR : 
  		DrawColor( lpDraw, m_clrBkGrd );
			break;
		case IDC_BUTTON_BAR_COLOR : 
  		DrawColor( lpDraw, m_clrBar );
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

void CDlgSetFillBarProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetFillBarProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}


void CDlgSetFillBarProperty::OnButtonBarColor() 
{
	// TODO: Add your control notification handler code here
	m_clrBar = ChooseColorRef( m_clrBar );
  Invalidate();
}

void CDlgSetFillBarProperty::OnSelchangeComboType() 
{
	// TODO: Add your control notification handler code here
	int j = SendDlgItemMessage(IDC_COMBO_TYPE, CB_GETCURSEL, 0, 0L);
	if ( j == 0 ) {
		m_fMoveMark = FALSE;
	}
	else {
		m_fMoveMark = TRUE;
	}
}
