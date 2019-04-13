// setprope.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetGraphProperty::SetData( BOOL fDisableSelectType, INT32 ulDataLength, INT32 ulInputType,
                                   COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate ) {
	m_fDisableSelectType = fDisableSelectType;
	m_ulDataLength = ulDataLength;
  m_ulInputDataType = ulInputType;
	m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_eScanRate = eScanRate / 1000;
}

void CDlgSetGraphProperty::GetData( INT32& ulDataLength, INT32& ulInputType, 
                                   COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate) {
	ulDataLength = m_ulDataLength;
  ulInputType = m_ulInputDataType;
	colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	ulScanRate = (INT32)(m_eScanRate * 1000);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGraphProperty dialog

CDlgSetGraphProperty::CDlgSetGraphProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetGraphProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetGraphProperty)
	m_ObjectName = "";
	m_ulDataLength = 0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetGraphProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetGraphProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_BUFFER_LENGTH, m_ulDataLength);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 1000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetGraphProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetGraphProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_TYPE, OnSelchangeComboInputType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGraphProperty message handlers

BOOL CDlgSetGraphProperty::OnInitDialog()
{
	CDialog::OnInitDialog();
  int a = 0;

  switch ( m_ulInputDataType ) {
  case tBOOL :
    a = 0;
    break;
  case tWORD :
    a = 1;
    break;
  case tDOUBLE :
    a = 2;
    break;
  default :
    ASSERT( 0 );
  }
  SendDlgItemMessage(IDC_COMBO_INPUT_TYPE, CB_SETCURSEL, a, 0L);
	if ( m_fDisableSelectType ) GetDlgItem(IDC_COMBO_INPUT_TYPE)->EnableWindow( FALSE );

  return( TRUE );
}

void CDlgSetGraphProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetGraphProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetGraphProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

void CDlgSetGraphProperty::OnSelchangeComboInputType() 
{
	// TODO: Add your control notification handler code here
  int j = SendDlgItemMessage(IDC_COMBO_INPUT_TYPE, CB_GETCURSEL, 0, 0L);
  switch ( j ) {
  case 0 :
    m_ulInputDataType = tBOOL;
    break;
  case 1 :
    m_ulInputDataType = tWORD;
    break;
  case 2 :
    m_ulInputDataType = tDOUBLE;
    break;
  default :
    ASSERT( 0 );
  }
}
