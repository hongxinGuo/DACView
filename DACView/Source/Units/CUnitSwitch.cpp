#include "stdafx.h"    

#include "..\\resource.h"
#include "cUnitBase.h"
#include "cUnitSwitch.h" 
#include "DlgUnitSwitch.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitSwitch, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitSwitch::sm_ptrParaName[] = 
  {{"Input1", tINPUT |tMODIFIABLE | tDOUBLE, 0},
   {"Input2", tINPUT |tMODIFIABLE | tDOUBLE, 1},
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,2},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 3},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 4}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  5},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 6}, 
   {"SwitchTo1", tINPUT | tOUTPUT | tBOOL, 7}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 8},
   {"ScanRate",tINPUT | tOUTPUT | tWORD,  9},
   {"",         0, 10},
   }; 
   
INT32 CUnitSwitch::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitSwitch::sm_ulDoubleEnd = 6;
const ULONG CUnitSwitch::sm_ulBoolEnd = 8;
const ULONG CUnitSwitch::sm_ulWordEnd   = 9;
const ULONG CUnitSwitch::sm_ulStringEnd = 9;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitSwitch::CUnitSwitch(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_eInput1 = m_eInput2 = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;
  m_fSwitchTo1 = true;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitSwitch::CUnitSwitch( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_eInput1 = m_eInput2 = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;
  m_fSwitchTo1 = true;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitSwitch::~CUnitSwitch() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitSwitch::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );
  INT64 a;

  if( ar.IsStoring() ) {
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit << (INT64)m_fSwitchTo1;
  }
  else {
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit >> a;
    m_fSwitchTo1 = (bool)a;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitSwitch::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 10));
  return(m_vfSelected[ulIndex]);
}

void CUnitSwitch::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 10; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitSwitch::GetClassNameStr( void ) {
  static CString str = "UnitSwitch";
  return(str);
}

CString CUnitSwitch::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitSwitch::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitSwitch::Exective( void ) {
  if ( m_fAutoExective ) {
	  if ( m_fSwitchTo1 ) {
	    m_eOutput = m_eInput1;
	  }
	  else {
	    m_eOutput = m_eInput2;
	  }
	}
  
  ExectiveDynLink();
}

INT32 CUnitSwitch::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitSwitch::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitSwitch::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitSwitch::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}
bool CUnitSwitch::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 7 :              
    return ( m_fSwitchTo1 );
  case 8 :
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitSwitch::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 7 :
    m_fSwitchTo1 = fValue;
    break;
  case 8 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}

LONG CUnitSwitch::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 9 :               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitSwitch::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 9:
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitSwitch::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput1 );
  case 1:               
    return ( m_eInput2 );
  case 2 :              
    return ( m_eOutput );
  case 3: 
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


bool CUnitSwitch::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0 : 
    m_eInput1 = eValue;
    break;
  case 1 :
    m_eInput2 = eValue;
    break;
  case 2 :
    m_eOutput = eValue;
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

void CUnitSwitch::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_SWCH ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitSwitch::SetProperty( void ) {
  CDlgSwch CDlg;
  
  CDlg.SetData(m_strName, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitSwitch diagnostics

#ifdef _DEBUG
void CUnitSwitch::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitSwitch::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

