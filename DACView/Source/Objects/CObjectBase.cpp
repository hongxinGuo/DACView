////////////////////////////////////////////////////////////////////////
//
// CObjectBase implementation file.
//
// Functions :
//
// CObjectBase()            : initialize function
// ~CObjectBase()           : clear up function.  
//
// Serialize()              : Store and Retrive data.
//
// ExectiveDynLink()           : Exective dynamic link of me.
// GetParaNameAddress()     : Get parameter address.
// GetDynLinkType()         : Get my dynamic link type of index.
// SelectParameter()        : Select parameter accord input type.
// GetParaName()            : Get parameter name 
// SetParameterLock()				: ���û����������ѡ���־. 
// CheckSelf()              : ��鱾���Ƿ��д���.
// GetIndex()               : Get parameter's index from sm_aulSuitable
//
// SetBool()                : 
// SetInteger()
// SetDouble()
// SetString()
// SetColor()
//
// BreakDynLink()           : Delete dynamic link that I have
//
// GetOffset()              : �õ�λ��ƫ��(����ڷ�����), ����Ϊ��.
//
// InIt()                   : Check weither my in rect.
// ProcessChar()            : process input char.
// AdjustCaretPos()         : Adjust caret position
//
// ToShowStatic()           : show myself
// ToShowSynamic(CDC *)     : Show myself dynamical
//
// AdjustInnerSize()        : adjust my children's size.
// AdjustDynamicInnerSize() : adjust my children's size dynamically.
//
// InFocus()                : 
// SetFocus()               : set myself in focus
// 
// SetProperty()            : set my property.
//
// GetSize()                : Get current my size.
// GetOriginSize()          : Get origin my size.
// GetLastSize()            : Get last time my size.
// GetClipRgn()             : Get current my clip region.
//
// SetDynLinkListAdderss()  : Set up my dynamic link's source unit address.
// CreateUniName()          : Create unitque name that differ from unit's in listUnit.
//
// AddToList() : Add myself and all my children to ObjectList.
// GetDynLinkList()
// GetBkGrdColor()
// GetForeGrdColor()
// 
// SetSize()
// SetDynamicSize()
// SetAreaLast()
// SetRectPosition()
// SetBkGrdColor()
// SetForeGrdColor()
// SetFocusFlag()
// IsSetFocus() 
//
// IsTransparent()
// SetTransparent()
// CanInSymbol()            : weither me can be symbol's child 
// IsRectShape()            : weither me is a rect shape
//
///////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "globedef.h"

#include "cobjectbase.h"
#include "CUnitBase.h"
#include "DlgObjRect.h"

#include "CObjSymbol.h"

#include "cObjDynLink.h"
#include "DlgObjDeleteDynLink.h"

IMPLEMENT_SERIAL(CObjectBase, CObjectPrimitive, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectBase::CObjectBase(const CString& Name, COLORREF BkGrd, COLORREF ForeGrd)
  : CObjectPrimitive(Name) {
  m_clrBkGrd = BkGrd;
  m_clrForeGrd = ForeGrd;

  dm_strName = Name;
  dm_clrBkGrd  = BkGrd;
  dm_clrForeGrd = ForeGrd;

  m_fDrawAll = FALSE;
  m_fNeedShowCaret = FALSE;
	m_fChangeSize = false;

  m_fTransparent = FALSE;
  m_fCreateMemoryDC = FALSE;

  m_pSymbolHaveMe = nullptr;

  m_lScanRateDynamicCount = 0;
  m_rgnClip.CreateRectRgn(0, 0, 0, 0);

  m_iReserved1 = m_iReserved2 = m_iReserved3 = m_iReserved4 = 0;
}      

CObjectBase::CObjectBase( void ) : CObjectPrimitive() { 
  m_strName.Empty();
  m_clrBkGrd = RGB(255, 255, 255);
  m_clrForeGrd = RGB(0, 0, 0);
  
  m_fDrawAll = FALSE;
  m_fNeedShowCaret = FALSE;

  m_fTransparent = FALSE;
  m_fCreateMemoryDC = FALSE;
	m_fChangeSize = false;

  m_pSymbolHaveMe = nullptr;

  m_lScanRateDynamicCount = 0;
  m_rgnClip.CreateRectRgn(0, 0, 0, 0);

  m_iReserved1 = m_iReserved2 = m_iReserved3 = m_iReserved4 = 0;
}

CObjectBase::~CObjectBase() { 
	CUnitBase * pcUnit;
  
  // delete my dynamic links
  for (auto it = m_listDynLink.begin(); it != m_listDynLink.end(); it++ ) {
    auto pcobjDL = *it;
		pcUnit = pcobjDL->GetUnit();
		if ( !pcobjDL->IsUnitToObject() ) {
			pcUnit->SetParameterLock( pcobjDL->GetUnitIndex(), FALSE );
		}
    delete pcobjDL;
    pcobjDL = nullptr;
  }
  // release dynamic list's memory
  m_listDynLink.clear(); 
  
	m_rgnClip.DeleteObject();

} 

void CObjectBase::Serialize( CArchive& ar ) {
  CObjectPrimitive::Serialize( ar );
  INT32 a;

  if( ar.IsStoring() ) {
    ar << m_clrBkGrd << m_clrForeGrd << (INT32)m_fTransparent << m_iReserved1 << m_iReserved2 << m_iReserved3 << m_iReserved4;
  }
  else {
    ar >> m_clrBkGrd >> m_clrForeGrd >> a >> m_iReserved1 >> m_iReserved2 >> m_iReserved3 >> m_iReserved4;
    m_fTransparent = (bool)a;
  }                                  
  
  CString strTemp;
  INT64 iTemp = m_listDynLink.size();
  
  if (ar.IsStoring())
  {
    // TODO: add storing code here
    ar << iTemp;
    for ( const auto pcDynLink : m_listDynLink ) { 
      ar << pcDynLink;
    }
  }
  else
  {
    // TODO: add loading code here
		ar >> iTemp;
    CObjectDynLink * pcDynLink = nullptr;
		for (int i = 0; i < iTemp; i++) {
			ar >> pcDynLink;
			pcDynLink->SetObject(this);
			m_listDynLink.push_back(pcDynLink);
		}
  }
}

bool CObjectBase::ExectiveDynLink( ) {
  ASSERT( FALSE );
  return(FALSE);
}

ParaName* CObjectBase::GetParaNameAddress( void ) {
  return( nullptr );
}
     
ULONG CObjectBase::GetDynLinkType(ULONG ) {
  ASSERT( FALSE );
  return(0);
}

bool CObjectBase::SetParameterSelected(ULONG ulIndex, bool fSelected)
{
  m_vfSelected[ulIndex] = fSelected;
  return true;
}

void CObjectBase::SelectParameter(ULONG ) {
  ASSERT( FALSE );
}

CString CObjectBase::GetParaName( ULONG  ) {
  ASSERT( FALSE );
  return("");
}

bool CObjectBase::SetParameterLock( ULONG , bool  ) {
  ASSERT( FALSE );
  return(false);
}

bool CObjectBase::CheckSelf( void ) {
  TRACE("CObjectBase's CheckSelf been called, error!\n");
  return( TRUE );
}

void CObjectBase::SetSymbolThatHaveMe(CObjectSymbol * pCSymbol) {
  m_pSymbolHaveMe = pCSymbol;
}

CObjectSymbol * CObjectBase::GetSymbolThatHaveMe( void ) {
  return( m_pSymbolHaveMe );
}

///////////////////////////////////////////////////////////////////////////
//
// ���������͵�Ԫ�Ĳ���ѡ���־��
//
// �����������������Ԫ�Ķ�̬����ʱ����Ҫ���õ�Ԫ�����������ѡ���־���Է�ֹ�ٴ������������˲���
//
///////////////////////////////////////////////////////////////////////////
bool CObjectBase::SetParameterSelected(void) {
  ULONG ulParaType = 0;

  for (const auto pODL : m_listDynLink) {
    ulParaType = pODL->GetUnitParaType();
    if (ulParaType & tINPUT) { // ��Ԫ��������Ϊ������
      // ���ò�����ѡ���־�����ڴ˶�̬���Ӳ�Ӱ��ִ�����ȼ����ʲ�����SetParameterLock
      pODL->GetUnit()->SetParameterSelected(pODL->GetUnitIndex(), true);
    }
    if (ulParaType & tOUTPUT) { // Object����Ϊ������
      m_vfSelected[pODL->GetObjectIndex()] = true;
    }
  }
  return(true);
}

INT32 CObjectBase::GetIndex( ULONG  ) {
  ASSERT( false );
  return((ULONG)0);
}

bool CObjectBase::SetBool(ULONG , bool ) {
  ASSERT(  FALSE );
  return(FALSE);
}

bool CObjectBase::SetInteger(ULONG , LONG ) {
  ASSERT(  false );
  return(false);
}

bool CObjectBase::SetDouble(ULONG , double ) {
  ASSERT(  false );
  return(false);
}
double CObjectBase::GetDouble(ULONG index)
{
	ASSERT(false);
	return 0.0;
}


bool CObjectBase::SetString(ULONG , const CString& ) {
  ASSERT(  FALSE );
  return(FALSE);
}   

bool CObjectBase::SetColor(ULONG , LONG ) {
  ASSERT(  FALSE );
  return(FALSE);
}

//////////////////////////////////////////////////////////////////////////
//
// ArrangeDynLink
//
// return : ��ɾ���˶�̬����ʱ����TRUE
//					��ûɾ����̬����ʱ����FALSE
//
///////////////////////////////////////////////////////////////////////////
bool CObjectBase::IsDeleteDynLink( void ) {
  CODelDynLink cDialogDel;
	INT_PTR iTotal = m_listDynLink.size();

  cDialogDel.SetLink( &m_listDynLink, this );
  cDialogDel.DoModal();
	if ( iTotal != m_listDynLink.size() ) {	// ɾ���˶�̬����?
		return( true );
		}
	else return( false );
}

bool CObjectBase::InIt( CDC * const , POINT const , int ) {
  ASSERT(  FALSE );
  return ( FALSE );
}

bool CObjectBase::ProcessChar( UINT, CPoint&  ) {
  return( FALSE );
}

bool CObjectBase::AdjustCaretPos( CDC * , CPoint&  ) {
  return( FALSE );
}

void CObjectBase::ToShowStatic( CDC * const, CPoint  ) {
  ASSERT(  FALSE );
}            

void CObjectBase::ToShowDynamic( CDC * const ) {
  ASSERT(  FALSE );
}

void CObjectBase::ToShow( CDC * const  ) {
	ASSERT( FALSE );
}

void CObjectBase::AdjustInnerSize( void ) {
  // do nothing , use by CObjectSymbol
  ASSERT(FALSE);

}

void CObjectBase::AdjustDynamicInnerSize( void ) {
  // do nothing, use by CObjectSymbol
  ASSERT(FALSE);

}

void CObjectBase::SetFocus( CDC * ) {
  ASSERT(  FALSE );
}
 
bool CObjectBase::SetProperty( void ) {
  ASSERT(  FALSE );
  return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
// CObjectBase diagnostics

#ifdef _DEBUG
void CObjectBase::AssertValid() const
{
  CObjectPrimitive::AssertValid();
}

void CObjectBase::Dump(CDumpContext& dc) const
{
  CObjectPrimitive::Dump(dc);
  for ( const auto pcobj : m_listDynLink ) {
    dc << pcobj << "\n";
  }
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

const CRect& CObjectBase::GetSize( void ) {
  ASSERT(  FALSE );
  static CRect r(0, 0, 0, 0);
  return( r );
}

//////////////////////////////////////////////////////////////////////////////
//
// GetOffset()
//
// ����ֵ: 
//      CPoint : �˶�������ֵ.
// ����:
//      ����˶��󱻰����ڷ�����,�򷵻ط��ŵ�ԭ��,���򷵻����.
//
////////////////////////////////////////////////////////////////////////
CPoint CObjectBase::GetOffset( void ) {
  CPoint pt( 0, 0 );
  CRect rect;

  if ( m_pSymbolHaveMe != nullptr ) {
    rect = m_pSymbolHaveMe->GetSize();
    pt = m_pSymbolHaveMe->GetOffset();
    pt.x += rect.left;
    pt.y += rect.top;
  }
  return( pt );
}

const CRect& CObjectBase::GetOriginSize( void ) {
  ASSERT(  FALSE );
  static CRect r(0, 0, 0, 0);
  return ( r );
} 

const CRect& CObjectBase::GetLastSize( void ) {
  ASSERT(  FALSE );
  static CRect r(0, 0, 0, 0);
  return ( r );
} 

CRgn * CObjectBase::GetClipRgn( const CPoint&  ) {
  ASSERT(  FALSE );
  return( &m_rgnClip );
}

//////////////////////////////////////////////////////////////////////////////////////
//
// CreateUniName		����Ψһ������
//
//
//
////////////////////////////////////////////////////////////////////////////////////// 
bool CObjectBase::CreateUniName( CObjectList& listObject ) {
  bool fFind = FALSE;
	INT_PTR iTemp = 1, iCount = listObject.GetCount();
  POSITION po = listObject.GetHeadPosition();
  CObjectBase * pcObj;
  char s[20];

  for ( int i = 0; i < iCount; i ++ ) {
    pcObj = listObject.GetNext( po );
    if ( m_strName == pcObj->GetName() ) {
      fFind = TRUE;
      break;
    }
  }

  bool fDone = FALSE;
  if ( fFind ) {
    while ( !fDone ) {
      _itoa(iTemp++, s, 10);
      m_strName = GetClassNameStr() + s;
      iCount = listObject.GetCount();
      po = listObject.GetHeadPosition();
      fDone = TRUE;
      for ( int i = 0; i < iCount; i ++ ) { 
        pcObj = listObject.GetNext( po );
        if ( m_strName == pcObj->GetName() ) {
          fDone = FALSE;
          break;
        }
      }
    }
  }
  return( TRUE );
}

void CObjectBase::AddToList( CObjectList& ObjectList ) {
  ObjectList.AddTail(this);
}

CODLList * CObjectBase::GetDynLinkList( void ) {
  return ( &m_listDynLink );
}

COLORREF CObjectBase::GetBkGrdColor( void ) const {
  return( m_clrBkGrd );
}

COLORREF CObjectBase::GetForeGrdColor( void ) const {
  return( m_clrForeGrd );
}

void CObjectBase::SetAllSize( const CRect& ) {
  ASSERT(  FALSE );
}

void CObjectBase::SetOriginSize( const CRect& ) {
  ASSERT(  FALSE );
}

void CObjectBase::SetDynamicSize( const CRect& ) {
  ASSERT(  FALSE );
}

bool CObjectBase::SetRectPosition(ULONG , LONG ) {
  ASSERT(  FALSE );
  return(FALSE);
}
  

void CObjectBase::SetBkGrdColor( COLORREF color ) {
  m_clrBkGrd = color;
}

void CObjectBase::SetForeGrdColor( COLORREF color ) {
  m_clrForeGrd = color;
}

bool CObjectBase::IsTransparent( void ) {
  return( m_fTransparent );
}

void CObjectBase::SetTransparentFlag( bool fFlag ) {
  m_fTransparent = fFlag;
}

bool CObjectBase::CanInSymbol( void ) {
  return( TRUE );
}

bool CObjectBase::IsRectShape( void ) {
  ASSERT( FALSE );
  return( TRUE );
}

////////////////////////////////////////////////////////////////////////
//
// DeleteDynlink()
//
// Parameter : 
//    CUnitBase * pUnit : point to unit that need deleted 
//
// Return :
//    bool          : always TRUE.
//
// Description :
//    ���������Ķ�̬��������pUnit,��ɾ���˶�̬���ӣ�
// �����뷽��Ϊ�Ӷ�������Ԫʱ��������������
//
/////////////////////////////////////////////////////////////////////////
bool CObjectBase::DeleteDynLink( CUnitBase * pUnit ) {
  CUnitBase * pc;
  
  for (auto it = m_listDynLink.begin(); it != m_listDynLink.end(); it++ ) {
    auto pcobjDynLink = *it;
    pc = pcobjDynLink->GetUnit();
    if ( pUnit == pc ) { //��Ҫɾ��
			if ( !pcobjDynLink->IsUnitToObject() ) { // ���뷽��Ϊ�Ӷ�������Ԫ
				pc->SetParameterLock( pcobjDynLink->GetUnitIndex(), FALSE );// �������
			}
      m_listDynLink.erase( ++it );
      it--;
      delete pcobjDynLink;
      pcobjDynLink = nullptr;
    }
  }
  return( true );
}
