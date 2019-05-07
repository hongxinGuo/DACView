#include "stdafx.h"

#include "math.h"   

#include "..\\resource.h"
#include "cUnitBase.h"
#include "cUnitSine.h"
#include "DlgUnitSine.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitSine, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitSine::sm_ptrParaName[] = {
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,0},
   {"K0",     tINPUT | tOUTPUT | tDOUBLE, 1},
   {"K1",     tINPUT | tOUTPUT | tDOUBLE, 2},
   {"K2",     tINPUT | tOUTPUT | tDOUBLE, 3},
   {"K3",     tINPUT | tOUTPUT | tDOUBLE, 4},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 5},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 6}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  7},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 8},
   {"AutoOrManual",tINPUT | tOUTPUT | tMODIFIABLE | tBOOL, 9}, 
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 10},
   {"",         0, 11},
   }; 
   
INT32 CUnitSine::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitSine::sm_ulDoubleEnd = 8;
const ULONG CUnitSine::sm_ulBoolEnd = 9;
const ULONG CUnitSine::sm_ulWordEnd   = 10;
const ULONG CUnitSine::sm_ulStringEnd = 10;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitSine::CUnitSine(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_ulCounter = 0;
  m_eOutput = 0.0;
  m_eK0 = 1.0;
  m_eK1 = 0.0;
  m_eK2 = 1.0;
  m_eK3 = 0.0;
  m_eHiRange = 1.0;
  m_eLoRange = -1.0;
  m_eHiLimit = 1.0;
  m_eLoLimit = -1.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitSine::CUnitSine( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_ulCounter = 0;
  m_eOutput = 0.0;
  m_eK0 = 1.0;
  m_eK1 = 0.0;
  m_eK2 = 1.0;
  m_eK3 = 0.0;
  m_eHiRange = 1.0;
  m_eLoRange = -1.0;
  m_eHiLimit = 1.0;
  m_eLoLimit = -1.0;
  
	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitSine::~CUnitSine() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitSine::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_eK0 << m_eK1 << m_eK2 << m_eK3;
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
  }
  else {
    ar >> m_eK0 >> m_eK1 >> m_eK2 >> m_eK3;
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
  }
} 

//////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitSine::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 11));
  return(m_vfSelected[ulIndex]);
}

void CUnitSine::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 11; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitSine::GetClassName( void ) {
  static CString str = "UnitSine";
  return(str);
}

bool CUnitSine::SetExectivePriority( ULONG  ) {
  m_lExectivePriority = 1;       // this type unit mast calculate first
  return true;    // 单向输出型单元永远为真，执行优先级永远为最高的1
} 

CString CUnitSine::GetParaName( ULONG index ) {
  return( sm_ptrParaName[index].Name );
}

ULONG CUnitSine::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitSine::Exective( void ) {
  if ( m_fAutoExective ) { // if  auto calculate 
    m_eOutput = m_eK0 * sin((m_eK1 + m_ulCounter * m_eK2)*3.1415926/180) + m_eK3;
    m_ulCounter++;
  }
  if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
    m_fOverFlow = true;;
  }
  else {
    m_fOverFlow = false;
  }
  
  ExectiveDynLink();
}

//////////////////////////////////////////////////////////////////////////////////
//
// CUnitSine类不允许联入动态链接，只有输出参数
// 
//
/////////////////////////////////////////////////////////////////////////////////
ULONG CUnitSine::GetUnitType( void ) {
  return( tOUTPUT );
}     

INT32 CUnitSine::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitSine::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitSine::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitSine::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitSine::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 9 :  
    return ( m_fAutoExective );
  default :
    break;
  };
  
  return(0);
}

bool CUnitSine::SetBool(ULONG index, bool fValue) {
  ASSERT((index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd));
  switch ( index ) {
  case 9 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}
         
LONG CUnitSine::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 10 :            
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitSine::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulDoubleEnd));
  switch ( index ) {
  case 10 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitSine::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eOutput );
  case 1:               
    return ( m_eK0 );
  case 2 :              
    return ( m_eK1 );
  case 3 :
    return ( m_eK2 );
  case 4 :
    return ( m_eK3 );
  case 5 :
    return ( m_eHiRange );
  case 6:               
    return ( m_eLoRange );
  case 7 :              
    return ( m_eHiLimit );
  case 8 :
    return ( m_eLoLimit );
  default :
    break;
  };
  
  return(0);
} 

bool CUnitSine::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0 : 
    m_eOutput = eValue;
    break;
  case 1 :
    m_eK0 = eValue;
    break;
  case 2 :
    m_eK1 = eValue;
    break;
  case 3 :
    m_eK2 = eValue;    
    break;
  case 4 :
    m_eK3 = eValue;
    break;
  case 5 :
    m_eHiRange = eValue;
    break;
  case 6 :
    m_eLoRange = eValue;
    break;
  case 7 :
    m_eHiLimit = eValue;
    break;
  case 8 :
    m_eLoLimit = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitSine::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_SINE ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
  CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitSine::SetProperty( void ) {
  CDlgSine CDlg;
  
  CDlg.SetData(m_strName, m_eK0, m_eK1, m_eK2, m_eK3, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eK0, m_eK1, m_eK2, m_eK3, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
               
}

bool CUnitSine::DumpCurrentStatus( char * pch ) const {
  sprintf_s(pch, 100, "%s : Output = %e", m_strName, m_eOutput);
  return( true );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitSine diagnostics

#ifdef _DEBUG
void CUnitSine::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitSine::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

