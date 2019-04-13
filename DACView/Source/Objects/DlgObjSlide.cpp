// setSlider.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjSlide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetSliderProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate,
																	double eHiLimit, double eLoLimit, bool fVertical ) {
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_eScanRate = eScanRate / 1000;
	m_eHiLimit = eHiLimit;
	m_eLoLimit = eLoLimit;
	m_fVertical = fVertical;
}

void CDlgSetSliderProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate,
																	double& eHiLimit, double& eLoLimit, bool& fVertical ) {
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	ulScanRate = (INT32)(m_eScanRate * 1000);
	eHiLimit = m_eHiLimit;
	eLoLimit = m_eLoLimit;
	fVertical = m_fVertical;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetSliderProperty dialog

CDlgSetSliderProperty::CDlgSetSliderProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetSliderProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetSliderProperty)
	m_ObjectName = "";
	m_fVertical = FALSE;
	m_eHiLimit = 0.0;
	m_eLoLimit = 0.0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetSliderProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetSliderProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Check(pDX, IDC_CHECK_VERTICAL, m_fVertical);
	DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
	DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 100000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetSliderProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetSliderProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetSliderProperty message handlers

BOOL CDlgSetSliderProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	return( TRUE );
}

void CDlgSetSliderProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetSliderProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetSliderProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

