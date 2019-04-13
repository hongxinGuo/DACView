// setprope.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjGauge.h"
#include "cObjGauge.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

static struct {	int LinkType;
								char * StringName; } LinkType[] =
							{ { TY_GAUGE,   						"Gauge" },
								{ TY_FILLVERTICAL,   			"Fill Vertical" },					
								{ TY_FILLHORIZON,					"Fill Horizon" },
								{ TY_END, 				"", },
							};
			
void CDlgSetGaugeProperty::SetData( bool fSaveFlag, INT32 ulAngle, INT32 ulType, 
													 COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate ) {
	m_ulAngle = ulAngle;
	m_ulType = ulType;
	m_fSaveToFile	= fSaveFlag;
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_eScanRate = eScanRate / 1000;
}

void CDlgSetGaugeProperty::GetData( bool &fSaveFlag, INT32 &ulAngle, INT32 &ulType, 
													 COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate ) {
	ulAngle = m_ulAngle;
	ulType = m_ulType;
	fSaveFlag = m_fSaveToFile;
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	ulScanRate = (INT32)(m_eScanRate * 1000);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGaugeProperty dialog

CDlgSetGaugeProperty::CDlgSetGaugeProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetGaugeProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetGaugeProperty)
	m_ObjectName = "";
	m_ulAngle = 0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetGaugeProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetGaugeProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_ANGLE, m_ulAngle);
	DDV_MinMaxDWord(pDX, m_ulAngle, 0, 360);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetGaugeProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetGaugeProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	ON_BN_CLICKED(IDC_CHECK_RECORD, OnCheckRecord)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGaugeProperty message handlers

BOOL CDlgSetGaugeProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_CHECK_RECORD)->EnableWindow( TRUE );
	SendDlgItemMessage(IDC_CHECK_RECORD, BM_SETCHECK, (WPARAM)m_fSaveToFile, (LPARAM)0);

	int j, i = 0;
	while ( LinkType[i].LinkType != TY_END ) {
		j = SendDlgItemMessage(IDC_COMBO_TYPE, CB_INSERTSTRING, (WPARAM)-1,                    
											 		(LPARAM)((LPSTR)LinkType[i].StringName));
		i++;
	}
  SendDlgItemMessage(IDC_COMBO_TYPE, CB_SETCURSEL, m_ulType, 0L);
  
	return( TRUE );
}

void CDlgSetGaugeProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetGaugeProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetGaugeProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}


void CDlgSetGaugeProperty::OnCheckRecord() 
{
	// TODO: Add your control notification handler code here
	m_fSaveToFile = !m_fSaveToFile;
}

void CDlgSetGaugeProperty::OnSelchangeComboType() 
{
	// TODO: Add your control notification handler code here
	m_ulType = (ULONG)SendDlgItemMessage(IDC_COMBO_TYPE, CB_GETCURSEL, 0, 0L);
}
