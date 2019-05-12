#include "stdafx.h"

#include "globedef.h"

#include "cObjComponent.h"

IMPLEMENT_SERIAL(CObjectComponentBase, CObjRectBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectComponentBase::CObjectComponentBase(const CString& s, CRect r) 
  : CObjRectBase(s, r) {
}

CObjectComponentBase::CObjectComponentBase( void ) 
  : CObjRectBase( ) {
}
	
CObjectComponentBase::~CObjectComponentBase() {
	// release Object's memory
  for (auto pcObj : m_CObjectList) {
    delete pcObj;
  } 
  // release list's memory
  m_CObjectList.clear();
}

void CObjectComponentBase::SetUpdateFlag( bool fUpdate ) {
  for (const auto pcObj : m_CObjectList) {
		pcObj->SetUpdateFlag(fUpdate);
	}
  m_fNeedUpdate = fUpdate;
}

bool CObjectComponentBase::SetParameterSelected(void) {
  for (auto pcObj : m_CObjectList) {
    pcObj->SetParameterSelected();
  }
  CObjectBase::SetParameterSelected();
  return(true);
}


bool CObjectComponentBase::CreateUniName( CObjectList& listObject ) {
  for (auto pcObj : m_CObjectList) {
    pcObj->CreateUniName( listObject );
  }
  CObjectBase::CreateUniName( listObject );
  return( TRUE );
}

//////////////////////////////////////////////////////////////////////////////////
//
//  将本身加入ObejectList中。
//  如果本身是复合Object，则先把本身加入，然后再把包含的其他Objects加入list。
//
//
//////////////////////////////////////////////////////////////////////////////////
void CObjectComponentBase::AddToList( CObjectList& ObjectList ) {
  // this compound object must at the front of all its children
  ObjectList.push_back(this);
  for (auto pcObj : m_CObjectList) {
    pcObj->AddToList( ObjectList );
  }
}

	               
/////////////////////////////////////////////////////////////////////////////
// CObjectComponentBase diagnostics

#ifdef _DEBUG
void CObjectComponentBase::AssertValid() const
{
	CObjRectBase::AssertValid();
}

void CObjectComponentBase::Dump(CDumpContext& dc) const
{
	CObjRectBase::Dump(dc);
  for (auto pcObj : m_CObjectList) {
    dc << pcObj;
  }
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectComponent member function

void CObjectComponentBase::Serialize( CArchive& ar ) {
	CObjRectBase::Serialize( ar );

  CObjectBase * pcobjTemp;
  INT64 iTemp = m_CObjectList.size();
                              
  if (ar.IsStoring())
  {
    // TODO: add storing code here
    ar << iTemp;
    for (const auto pcObj : m_CObjectList) {
      ar << pcObj;
    }
  }
  else {
    // TODO: add loading code here
		ar >> iTemp;
		// test whether a Dacview file
		for (int i = 0; i < iTemp; i++) {
			ar >> pcobjTemp;
			m_CObjectList.push_back(pcobjTemp);
		}
  } 
} 

void CObjectComponentBase::ToShowStatic( CDC * const , CPoint  ) {
  ASSERT( FALSE );
}  

void CObjectComponentBase::ToShowDynamic( CDC * const  ) {
  ASSERT( FALSE );
}  

bool CObjectComponentBase::ExectiveDynLink( ) {
  return( CObjRectBase::ExectiveDynLink() );
}

bool CObjectComponentBase::SetInteger(ULONG index, LONG lValue) {
  CObjRectBase::SetInteger(index, lValue);

	if ( m_fChangeSize ) {
    m_fNeedUpdate = TRUE;
    m_fDrawAll = TRUE;
		m_fChangeSize = FALSE;
	}
	return ( TRUE );
}

void CObjectComponentBase::SetUpperObjectList( CObjectList * pObjectList ) {
  m_pObjectListUpper = pObjectList;
}

CObjectList * CObjectComponentBase::GetUpperObjectList( void ) {
  return ( m_pObjectListUpper );
}

CObjectList * CObjectComponentBase::GetObjectList( void ) {
  return ( &m_CObjectList );
}

void CObjectComponentBase::SetUpperObjectComponent( CObjectComponentBase * pObjectComponent ) {
  m_pObjectComponentUpper = pObjectComponent;
}

CObjectComponentBase * CObjectComponentBase::GetUpperObjectComponent( void ) {
  return ( m_pObjectComponentUpper );
}                                 

void CObjectComponentBase::SetUpperScrollPosition(CPoint pt) {
  m_ptScrollPositionUpper = pt;
}

CPoint CObjectComponentBase::GetUpperScrollPosition(void) {
  return (m_ptScrollPositionUpper);
}

void CObjectComponentBase::ViewOut( void ) {
  return;
}



