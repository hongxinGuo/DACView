#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitFFLP.h" 
#include "DlgUnitFflp.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitFFLP, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitFFLP::sm_ptrParaName[] = 
  {{"InputStatus", tINPUT |tMODIFIABLE | tBOOL, 0},
   {"InputFlip", tINPUT |tMODIFIABLE | tBOOL, 1},
   {"Output", tOUTPUT |tMODIFIABLE | tBOOL,2},
   {"EAlarm", tINPUT |tMODIFIABLE | tBOOL, 3},
   {"Alarm",  tOUTPUT |tMODIFIABLE | tBOOL, 4}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 5},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 6},
   {"",         0, 7},
   }; 
   
INT32 CUnitFFLP::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitFFLP::sm_ulDoubleEnd = 0;
const ULONG CUnitFFLP::sm_ulBoolEnd = 5;
const ULONG CUnitFFLP::sm_ulWordEnd   = 6;
const ULONG CUnitFFLP::sm_ulStringEnd = 6;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitFFLP::CUnitFFLP(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;
  
  // initial parameter
	m_fInputStatus = m_fInputFlip = m_fOutput = false;
  m_fInputFlipBefore = false;
  m_fEnableAlarm = true;
  m_fAlarmHigh = true;
  m_fAlarm = false;

  m_pfSelected = new bool[7];
  for (int i = 0; i < 7; i++) {
    m_pfSelected[i] = false;
  }

}      

CUnitFFLP::CUnitFFLP( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  
  // initial parameter
  m_fInputStatus = m_fInputFlip = m_fOutput = false;
  m_fInputFlipBefore = false;
  m_fEnableAlarm = false;
  m_fAlarmHigh = false;
  m_fAlarm = false;

  m_pfSelected = new bool[7];
  for (int i = 0; i < 7; i++) {
    m_pfSelected[i] = false;
  }

}

CUnitFFLP::~CUnitFFLP() {
} 
                              
void CUnitFFLP::Serialize( CArchive& ar ) {
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

bool CUnitFFLP::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 7));
  return(m_pfSelected[ulIndex]);
}

void CUnitFFLP::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 7; i++) {
    m_pfSelected[i] = false;
  }
}


const CString& CUnitFFLP::GetClassNameStr( void ) {
  static CString str = "FFLP";
  return(str);
}

CString CUnitFFLP::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitFFLP::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

////////////////////////////////////////////////////////////////
//
// 信号触发器。
//
// 当输入信号为0时，输出也为0，系统复原；当输入信号为1时，当允许触发信号
// 输入时，输出为1。
//
////////////////////////////////////////////////////////////////
void CUnitFFLP::Exective( void ) {
  if ( m_fAutoExective ) {
    if ( m_fInputStatus  == false ) {
    	m_fOutput = false;
			m_fInputFlipBefore = false;
		}
    else {
      if ( m_fInputFlipBefore== false ) {
        if ( m_fInputFlip == true ) {
          m_fInputFlipBefore = true;
          m_fOutput = true;
        }
      }
    }
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
  
  ExectiveDynLink();
}

INT32 CUnitFFLP::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitFFLP::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitFFLP::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitFFLP::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitFFLP::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd)  );
  switch ( index ) {
  case 0: 
    return ( m_fInputStatus );
  case 1:               
    return ( m_fInputFlip );
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

bool CUnitFFLP::SetBool(ULONG index, bool fValue) {
  ASSERT(index <= sm_ulBoolEnd);
  switch ( index ) {
  case 0: 
    m_fInputStatus = fValue;
    break;
  case 1:               
    m_fInputFlip = fValue;
    break;
  case 2 :              
    // Output为输出，不能设置其状态
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
         
LONG CUnitFFLP::GetInteger(ULONG index) {
  ASSERT( ( index <= sm_ulWordEnd ) && ( index > sm_ulBoolEnd ) );
  switch ( index ) {
  case 6 :               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitFFLP::SetInteger(ULONG index, LONG lValue) {
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

void CUnitFFLP::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_FFLP ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitFFLP::SetProperty( void ) {
  CDlgFFLP CDlg;
  
  CDlg.SetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitFFLP diagnostics

#ifdef _DEBUG
void CUnitFFLP::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitFFLP::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

