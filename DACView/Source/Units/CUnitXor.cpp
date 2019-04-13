#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitXor.h" 
#include "DlgXor.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitXor, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitXor::sm_ptrParaName[] = 
  {{"Input1", tINPUT |tMODIFIABLE | tBOOL, 0},
   {"Input2", tINPUT |tMODIFIABLE | tBOOL, 1},
   {"Output", tOUTPUT |tMODIFIABLE | tBOOL,2},
   {"EAlarm", tINPUT |tMODIFIABLE | tBOOL, 3},
   {"Alarm",  tOUTPUT |tMODIFIABLE | tBOOL, 4}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 5},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 6},
   {"",         0, 7},
   }; 
   
INT32 CUnitXor::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitXor::sm_ulDoubleEnd = 0;
const ULONG CUnitXor::sm_ulBoolEnd = 5;
const ULONG CUnitXor::sm_ulWordEnd   = 6;
const ULONG CUnitXor::sm_ulStringEnd = 6;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitXor::CUnitXor(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_fInput1 = m_fInput2 = m_fOutput = false;
  m_fEnableAlarm = true;
  m_fAlarmHigh = true;
  m_fAlarm = false;

  m_pfSelected = new bool[7];
  for (int i = 0; i < 7; i++) {
    m_pfSelected[i] = false;
  }

}      

CUnitXor::CUnitXor( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  
  // initial parameter
  m_fInput1 = m_fInput2 = m_fOutput = false;
  m_fEnableAlarm = true;
  m_fAlarmHigh = true;
  m_fAlarm = false;

  m_pfSelected = new bool[7];
  for (int i = 0; i < 7; i++) {
    m_pfSelected[i] = false;
  }

}

CUnitXor::~CUnitXor() {
} 
                              
void CUnitXor::Serialize( CArchive& ar ) {
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

bool CUnitXor::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 7));
  return(m_pfSelected[ulIndex]);
}

void CUnitXor::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 7; i++) {
    m_pfSelected[i] = false;
  }
}

const CString& CUnitXor::GetClassNameStr( void ) {
  static CString str = "Xor";
  return(str);
}

CString CUnitXor::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitXor::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitXor::Exective( void ) {
  if ( m_fAutoExective ) {
	  m_fOutput = (m_fInput1 || m_fInput2) && (!(m_fInput1 && m_fInput2));
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

INT32 CUnitXor::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitXor::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitXor::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitXor::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitXor::GetBool(ULONG index) {
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

bool CUnitXor::SetBool(ULONG index, bool fValue) {
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
              
LONG CUnitXor::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 6 :              
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitXor::SetInteger(ULONG index, LONG lValue) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 6 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

void CUnitXor::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_XOR ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitXor::SetProperty( void ) {
  CDlgXor CDlg;
  
  CDlg.SetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitXor diagnostics

#ifdef _DEBUG
void CUnitXor::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitXor::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

