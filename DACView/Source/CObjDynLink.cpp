#include "stdafx.h"

#include "CUnitBase.h"
#include "CObjectBase.h"
#include "cObjDynLink.h"

IMPLEMENT_SERIAL(CObjectDynLink, CObjectPrimitive, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectDynLink::CObjectDynLink(CString Name)
  : CObjectPrimitive(Name) {
	m_pCUnit = nullptr;
  m_pCObject = nullptr;
	m_lScanRate = 100000000;
  m_lUnitIndex = m_lObjectIndex = -1;
  m_fDeleteMe = FALSE;
	m_lLinkMethod = 0;
}      

CObjectDynLink::CObjectDynLink( void ) : CObjectPrimitive() { 
	m_pCUnit = nullptr;
  m_pCObject = nullptr;
  m_strName.Empty();
  m_lScanRate = 100000000;
  m_lUnitIndex = m_lObjectIndex = -1;
  m_fDeleteMe = FALSE;
	m_lLinkMethod = 0;
}


CObjectDynLink::~CObjectDynLink() {
} 

////////////////////////////////////////////////////////////////////////
//
//  直接存储单元指针即可，系统会自动完成其他的工作。
//
////////////////////////////////////////////////////////////////////////
void CObjectDynLink::Serialize( CArchive& ar ) {
  CObjectPrimitive::Serialize( ar );
  INT32 a;
  CUnitBase * punit;
  
  if( ar.IsStoring() ) {
    ar << m_pCUnit.get() << m_lUnitIndex << m_lObjectIndex
       << m_lLinkMethod << (INT32)m_fUnitToObject << m_strComment;
  }
  else {
    ar >> punit >> m_lUnitIndex >> m_lObjectIndex 
       >> m_lLinkMethod >> a >> m_strComment;
    m_fUnitToObject = (BOOL)a;
    m_pCUnit.reset(punit);
  } 
}

///////////////////////////////////////////////////////////////////////////
//
// Copy()
//
////////////////////////////////////////////////////////////////////////////
void CObjectDynLink::Copy( CObjectDynLink * pDynLink ) {
	m_strUnitName			= pDynLink->m_strUnitName;
	m_lUnitIndex			= pDynLink->m_lUnitIndex;
	m_lObjectIndex		= pDynLink->m_lObjectIndex;
	m_lLinkMethod			= pDynLink->m_lLinkMethod;
	m_fUnitToObject		= pDynLink->m_fUnitToObject;
	m_strComment			= pDynLink->m_strComment;
	m_pCUnit					= pDynLink->m_pCUnit;
	m_pCObject				= pDynLink->m_pCObject;
	m_fDeleteMe				= pDynLink->m_fDeleteMe;
}

/////////////////////////////////////////////////////////////////////////////
// CObjectDynLink diagnostics

#ifdef _DEBUG
void CObjectDynLink::AssertValid() const
{
  CObjectPrimitive::AssertValid();
}

void CObjectDynLink::Dump(CDumpContext& dc) const
{
  CObjectPrimitive::Dump(dc);
  dc << "m_strUnitName = " << m_pCUnit->GetName();
  dc << "\nm_lUnitIndex = " << m_pCUnit->GetParaName(m_lUnitIndex);
  dc << "\nm_lObjectIndex = " << m_pCObject->GetParaName(m_lObjectIndex);
  dc << "\nm_lLinkMethod = " << m_lLinkMethod << "\n";
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

CString CObjectDynLink::GetUnitName( void ) const {
  return( m_pCUnit->GetName() );
}

CUnitBasePtr CObjectDynLink::GetUnit( void ) const {
  return( m_pCUnit );
}

CObjectBase * CObjectDynLink::GetObject( void ) const {
  return( m_pCObject );
}
     
INT32 CObjectDynLink::GetUnitIndex( void ) const {
  return( m_lUnitIndex );
}     

INT32 CObjectDynLink::GetObjectIndex( void ) const {
  return( m_lObjectIndex );
}
     
ULONG CObjectDynLink::GetLinkMethod( void ) const {
  return( m_lLinkMethod );
}

ULONG CObjectDynLink::GetObjectDynLinkType( void ) const {
  return( m_pCObject->GetDynLinkType(m_lObjectIndex) );
}

ULONG CObjectDynLink::GetUnitDynLinkType(void) const {
  return(m_pCUnit->GetDynLinkType(m_lUnitIndex));
}

ULONG CObjectDynLink::GetUnitParaType(void) const {
  return(m_pCUnit->GetParaType(m_lUnitIndex));
}

bool  CObjectDynLink::IsUnitToObject( void ) const {
  return( m_fUnitToObject );
}

CString& CObjectDynLink::GetComment( void ) {
  return( m_strComment );
}

void CObjectDynLink::SetDeleteMeFlag( BOOL fFlag ) {
  m_fDeleteMe = fFlag;
}

bool CObjectDynLink::IsDeleteMe( void ) const {
  return( m_fDeleteMe );
}

void CObjectDynLink::SetUnit( CUnitBasePtr pcSrc) {
  ASSERT( pcSrc != nullptr );
  m_pCUnit = pcSrc;
  m_strUnitName = pcSrc->GetName();
}

void CObjectDynLink::SetObject( CObjectBase * pcSrc ) {
  ASSERT( pcSrc != nullptr );
  m_pCObject = pcSrc;
}

void CObjectDynLink::SetUnitIndex( ULONG ulIndex ) {
  m_lUnitIndex = ulIndex;
}

void CObjectDynLink::SetLinkMethod( ULONG ulIndex) {
  m_lLinkMethod = ulIndex;
}

void CObjectDynLink::SetObjectIndex( ULONG ulIndex) {
  m_lObjectIndex = ulIndex;
}

void CObjectDynLink::SetDataFlowUnitToObject( bool flag ) {
  m_fUnitToObject = flag;
}

void CObjectDynLink::SetComment( CString strComment ) {
  m_strComment = strComment;
}
