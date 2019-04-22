#include "stdafx.h"    

#include "..\\resource.h"
#include "cUnitBase.h"
#include "cUnitTTB.h" 
#include "DlgUnitTTB.h"

#include "typedef.h"

IMPLEMENT_SERIAL(CUnitTTB, CUnitBase, 1 | VERSIONABLE_SCHEMA); 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

// globe static data declaration
ParaName CUnitTTB::sm_ptrParaName[] = 
  {{"Input1", tINPUT |tMODIFIABLE | tBOOL, 0},
   {"Input2", tINPUT |tMODIFIABLE | tBOOL, 1},
   {"Input3", tINPUT |tMODIFIABLE | tBOOL, 2},
   {"Input4", tINPUT |tMODIFIABLE | tBOOL, 3},
   {"Input5", tINPUT |tMODIFIABLE | tBOOL, 4},
   {"Input6", tINPUT |tMODIFIABLE | tBOOL, 5},
   {"Input7", tINPUT |tMODIFIABLE | tBOOL, 6},
   {"Input7", tINPUT |tMODIFIABLE | tBOOL, 7},
   {"Input8", tINPUT |tMODIFIABLE | tBOOL, 8},
   {"Input10", tINPUT |tMODIFIABLE | tBOOL, 9},
   {"Input11", tINPUT |tMODIFIABLE | tBOOL, 10},
   {"Input12", tINPUT |tMODIFIABLE | tBOOL, 11},
   {"Input13", tINPUT |tMODIFIABLE | tBOOL, 12},
   {"Input14", tINPUT |tMODIFIABLE | tBOOL, 13},
   {"Input15", tINPUT |tMODIFIABLE | tBOOL, 14},
   {"Input16", tINPUT |tMODIFIABLE | tBOOL, 15},
   {"Output1", tOUTPUT |tMODIFIABLE | tBOOL,16},
   {"Output2", tOUTPUT |tMODIFIABLE | tBOOL,17},
   {"Output3", tOUTPUT |tMODIFIABLE | tBOOL,18},
   {"Output4", tOUTPUT |tMODIFIABLE | tBOOL,19},
   {"Output5", tOUTPUT |tMODIFIABLE | tBOOL,20},
   {"Output6", tOUTPUT |tMODIFIABLE | tBOOL,21},
   {"Output7", tOUTPUT |tMODIFIABLE | tBOOL,22},
   {"Output8", tOUTPUT |tMODIFIABLE | tBOOL,23},
   {"Output9", tOUTPUT |tMODIFIABLE | tBOOL,24},
   {"Output10", tOUTPUT |tMODIFIABLE | tBOOL,25},
   {"Output11", tOUTPUT |tMODIFIABLE | tBOOL,26},
   {"Output12", tOUTPUT |tMODIFIABLE | tBOOL,27},
   {"Output13", tOUTPUT |tMODIFIABLE | tBOOL,28},
   {"Output14", tOUTPUT |tMODIFIABLE | tBOOL,29},
   {"Output15", tOUTPUT |tMODIFIABLE | tBOOL,30},
   {"Output16", tOUTPUT |tMODIFIABLE | tBOOL,31},
   {"Step1 True", tOUTPUT |tMODIFIABLE | tBOOL,32}, 
   {"Step2 True", tOUTPUT |tMODIFIABLE | tBOOL,33}, 
   {"Step3 True", tOUTPUT |tMODIFIABLE | tBOOL,34}, 
   {"Step4 True", tOUTPUT |tMODIFIABLE | tBOOL,35}, 
   {"Step5 True", tOUTPUT |tMODIFIABLE | tBOOL,36}, 
   {"Step6 True", tOUTPUT |tMODIFIABLE | tBOOL,37}, 
   {"Step7 True", tOUTPUT |tMODIFIABLE | tBOOL,38}, 
   {"Step8 True", tOUTPUT |tMODIFIABLE | tBOOL,39}, 
   {"AutoManual",tINPUT |tMODIFIABLE | tBOOL, 40},
   {"ScanRate",tINPUT | tOUTPUT | tWORD, 41},
   {"",                                   0, 42},
   }; 
         
INT32 CUnitTTB::sm_aulSuitable[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
										                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    
										                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    
										                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    
										                  0, 0 };
const ULONG CUnitTTB::sm_ulDoubleEnd = 0;
const ULONG CUnitTTB::sm_ulBoolEnd = 40;
const ULONG CUnitTTB::sm_ulWordEnd   = 41;
const ULONG CUnitTTB::sm_ulStringEnd = 41;

////////////////////////////////////////////////////////////////////////////////                                                                                                                                                                        
                                                                        

CUnitTTB::CUnitTTB(const CString& Name, CPoint pt): CUnitBase(Name, pt) {
  m_rectArea.left       = pt.x - 65;
  m_rectArea.right  = pt.x + 65;
  m_rectArea.top    = pt.y - 20;
  m_rectArea.bottom = pt.y + 20;

  // initial parameter
  m_ulNumberInput = 16;
  m_ulNumberStep = 8;
  m_bInitCoil = 0;
  m_wInitOutput = 0;
  for ( int i = 0; i < 8; i++ ) {
    m_bCoilPattern[i] = 0;
    m_bCoilPatternMask[i] = 0XFF;
    m_wInputPattern[i] = 0;
    m_wInputPatternMask[i] = 0XFFFF;
    m_bCoilOutput[i] = 0;
    m_bCoilOutputMask[i] = 0XFF;
    m_wDigitalOutput[i] = 0;
    m_wDigitalOutputMask[i] = 0XFFFF;
    m_fMatchingCriteria[i] = 0;
  }

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitTTB::CUnitTTB( void ) : CUnitBase() { 
  CRect rectTemp(0, 0, 0, 0);
        
  m_strName.Empty();
  m_rectArea = rectTemp;

  // initial parameter
  m_ulNumberInput = 16;
  m_ulNumberStep = 8;
  m_bInitCoil = 0;
  m_wInitOutput = 0;
  for ( int i = 0; i < 8; i++ ) {
    m_bCoilPattern[i] = 0;
    m_bCoilPatternMask[i] = 0XFF;
    m_wInputPattern[i] = 0;
    m_wInputPatternMask[i] = 0XFFFF;
    m_bCoilOutput[i] = 0;
    m_bCoilOutputMask[i] = 0XFF;
    m_wDigitalOutput[i] = 0;
    m_wDigitalOutputMask[i] = 0XFFFF;
    m_fMatchingCriteria[i] = 0;
  }

	for (int i = 0; i < sm_ulStringEnd + 1; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitTTB::~CUnitTTB() {
	ASSERT(m_vfSelected.size() == sm_ulStringEnd + 1);
}
                              
void CUnitTTB::Serialize( CArchive& ar ) {
  ULONG i, c;
  
  CUnitBase::Serialize( ar );
  if( ar.IsStoring() ) {
    for ( i = 0; i < 8; i++ ) {
      ar << m_bCoilPatternMask[i] << m_bCoilPattern[i] 
         << m_wInputPatternMask[i] <<m_wInputPattern[i] 
         << m_bCoilOutputMask[i] << m_bCoilOutput[i] 
         << m_wDigitalOutputMask[i] << m_wDigitalOutput[i]
         << (ULONG)m_fMatchingCriteria[i];
    }
    ar << m_ulNumberInput << m_ulNumberStep 
       << m_bInitCoil << m_wInitOutput;
  }
  else {
    for ( i = 0; i < 8; i++ ) {
      ar >> m_bCoilPatternMask[i] >> m_bCoilPattern[i] 
         >> m_wInputPatternMask[i] >> m_wInputPattern[i] 
         >> m_bCoilOutputMask[i] >> m_bCoilOutput[i] 
         >> m_wDigitalOutputMask[i] >> m_wDigitalOutput[i]
         >> c;
      m_fMatchingCriteria[i] = (bool)c;
    }
    ar >> m_ulNumberInput >> m_ulNumberStep 
       >> m_bInitCoil >> m_wInitOutput;
  }
} 

////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitTTB::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 42));
  return(m_vfSelected[ulIndex]);
}

void CUnitTTB::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 42; i++) {
    m_vfSelected[i] = false;
  }
}

const CString& CUnitTTB::GetClassNameStr( void ) {
  static CString str = "TTB";
  return(str);
}

CString CUnitTTB::GetParaName( ULONG index ) {
	return( sm_ptrParaName[index].Name );
}

ULONG CUnitTTB::GetParaType( ULONG index ) {
	return( sm_ptrParaName[index].ulType );
}

void CUnitTTB::Exective( void ) {
  WORD wInputTemp = 0;
  BYTE bCoilTemp;

  if ( m_fAutoExective ) {
	  for ( WORD i = 0; i < m_ulNumberStep; i++ ) {
	    bCoilTemp = m_bInnerRegister;
	    if ( m_fMatchingCriteria[i] ) { // ALL
	      wInputTemp = (WORD)(m_wInput & (m_wInputPatternMask[i] ^ (WORD)0x0ffff));
	      bCoilTemp = (BYTE)(m_bInnerRegister & (m_bCoilPatternMask[i] ^ (BYTE)0xff));
	      if ( (wInputTemp == m_wInputPattern[i]) && (bCoilTemp == m_bCoilPattern[i] ) ) {
	        m_bInnerRegister = m_bCoilOutput[i];
	        m_wOutput &= m_wDigitalOutputMask[i];
	        m_wOutput |= m_wDigitalOutput[i];
	      }
	    }
	    else { // ONE
	      wInputTemp = (WORD)(m_wInput & (m_wInputPatternMask[i] ^ (WORD)0XFFFF));
	      bCoilTemp = (BYTE)(m_bInnerRegister & (m_bCoilPatternMask[i] ^ (BYTE)0xff));
	      if ( (wInputTemp != 0X0000) || (bCoilTemp != (BYTE)0X00) ) {
	        m_bInnerRegister = m_bCoilOutput[i];
	        m_wOutput &= m_wDigitalOutputMask[i];
	        m_wOutput |= m_wDigitalOutput[i];
	      }
	    }       
	  }
	}

  ExectiveDynLink();
}

INT32 CUnitTTB::GetIndex( ULONG ulIndex ) {
  return( sm_aulSuitable[ulIndex] );
}                   

ParaName* CUnitTTB::GetParaNameAddress( void ) {
  return ( sm_ptrParaName );
}

INT32* CUnitTTB::GetArrayIndex( void ) {
	return( sm_aulSuitable );
}

ULONG CUnitTTB::GetDynLinkType(ULONG ulIndex) {
  return(sm_ptrParaName[ulIndex].ulType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUnitTTB::GetBool(ULONG index) {
  ASSERT( index <= sm_ulBoolEnd );
  switch ( index ) {
  case 0: 
    return ( m_fDI1 );
  case 1:               
    return ( m_fDI2 );
  case 2: 
    return ( m_fDI3 );
  case 3:               
	  return ( m_fDI4 );
	case 4: 
	  return ( m_fDI5 );
  case 5:               
    return ( m_fDI6 );
  case 6: 
    return ( m_fDI7 );
  case 7:               
    return ( m_fDI8 );
  case 8: 
    return ( m_fDI9 );
  case 9:               
    return ( m_fDI10 );
  case 10: 
    return ( m_fDI11 );
  case 11:               
    return ( m_fDI12 );
  case 12: 
    return ( m_fDI13 );
  case 13:               
    return ( m_fDI14 );
  case 14: 
    return ( m_fDI15 );
  case 15:               
    return ( m_fDI16 );
  case 16: 
    return ( m_fDO1 );
  case 17:               
    return ( m_fDO2 );
  case 18: 
    return ( m_fDO3 );
  case 19:               
    return ( m_fDO4 );
  case 20: 
    return ( m_fDO5 );
  case 21:               
    return ( m_fDO6 );
  case 22: 
    return ( m_fDO7 );
  case 23:               
    return ( m_fDO8 );
  case 24: 
    return ( m_fDO9 );
  case 25:               
    return ( m_fDO10 );
  case 26: 
    return ( m_fDO11 );
  case 27:               
    return ( m_fDO12 );
  case 28: 
    return ( m_fDO13 );
  case 29:               
    return ( m_fDO14 );
  case 30: 
    return ( m_fDO15 );
  case 31:               
  	return ( m_fDO16 );
  case 32:
    return ( m_fStep1 );
  case 33:
    return ( m_fStep2 );
  case 34:
    return ( m_fStep3 );
  case 35:
    return ( m_fStep4 );
  case 36:
    return ( m_fStep5 );
  case 37:
    return ( m_fStep6 );
  case 38:
    return ( m_fStep7 );
  case 39:
    return ( m_fStep8 );
  case 40:
  	return ( m_fAutoExective );
  default :
    break;
  };
  return(0);
}

bool CUnitTTB::SetBool(ULONG index, bool fValue) {
  ASSERT( index <= sm_ulBoolEnd );
  switch ( index ) {
  case 0: 
    m_fDI1 = fValue;
    break;
  case 1:               
    m_fDI2 = fValue;
    break;
  case 2: 
    m_fDI3 = fValue;
    break;
  case 3:               
	  m_fDI4 = fValue;
	  break;
  case 4: 
    m_fDI5 = fValue;
    break;
  case 5:               
    m_fDI6 = fValue;
    break;
  case 6: 
    m_fDI7 = fValue;
    break;
  case 7:               
    m_fDI8 = fValue;
    break;
  case 8: 
    m_fDI9 = fValue;
    break;
  case 9:               
    m_fDI10 = fValue;
    break;
  case 10: 
    m_fDI11 = fValue;
    break;
  case 11:               
    m_fDI12 = fValue;
    break;
  case 12: 
    m_fDI13 = fValue;
    break;
  case 13:               
    m_fDI14 = fValue;
    break;
  case 14: 
    m_fDI15 = fValue;
    break;
  case 15:               
    m_fDI16 = fValue;
    break;
  case 16: 
    m_fDO1 = fValue;
    break;
  case 17:               
    m_fDO2 = fValue;
    break;
  case 18: 
    m_fDO3 = fValue;
    break;
  case 19:               
    m_fDO4 = fValue;
    break;
  case 20: 
    m_fDO5 = fValue;
    break;
  case 21:               
    m_fDO6 = fValue;
    break;
  case 22: 
    m_fDO7 = fValue;
    break;
  case 23:               
    m_fDO8 = fValue;
    break;
  case 24: 
    m_fDO9 = fValue;
    break;
  case 25:               
    m_fDO10 = fValue;
    break;
  case 26: 
    m_fDO11 = fValue;
    break;
  case 27:               
    m_fDO12 = fValue;
    break;
  case 28: 
    m_fDO13 = fValue;
    break;
  case 29:               
    m_fDO14 = fValue;
    break;
  case 30: 
    m_fDO15 = fValue;
    break;
  case 31:               
    m_fDO16 = fValue;
    break;
  case 32 :
    m_fStep1 = fValue;
    break;
  case 33 :
    m_fStep2 = fValue;
    break;
  case 34 :
    m_fStep3 = fValue;
    break;
  case 35 :
    m_fStep4 = fValue;
    break;
  case 36 : 
    m_fStep5 = fValue;
    break;
  case 37 :
    m_fStep6 = fValue;
    break;
  case 38 :
    m_fStep7 = fValue;
    break;
  case 39 :
    m_fStep8 = fValue;
    break;
  case 40 :
	  m_fAutoExective = fValue;
	  break;
  default :
    return( false );
  }
  return( true );
}
                     
LONG CUnitTTB::GetInteger(ULONG index) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 41 :               
    return ( (LONG)m_lScanRate );
  default :
        break;
  };
  return(0);
}

bool CUnitTTB::SetInteger(ULONG index, LONG lValue) {
  ASSERT( (index <= sm_ulWordEnd) && (index > sm_ulBoolEnd) );
  switch ( index ) {
  case 41 :
    m_lScanRate = lValue;
    break;
  default :
    return( false );
  }
  return( true );
}

void CUnitTTB::ToShow( CDC * const pdc ) {
  if ( pdc->RectVisible(m_rectArea) ) {   // if I need to redraw ?
  CBrush cb, * pcb;
  
  cb.CreateSolidBrush( RGB(255, 255, 255) );
  pcb = (CBrush *)pdc->SelectObject(&cb);
  ASSERT( pcb != nullptr );
  pdc->Rectangle(m_rectArea);       // draw myself
  int a = m_rectArea.left + 8;
  int b = ( m_rectArea.top + m_rectArea.bottom) / 2 - 8;
  pdc->TextOut( a, b, m_strName, min(m_strName.GetLength(), 15) );
  pdc->SelectObject(pcb);
  }
	CUnitBase::ToShow(pdc);   // show status and dynamic link
}            

bool CUnitTTB::SetProperty( void ) {
  CDlgTTB CDlg;
  
  CDlg.SetData(m_strName, m_ulNumberInput, m_ulNumberStep, 
               m_bInitCoil, m_wInitOutput, m_bCoilPattern, m_bCoilPatternMask,
               m_wInputPattern, m_wInputPatternMask, m_bCoilOutput, m_bCoilOutputMask,
               m_wDigitalOutput, m_wDigitalOutputMask, m_fMatchingCriteria, 
               m_lScanRate, m_cstrComment);
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData(m_strName, m_ulNumberInput, m_ulNumberStep, 
	               m_bInitCoil, m_wInitOutput, m_bCoilPattern, m_bCoilPatternMask,
	               m_wInputPattern, m_wInputPatternMask, m_bCoilOutput, m_bCoilOutputMask,
	               m_wDigitalOutput, m_wDigitalOutputMask, m_fMatchingCriteria, 
	               m_lScanRate, m_cstrComment);
    return( true );
  }
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitTTB diagnostics

#ifdef _DEBUG
void CUnitTTB::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitTTB::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

