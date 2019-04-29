#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "CUnitIPOP.h" 
#include "DlgUnitIPOP.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitInputOutput, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitInputOutput::sm_ptrParaName[] = 
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
   
INT32 CUnitInputOutput::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitInputOutput::sm_ulDoubleEnd = 5;
const ULONG CUnitInputOutput::sm_ulBoolEnd = 6;
const ULONG CUnitInputOutput::sm_ulWordEnd   = 7;
const ULONG CUnitInputOutput::sm_ulStringEnd = 7;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitInputOutput::CUnitInputOutput(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 20;
  m_rectArea.right  = pt.x + 20;
  m_rectArea.top    = pt.y - 30;
  m_rectArea.bottom = pt.y + 30;
  
  // initial parameter
  m_eInput = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitInputOutput::CUnitInputOutput( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  
  // initial parameter
  m_eInput = m_eOutput = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitInputOutput::~CUnitInputOutput() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitInputOutput::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

  if( ar.IsStoring() ) {
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
  }
  else {
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitInputOutput::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 8));
  return(m_vfSelected[ulIndex]);
}

void CUnitInputOutput::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 8; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitInputOutput::GetClassNameStr( void ) {
  static CString str = "IPOP";
  return(str);
}

bool CUnitInputOutput::SetExectivePriority( ULONG  ) {
	m_lExectivePriority = 1;	// this type unit mast calculate first
  return true;    // 单向输出型单元永远为真，执行优先级永远为最高的1
} 

CString CUnitInputOutput::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitInputOutput::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitInputOutput::Exective( void ) {
  if ( m_fAutoExective ) {
    m_eOutput = m_eInput;
  }
	if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

//////////////////////////////////////////////////////////////////////////////////
//
// CUnitInputOutput类不允许联入动态链接，只有输出参数
// 
//
/////////////////////////////////////////////////////////////////////////////////
ULONG CUnitInputOutput::GetUnitType( void ) {
  return( tOUTPUT );
}     

ULONG CUnitInputOutput::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

INT32 CUnitInputOutput::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitInputOutput::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitInputOutput::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

bool CUnitInputOutput::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 6 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitInputOutput::SetBool(ULONG index, bool fValue ) {
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

LONG CUnitInputOutput::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 7:               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitInputOutput::SetInteger(ULONG index, LONG lValue) {
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

double CUnitInputOutput::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput );
  case 1:               
    return ( m_eOutput );
  case 2 :              
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


bool CUnitInputOutput::SetDouble(ULONG index, double eValue) {
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

void CUnitInputOutput::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_IPOP ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitInputOutput::SetProperty( void ) {
  CDlgInputOutput CDlg;
  
  CDlg.SetData(m_strName, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitInputOutput diagnostics

#ifdef _DEBUG
void CUnitInputOutput::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitInputOutput::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

