#include "stdafx.h" 

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitAdd.h" 
#include "DlgUnitAdd.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitAdd, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitAdd::sm_ptrParaName[] = 
  {{"Input1", tINPUT |  tMODIFIABLE | tDOUBLE, 0},
   {"Input2", tINPUT |  tMODIFIABLE | tDOUBLE, 1},
   {"Output", tOUTPUT | tMODIFIABLE | tDOUBLE,2},
   {"K1",     tINPUT | tOUTPUT | tDOUBLE, 3},
   {"K2",     tINPUT | tOUTPUT | tDOUBLE, 4},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 5},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 6}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  7},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 8}, 
   {"AutoManual",tINPUT | tMODIFIABLE | tBOOL, 9},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 10},
   {"",         0, 11},
   }; 
   
INT32 CUnitAdd::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitAdd::sm_ulDoubleEnd = 8;
const ULONG CUnitAdd::sm_ulBoolEnd = 9;
const ULONG CUnitAdd::sm_ulWordEnd   = 10;
const ULONG CUnitAdd::sm_ulStringEnd = 10;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitAdd::CUnitAdd(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;
  
  // initial parameter
  m_eOutput = m_eInput1 = m_eInput2 = 0.0;
  m_eK1 = 1.0;
  m_eK2 = 1.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitAdd::CUnitAdd( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;
  
  // initial parameter
  m_eOutput = m_eInput1 = m_eInput2 = 0.0;
  m_eK1 = 1.0;
  m_eK2 = 1.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;
  
	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitAdd::~CUnitAdd() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitAdd::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );

  if( ar.IsStoring() ) {
    ar << m_eK1 << m_eK2;
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
  }
  else {
    ar >>m_eK1 >> m_eK2;
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitAdd::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 11));
  return(m_vfSelected[ulIndex]);
}

void CUnitAdd::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 11; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitAdd::GetClassNameStr( void ) {
  static CString str = "Add";
  return(str);
}

CString CUnitAdd::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitAdd::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitAdd::Exective( void ) {
  if ( m_fAutoExective ) {
    m_eOutput = m_eK1 * m_eInput1 + m_eK2 * m_eInput2;
  }
	if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitAdd::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitAdd::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitAdd::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitAdd::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitAdd::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 9 :              
    return ( m_fAutoExective );
  default :
		ASSERT( 0 );
    break;
  };
  return(0);
}

bool CUnitAdd::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 9 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}

LONG CUnitAdd::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 10 :               
    return ( m_lScanRate );
  default :
		ASSERT( 0 );
    break;
  };
  
  return(0);
}

bool CUnitAdd::SetInteger(ULONG index, LONG lValue) {
  ASSERT((index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 10 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

double CUnitAdd::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput1 );
  case 1:               
    return ( m_eInput2 );
  case 2 :              
    return ( m_eOutput );
  case 3 :
    return ( m_eK1 );
  case 4 :              
    return ( m_eK2 );
  case 5: 
    return ( m_eHiRange );
  case 6:               
    return ( m_eLoRange );
  case 7 :              
    return ( m_eHiLimit );
  case 8 :
    return ( m_eLoLimit );
  default :
		ASSERT( 0 );
    break;
  };
  
  return(0);
} 


bool CUnitAdd::SetDouble(ULONG index, double eValue) {
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
    m_eK1 = eValue;    
    break;
  case 4 :
    m_eK2 = eValue;
    break;
  case 5 : 
    m_eHiRange = eValue;
    break;
  case 6 :
    m_eLoRange = eValue;
    break;
  case 7 :
    m_eHiLimit = eValue;
    break;
  case 8 :
    m_eLoLimit = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitAdd::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_ADD ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitAdd::SetProperty( void ) {
  CDlgAdd CDlg;
  
  CDlg.SetData(m_strName, m_eK1, m_eK2, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eK1, m_eK2, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

bool CUnitAdd::DumpCurrentStatus( char * pch ) const {
  sprintf_s(pch, 100, "%s:  Input1 = %e   Input2 = %e   Output = %e", 
                m_strName, m_eInput1, m_eInput2, m_eOutput );
  return( true );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitAdd diagnostics

#ifdef _DEBUG
void CUnitAdd::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitAdd::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         