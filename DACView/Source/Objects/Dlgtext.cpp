// setTextprope.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetTextProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, CString ShowName, double eScanRate ) {
  m_clrForeGrd = colorFore;
  m_clrBkGrd = colorBk;
  m_ObjectName = Name;
  m_strShowName = ShowName;
  m_eScanRate = eScanRate / 1000;
}

void CDlgSetTextProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, CString& ShowName, INT32& ulScanRate) {
  colorFore = m_clrForeGrd;
  colorBk = m_clrBkGrd;
  Name = m_ObjectName;
  ShowName = m_strShowName;
	ulScanRate = (INT32)(m_eScanRate * 1000);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetTextProperty dialog

CDlgSetTextProperty::CDlgSetTextProperty(CWnd* pParent /*=nullptr*/)
  : CDlgObject(CDlgSetTextProperty::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgSetTextProperty)
  m_ObjectName = "";
  m_strShowName = "";
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetTextProperty::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgSetTextProperty)
  DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
  DDX_Text(pDX, IDC_EDIT_SHOW_NAME, m_strShowName);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetTextProperty, CDlgObject)
  //{{AFX_MSG_MAP(CDlgSetTextProperty)
  ON_WM_DRAWITEM()
  ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
  ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetTextProperty message handlers

BOOL CDlgSetTextProperty::OnInitDialog()
{
  CDialog::OnInitDialog();

  return( TRUE );
}

void CDlgSetTextProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetTextProperty::OnClickedButtonbackcolor()
{
  // TODO: Add your control notification handler code here
  m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetTextProperty::OnClickedButtonforecolor()
{
  // TODO: Add your control notification handler code here
  m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

