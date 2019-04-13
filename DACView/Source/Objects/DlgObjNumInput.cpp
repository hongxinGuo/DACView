// setprope.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgObjNumInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDlgSetNumberInputProperty::SetData( CString FontName, INT32 lHeight, bool fBold,
                                        bool fItalic, bool fUnderline, bool fStrikeOut,COLORREF colorFore, COLORREF colorBk, 
																				CString Name, double eScanRate ) {
	m_strFontName = FontName;
  m_lHeight = lHeight;
  m_fBold = fBold;
  m_fItalic = fItalic;
  m_fUnderline = fUnderline;
  m_fStrikeOut = fStrikeOut;

  m_clrForeGrd = colorFore;
	m_clrBkGrd = colorBk;
	m_ObjectName = Name;
	m_eScanRate = eScanRate / 1000;
}

void CDlgSetNumberInputProperty::GetData( CString& FontName, INT32& lHeight, bool& fBold,
                                        bool& fItalic, bool& fUnderline, bool& fStrikeOut, COLORREF &colorFore, COLORREF &colorBk, 
																				CString &Name, INT32& ulScanRate) {
	FontName = m_strFontName;
  lHeight = m_lHeight;
  fBold = m_fBold;
  fItalic = m_fItalic;
  fUnderline = m_fUnderline;
  fStrikeOut = m_fStrikeOut;
  
  colorFore = m_clrForeGrd;
	colorBk = m_clrBkGrd;
	Name = m_ObjectName;
	ulScanRate = (INT32)(m_eScanRate * 1000);
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSetNumberInputProperty dialog

CDlgSetNumberInputProperty::CDlgSetNumberInputProperty(CWnd* pParent /*=nullptr*/)
	: CDlgObject(CDlgSetNumberInputProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetNumberInputProperty)
	m_ObjectName = "";
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSetNumberInputProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetNumberInputProperty)
	DDX_Text(pDX, IDC_ObjectName, m_ObjectName);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSetNumberInputProperty, CDlgObject)
	//{{AFX_MSG_MAP(CDlgSetNumberInputProperty)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTONBACKCOLOR, OnClickedButtonbackcolor)
	ON_BN_CLICKED(IDC_BUTTONFORECOLOR, OnClickedButtonforecolor)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_FONT, OnButtonChooseFont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetNumberInputProperty message handlers

BOOL CDlgSetNumberInputProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	return( TRUE );
}

void CDlgSetNumberInputProperty::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDraw)
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

void CDlgSetNumberInputProperty::OnClickedButtonbackcolor()
{
	// TODO: Add your control notification handler code here
	m_clrBkGrd = ChooseColorRef( m_clrBkGrd );
  Invalidate();
}

void CDlgSetNumberInputProperty::OnClickedButtonforecolor()
{
	// TODO: Add your control notification handler code here
	m_clrForeGrd = ChooseColorRef( m_clrForeGrd );
  Invalidate();
}

void CDlgSetNumberInputProperty::OnButtonChooseFont() 
{
	// TODO: Add your control notification handler code here
	LOGFONT lf;
  CHOOSEFONT cf;

  strcpy(lf.lfFaceName, (LPCTSTR)m_strFontName);
  lf.lfHeight = m_lHeight;
  lf.lfWidth = 0;
  if ( m_fBold ) lf.lfWeight = 700;
  else lf.lfWeight = 400;
  lf.lfItalic = (BYTE)m_fItalic;
  lf.lfUnderline = (BYTE)m_fUnderline;
  lf.lfStrikeOut = (BYTE)m_fStrikeOut;
  lf.lfCharSet = OEM_CHARSET;
  lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
  lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
  lf.lfQuality = DEFAULT_QUALITY;
  lf.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

  cf.lStructSize = sizeof(CHOOSEFONT);
  cf.hwndOwner = nullptr;
  cf.hDC = nullptr;
  cf.lpLogFont = &lf;
  cf.iPointSize = 0;
  cf.Flags = CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT; 
  cf.rgbColors = m_clrForeGrd;

  cf.lpfnHook = nullptr;
  cf.lpTemplateName = nullptr;
  cf.hInstance = nullptr;
  cf.lpszStyle = nullptr;
  cf.nFontType = 0;
  cf.nSizeMin = 0;
  cf.nSizeMax = 0;
  
  if ( ChooseFont( &cf ) ) {
    m_strFontName = lf.lfFaceName;
    m_lHeight = lf.lfHeight;
    if ( lf.lfWeight == 700 ) m_fBold = TRUE;
    else m_fBold = FALSE;
    m_fItalic = lf.lfItalic;
    m_fUnderline = lf.lfUnderline;
    m_fStrikeOut = lf.lfStrikeOut;
    m_clrForeGrd = cf.rgbColors;
  }
}
