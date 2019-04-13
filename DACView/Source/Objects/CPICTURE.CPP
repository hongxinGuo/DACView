#include "stdafx.h"

#include "CUnitBase.h"

#include "cPicture.h"
//#include "DlgPic.h"

#include "globedef.h"

IMPLEMENT_SERIAL(CObjectPicture, CRectVirtual, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__;
#endif   

static CString s_strClassName = "Picture";

ParaName CObjectPicture::sm_ptrParaName[] = 
    { {""     , 0, 1},
    }; 
    
ULONG CObjectPicture::sm_ulArrayIndex[] = { 0 };

const ULONG CObjectPicture::sm_ulDoubleEnd = 0;
const ULONG CObjectPicture::sm_ulWordEnd   = 0;
const ULONG CObjectPicture::sm_ulStringEnd = 0;                

CObjectPicture::CObjectPicture(const CString& s, CRect r) : CRectVirtual(s, r) {
  m_fCreateMemoryDC = FALSE;

  if ( m_rectArea.Width() < 60 ) m_rectArea.right = m_rectArea.left + 60;
  if ( m_rectArea.Height() < 60 )  m_rectArea.bottom = m_rectArea.top + 60;

}

CObjectPicture::CObjectPicture( void ) : CRectVirtual( ) {
  m_fCreateMemoryDC = FALSE;
}

CObjectPicture::~CObjectPicture() {
}  

/////////////////////////////////////////////////////////////////////////////
// CObjectPicture diagnostics

#ifdef _DEBUG
void CObjectPicture::AssertValid() const
{
  CRectVirtual::AssertValid();
}

void CObjectPicture::Dump(CDumpContext& dc) const
{
  CRectVirtual::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectRect member function

void CObjectPicture::Serialize( CArchive& ar ) {
  CRectVirtual::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_strBitmapName;
  }
  else {
    switch ( ar.GetObjectSchema() ) {
    case 1 :
    case -1 :
      ar >> m_strBitmapName;
      break;
    default :
      ShowMessage(IDS_WARN_LOAD_OBJECT_VERSION, s_strClassName);
      break;
    }
  }
}

const CString& CObjectPicture::GetClassNameStr( void ) {
  return(s_strClassName);
}

BOOL CObjectPicture::ExectiveLink( void ) {
  // Dynamic Link Button
  return( TRUE );
}

void CObjectPicture::ToShowStatic( CDC * const pdc, CPoint  ) {
}  

void CObjectPicture::ToShowDynamic( CDC * const pdc, CPoint  ) {
}  

ParaName* CObjectPicture::GetParaNameAddress( void ) {
  return( sm_ptrParaName );
}  

char * CObjectPicture::GetParaName( ULONG index ) {
  ASSERT( index == 0 );
}

ULONG CObjectPicture::GetDynLinkType(ULONG ulIndex) {
  ASSERT(0);
  return(0);
}

void CObjectPicture::SelectParameter(ULONG ulType) {
}

ULONG CObjectPicture::GetIndex( ULONG ulIndex ) {
  return( 0 );
}

void CObjectPicture::SetProperty( void ) {
	CDlgSetMeterProperty CDlg;
  CDlg.SetData( m_strName, m_ulScanRate, m_strBitmapName );
  CDlg.DoModal();
  CDlg.GetData( m_strName, m_ulScanRate, m_strBitmapName );
}													 
  
BOOL CObjectPicture::CheckSelf( void ) {
	return( TRUE );
}
