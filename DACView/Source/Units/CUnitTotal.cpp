#include "stdafx.h"    

#include "..\\resource.h"
#include "cUnitBase.h"
#include "cUnitTotal.h" 
#include "DlgUnitTOT.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitTOT, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitTOT::sm_ptrParaName[] = 
  {{"Input", tINPUT |tMODIFIABLE | tDOUBLE, 0},
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,1},
   {"Gain",     tINPUT | tOUTPUT | tDOUBLE, 2},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 3},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 4}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  5},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 6}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 7},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 8},
   {"",         0, 9},
   }; 
   
INT32 CUnitTOT::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitTOT::sm_ulDoubleEnd = 6;
const ULONG CUnitTOT::sm_ulBoolEnd = 7;
const ULONG CUnitTOT::sm_ulWordEnd   = 8;
const ULONG CUnitTOT::sm_ulStringEnd = 8;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitTOT::CUnitTOT(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 25;
  m_rectArea.right  = pt.x + 25;
  m_rectArea.top    = pt.y - 20;
  m_rectArea.bottom = pt.y + 20;
  
	m_ulNumber = 10;  


  // initial parameter
  m_eInput = m_eInputOld = m_eOutputOld = m_eOutput = 0.0;
  m_peDataCurrent = m_peData.begin();
  m_eGain = 1.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitTOT::CUnitTOT( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  m_ulNumber = 10;  

  // initial parameter
  m_eInput = m_eInputOld = m_eOutputOld = m_eOutput = 0.0;
  m_peDataCurrent = m_peData.begin();
  m_eGain = 1.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitTOT::~CUnitTOT() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
} 
                              
void CUnitTOT::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

  if( ar.IsStoring() ) {
    ar << m_eGain << m_ulNumber;
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
  }
  else {
    ar >> m_eGain >> m_ulNumber;
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
		m_peDataCurrent = m_peData.begin();
    m_peData.resize(m_ulNumber);
		for ( ULONG i = 0; i < m_ulNumber; i++ ) {
			*m_peDataCurrent++ = 0.0;
		}
    m_peDataCurrent = m_peData.begin();;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitTOT::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 9));
  return(m_vfSelected[ulIndex]);
}

void CUnitTOT::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 9; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitTOT::GetClassNameStr( void ) {
  static CString str = "Total";
  return(str);
}

CString CUnitTOT::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitTOT::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitTOT::Exective( void ) {
  if ( m_fAutoExective ) {
    m_eOutput = m_eGain * (m_eOutputOld + (m_eInput + m_eInputOld)/2);
		m_eOutputOld = m_eOutput - *m_peDataCurrent;
		m_eInputOld = m_eInput;
		*m_peDataCurrent++ = m_eInput;
		if (m_peDataCurrent == m_peData.end()) {
			m_peDataCurrent = m_peData.begin();
		}
  }
	if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitTOT::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitTOT::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitTOT::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitTOT::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitTOT::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 7 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitTOT::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 7 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}

LONG CUnitTOT::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 8:               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitTOT::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 8 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitTOT::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulBoolEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput );
  case 1:               
    return ( m_eOutput );
  case 2 :              
    return ( m_eGain );
  case 3 :              
    return ( m_eHiRange );
  case 4:              
    return ( m_eLoRange );
  case 5 :              
    return ( m_eHiLimit );
  case 6 :
    return ( m_eLoLimit );
  default :
    break;
  };
  
  return(0);
} 

bool CUnitTOT::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0 : 
    m_eInput = eValue;
    break;
  case 1 :
    m_eOutput = eValue;
    break;
  case 2 :
    m_eGain = eValue;    
    break;
  case 3 :
    m_eHiRange = eValue;
    break;
  case 4 :
    m_eLoRange = eValue;
    break;
  case 5 :
    m_eHiLimit = eValue;
    break;
  case 6 :
    m_eLoLimit = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitTOT::ToShow( CDC * const pdc ) {
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    CBrush cb, * pcb;
    
    cb.CreateSolidBrush( RGB(255, 255, 255) );
    pcb = (CBrush *)pdc->SelectObject(&cb);
    ASSERT( pcb != nullptr );
    pdc->Rectangle(m_rectArea);           // draw myself
    int a = m_rectArea.left + 8;
    int b = ( m_rectArea.top + m_rectArea.bottom) / 2 - 8;
    pdc->TextOut( a, b, "TOT", 3 );
    pdc->SelectObject(pcb);
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitTOT::SetProperty( void ) {
  CDlgTOT CDlg;
  
  CDlg.SetData(m_strName, m_eGain, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_ulNumber, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eGain, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_ulNumber, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitTOT diagnostics

#ifdef _DEBUG
void CUnitTOT::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitTOT::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

