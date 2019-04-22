#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitPID.h" 
#include "DlgUnitPID.h"

#include "globedef.h"

IMPLEMENT_SERIAL(CUnitPID, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitPID::sm_ptrParaName[] = 
  {{"Meas", 			tINPUT |tMODIFIABLE | tDOUBLE,0},
   {"Output", 		tOUTPUT |tMODIFIABLE | tDOUBLE,1},
   {"SETP", 			tINPUT |tMODIFIABLE | tDOUBLE,2},
   {"PB",  				tINPUT | tOUTPUT | tDOUBLE, 3},
   {"INTG", 			tINPUT | tOUTPUT | tDOUBLE, 4},
	 {"DERV",   		tINPUT | tOUTPUT | tDOUBLE, 5},
   {"HiRange",		tINPUT | tOUTPUT | tDOUBLE, 6},
   {"LowRange",		tINPUT | tOUTPUT | tDOUBLE, 7}, 
   {"HiLimit",		tINPUT | tOUTPUT | tDOUBLE, 8},
   {"LowLimit",		tINPUT | tOUTPUT | tDOUBLE, 9}, 
   {"AutoManual",	tINPUT |tMODIFIABLE | tBOOL, 10},
   {"DALM",				tOUTPUT |tMODIFIABLE | tBOOL, 11},
   {"ScanRate",		tINPUT | tOUTPUT | tWORD, 12},
   {"",         0, 13},
   }; 
   
INT32 CUnitPID::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitPID::sm_ulDoubleEnd = 9;
const ULONG CUnitPID::sm_ulBoolEnd = 11;
const ULONG CUnitPID::sm_ulWordEnd   = 12;
const ULONG CUnitPID::sm_ulStringEnd = 12;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitPID::CUnitPID(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_eSetPoint = m_eMeas = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100;
  m_eLoLimit = 0.0;

	m_eDeriveTime = 600;
	m_eIntegralTime = 600;
	m_ulSampleNumber = 60;
	m_ulSampleTime = 10;
	m_ePB = 0.5;
	
	m_fEnable = true;

	m_eUp = m_eUp1 = m_eUi = m_eUi1 = m_eUd 
				= m_eUd1 = m_eEkh = m_eEkh1 = m_eKh = m_eKh1 = m_eB = 0.0;
	m_eSetPoint = 4;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitPID::CUnitPID( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_eSetPoint = m_eMeas = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	m_eDeriveTime = 600;
	m_eIntegralTime = 600;
	m_ulSampleNumber = 60;
	m_ulSampleTime = 10;
	m_ePB = 0.5;
	
	m_fEnable = true;

	m_eUp = m_eUp1 = m_eUi = m_eUi1 = m_eUd = m_eUd1 = 0.0;
	m_eEkh = m_eEkh1 = m_eKh = m_eKh1 = m_eB = 0.0;
	m_eSetPoint = 4;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitPID::~CUnitPID() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitPID::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

  if( ar.IsStoring() ) {
    ar << m_eDeriveTime << m_eIntegralTime << m_ulSampleTime 
    	 << m_ulSampleNumber << m_ePB;
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
  }
  else {
    ar >> m_eDeriveTime >> m_eIntegralTime >> m_ulSampleTime 
    	  >> m_ulSampleNumber >> m_ePB;
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitPID::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 13));
  return(m_vfSelected[ulIndex]);
}

void CUnitPID::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 13; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitPID::GetClassNameStr( void ) {
  static CString s_strClassName = "PID";
  return(s_strClassName);
}

CString CUnitPID::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitPID::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitPID::Exective( void ) {
  if ( m_fAutoExective ) {
   	m_eEkh = m_eSetPoint - m_eMeas;
		m_eUp = m_ePB * m_eEkh;
		m_eUi = m_eUi1 + m_ePB * m_ulSampleTime * m_eEkh / m_eIntegralTime;
		m_eB = 	m_eDeriveTime / ( m_eDeriveTime + m_ulSampleTime * m_ulSampleNumber);
		m_eUd = m_eB * m_eUd1 - m_ePB * m_eDeriveTime * ( 1 - m_eB) * ( m_eEkh - m_eEkh1) / m_ulSampleTime;
		m_eOutput =  (m_eUp + m_eUi + m_eUd) * ( m_eHiLimit - m_eLoLimit ) / (m_eHiRange - m_eLoRange);
		m_eMeas1 = m_eMeas;
		m_eUi1 = m_eUi;		// calculate Integral only when not overflow
		m_eUd1 = m_eUd;
		m_eEkh1 = m_eEkh;
  }
	if ( m_eOutput > m_eHiLimit ) {
		m_eOutput = m_eHiLimit;
		m_fOverFlow = true;;
	}
	else if ( m_eOutput < m_eLoLimit ) {
		m_eOutput = m_eLoLimit;
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitPID::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitPID::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitPID::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitPID::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitPID::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 10:              
    return ( m_fAutoExective );
  case 11 :               
		return ( m_fDALM );
  default :
    break;
  };
  return(0);
}

bool CUnitPID::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 10 :
    m_fAutoExective = fValue;
    break;
  case 11 :
    m_fDALM = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}

LONG CUnitPID::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
	case 12 :
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitPID::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 12 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitPID::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eMeas );
  case 1:               
    return ( m_eOutput );
  case 2 :              
    return ( m_eSetPoint );
  case 3 :
    return ( m_ePB );
  case 4 :              
    return ( m_eIntegralTime );
	case 5 :
		return ( m_eDeriveTime );
  case 6: 
    return ( m_eHiRange );
  case 7:               
    return ( m_eLoRange );
  case 8 :              
    return ( m_eHiLimit );
  case 9 :
    return ( m_eLoLimit );
  default :
    break;
  };
  
  return(0);
} 


bool CUnitPID::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0 : 
    m_eMeas = eValue;
    break;
  case 1 :
    m_eOutput = eValue;
    break;
  case 2 :
    m_eSetPoint = eValue;
    break;
  case 3 :
    m_ePB = eValue;    
    break;
  case 4 :
		m_eIntegralTime = eValue;
    break;
  case 5 : 
		m_eDeriveTime = eValue;
		break;
	case 6 :
    m_eHiRange = eValue;
    break;
  case 7 :
    m_eLoRange = eValue;
    break;
  case 8 :
    m_eHiLimit = eValue;
    break;
  case 9 :
    m_eLoLimit = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitPID::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_PID ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitPID::SetProperty( void ) {
  CDlgPID CDlg;
  
  CDlg.SetData(m_strName, m_ePB, m_eIntegralTime, m_eDeriveTime, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_ePB, m_eIntegralTime, m_eDeriveTime, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitPID diagnostics

#ifdef _DEBUG
void CUnitPID::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitPID::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

