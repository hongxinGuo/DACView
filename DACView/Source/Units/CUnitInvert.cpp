#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitInvert.h" 
#include "DlgUnitInvert.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitInvert, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitInvert::sm_ptrParaName[] = 
  {{"Input", tINPUT |tMODIFIABLE | tBOOL, 0},
   {"Output", tOUTPUT |tMODIFIABLE | tBOOL,1},
   {"EAlarm",  tOUTPUT |tMODIFIABLE | tBOOL, 2}, 
   {"Alarm", tINPUT |tMODIFIABLE | tBOOL, 3},
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 4},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 5},
   {"",         0, 6},
   }; 
   
INT32 CUnitInvert::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitInvert::sm_ulDoubleEnd = 0;
const ULONG CUnitInvert::sm_ulBoolEnd = 4;
const ULONG CUnitInvert::sm_ulWordEnd   = 5;
const ULONG CUnitInvert::sm_ulStringEnd = 5;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitInvert::CUnitInvert(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;
  
  // initial parameter
  m_fInput = m_fOutput = false;
  m_fEnableAlarm = 1;
  m_fAlarmHigh = 1;
  m_fAlarm = 0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitInvert::CUnitInvert( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_fInput = m_fOutput = false;
  m_fAlarmHigh = 1;
  m_fAlarm = 0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitInvert::~CUnitInvert() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitInvert::Serialize( CArchive& ar ) {
  INT64 a, b;
  
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << (INT64)m_fEnableAlarm << (INT64)m_fAlarmHigh;
  }
  else {
    ar >> a >> b;
    m_fEnableAlarm = (bool)a;
    m_fAlarmHigh = (bool)b;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitInvert::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 6));
  return(m_vfSelected[ulIndex]);
}

void CUnitInvert::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 6; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitInvert::GetClassNameStr( void ) {
  static CString str = "Invert";
  return(str);
}

CString CUnitInvert::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitInvert::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitInvert::Exective( void ) {
  if (  m_fAutoExective ) {
	  if ( m_fInput == false ) {
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

INT32 CUnitInvert::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitInvert::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitInvert::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitInvert::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitInvert::GetBool(ULONG index) {
  ASSERT( index <= sm_ulBoolEnd );
  switch ( index ) {
  case 0: 
    return ( m_fInput );
  case 1 :              
    return ( m_fOutput );
  case 2 :
    return (m_fEnableAlarm);
  case 3 :
    return ( m_fAlarm );
  case 4 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitInvert::SetBool(ULONG index, bool fValue) {
  ASSERT(index <= sm_ulBoolEnd);
  switch ( index ) {
  case 0: 
    m_fInput = fValue;
    break;
  case 1 :              
    m_fOutput = fValue;
    break;
  case 2 :              
    m_fEnableAlarm = fValue;
    break;
  case 3 :
    m_fAlarm = fValue;
    break;
  case 4 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}
         
LONG CUnitInvert::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 5 :              
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitInvert::SetInteger(ULONG index, LONG lValue) {
  ASSERT( ( index <= sm_ulWordEnd ) && ( index > sm_ulBoolEnd ) );
  switch ( index ) {
  case 5 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

void CUnitInvert::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_INVERT ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitInvert::SetProperty( void ) {
  CDlgInv CDlg;
  
  CDlg.SetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitInvert diagnostics

#ifdef _DEBUG
void CUnitInvert::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitInvert::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

