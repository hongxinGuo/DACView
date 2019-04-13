// setDlgMeter.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjMeter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

void CDlgSetMeterProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate,
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

void CDlgSetMeterProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate,
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
// CDlgSetMeterProperty dialog

CDlgSetMeterProperty::CDlgSetMeterProperty(CWnd* pParent /*=nullptr*/)
	: CDialog(CDlgSetMeterProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetMeterProperty)
	m_ObjectName = "";
	m_eHiLimit = 0.0;
	m_eLoLimit = 0.0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetMeterProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetMeterProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
	DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetMeterProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgSetMeterProperty)
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
// CDlgSetMeterProperty message handlers

BOOL CDlgSetMeterProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	return( TRUE );
}

void CDlgSetMeterProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
{
	// TODO: Add your message handler code here and/or call default
	HBRUSH hb, hbOld;
  		
	switch ( lpDraw->CtlType ) {
	case ODT_BUTTON :
		switch ( nIDCtl ) {
		case IDC_BUTTONFORECOLOR :
			hb = CreateSolidBrush( m_clrForeGrd );
			hbOld = (HBRUSH)SelectObject( lpDraw->hDC, hb );
  		FillRect(lpDraw->hDC, &lpDraw->rcItem, hb);   // Yes, to draw myself
  		SelectObject(lpDraw->hDC, hbOld);
  		DeleteObject( hb );
			break;
		case IDC_BUTTONBACKCOLOR : 
  		hb = CreateSolidBrush( m_clrBkGrd );
  		hbOld = (HBRUSH)SelectObject( lpDraw->hDC, hb );
  		FillRect(lpDraw->hDC, &lpDraw->rcItem, hb);  // Yes, to draw myself		
			SelectObject(lpDraw->hDC, hbOld);
  		DeleteObject( hb );
			break;
		case IDC_BUTTONNORMAL :
			hb = CreateSolidBrush( m_clrNormal );
  		hbOld = (HBRUSH)SelectObject( lpDraw->hDC, hb );
  		FillRect(lpDraw->hDC, &lpDraw->rcItem, hb);  // Yes, to draw myself		
			SelectObject(lpDraw->hDC, hbOld);
  		DeleteObject( hb );
			break;
		case IDC_BUTTONWARN :
			hb = CreateSolidBrush( m_clrWarn );
  		hbOld = (HBRUSH)SelectObject( lpDraw->hDC, hb );
  		FillRect(lpDraw->hDC, &lpDraw->rcItem, hb);  // Yes, to draw myself		
			SelectObject(lpDraw->hDC, hbOld);
  		DeleteObject( hb );
			break;
		case IDC_BUTTONERROR :
			hb = CreateSolidBrush( m_clrError );
  		hbOld = (HBRUSH)SelectObject( lpDraw->hDC, hb );
  		FillRect(lpDraw->hDC, &lpDraw->rcItem, hb);  // Yes, to draw myself		
			SelectObject(lpDraw->hDC, hbOld);
  		DeleteObject( hb );
			break;
		case IDC_BUTTONNODDLE :
			hb = CreateSolidBrush( m_clrNoddle );
  		hbOld = (HBRUSH)SelectObject( lpDraw->hDC, hb );
  		FillRect(lpDraw->hDC, &lpDraw->rcItem, hb);  // Yes, to draw myself		
			SelectObject(lpDraw->hDC, hbOld);
  		DeleteObject( hb );
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

void CDlgSetMeterProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	CHOOSECOLOR cc; 
	COLORREF aclrCust[16];
	
	for ( int i = 0; i < 16; i++ ) {
		aclrCust[i] = RGB(255, 255, 255);
	}
	memset( &cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = nullptr;
	cc.rgbResult = m_clrBkGrd;
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_FULLOPEN;
	if ( ChooseColor(&cc) )	{
		m_clrBkGrd = cc.rgbResult;
	}	
	Invalidate();
}

void CDlgSetMeterProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	CHOOSECOLOR cc; 
	COLORREF aclrCust[16];
	
	for ( int i = 0; i < 16; i++ ) {
		aclrCust[i] = RGB(255, 255, 255);
	}
	memset( &cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = nullptr;
	cc.rgbResult = m_clrForeGrd;
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_FULLOPEN;
	if ( ChooseColor(&cc) )	{
		m_clrForeGrd = cc.rgbResult;
	}	
	Invalidate();
}


void CDlgSetMeterProperty::OnButtonnormal() 
{
	// TODO: Add your control notification handler code here
	CHOOSECOLOR cc; 
	COLORREF aclrCust[16];
	
	for ( int i = 0; i < 16; i++ ) {
		aclrCust[i] = RGB(255, 255, 255);
	}
	memset( &cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = nullptr;
	cc.rgbResult = m_clrNormal;
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_FULLOPEN;
	if ( ChooseColor(&cc) )	{
		m_clrNormal = cc.rgbResult;
	}	
	Invalidate();
}

void CDlgSetMeterProperty::OnButtonerror() 
{
	// TODO: Add your control notification handler code here
	CHOOSECOLOR cc; 
	COLORREF aclrCust[16];
	
	for ( int i = 0; i < 16; i++ ) {
		aclrCust[i] = RGB(255, 255, 255);
	}
	memset( &cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = nullptr;
	cc.rgbResult = m_clrError;
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_FULLOPEN;
	if ( ChooseColor(&cc) )	{
		m_clrError = cc.rgbResult;
	}	
	Invalidate();
}

void CDlgSetMeterProperty::OnButtonwarn() 
{
	// TODO: Add your control notification handler code here
	CHOOSECOLOR cc; 
	COLORREF aclrCust[16];
	
	for ( int i = 0; i < 16; i++ ) {
		aclrCust[i] = RGB(255, 255, 255);
	}
	memset( &cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = nullptr;
	cc.rgbResult = m_clrWarn;
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_FULLOPEN;
	if ( ChooseColor(&cc) )	{
		m_clrWarn = cc.rgbResult;
	}	
	Invalidate();
}

void CDlgSetMeterProperty::OnButtonnoddle() 
{
	// TODO: Add your control notification handler code here
	CHOOSECOLOR cc; 
	COLORREF aclrCust[16];
	
	for ( int i = 0; i < 16; i++ ) {
		aclrCust[i] = RGB(255, 255, 255);
	}
	memset( &cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = nullptr;
	cc.rgbResult = m_clrNoddle;
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_FULLOPEN;
	if ( ChooseColor(&cc) )	{
		m_clrNoddle = cc.rgbResult;
	}	
	Invalidate();
}
