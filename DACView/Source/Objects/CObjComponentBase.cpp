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
  POSITION Po = m_CObjectList.GetHeadPosition();
  CObjectBase * pcobjTemp;
	INT_PTR i, iTemp = m_CObjectList.GetCount();
  for ( i = 0; i < iTemp; i++ ) {
    pcobjTemp = m_CObjectList.GetNext(Po);
    delete pcobjTemp;
		pcobjTemp = nullptr;
  } 
  TRACE("%d objects deleted\n", i);                  
  // release list's memory
  m_CObjectList.RemoveAll();
}

void CObjectComponentBase::SetUpdateFlag( bool fUpdate ) {
	POSITION Po = m_CObjectList.GetHeadPosition();
	CObjectBase * pcobjTemp;

	for ( int i = 0; i < m_CObjectList.GetCount(); i++ ) {
		pcobjTemp = m_CObjectList.GetNext(Po);
		pcobjTemp->SetUpdateFlag(fUpdate);
	}
  m_fNeedUpdate = fUpdate;
}

bool CObjectComponentBase::SetParameterSelected(void) {
  INT_PTR iCount = m_CObjectList.GetCount();
  POSITION po = m_CObjectList.GetHeadPosition();
  CObjectBase * pcObj;

  for (int i = 0; i < iCount; i++) {
    pcObj = m_CObjectList.GetNext(po);
    pcObj->SetParameterSelected();
  }
  CObjectBase::SetParameterSelected();
  return(true);
}


bool CObjectComponentBase::CreateUniName( CObjectList& listObject ) {
	INT_PTR i, iCount = m_CObjectList.GetCount();
  POSITION po = m_CObjectList.GetHeadPosition();
  CObjectBase * pcObj;

  for ( i = 0; i < iCount; i ++ ) {
    pcObj = listObject.GetNext( po );
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
  POSITION poObject = m_CObjectList.GetHeadPosition();
	INT_PTR iObjectCount = m_CObjectList.GetCount();
  CObjectBase * pcObj;
  
  // this compound object must at the front of all its children
  ObjectList.AddTail(this);
  for ( int i = 0; i < iObjectCount; i++ ) {
    pcObj = m_CObjectList.GetNext(poObject);
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
  POSITION po = m_CObjectList.GetHeadPosition();
  CObjectBase * pcobj;
  for ( int i = 0; i < m_CObjectList.GetCount(); i++ ) {
    pcobj = m_CObjectList.GetNext( po );
    dc << pcobj;
  }
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectComponent member function

void CObjectComponentBase::Serialize( CArchive& ar ) {
	CObjRectBase::Serialize( ar );

  CObjectBase * pcobjTemp;
  POSITION pos = m_CObjectList.GetHeadPosition();                          
  INT64 iTemp = m_CObjectList.GetCount();
                              
  if (ar.IsStoring())
  {
    // TODO: add storing code here
    ar << iTemp;
    for ( int i = 0; i < iTemp; i ++ ) { 
      pcobjTemp = m_CObjectList.GetNext(pos);
      ar << pcobjTemp;
    }
  }
  else {
    // TODO: add loading code here
		ar >> iTemp;
		// test whether a Dacview file
		for (int i = 0; i < iTemp; i++) {
			ar >> pcobjTemp;
			m_CObjectList.AddTail(pcobjTemp);
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

bool CObjectComponentBase::CanViewIn( void ) const {
  return( TRUE );
}

void CObjectComponentBase::ViewOut( void ) {
  return;
}



