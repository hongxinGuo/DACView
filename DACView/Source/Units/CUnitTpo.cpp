#include "stdafx.h"    

#include "..\\resource.h"
#include "cUnitbase.h"
#include "cUnitTPO.h" 
#include "DlgUnitTPO.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitTPO, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitTPO::sm_ptrParaName[] = 
  {{"Input", tINPUT |tMODIFIABLE | tDOUBLE, 0},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 1},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 2}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  3},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 4}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 5},
	 {"Output", tOUTPUT |tMODIFIABLE | tBOOL, 6},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 7},
   {"",         0, 8},
   }; 
   
INT32 CUnitTPO::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitTPO::sm_ulDoubleEnd = 4;
const ULONG CUnitTPO::sm_ulBoolEnd = 6;
const ULONG CUnitTPO::sm_ulWordEnd   = 7;
const ULONG CUnitTPO::sm_ulStringEnd = 7;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitTPO::CUnitTPO(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 25;
  m_rectArea.right  = pt.x + 25;
  m_rectArea.top    = pt.y - 20;
  m_rectArea.bottom = pt.y + 20;

  // initial parameter
	m_eInput = 0.0;
  m_fOutput = false;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	m_ulPeriodTime = m_lScanRate * 32;

	m_fDo = false;

  m_pfSelected = new bool[8];
  for (int i = 0; i < 8; i++) {
    m_pfSelected[i] = false;
  }

}      

CUnitTPO::CUnitTPO( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
	m_eInput = 0.0;
  m_fOutput = false;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;
  
	m_ulPeriodTime = m_lScanRate * 32;
	m_fDo = false;

  m_pfSelected = new bool[8];
  for (int i = 0; i < 8; i++) {
    m_pfSelected[i] = false;
  }

}

CUnitTPO::~CUnitTPO() {
} 
                              
void CUnitTPO::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

  if( ar.IsStoring() ) {
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
		ar << m_ulPeriodTime;
  }
  else {
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
		ar >> m_ulPeriodTime;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitTPO::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 8));
  return(m_pfSelected[ulIndex]);
}

void CUnitTPO::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 8; i++) {
    m_pfSelected[i] = false;
  }
}


const CString& CUnitTPO::GetClassNameStr( void ) {
  static CString str = "TPO";
  return(str);
}

CString CUnitTPO::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitTPO::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitTPO::Exective( void ) {
  if ( m_fAutoExective ) {
		if ( m_fDo ) {
			if ( m_ulCountHighTime < m_ulHighTime ) {
				m_fOutput = true;
				m_ulCountHighTime += m_lScanRate;
			}
			else if ( m_ulCountLowTime < m_ulLowTime ) {
				m_fOutput = false;
				m_ulCountLowTime += m_lScanRate;
			}
			else {
				m_fDo = false;
			}
		}
		else {
			m_ulHighTime = (ULONG)(( m_eInput / (m_eHiRange - m_eLoRange) ) * m_ulPeriodTime);
			m_ulLowTime = m_ulPeriodTime - m_ulHighTime;
			m_ulCountHighTime = m_ulCountLowTime = 0;
			m_fDo = true;
		}
	}
	if ( (m_eInput > m_eHiLimit) || ( m_eInput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitTPO::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitTPO::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitTPO::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitTPO::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitTPO::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 5 :
		return ( m_fAutoExective );
  case 6 :              
    return ( m_fOutput );
	default :
    break;
  };
  return(0);
}

bool CUnitTPO::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 5 :
    m_fAutoExective = fValue;
    break;
  case 6 :
		m_fOutput = fValue;
		break;
	default :
    return( false );
  }
  return( true );
}

LONG CUnitTPO::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 7 :               
    return ( m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitTPO::SetInteger(ULONG index, LONG lValue) {
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

double CUnitTPO::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput );
  case 1:               
    return ( m_eHiRange );
  case 2:               
    return ( m_eLoRange );
  case 3 :              
    return ( m_eHiLimit );
  case 4 :
    return ( m_eLoLimit );
  default :
    break;
  };
  
  return(0);
} 


bool CUnitTPO::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0 : 
    m_eInput = eValue;
    break;
  case 1 :
    m_eHiRange = eValue;
    break;
  case 2 :
    m_eLoRange = eValue;
    break;
  case 3 :
    m_eHiLimit = eValue;
    break;
  case 4 :
    m_eLoLimit = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitTPO::ToShow( CDC * const pdc ) {
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    CBrush cb, * pcb;
    
    cb.CreateSolidBrush( RGB(255, 255, 255) );
    pcb = (CBrush *)pdc->SelectObject(&cb);
    ASSERT( pcb != nullptr );
    pdc->Rectangle(m_rectArea);           // draw myself
    int a = m_rectArea.left + 8;
    int b = ( m_rectArea.top + m_rectArea.bottom) / 2 - 8;
    pdc->TextOut( a, b, "TPO", 3 );
    pdc->SelectObject(pcb);
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitTPO::SetProperty( void ) {
  CDlgTPO CDlg;
  
  CDlg.SetData(m_strName, m_ulPeriodTime, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_ulPeriodTime, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitTPO diagnostics

#ifdef _DEBUG
void CUnitTPO::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitTPO::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

