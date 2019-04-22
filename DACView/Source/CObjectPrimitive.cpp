////////////////////////////////////////////////////////////////////////////
//
// CObjectPrimitive实现文件.
//
// CanViewIn			: 是否允许看进去.Component, Symbol, Component允许, 其他不允许.
// CheckSelf			: 自我检查.
// CountDown			: 到数计数, 用于Exective().
// GetCurrentStatus :
//
// Serialize			:
// FormatRead			: 
// FormatSave			:
//
// GetName				:
// GetClassName   :
// SetName				:
// 
//
// SetScanRate		:
// GetScanRate		:
//
// SetSelect			:
// IsSelect				:
//
// SetUpdateFlag  :
// IsNeedUpdate		:
//
//
/////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include"afxwin.h"
//#include"pch.h"
#include "..\\resource.h"

#include "cObjectPrimitive.h"
#include "globedef.h" 

IMPLEMENT_SERIAL(CObjectPrimitive, CObject, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectPrimitive::CObjectPrimitive(const CString& Name)
  : CObject() {
  m_strName = Name;
  m_lScanRate = 100; // 默认扫描速率为100ms每次。
	m_lScanRateDynamicCount = 0;
  m_fNeedUpdate = FALSE;
  m_fSelected = FALSE;

  m_lReserved1 = m_lReserved2 = 0;

  m_pfSelected = nullptr;
}      

CObjectPrimitive::CObjectPrimitive( void ) : CObject() {
  m_strName = "";
	m_lScanRate = 100; // 默认扫描速率为100ms每次。
	m_lScanRateDynamicCount = 0;
  m_fNeedUpdate = FALSE;
  m_fSelected = FALSE;

  m_lReserved1 = m_lReserved2 = 0;
  
  m_pfSelected = nullptr;
}

CObjectPrimitive::~CObjectPrimitive() { 
  if (m_pfSelected != nullptr) { // 此变量必须被派生类初始化
    delete []m_pfSelected;
  }
} 

void CObjectPrimitive::Serialize( CArchive& ar ) {
  CObject::Serialize( ar );
  if( ar.IsStoring() ) {
		// 0.1版用1ms/次, 故m_lScanRate应大于等于1.
    ASSERT(m_lScanRate >= 1);
		if ( m_lScanRate < 1 ) m_lScanRate = 1;
    ar << m_strName << m_lScanRate << m_lReserved1 << m_lReserved2;
  }
  else {
    ar >> m_strName >> m_lScanRate >> m_lReserved1 >> m_lReserved2;
		// 0.1版用1ms/次, 故m_lScanRate应大于等于1.
		ASSERT( m_lScanRate >= 1 );
    m_lScanRateDynamicCount = m_lScanRate; 
  }                                  
}

////////////////////////////////////////////////////////////////////////////////////
//
// 最基本类的名称返回为自己的名字
//
//
/////////////////////////////////////////////////////////////////////////////////////
CString CObjectPrimitive::GetName(void)
{
  return(m_strName);
}


/////////////////////////////////////////////////////////////////////////////
// CObjectBase diagnostics

#ifdef _DEBUG
void CObjectPrimitive::AssertValid() const
{
  CObject::AssertValid();
}

void CObjectPrimitive::Dump(CDumpContext& dc) const
{
  CObject::Dump(dc);
  dc << "Name = " << m_strName;
  dc << "\nm_strName = " << m_strName;
  dc << "\nm_lScanRate = " << m_lScanRate << "\n";
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////////////////
         
void CObjectPrimitive::FormatSave( CArchive&  ) {
}

void CObjectPrimitive::FormatRead( CArchive&  ) {
}

const CString& CObjectPrimitive::GetClassNameStr( void ) {
  static CString str= "CObjectPrimitive";
  TRACE("Call CObjectPrimitive's GetClassNameStr, problely error\n");
  return( str );
}

void CObjectPrimitive::SetUpdateFlag( bool fUpdate ) {
  m_fNeedUpdate = fUpdate;
}

bool CObjectPrimitive::IsNeedUpdate( void ) const {
  return( m_fNeedUpdate );
}                         

void CObjectPrimitive::SetSelect( bool flags ) {
  m_fSelected = flags;
}   

bool CObjectPrimitive::IsSelect( void ) const {
  return ( m_fSelected );
}

void CObjectPrimitive::ClearParaSelectedFlag(void) {
  ASSERT(0);
}

bool CObjectPrimitive::SetParameterLock(ULONG ulIndex, bool fSelected)
{
  ASSERT(0);
  return(false);
}

bool CObjectPrimitive::IsParameterLocked(ULONG ulIndex)
{
  ASSERT(0);
  return(false);
}

bool CObjectPrimitive::SetParameterSelected(ULONG ulIndex, bool fSelected)
{
  ASSERT(0);
  return false;
}

void CObjectPrimitive::SetScanRate( ULONG ulScanRate ) {
  m_lScanRate = ulScanRate;
}

ULONG CObjectPrimitive::GetScanRate( void ) const {
  return( m_lScanRate );
}
  
/////////////////////////////////////////////////////////////
//
//  此函数被实时线程Exective()函数调用，使用时注意。
//
//  return :
//  TRUE if (m_lScanRateDynamicCount - ulTick) <= 0;
//  else return FALSE.
//
///////////////////////////////////////////////////////////////
bool CObjectPrimitive::CountDown( ULONG ulTick ) {
  if ( (m_lScanRateDynamicCount -= ulTick) <= 0 ) {
    m_lScanRateDynamicCount = m_lScanRate;			// 重置计数器
    return( TRUE );
  }
  return( FALSE );
}

bool CObjectPrimitive::CheckSelf( void ) {
  TRACE("CObjectPrimitive's CheckSelf been called!\n");
  return( TRUE );
}

bool CObjectPrimitive::CanViewIn( void ) const {
  return( FALSE );
} 

bool CObjectPrimitive::DumpCurrentStatus( char * pch ) const {
  TRACE( "Call CObjectPrimitive's GetStatus\n" );
  sprintf(pch, "Call CObjectPrimitive" );

  return( FALSE );
}