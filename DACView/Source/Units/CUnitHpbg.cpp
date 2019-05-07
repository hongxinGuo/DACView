#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitHPBG.h" 
#include "DlgUnitHPBG.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitHighPassBargin, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitHighPassBargin::sm_ptrParaName[] = 
  {{"Input", tINPUT |tMODIFIABLE | tWORD | tDOUBLE, 0},
  {"Bargin", tINPUT |tMODIFIABLE | tWORD | tDOUBLE, 1},
   {"Output", tOUTPUT |tMODIFIABLE | tBOOL,2},
   {"EAlarm",  tOUTPUT |tMODIFIABLE | tBOOL, 3}, 
   {"Alarm", tINPUT |tMODIFIABLE | tBOOL, 4},
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 5},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 6},
   {"",         0, 7},
   }; 
   
INT32 CUnitHighPassBargin::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitHighPassBargin::sm_ulDoubleEnd = 1;
const ULONG CUnitHighPassBargin::sm_ulBoolEnd = 5;
const ULONG CUnitHighPassBargin::sm_ulWordEnd   = 6;
const ULONG CUnitHighPassBargin::sm_ulStringEnd = 6;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitHighPassBargin::CUnitHighPassBargin(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
	m_fOutput = false;
	m_eBargin = 1.0;
  m_fEnableAlarm = 1;
  m_fAlarmHigh = 1;
  m_fAlarm = 0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitHighPassBargin::CUnitHighPassBargin( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  
  // initial parameter
	m_fOutput = false;
	m_eBargin = 1.0;
  m_fAlarmHigh = 1;
  m_fAlarm = 0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitHighPassBargin::~CUnitHighPassBargin() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitHighPassBargin::Serialize( CArchive& ar ) {
  INT64 a, b, c;
  
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_eBargin << (INT64)m_fEnableAlarm << (INT64)m_fAlarmHigh << (INT64)m_fHighPass;
  }
  else {
    ar >> m_eBargin >> a >> b >> c;
    m_fEnableAlarm = (bool)a;
    m_fAlarmHigh = (bool)b;
    m_fHighPass = (bool)c;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitHighPassBargin::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 7));
  return(m_vfSelected[ulIndex]);
}

void CUnitHighPassBargin::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 7; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitHighPassBargin::GetClassNameStr( void ) {
  static CString str = "HPBG";
  return(str);
}

CString CUnitHighPassBargin::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitHighPassBargin::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitHighPassBargin::Exective( void ) {
  if ( m_fAutoExective ) {
    if ( m_eInput >= m_eBargin ) {
      m_fOutput = true;
    }
    else {
      m_fOutput = false;
    }
    if ( m_fEnableAlarm ) {
      if ( m_fAlarmHigh ) {
        if ( m_fOutput != false ) m_fAlarm = true;
        else m_fAlarm = false;
      }
      else {
        if ( m_fOutput == false ) m_fAlarm = true;
        else m_fAlarm = false;
      }
    }
  }
  
  ExectiveDynLink();
}

INT32 CUnitHighPassBargin::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitHighPassBargin::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitHighPassBargin::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitHighPassBargin::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitHighPassBargin::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && ( index > sm_ulDoubleEnd ) );
  switch ( index ) {
  case 2 :              
    return ( m_fOutput );
  case 3 :
    return ( m_fEnableAlarm );
  case 4 :   
    return ( m_fAlarm );
  case 5 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitHighPassBargin::SetBool(ULONG index, bool fValue) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
 case 2 :              
    m_fOutput = fValue;
    break;
  case 3 :              
    m_fEnableAlarm = fValue;
    break;
  case 4 :
    m_fAlarm = fValue;
    break;
  case 5 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}
         
LONG CUnitHighPassBargin::GetInteger(ULONG index) {
  ASSERT( (index == 0 ) || ((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd)) );
  switch ( index ) {
  case 6 :               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitHighPassBargin::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 6 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitHighPassBargin::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput );
    break;
  case 1:
    return(m_eBargin);
    break;
  default :
    break;
  };
  return(0);
}

bool CUnitHighPassBargin::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    m_eInput = eValue;
    break;
  case 1:
    m_eBargin = eValue;
    break;
  default :
    return(false);
  }
  return(true);
}


void CUnitHighPassBargin::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_HPBG ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitHighPassBargin::SetProperty( void ) {
  CDlgHighPassBargin CDlg;
  
  CDlg.SetData(m_strName, m_eBargin, m_fEnableAlarm, m_fAlarmHigh, m_fHighPass, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eBargin, m_fEnableAlarm, m_fAlarmHigh, m_fHighPass, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitHighPassBargin diagnostics

#ifdef _DEBUG
void CUnitHighPassBargin::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitHighPassBargin::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

