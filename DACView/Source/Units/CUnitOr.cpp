#include "stdafx.h"    

#include"globedef.h"

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitOr.h" 
#include "DlgUnitOr.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitOr, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitOr::sm_ptrParaName[] = 
  {{"Input1", tINPUT |tMODIFIABLE | tBOOL, 0},
   {"Input2", tINPUT |tMODIFIABLE | tBOOL, 1},
   {"Output", tOUTPUT |tMODIFIABLE | tBOOL,2},
   {"EAlarm", tINPUT |tMODIFIABLE | tBOOL, 3},
   {"Alarm",  tOUTPUT |tMODIFIABLE | tBOOL, 4}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 5},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 6},
   {"",         0, 7},
   }; 
   
INT32 CUnitOr::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitOr::sm_ulDoubleEnd = 0;
const ULONG CUnitOr::sm_ulBoolEnd = 5;
const ULONG CUnitOr::sm_ulWordEnd   = 6;
const ULONG CUnitOr::sm_ulStringEnd = 6;

int         CUnitOr::sm_iVersion = 2;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitOr::CUnitOr(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_fInput1 = m_fInput2 = m_fOutput = false;
  m_fEnableAlarm = true;
  m_fAlarmHigh = true;
  m_fAlarm = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitOr::CUnitOr( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  
  // initial parameter
  m_fInput1 = m_fInput2 = m_fOutput = false;
  m_fEnableAlarm = true;
  m_fAlarmHigh = true;
  m_fAlarm = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitOr::~CUnitOr() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitOr::Serialize( CArchive& ar ) {
  INT64 a, b;
  
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << (INT64)m_fEnableAlarm << (INT64)m_fAlarmHigh;
  }
  else {
    int iVersion = ar.GetObjectSchema();
    if (iVersion != -1) sm_iVersion = iVersion;
    switch (sm_iVersion) {
    case 1:
      ar >> a >> b;
      m_fEnableAlarm = (bool)a;
      m_fAlarmHigh = (bool)b;
      break;
    case 2:
      ar >> a >> b;
      m_fEnableAlarm = (bool)a;
      m_fAlarmHigh = (bool)b;
      break;
    default:
      break;
    }
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitOr::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 7));
  return(m_vfSelected[ulIndex]);
}

void CUnitOr::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 7; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitOr::GetClassNameStr( void ) {
  static CString str = "Or";
  return(str);
}

CString CUnitOr::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitOr::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitOr::Exective( void ) {
  if ( m_fAutoExective ) {
	  m_fOutput = m_fInput1 || m_fInput2;
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

INT32 CUnitOr::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitOr::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitOr::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitOr::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}
bool CUnitOr::GetBool(ULONG index) {
  ASSERT( index <= sm_ulBoolEnd );
  switch ( index ) {
  case 0: 
    return ( m_fInput1 );
  case 1:               
    return ( m_fInput2 );
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

bool CUnitOr::SetBool(ULONG index, bool fValue) {
  ASSERT( index <= sm_ulBoolEnd );
  switch ( index ) {
  case 0: 
    m_fInput1 = fValue;
    break;
  case 1:               
    m_fInput2 = fValue;
    break;
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
         
LONG CUnitOr::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 6 :              
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitOr::SetInteger(ULONG index, LONG lValue) {
  ASSERT( ( index <= sm_ulWordEnd ) && ( index > sm_ulBoolEnd ) );
  switch ( index ) {
 	case 6 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

void CUnitOr::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_OR ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitOr::SetProperty( void ) {
  CDlgOr CDlg;
  
  CDlg.SetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitOr diagnostics

#ifdef _DEBUG
void CUnitOr::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitOr::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

