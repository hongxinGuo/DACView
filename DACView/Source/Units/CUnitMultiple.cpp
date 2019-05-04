#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitMultiple.h" 
#include "DlgUnitMultiple.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitMultiple, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitMultiple::sm_ptrParaName[] = 
  {{"Input1", tINPUT |tMODIFIABLE | tDOUBLE, 0},
   {"Input2", tINPUT |tMODIFIABLE | tDOUBLE, 1},
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,2},
   {"K1",     tINPUT | tOUTPUT | tDOUBLE, 3},
   {"K2",     tINPUT | tOUTPUT | tDOUBLE, 4},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 5},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 6}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  7},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 8}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 9},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 10},
   {"",         0, 11},
   }; 
   
INT32 CUnitMultiple::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitMultiple::sm_ulDoubleEnd = 8;
const ULONG CUnitMultiple::sm_ulBoolEnd = 9;
const ULONG CUnitMultiple::sm_ulWordEnd   = 10;
const ULONG CUnitMultiple::sm_ulStringEnd = 10;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitMultiple::CUnitMultiple(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
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

CUnitMultiple::CUnitMultiple( void ) : CUnitBase() { 
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

CUnitMultiple::~CUnitMultiple() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitMultiple::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    ar << m_eK1 << m_eK2;
    ar << m_eHiRange << m_eLoRange << m_eHiLimit << m_eLoLimit;
  }
  else {
    ar >> m_eK1 >> m_eK2;
    ar >> m_eHiRange >> m_eLoRange >> m_eHiLimit >> m_eLoLimit;
  }
} 

/////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitMultiple::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 11));
  return(m_vfSelected[ulIndex]);
}

void CUnitMultiple::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 11; i++) {
    m_vfSelected[i] = false;
  }
}


const CString& CUnitMultiple::GetClassNameStr( void ) {
  static CString str = "Multiple";
  return(str);
}

CString CUnitMultiple::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitMultiple::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitMultiple::Exective( void ) {
  if ( m_fAutoExective ) {
    m_eOutput = m_eK1 * m_eInput1 * m_eK2 * m_eInput2;
  }
  if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitMultiple::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitMultiple::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitMultiple::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitMultiple::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}
bool CUnitMultiple::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 9 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  
  return(0);
}

bool CUnitMultiple::SetBool(ULONG index, bool fValue) {
  ASSERT((index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 9 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}
         
LONG CUnitMultiple::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 10 :               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitMultiple::SetInteger(ULONG index, LONG lValue) {
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

double CUnitMultiple::GetDouble(ULONG index) {
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
    break;
  };
  
  return(0);
} 

bool CUnitMultiple::SetDouble(ULONG index, double eValue) {
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

void CUnitMultiple::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_MULTIPLE ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitMultiple::SetProperty( void ) {
  CDlgMultiple CDlg;
  
  CDlg.SetData(m_strName, m_eK1, m_eK2, m_eHiRange, m_eLoRange, 
               m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_eK1, m_eK2, m_eHiRange, m_eLoRange, 
                 m_eHiLimit, m_eLoLimit, m_lScanRate, m_strComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitMultiple diagnostics

#ifdef _DEBUG
void CUnitMultiple::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitMultiple::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectMultiple member function
