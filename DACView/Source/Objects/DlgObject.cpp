// dlgobject.cpp : implementation file
//


#include "stdafx.h"
#include "..\\resource.h"
#include "dlgObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObject dialog


CDlgObject::CDlgObject(UINT IDD, CWnd* pParent /*=nullptr*/)
        : CDialog(IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgObject)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  m_ulTimerId = 0;
  m_fDynUpdate = FALSE;
}


void CDlgObject::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgObject)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObject, CDialog)
  //{{AFX_MSG_MAP(CDlgObject)
  ON_WM_CREATE()
  ON_WM_DESTROY()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgObject message handlers

int CDlgObject::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;
  
  // TODO: Add your specialized creation code here
  m_ulTimerId = SetTimer(2, 1000, nullptr);

  return 0;
}

void CDlgObject::OnDestroy() 
{
  CDialog::OnDestroy();
  
  // TODO: Add your message handler code here
  KillTimer( m_ulTimerId );
}

void CDlgObject::DrawColor( LPDRAWITEMSTRUCT lpDraw, COLORREF clr ) {
	HBRUSH hb, hbOld;
  
  hb = CreateSolidBrush( clr );
	hbOld = (HBRUSH)SelectObject( lpDraw->hDC, hb );
  FillRect(lpDraw->hDC, &lpDraw->rcItem, hb);
  SelectObject(lpDraw->hDC, hbOld);
  DeleteObject( hb );
}

COLORREF CDlgObject::ChooseColorRef( COLORREF clr ) {
  CHOOSECOLOR cc; 
	COLORREF aclrCust[16];
	
	for ( int i = 0; i < 16; i++ ) {
		aclrCust[i] = RGB(255, 255, 255);
	}
	memset( &cc, 0, sizeof(CHOOSECOLOR));
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = nullptr;
	cc.rgbResult = clr;
	cc.lpCustColors = aclrCust;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	if ( ChooseColor(&cc) )	{
		return ( cc.rgbResult );
	}
  else return ( clr );
}

