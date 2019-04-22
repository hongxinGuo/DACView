#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitAnd.h" 
#include "DlgUnitAnd.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitAnd, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitAnd::sm_ptrParaName[] = 
  {{"Input1", tINPUT | tMODIFIABLE | tBOOL, 0},
   {"Input2", tINPUT | tMODIFIABLE | tBOOL, 1},
   {"Output", tOUTPUT | tMODIFIABLE | tBOOL,2},
   {"EAlarm", tINPUT |  tMODIFIABLE | tBOOL, 3},
   {"Alarm",  tOUTPUT | tMODIFIABLE | tBOOL, 4}, 
   {"AutoManual",tINPUT | tMODIFIABLE | tBOOL, 5},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 6},
   {"",         0, 7},
   }; 

// 在生成合适的字典时，存储参数位置的索引
// 如CUnitAnd类需要输出型（tOUTPUT)的字典，则只有第二项和第四项适合，则sm_aulSuitable[] = { 2, 4, 0, 0, 0, 0, 0 }
INT32 CUnitAnd::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitAnd::sm_ulDoubleEnd = 0;
const ULONG CUnitAnd::sm_ulBoolEnd = 5;
const ULONG CUnitAnd::sm_ulWordEnd   = 6;
const ULONG CUnitAnd::sm_ulStringEnd = 6;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitAnd::CUnitAnd(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;

  // initial parameter
  m_fEnableAlarm = true;
  m_fAlarmHigh = true;
  m_fAlarm = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitAnd::CUnitAnd( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_fEnableAlarm = true;
  m_fAlarmHigh = true;
  m_fAlarm = false;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitAnd::~CUnitAnd() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
} 
                              
void CUnitAnd::Serialize( CArchive& ar ) {
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

bool CUnitAnd::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 7));
  return(m_vfSelected[ulIndex]);
}

void CUnitAnd::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 7; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitAnd::GetClassNameStr( void ) {
  static CString str = "And";
  return(str);
}

CString CUnitAnd::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitAnd::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitAnd::Exective( void ) {
  if ( m_fAutoExective ) {
    m_fOutput = m_fInput1 && m_fInput2;
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

INT32 CUnitAnd::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitAnd::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitAnd::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitAnd::GetDynLinkType( ULONG ulIndex ) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitAnd::GetBool(ULONG index) {
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

bool CUnitAnd::SetBool(ULONG index, bool fValue) {
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
         
LONG CUnitAnd::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && ( index > sm_ulBoolEnd ) );
  switch ( index ) {
  case 6 :              
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  return(0);
}

bool CUnitAnd::SetInteger(ULONG index, LONG lValue) {
  ASSERT( ( index <= sm_ulWordEnd) && ( index > sm_ulBoolEnd ) );
  switch ( index ) {
 case 6 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

void CUnitAnd::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_AND ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitAnd::SetProperty( void ) {
  CDlgAnd CDlg;
  
  CDlg.SetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_fEnableAlarm, m_fAlarmHigh, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitAnd diagnostics

#ifdef _DEBUG
void CUnitAnd::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitAnd::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// CUnitAnd member function
bool CUnitAnd::DumpCurrentStatus( char * pch ) const {
	CString str1, str2, str3;
	if ( m_fInput1 ) str1 = "true"; else str1 = "false";
	if ( m_fInput2 ) str2 = "true"; else str2= "false";
	if ( m_fOutput ) str3 = "true"; else str3 = "false";
  sprintf(pch, "%s : Input1 = %s, Input2 = %s, Output = %s", 
								m_strName, str1, str2, str3);
  return( true );
}

