// setprope.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjCompd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

void CDlgSetComponentProperty::SetData( COLORREF colorFore, COLORREF colorBk, CString Name, CString Bitmap, double eScanRate ) {
  m_clrForeGrd = colorFore;
  m_clrBkGrd = colorBk;
  m_ObjectName = Name;
  m_strBitmap = Bitmap;
  m_eScanRate = eScanRate / 1000;
}

void CDlgSetComponentProperty::GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, CString& Bitmap, INT32& ulScanRate) {
  colorFore = m_clrForeGrd;
  colorBk = m_clrBkGrd;
  Name = m_ObjectName;
  Bitmap = m_strBitmap;
	ulScanRate = (INT32)(m_eScanRate * 100);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetComponentProperty dialog

CDlgSetComponentProperty::CDlgSetComponentProperty(CWnd* pParent /*=nullptr*/)
  : CDlgObject(CDlgSetComponentProperty::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgSetComponentProperty)
  m_ObjectName = "";
  m_strBitmap = "";
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetComponentProperty::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgSetComponentProperty)
  DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
  DDX_Text(pDX, IDC_BITMAP, m_strBitmap);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 100000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetComponentProperty, CDlgObject)
  //{{AFX_MSG_MAP(CDlgSetComponentProperty)
  ON_WM_DRAWITEM()
  ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
  ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetComponentProperty message handlers

BOOL CDlgSetComponentProperty::OnInitDialog()
{
  CDialog::OnInitDialog();

  return( TRUE );
}

void CDlgSetComponentProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetComponentProperty::OnClickedButtonbackcolor()
{
  // TODO: Add your control notification handler code here
  m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetComponentProperty::OnClickedButtonforecolor()
{
  // TODO: Add your control notification handler code here
  m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

