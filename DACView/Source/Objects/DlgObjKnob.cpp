// setDlgKnob.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjKnob.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetKnobProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate,
																	COLORREF colorNormal, COLORREF colorWarn, 
																	COLORREF colorError, COLORREF colorNoddle,
																	double eHiLimit, double eLoLimit) {
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_clrNormal = colorNormal;
	m_clrWarn = colorWarn;
	m_clrError = colorError;
	m_clrNoddle = colorNoddle;
	m_ObjectName = Name;
	m_eScanRate = eScanRate / 1000;
	m_eHiLimit = eHiLimit;
	m_eLoLimit = eLoLimit;
}

void CDlgSetKnobProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate,
																	COLORREF& colorNormal, COLORREF& colorWarn, 
																	COLORREF& colorError, COLORREF& colorNoddle,
																	double& eHiLimit, double& eLoLimit) {
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	colorNormal = m_clrNormal;
	colorWarn = m_clrWarn;
	colorError = m_clrError;
	colorNoddle = m_clrNoddle;
	Name = m_ObjectName;
	ulScanRate = (INT32)(m_eScanRate * 1000);
	eHiLimit = m_eHiLimit;
	eLoLimit = m_eLoLimit;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetKnobProperty dialog

CDlgSetKnobProperty::CDlgSetKnobProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetKnobProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetKnobProperty)
	m_ObjectName = "";
	m_eHiLimit = 0.0;
	m_eLoLimit = 0.0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetKnobProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetKnobProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
	DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetKnobProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetKnobProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	ON_BN_CLICKED(IDC_BUTTONNORMAL, OnButtonnormal)
	ON_BN_CLICKED(IDC_BUTTONERROR, OnButtonerror)
	ON_BN_CLICKED(IDC_BUTTONWARN, OnButtonwarn)
	ON_BN_CLICKED(IDC_BUTTONNODDLE, OnButtonnoddle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetKnobProperty message handlers

BOOL CDlgSetKnobProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	return( TRUE );
}

void CDlgSetKnobProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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
		case IDC_BUTTONNORMAL :
			DrawColor( lpDraw, m_clrNormal );
			break;
		case IDC_BUTTONWARN :
			DrawColor( lpDraw, m_clrWarn );
			break;
		case IDC_BUTTONERROR :
			DrawColor( lpDraw, m_clrError );
			break;
		case IDC_BUTTONNODDLE :
			DrawColor( lpDraw, m_clrNoddle );
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

void CDlgSetKnobProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetKnobProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
  m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}


void CDlgSetKnobProperty::OnButtonnormal() 
{
	// TODO: Add your control notification handler code here
  m_clrNormal = ChooseColorRef( m_clrNormal );
	Invalidate();
}

void CDlgSetKnobProperty::OnButtonerror() 
{
	// TODO: Add your control notification handler code here
	m_clrError = ChooseColorRef( m_clrError );
	Invalidate();	
}

void CDlgSetKnobProperty::OnButtonwarn() 
{
	// TODO: Add your control notification handler code here
	m_clrWarn = ChooseColorRef( m_clrWarn );
	Invalidate();	
}

void CDlgSetKnobProperty::OnButtonnoddle() 
{
	// TODO: Add your control notification handler code here
	m_clrNoddle = ChooseColorRef( m_clrNoddle );
	Invalidate();
}
