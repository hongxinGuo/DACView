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
// SetParameterLock()				: 设置或清除参数被选择标志. 
// CheckSelf()              : 检查本身是否有错误.
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
// GetOffset()              : 得到位置偏移(如果在符号内), 否则为零.
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
  CObjectDynLink * pcobjDL;
	CUnitBase * pcUnit;
  POSITION Po = m_listDynLink.GetHeadPosition();
	INT_PTR iCount = m_listDynLink.GetCount();
  
  // delete my dynamic links
  for ( int i = 0; i < iCount; i++ ) {
    pcobjDL = m_listDynLink.GetNext(Po);
		pcUnit = pcobjDL->GetUnit();
		if ( !pcobjDL->IsUnitToObject() ) {
			pcUnit->SetParameterLock( pcobjDL->GetUnitIndex(), FALSE );
		}
    delete pcobjDL;
    pcobjDL = nullptr;
  }
  if (iCount) TRACE("%d Dynamic link name deleted\n", iCount);
  // release dynamic list's memory
  m_listDynLink.RemoveAll(); 
  
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
  CObjectDynLink * pcDynLinkTemp;
  POSITION pos = m_listDynLink.GetHeadPosition();                          
  INT64 iTemp = m_listDynLink.GetCount();
  
  if (ar.IsStoring())
  {
    // TODO: add storing code here
    ar << iTemp;
    for ( int i = 0; i < iTemp; i ++ ) { 
      pcDynLinkTemp = m_listDynLink.GetNext(pos);
      ar << pcDynLinkTemp;
    }
  }
  else
  {
    // TODO: add loading code here
		ar >> iTemp;
		for (int i = 0; i < iTemp; i++) {
			ar >> pcDynLinkTemp;
			pcDynLinkTemp->SetObject(this);
			m_listDynLink.AddTail(pcDynLinkTemp);
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
  m_pfSelected[ulIndex] = fSelected;
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
// 设置输入型单元的参数选择标志。
//
// 当生成输出数据至单元的动态链接时，需要设置单元的输入类参数选择标志，以防止再次链接数据至此参数
//
///////////////////////////////////////////////////////////////////////////
bool CObjectBase::SetParameterSelected(void) {
  CObjectDynLink * pODL;
  POSITION poDL;
  INT64 iTotal;
  ULONG ulParaType = 0;

  poDL = m_listDynLink.GetHeadPosition();
  iTotal = m_listDynLink.GetCount();
  for (int i = 0; i < iTotal; i++) {
    pODL = m_listDynLink.GetNext(poDL);
    ulParaType = pODL->GetUnitParaType();
    if (ulParaType & tINPUT) { // 单元参数类型为输入型
      // 设置参数被选择标志，由于此动态链接不影响执行优先级，故不调用SetParameterLock
      pODL->GetUnit()->SetParameterSelected(pODL->GetUnitIndex(), true);
    }
    if (ulParaType & tOUTPUT) { // Object参数为输入型
      m_pfSelected[pODL->GetObjectIndex()] = true;
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
// return : 当删除了动态连接时返回TRUE
//					当没删除动态连接时返回FALSE
//
///////////////////////////////////////////////////////////////////////////
bool CObjectBase::IsDeleteDynLink( void ) {
  CODelDynLink cDialogDel;
	INT_PTR iTotal = m_listDynLink.GetCount();

  cDialogDel.SetLink( &m_listDynLink, this );
  cDialogDel.DoModal();
	if ( iTotal != m_listDynLink.GetCount() ) {	// 删除了动态连接?
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
  CObjectDynLink * pcobj;
  POSITION po = m_listDynLink.GetHeadPosition();
  for ( int i = 0; i < m_listDynLink.GetCount(); i++ ) {
    pcobj = m_listDynLink.GetNext(po);
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
// 返回值: 
//      CPoint : 此对象的相对值.
// 描述:
//      如果此对象被包含于符号中,则返回符号的原点,否则返回零点.
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
// CreateUniName		生成唯一的名称
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
//    如果本对象的动态连接中有pUnit,则删除此动态连接，
// 当联入方向为从对象至单元时，还须解参数锁。
//
/////////////////////////////////////////////////////////////////////////
bool CObjectBase::DeleteDynLink( CUnitBase * pUnit ) {
	INT_PTR i, iTemp = m_listDynLink.GetCount();
  CObjectDynLink * pcobjDynLink;
  POSITION pos, po = m_listDynLink.GetHeadPosition();
  CUnitBase * pc;
  
  for ( i = 0; i < iTemp; i++ ) {
    pcobjDynLink = m_listDynLink.GetNext(po);
    pc = pcobjDynLink->GetUnit();
    if ( pUnit == pc ) { //需要删除
      pos = m_listDynLink.Find( pcobjDynLink );
      ASSERT( pos != nullptr );
			if ( !pcobjDynLink->IsUnitToObject() ) { // 联入方向为从对象至单元
				pc->SetParameterLock( pcobjDynLink->GetUnitIndex(), FALSE );// 解参数锁
			}
      m_listDynLink.RemoveAt( pos );
      delete pcobjDynLink;
      pcobjDynLink = nullptr;
    }
  }
  return( TRUE );
}
