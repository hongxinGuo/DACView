#include "stdafx.h"    

#include "..\\resource.h"
#include "CUnitBase.h"
#include "cUnitHSel.h" 
#include "DlgUnitHSel.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitHighSelect, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitHighSelect::sm_ptrParaName[] = 
  {{"Input1", tINPUT |tMODIFIABLE | tDOUBLE, 0},
   {"Input2", tINPUT |tMODIFIABLE | tDOUBLE, 1},
   {"Input3", tINPUT |tMODIFIABLE | tDOUBLE, 2},
   {"Output", tOUTPUT |tMODIFIABLE | tDOUBLE,3},
   {"HiRange",tINPUT | tOUTPUT | tDOUBLE, 4},
   {"LowRange",tINPUT | tOUTPUT | tDOUBLE, 5}, 
   {"HiLimit",tINPUT | tOUTPUT | tDOUBLE,  6},
   {"LowLimit",tINPUT | tOUTPUT | tDOUBLE, 7}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 8},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 9},
   {"",         0, 10},
   }; 
   
INT32 CUnitHighSelect::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
   
const ULONG CUnitHighSelect::sm_ulDoubleEnd = 7;
const ULONG CUnitHighSelect::sm_ulBoolEnd = 8;
const ULONG CUnitHighSelect::sm_ulWordEnd   = 9;
const ULONG CUnitHighSelect::sm_ulStringEnd = 9;

////////////////////////////////////////////////////////////////////////////////                                          
                  

CUnitHighSelect::CUnitHighSelect(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left   = pt.x - 15;
  m_rectArea.right  = pt.x + 15;
  m_rectArea.top    = pt.y - 15;
  m_rectArea.bottom = pt.y + 15;
 
  // initial parameter
  m_eOutput = m_eInput1 = m_eInput2 = m_eInput3 = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

  m_pfSelected = new bool[10];
  for (int i = 0; i < 10; i++) {
    m_pfSelected[i] = false;
  }

}      

CUnitHighSelect::CUnitHighSelect( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
  
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_eOutput = m_eInput1 = m_eInput2 = m_eInput3 = 0.0;
  m_eHiRange = 100.0;
  m_eLoRange = 0.0;
  m_eHiLimit = 100.0;
  m_eLoLimit = 0.0;

  m_pfSelected = new bool[10];
  for (int i = 0; i < 10; i++) {
    m_pfSelected[i] = false;
  }

}

CUnitHighSelect::~CUnitHighSelect() {
} 
                              
void CUnitHighSelect::Serialize( CArchive& ar ) {
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

bool CUnitHighSelect::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 10));
  return(m_pfSelected[ulIndex]);
}

void CUnitHighSelect::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 10; i++) {
    m_pfSelected[i] = false;
  }
}


const CString& CUnitHighSelect::GetClassNameStr( void ) {
  static CString str = "HighSelect";
  return(str);
}

CString CUnitHighSelect::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitHighSelect::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitHighSelect::Exective( void ) {
  if ( m_fAutoExective ) {
	  if ( m_eInput1 > m_eInput2 ) {
	    if ( m_eInput1 > m_eInput3 ) {
	      m_eOutput = m_eInput1;
	    }
	    else {
	      m_eOutput = m_eInput3;
	    }
	  }
	  else {
	    if ( m_eInput2 > m_eInput3 ) {
	      m_eOutput = m_eInput2;
	    }
	    else {
	      m_eOutput = m_eInput3;
	    }
	  }
	}
  if ( (m_eOutput > m_eHiLimit) || ( m_eOutput < m_eLoLimit) ) {
		m_fOverFlow = true;;
	}
	else {
		m_fOverFlow = false;
	}
  
  ExectiveDynLink();
}

INT32 CUnitHighSelect::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitHighSelect::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitHighSelect::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitHighSelect::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitHighSelect::GetBool(ULONG index) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 8 :              
    return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitHighSelect::SetBool(ULONG index, bool fValue ) {
  ASSERT( (index <= sm_ulBoolEnd) && (index > sm_ulDoubleEnd) );
  switch ( index ) {
  case 8 :
    m_fAutoExective = fValue;
    break;
  default :
    return( false );
  }
  return( true );
}

LONG CUnitHighSelect::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 9 :               
    return ( (LONG)m_lScanRate );
  default :
    break;
  };
  
  return(0);
}

bool CUnitHighSelect::SetInteger(ULONG index, LONG lValue) {
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

double CUnitHighSelect::GetDouble(ULONG index) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0: 
    return ( m_eInput1 );
  case 1:               
    return ( m_eInput2 );
  case 2 :              
    return ( m_eInput3 );
  case 3 :
    return ( m_eOutput );
  case 4 :              
    return ( m_eHiRange );
  case 5:               
    return ( m_eLoRange );
  case 6 :              
    return ( m_eHiLimit );
  case 7 :
    return ( m_eLoLimit );
  default :
    break;
  };
  
  return(0);
} 


bool CUnitHighSelect::SetDouble(ULONG index, double eValue) {
  ASSERT( index <= sm_ulDoubleEnd );
  switch ( index ) {
  case 0 : 
    m_eInput1 = eValue;
    break;
  case 1 :
    m_eInput2 = eValue;
    break;
  case 2 :
    m_eInput3 = eValue;
    break;
  case 3 :
    m_eOutput = eValue;
    break;
  case 4 :
    m_eHiRange = eValue;
    break;
  case 5 :
    m_eLoRange = eValue;
    break;
  case 6 :
    m_eHiLimit = eValue;
    break;
  case 7 :
    m_eLoLimit = eValue;
    break;
  default :
    return( false );
  }
  return( true );
} 

void CUnitHighSelect::ToShow( CDC * const pdc ) {
  static bool fLoad = false;
  static CBitmap sm_bitmap;

  if ( !fLoad ) {
    VERIFY(sm_bitmap.LoadBitmap( IDB_UNIT_HSEL ));
    fLoad = true;
  }
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
    DrawBitmap( pdc, sm_bitmap, m_rectArea );
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitHighSelect::SetProperty( void ) {
  CDlgHSel CDlg;
  
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
// CUnitHighSelect diagnostics

#ifdef _DEBUG
void CUnitHighSelect::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitHighSelect::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

