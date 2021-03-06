//////////////////////////////////////////////////////////////////////////////
//
// GetUnit()								: Get unit pointer from dictionary.
// GetUnitName()						: Get unit's name from dictionary.
// GetIndex()								: Get Unitparameter's index from dictionary.
// GetAttr()								: Get unit's attribute from dictionary.
// GetParaName()						: Get unit's parameter's name.
//
//
/////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "typedef.h"

#include "CUnitDictionary.h"									 
#include "cUnitBase.h"

IMPLEMENT_SERIAL(CUnitDictionary, CObject, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CUnitDictionary::CUnitDictionary( CUnitBase * pcunit, INT64 ulIndex, INT64 ulType ) 
	: CObject() {
	m_pCUnit		= pcunit;
	m_lUnitIndex		= ulIndex;
	m_lType		= ulType;
	m_lNumber = 0;  
  
  m_lIndexNumber = 0;

}

CUnitDictionary::CUnitDictionary( void ) : CObject() {
  m_pCUnit = nullptr;
  m_lUnitIndex = -1;
  m_lType = 0;
	m_lNumber = 0;

  m_lIndexNumber = 0;

}

CUnitDictionary::~CUnitDictionary() {

}

///////////////////////////////////////////////////////////////////////////////////////
//
// GetUnit()
//
///////////////////////////////////////////////////////////////////////////////////////
CUnitBase * CUnitDictionary::GetUnit( void ) {
	return( m_pCUnit );
}

///////////////////////////////////////////////////////////////////////////////////////
//
// GetUnitName()
//
///////////////////////////////////////////////////////////////////////////////////////
CString CUnitDictionary::GetUnitName( void ) {
	return( m_pCUnit->GetName() );
}

///////////////////////////////////////////////////////////////////////////////////////
//
// GetIndex()
//
///////////////////////////////////////////////////////////////////////////////////////
INT64 CUnitDictionary::GetUnitIndex( void ) {
	return( m_lUnitIndex );
}

///////////////////////////////////////////////////////////////////////////////////////
//
// GetAttr()
//
///////////////////////////////////////////////////////////////////////////////////////
INT64 CUnitDictionary::GetType( void ) {
	return( m_lType );
}

///////////////////////////////////////////////////////////////////////////////////////
//
// GetParaName()
//
///////////////////////////////////////////////////////////////////////////////////////
CString CUnitDictionary::GetParaName( void ) {
	return( m_pCUnit->GetParaName( m_lUnitIndex ) );
}

void CUnitDictionary::Serialize( CArchive&  ) {

}

/////////////////////////////////////////////////////////////////////////////
// CUnitDictionary diagnostics

#ifdef _DEBUG
void CUnitDictionary::AssertValid() const
{
  CObject::AssertValid();
}

void CUnitDictionary::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
