#include "stdafx.h"    

#include "..\\resource.h"
#include "cUnitBase.h"
#include "cUnitQFLT.h" 
#include "DlgUnitQFLT.h"

#include "globedef.h"

IMPLEMENT_SERIAL(CUnitQuadFilt, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitQuadFilt::sm_ptrParaName[] = 
  {{"Input", tINPUT |tMODIFIABLE | tDOUBLE, 0},
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,1},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 2},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 3}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  4},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 5}, 
   {"FiltFactor", tINPUT | tOUTPUT | tDOUBLE, 6}, 
   {"DeadTime", tINPUT | tOUTPUT | tDOUBLE, 7}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 8},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 9},
   {"",         0, 10},
   }; 
   
INT32 CUnitQuadFilt::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitQuadFilt::sm_ulDoubleEnd = 7;
const ULONG CUnitQuadFilt::sm_ulBoolEnd = 8;
const ULONG CUnitQuadFilt::sm_ulWordEnd   = 9;
const ULONG CUnitQuadFilt::sm_ulStringEnd = 9;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitQuadFilt::CUnitQuadFilt(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 65;
  m_rectArea.right  = pt.x + 65;
  m_rectArea.top    = pt.y - 20;
  m_rectArea.bottom = pt.y + 20;
  
  // initial parameter
  m_eInput = m_eInput1 = m_eInput2 = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;
  m_eDeadBand = 0.0;
  m_eFiltFactor = 0.5;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitQuadFilt::CUnitQuadFilt( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  
  // initial parameter
  m_eInput = m_eInput1 = m_eInput2 = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;
  m_eDeadBand = 0.0;
  m_eFiltFactor = 0.5;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitQuadFilt::~CUnitQuadFilt() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitQuadFilt::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

  if( ar.IsStoring() ) {
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit
       << m_eFiltFactor << m_eDeadBand;
  }
  else {
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit
        >> m_eFiltFactor >> m_eDeadBand;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitQuadFilt::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 10));
  return(m_vfSelected[ulIndex]);
}

void CUnitQuadFilt::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 10; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitQuadFilt::GetClassName( void ) {
  static CString s_strClassName = "QFilt";
  return(s_strClassName);
}

CString CUnitQuadFilt::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitQuadFilt::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitQuadFilt::Exective( void ) {
  if ( m_fAutoExective ) {
	  m_eOutput =  2 * m_eFiltFactor * m_eInput1  
	               - m_eFiltFactor * m_eFiltFactor * m_eInput2
	               + (1 - m_eFiltFactor) * (1 - m_eFiltFactor) * m_eInput;
	  m_eInput2 = m_eInput1;
	  m_eInput1 = m_eInput;
	}
  if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitQuadFilt::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitQuadFilt::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitQuadFilt::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitQuadFilt::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitQuadFilt::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 8 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitQuadFilt::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 8 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}

LONG CUnitQuadFilt::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 9 :               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitQuadFilt::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 9 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitQuadFilt::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput );
  case 1:               
    return ( m_eOutput );
  case 2 :
    return ( m_eHiRange );
  case 3:               
    return ( m_eLoRange );
  case 4 :              
    return ( m_eHiLimit );
  case 5 :
    return ( m_eLoLimit );
  case 6 :
    return ( m_eFiltFactor );
  case 7 :
    return ( m_eDeadBand );
  default :
    break;
  };
  
  return(0);
} 


bool CUnitQuadFilt::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0 : 
    m_eInput = eValue;
    break;
  case 1 :
    m_eOutput = eValue;
    break;
  case 2 :
    m_eHiRange = eValue;
    break;
  case 3 :
    m_eLoRange = eValue;
    break;
  case 4 :
    m_eHiLimit = eValue;
    break;
  case 5 :
    m_eLoLimit = eValue;
    break;
  case 6 :
    m_eFiltFactor = eValue;
    break;
  case 7 :
    m_eDeadBand = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitQuadFilt::ToShow( CDC * const pdc ) {
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

bool CUnitQuadFilt::SetProperty( void ) {
  CDlgQFLT CDlg;
  
  CDlg.SetData(m_strName, m_eHiRange, m_eLoRange, m_eHiLimit, m_eLoLimit, 
               m_eFiltFactor, m_eDeadBand, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eHiRange, m_eLoRange, m_eHiLimit, m_eLoLimit, 
                 m_eFiltFactor, m_eDeadBand, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitQuadFilt diagnostics

#ifdef _DEBUG
void CUnitQuadFilt::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitQuadFilt::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

