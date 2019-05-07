#include "stdafx.h"    

#include "..\\resource.h"
#include "cUnitBase.h"
#include "cUnitQuad.h"
#include "DlgUnitQuad.h"

#include "globedef.h"

IMPLEMENT_SERIAL(CUnitQuad, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitQuad::sm_ptrParaName[] = {
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,0},
   {"K0",     tINPUT | tOUTPUT | tDOUBLE, 1},
   {"K1",     tINPUT | tOUTPUT | tDOUBLE, 2},
   {"K2",     tINPUT | tOUTPUT | tDOUBLE, 3},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 4},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 5}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  6},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 7}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 8},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 9},
   {"",         0, 10},
   }; 
   
INT32 CUnitQuad::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitQuad::sm_ulDoubleEnd = 7;
const ULONG CUnitQuad::sm_ulBoolEnd = 8;
const ULONG CUnitQuad::sm_ulWordEnd   = 9;
const ULONG CUnitQuad::sm_ulStringEnd = 9;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitQuad::CUnitQuad(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 65;
  m_rectArea.right  = pt.x + 65;
  m_rectArea.top    = pt.y - 20;
  m_rectArea.bottom = pt.y + 20;

  // initial parameter
  m_ulCounter = 0;
  m_eOutput = 0.0;
  m_eK0 = 1.0;
  m_eK1 = 1.0;         
  m_eK2 = 1.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitQuad::CUnitQuad( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_ulCounter = 0;
  m_eOutput = 0.0;
  m_eK0 = 1.0;
  m_eK1 = 1.0;
  m_eK2 = 1.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitQuad::~CUnitQuad() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitQuad::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_eK0 << m_eK1 << m_eK2;
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
  }
  else {
    ar >> m_eK0 >> m_eK1 >> m_eK2;
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
  }
} 

/////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitQuad::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 10));
  return(m_vfSelected[ulIndex]);
}

void CUnitQuad::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 10; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitQuad::GetClassName( void ) {
  static CString s_strClassName = "Quad";
  return(s_strClassName);
}

bool CUnitQuad::SetExectivePriority( ULONG  ) {
	m_lExectivePriority = 1;	// this type unit mast calculate first
  return true;    // 单向输出型单元永远为真，执行优先级永远为最高的1
}

CString CUnitQuad::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitQuad::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitQuad::Exective( void ) {
  if ( m_fAutoExective ) {
  	m_eOutput = m_eK2 * m_ulCounter * m_ulCounter + m_eK1 * m_ulCounter + m_eK0;
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
// CUnitQuad类不允许联入动态链接，只有输出参数
// 
//
/////////////////////////////////////////////////////////////////////////////////
ULONG CUnitQuad::GetUnitType( void ) {
  return( tOUTPUT );
}     

INT32 CUnitQuad::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitQuad::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitQuad::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitQuad::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitQuad::GetBool( ULONG index ) {
	ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
	switch ( index ) {
	case 8 :
		return( m_fAutoExective );
	default :
		break;
	}
	return( true );
}

bool CUnitQuad::SetBool(ULONG index, bool fValue) {
  ASSERT((index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd));
  switch ( index ) {
  case 8 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}
         
LONG CUnitQuad::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 9 :              
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitQuad::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd));
  switch ( index ) {
  case 9 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitQuad::GetDouble(ULONG index) {
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
    return ( m_eHiRange );
  case 5:               
    return ( m_eLoRange );
  case 6 :              
    return ( m_eHiLimit );
  case 7 :
    return ( m_eLoLimit );
  default :
    break;
  };
  
  return(0);
} 

bool CUnitQuad::SetDouble(ULONG index, double eValue) {
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
    m_eHiRange = eValue;
    break;
  case 5 :
    m_eLoRange = eValue;
    break;
  case 6 :
    m_eHiLimit = eValue;
    break;
  case 7 :
    m_eLoLimit = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitQuad::ToShow( CDC * const pdc) {
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    CBrush cb, * pcb;
    
    cb.CreateSolidBrush( RGB(255, 255, 255) );
    pcb = (CBrush *)pdc->SelectObject(&cb);
    ASSERT( pcb != nullptr );
    pdc->Rectangle(m_rectArea);           // draw myself
    int a = m_rectArea.left + 8;
    int b = ( m_rectArea.top + m_rectArea.bottom) / 2 - 8;
    pdc->TextOut( a, b, m_strName, min(m_strName.GetLength(), 15) );
    pdc->SelectObject(pcb);
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitQuad::SetProperty( void ) {
  CDlgQuad CDlg;
  
  CDlg.SetData(m_strName, m_eK0, m_eK1, m_eK2, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eK0, m_eK1, m_eK2, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitQuad diagnostics

#ifdef _DEBUG
void CUnitQuad::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitQuad::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

