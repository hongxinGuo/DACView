#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitDelayTime.h" 
#include "DlgUnitDelayTime.h"

//#include "typedef.h"

IMPLEMENT_SERIAL(CUnitDelayTime, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitDelayTime::sm_ptrParaName[] = 
  {{"Input", tINPUT |tMODIFIABLE | tDOUBLE, 0},
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,1},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 2},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 3}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  4},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 5}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 6},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 7},
   {"",         0, 8},
   }; 
   
INT32 CUnitDelayTime::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitDelayTime::sm_ulDoubleEnd = 5;
const ULONG CUnitDelayTime::sm_ulBoolEnd = 6;
const ULONG CUnitDelayTime::sm_ulWordEnd   = 7;
const ULONG CUnitDelayTime::sm_ulStringEnd = 7;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitDelayTime::CUnitDelayTime(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_eInput = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	m_eDelayTime = 1.0;

  m_pDataCurrent = m_peData.begin();

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitDelayTime::CUnitDelayTime( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
 
  // initial parameter
  m_eOutput = m_eInput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	m_eDelayTime = 1.0;
  
  m_pDataCurrent = m_peData.begin();

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitDelayTime::~CUnitDelayTime() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitDelayTime::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

  if( ar.IsStoring() ) {
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit << m_eDelayTime;
  }
  else {
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit >> m_eDelayTime;
		m_ulDataLength = (ULONG)(((m_eDelayTime + 0.01) * 1000) / m_lScanRate);
		m_pDataCurrent = m_peData.begin();
    m_peData.resize(m_ulDataLength);
		for ( ULONG i = 0; i < m_ulDataLength; i ++ ) {
			m_peData[i] = 0.0;
		}
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitDelayTime::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 8));
  return(m_vfSelected[ulIndex]);
}

void CUnitDelayTime::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 8; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitDelayTime::GetClassNameStr( void ) {
  static CString str = "DelayTime";
  return(str);
}

CString CUnitDelayTime::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitDelayTime::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitDelayTime::Exective( void ) {
  if ( m_fAutoExective ) {
    *m_pDataCurrent++ = m_eInput;
		if (m_pDataCurrent == m_peData.end()) {
			m_pDataCurrent = m_peData.begin();
		}
		m_eOutput = *m_pDataCurrent;
  }
	if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitDelayTime::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitDelayTime::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitDelayTime::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitDelayTime::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitDelayTime::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 6 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitDelayTime::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 6 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}

LONG CUnitDelayTime::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 7 :               
    return ( m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitDelayTime::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 7 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitDelayTime::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput );
  case 1:               
    return ( m_eOutput );
  case 2: 
    return ( m_eHiRange );
  case 3:               
    return ( m_eLoRange );
  case 4 :              
    return ( m_eHiLimit );
  case 5 :
    return ( m_eLoLimit );
  default :
    break;
  };
  
  return(0);
} 


bool CUnitDelayTime::SetDouble(ULONG index, double eValue) {
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
  default :
    return( false );
  }
  return( true );
} 

void CUnitDelayTime::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_DTIME ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitDelayTime::SetProperty( void ) {
  CDlgDelayTime CDlg;
  
  CDlg.SetData(m_strName, m_eDelayTime, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eDelayTime, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitDelayTime diagnostics

#ifdef _DEBUG
void CUnitDelayTime::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitDelayTime::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

