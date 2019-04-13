// dlgTTB.cpp : implementation file
//

#include "stdafx.h"
#include "..//resource.h"
#include "dlgUnitTTB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTTB dialog

static WORD wMask[16] = { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080,
                                                                                                  0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000 };
static BYTE bMask[8] =  { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080 };

int CDlgTTB::m_iCoilPattern[8] = {IDC_STEP1_COIL_PATTERN, IDC_STEP2_COIL_PATTERN, 
                                  IDC_STEP3_COIL_PATTERN, IDC_STEP4_COIL_PATTERN, 
                                  IDC_STEP5_COIL_PATTERN, IDC_STEP6_COIL_PATTERN, 
                                  IDC_STEP7_COIL_PATTERN, IDC_STEP8_COIL_PATTERN, };

int CDlgTTB::m_iCoilOutput[8] = { IDC_STEP1_COIL_OUTPUT, IDC_STEP2_COIL_OUTPUT, 
                                  IDC_STEP3_COIL_OUTPUT, IDC_STEP4_COIL_OUTPUT, 
                                  IDC_STEP5_COIL_OUTPUT, IDC_STEP6_COIL_OUTPUT, 
                                  IDC_STEP7_COIL_OUTPUT, IDC_STEP8_COIL_OUTPUT };

int CDlgTTB::m_iInputPattern[8] = { IDC_STEP1_INPUT_PATTERN, IDC_STEP2_INPUT_PATTERN,
                                    IDC_STEP3_INPUT_PATTERN, IDC_STEP4_INPUT_PATTERN,
                                    IDC_STEP5_INPUT_PATTERN, IDC_STEP6_INPUT_PATTERN,
                                    IDC_STEP7_INPUT_PATTERN, IDC_STEP8_INPUT_PATTERN,       };

int CDlgTTB::m_iDigitalOutput[8] = { IDC_STEP1_DIGITAL_OUTPUT, IDC_STEP2_DIGITAL_OUTPUT,
                                            IDC_STEP3_DIGITAL_OUTPUT, IDC_STEP4_DIGITAL_OUTPUT,
                                            IDC_STEP5_DIGITAL_OUTPUT, IDC_STEP6_DIGITAL_OUTPUT,
                                            IDC_STEP7_DIGITAL_OUTPUT, IDC_STEP8_DIGITAL_OUTPUT, };

int     CDlgTTB::m_iMatchingCriteria[8] = { IDC_STEP1_MC, IDC_STEP2_MC, IDC_STEP3_MC, IDC_STEP4_MC,
                                                        IDC_STEP5_MC, IDC_STEP6_MC, IDC_STEP7_MC, IDC_STEP8_MC, };

CDlgTTB::CDlgTTB(CWnd* pParent /*=nullptr*/)
        : CDialog(CDlgTTB::IDD, pParent)
{
        //{{AFX_DATA_INIT(CDlgTTB)
  m_strComment = "";
  m_strName = "";
  m_dwNumberInput = 0;
  m_dwNumberStep = 0;
  m_wInitOutput = 0;
  m_fStep1MC = FALSE;
  m_fStep2MC = FALSE;
  m_fStep3MC = FALSE;
  m_fStep4MC = FALSE;
  m_fStep5MC = FALSE;
  m_fStep6MC = FALSE;
  m_fStep7MC = FALSE;
  m_fStep8MC = FALSE;
        m_ulInitCoil = 0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
        
  for ( int i = 0; i < 8; i++ ) {
    int j;
    for ( j = 0; j < 8; j++ ) {
      m_strCoilPattern[i][j] = '.';
      m_strCoilOutput[i][j] = '.';
    }
    m_strCoilPattern[i][j] = 0x000;
    m_strCoilOutput[i][j] = 0x000;
    for ( j = 0; j < 16; j++ ) {
      m_strInputPattern[i][j] = '.';
      m_strDigitalOutput[i][j] = '.';
    }
    m_strInputPattern[i][j] = 0x000;
    m_strDigitalOutput[i][j] = 0x000;
  }
}


void CDlgTTB::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgTTB)
  DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
  DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
  DDX_Text(pDX, IDC_NUMBER_OF_INPUT, m_dwNumberInput);
  DDV_MinMaxDWord(pDX, m_dwNumberInput, 1, 16);
  DDX_Text(pDX, IDC_NUMBER_OF_STEP, m_dwNumberStep);
  DDV_MinMaxDWord(pDX, m_dwNumberStep, 2, 16);
  DDX_Text(pDX, IDC_INIT_OUTPUT, m_wInitOutput);
  DDX_Check(pDX, IDC_STEP1_MC, m_fStep1MC);
  DDX_Check(pDX, IDC_STEP2_MC, m_fStep2MC);
  DDX_Check(pDX, IDC_STEP3_MC, m_fStep3MC);
  DDX_Check(pDX, IDC_STEP4_MC, m_fStep4MC);
  DDX_Check(pDX, IDC_STEP5_MC, m_fStep5MC);
  DDX_Check(pDX, IDC_STEP6_MC, m_fStep6MC);
  DDX_Check(pDX, IDC_STEP7_MC, m_fStep7MC);
  DDX_Check(pDX, IDC_STEP8_MC, m_fStep8MC);
        DDX_Text(pDX, IDC_INIT_COIL, m_ulInitCoil);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 1000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTTB, CDialog)
  //{{AFX_MSG_MAP(CDlgTTB)
  ON_EN_KILLFOCUS(IDC_STEP1_COIL_OUTPUT, OnKillfocusStep1CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP1_COIL_PATTERN, OnKillfocusStep1CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP1_INPUT_PATTERN, OnKillfocusStep1InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP2_COIL_OUTPUT, OnKillfocusStep2CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP2_COIL_PATTERN, OnKillfocusStep2CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP2_DIGITAL_OUTPUT, OnKillfocusStep2DigitalOutput)
  ON_EN_KILLFOCUS(IDC_STEP2_INPUT_PATTERN, OnKillfocusStep2InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP3_COIL_OUTPUT, OnKillfocusStep3CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP3_COIL_PATTERN, OnKillfocusStep3CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP3_DIGITAL_OUTPUT, OnKillfocusStep3DigitalOutput)
  ON_EN_KILLFOCUS(IDC_STEP3_INPUT_PATTERN, OnKillfocusStep3InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP4_COIL_OUTPUT, OnKillfocusStep4CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP4_COIL_PATTERN, OnKillfocusStep4CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP4_DIGITAL_OUTPUT, OnKillfocusStep4DigitalOutput)
  ON_EN_KILLFOCUS(IDC_STEP4_INPUT_PATTERN, OnKillfocusStep4InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP5_COIL_OUTPUT, OnKillfocusStep5CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP5_COIL_PATTERN, OnKillfocusStep5CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP5_DIGITAL_OUTPUT, OnKillfocusStep5DigitalOutput)
  ON_EN_KILLFOCUS(IDC_STEP5_INPUT_PATTERN, OnKillfocusStep5InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP6_COIL_OUTPUT, OnKillfocusStep6CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP6_COIL_PATTERN, OnKillfocusStep6CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP6_DIGITAL_OUTPUT, OnKillfocusStep6DigitalOutput)
  ON_EN_KILLFOCUS(IDC_STEP6_INPUT_PATTERN, OnKillfocusStep6InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP7_COIL_OUTPUT, OnKillfocusStep7CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP7_COIL_PATTERN, OnKillfocusStep7CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP7_DIGITAL_OUTPUT, OnKillfocusStep7DigitalOutput)
  ON_EN_KILLFOCUS(IDC_STEP7_INPUT_PATTERN, OnKillfocusStep7InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP8_COIL_OUTPUT, OnKillfocusStep8CoilOutput)
  ON_EN_KILLFOCUS(IDC_STEP8_COIL_PATTERN, OnKillfocusStep8CoilPattern)
  ON_EN_KILLFOCUS(IDC_STEP8_DIGITAL_OUTPUT, OnKillfocusStep8DigitalOutput)
  ON_EN_KILLFOCUS(IDC_STEP8_INPUT_PATTERN, OnKillfocusStep8InputPattern)
  ON_EN_KILLFOCUS(IDC_STEP1_DIGITAL_OUTPUT, OnKillfocusStep1DigitalOutput)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgTTB::SetData(CString cstrName, INT32 ulNumberInput, INT32 ulNumberStep, 
                     BYTE bInitCoil, WORD wInitOutput, BYTE bCoilPattern[8], BYTE bCoilPatternMask[8],
                     WORD wInputPattern[8], WORD wInputPatternMask[8], BYTE bCoilOutput[8], 
                     BYTE bCoilOutputMask[8], WORD wDigitalOutput[8], WORD wDigitalOutputMask[8],
                     bool fMC[8], double ulScanRate, CString cstrComment) {
  m_strName = cstrName;
  m_dwNumberInput = ulNumberInput;
  m_dwNumberStep = ulNumberStep;
  m_ulInitCoil = bInitCoil;
  m_wInitOutput = wInitOutput;
  for ( int i = 0; i < 8; i++ ) {
    int j;
    for ( j = 0; j < 8; j++ ) {                                                      
      if ( (bCoilPatternMask[i] & bMask[j]) == 0 ) {  // if this bit valid
              if ( (bCoilPattern[i] & bMask[j]) != 0 ) {
                m_strCoilPattern[i][j] = '1';
              }
              else {
                m_strCoilPattern[i][j] = '0';
              }
      }
      if ( (bCoilOutputMask[i] & bMask[j]) == 0 ) {    // if this bit valid
        if ( (bCoilOutput[i] & bMask[j]) != 0 ) {
          m_strCoilOutput[i][j] = '1';
        }
        else {
          m_strCoilOutput[i][j] = '0';
        }
      }
    }
    for ( j = 0; j < 16; j++ ) {
      if ( (wInputPatternMask[i] & wMask[j]) == 0 ) {  // if this bit valid
        if ( (wInputPattern[i] & wMask[j]) != 0 ) {
          m_strInputPattern[i][j] = '1';
        }
        else { 
          m_strInputPattern[i][j] = '0';
        }
      }
      if ( (wDigitalOutputMask[i] & wMask[j]) == 0 ) {        // if this bit is valid
        if ( (wDigitalOutput[i] & wMask[j]) != 0 ) {
          m_strDigitalOutput[i][j] = '1';
        }
        else {
          m_strDigitalOutput[i][j] = '0';
        }
      }
    }
  }       

  m_fStep1MC = fMC[0];
  m_fStep2MC = fMC[1];
  m_fStep3MC = fMC[2];
  m_fStep4MC = fMC[3];
  m_fStep5MC = fMC[4];
  m_fStep6MC = fMC[5];
  m_fStep7MC = fMC[6];
  m_fStep8MC = fMC[7];

  m_eScanRate = ulScanRate / 1000;
  m_strComment = cstrComment;
  
}

void CDlgTTB::GetData(CString& name, INT32& ulNumberInput, INT32& ulNumberStep, 
                     BYTE& bInitCoil, WORD& wInitOutput, BYTE bCoilPattern[8], BYTE bCoilPatternMask[8],
                     WORD wInputPattern[8], WORD wInputPatternMask[8], BYTE bCoilOutput[8], 
                     BYTE bCoilOutputMask[8], WORD wDigitalOutput[8], WORD wDigitalOutputMask[8],
                     bool fMC[8], INT32& ulScanRate, CString& cstrComment) {
  static BYTE chMask = 0x0ff;
  static WORD wdMask = 0x0ffff;

  name = m_strName;
  ulNumberInput = m_dwNumberInput;
  ulNumberStep = m_dwNumberStep;
  bInitCoil = (BYTE)m_ulInitCoil;
  wInitOutput = (WORD)m_wInitOutput;
  for ( int i = 0; i < 8; i++ ) {
    int j;
    for ( j = 0; j < 8; j++ ) {
      switch ( m_strCoilPattern[i][j] ) {
      case '1':
        bCoilPatternMask[i] &= (chMask ^ bMask[j]);
        bCoilPattern[i] |= bMask[j];
        break;
      case '0':
        bCoilPatternMask[i] &= (chMask ^ bMask[j]);
        bCoilPattern[i] &= (chMask ^ bMask[j]);
        break;
      case '.':
        bCoilPatternMask[i] |= bMask[j];
        bCoilPattern[i] &= (chMask ^ bMask[j]);
        break;
      default :
        ASSERT(FALSE);
      }       // switch 
      switch ( m_strCoilOutput[i][j] ) {
      case '1':
              bCoilOutputMask[i] &= (chMask ^ bMask[j]);
              bCoilOutput[i] |= bMask[j];
              break;
      case '0':
              bCoilOutputMask[i] &= (chMask ^ bMask[j]);
              bCoilOutput[i] &= (chMask ^ bMask[j]);
              break;
      case '.':
              bCoilOutputMask[i] |= bMask[j];
              bCoilOutput[i] &= (chMask ^ bMask[j]);
              break;
      default :
              ASSERT(FALSE);
      }        // switch
    } // for j
    for ( j = 0; j < 16; j++ ) {
    switch ( m_strInputPattern[i][j] ) {
      case '1':
              wInputPatternMask[i] &= (wdMask ^ wMask[j]);
              wInputPattern[i] |= wMask[j];
              break;
      case '0':
              wInputPatternMask[i] &= (wdMask ^ wMask[j]);
              wInputPattern[i] &= (wdMask ^ wMask[j]);
              break;
      case '.':
              wInputPatternMask[i] |= wMask[j];
              wInputPattern[i] &= (wdMask ^ wMask[j]);
              break;
      default :
              ASSERT(FALSE);
      }       // switch 
      switch ( m_strDigitalOutput[i][j] ) {
      case '1':
              wDigitalOutputMask[i] &= (wdMask ^ wMask[j]);
              wDigitalOutput[i] |= wMask[j];
              break;
      case '0':
              wDigitalOutputMask[i] &= (wdMask ^ wMask[j]);
              wDigitalOutput[i] &= (wdMask ^ wMask[j]);
              break;
      case '.':
              wDigitalOutputMask[i] |= wMask[j];
              wDigitalOutput[i] &= (wdMask ^ wMask[j]);
              break;
      default :
              ASSERT(FALSE);
      }        // switch
    } // for j
  } // for i
  fMC[0] = m_fStep1MC;
  fMC[1] = m_fStep2MC;
  fMC[2] = m_fStep3MC;
  fMC[3] = m_fStep4MC;
  fMC[4] = m_fStep5MC;
  fMC[5] = m_fStep6MC;
  fMC[6] = m_fStep7MC;
  fMC[7] = m_fStep8MC;

  ulScanRate = (INT32)(m_eScanRate * 1000);
  cstrComment = m_strComment;
}  

void CDlgTTB::UpdateString( int iLabel, char * strName, int iNumber) {
  char pch[18];
  int iChNum;
  BOOL fCopy = TRUE;

  for ( iChNum = 0; iChNum < 18; iChNum++ ) {
    pch[iChNum] = 0x00;
  }
  pch[0] = (char)0xffb;
  iChNum = SendDlgItemMessage(iLabel, EM_GETLINE, (WPARAM)0L, (LPARAM)(LPSTR)pch);
  if ( iChNum !=  iNumber ) {
    fCopy = FALSE;
  }
  for ( int i = 0; i < iNumber; i++ ) {
    switch ( pch[i] ) {
    case '1':
    case '0':
    case '.':
      break;
    default :
      fCopy = FALSE;
      break;
    } // switch 
  }
  if ( fCopy ) {
    strncpy(strName, pch, iNumber);
  }
  else {
    Beep(800, 500);
    SendDlgItemMessage(iLabel, WM_SETTEXT, 0L, (LPARAM)(LPSTR)strName);
    SendDlgItemMessage(iLabel, EM_SETSEL, (WPARAM)0L, (LPARAM)iNumber); 
  }
}
                
/////////////////////////////////////////////////////////////////////////////
// CDlgTTB message handlers

void CDlgTTB::OnOK() 
{
  // TODO: Add extra validation here
        
  CDialog::OnOK();
}

void CDlgTTB::OnKillfocusStep1CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP1_COIL_OUTPUT, m_strCoilOutput[0], 8);
}

void CDlgTTB::OnKillfocusStep1CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP1_COIL_PATTERN, m_strCoilPattern[0], 8);
}

void CDlgTTB::OnKillfocusStep1InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP1_INPUT_PATTERN, m_strInputPattern[0], 16);
}

void CDlgTTB::OnKillfocusStep1DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP1_DIGITAL_OUTPUT, m_strDigitalOutput[0], 16);
}

void CDlgTTB::OnKillfocusStep2CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP2_COIL_OUTPUT, m_strCoilOutput[1], 8);
}

void CDlgTTB::OnKillfocusStep2CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP2_COIL_PATTERN, m_strCoilPattern[1], 8);
}

void CDlgTTB::OnKillfocusStep2DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP2_DIGITAL_OUTPUT, m_strDigitalOutput[1], 16);
}

void CDlgTTB::OnKillfocusStep2InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP2_INPUT_PATTERN, m_strInputPattern[1], 16);
}

void CDlgTTB::OnKillfocusStep3CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP3_COIL_OUTPUT, m_strCoilOutput[2], 8);
}

void CDlgTTB::OnKillfocusStep3CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP3_COIL_PATTERN, m_strCoilPattern[2], 8);
}

void CDlgTTB::OnKillfocusStep3DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP3_DIGITAL_OUTPUT, m_strDigitalOutput[2], 16);
}

void CDlgTTB::OnKillfocusStep3InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP3_INPUT_PATTERN, m_strInputPattern[2], 16);
}

void CDlgTTB::OnKillfocusStep4CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP4_COIL_OUTPUT, m_strCoilOutput[3], 8);
}

void CDlgTTB::OnKillfocusStep4CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP4_COIL_PATTERN, m_strCoilPattern[3], 8);
}

void CDlgTTB::OnKillfocusStep4DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP4_DIGITAL_OUTPUT, m_strDigitalOutput[3], 16);
}

void CDlgTTB::OnKillfocusStep4InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP4_INPUT_PATTERN, m_strInputPattern[3], 16);
}

void CDlgTTB::OnKillfocusStep5CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP5_COIL_OUTPUT, m_strCoilOutput[4], 8);
}

void CDlgTTB::OnKillfocusStep5CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP5_COIL_PATTERN, m_strCoilPattern[4], 8);
}

void CDlgTTB::OnKillfocusStep5DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP5_DIGITAL_OUTPUT, m_strDigitalOutput[4], 16);
}

void CDlgTTB::OnKillfocusStep5InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP5_INPUT_PATTERN, m_strInputPattern[4], 16);
}

void CDlgTTB::OnKillfocusStep6CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP6_COIL_OUTPUT, m_strCoilOutput[5], 8);
}

void CDlgTTB::OnKillfocusStep6CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP6_COIL_PATTERN, m_strCoilPattern[5], 8);
}

void CDlgTTB::OnKillfocusStep6DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP6_DIGITAL_OUTPUT, m_strDigitalOutput[5], 16);
}

void CDlgTTB::OnKillfocusStep6InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP6_INPUT_PATTERN, m_strInputPattern[5], 16);
}

void CDlgTTB::OnKillfocusStep7CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP7_COIL_OUTPUT, m_strCoilOutput[6], 8);
}

void CDlgTTB::OnKillfocusStep7CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP7_COIL_PATTERN, m_strCoilPattern[6], 8);   
}

void CDlgTTB::OnKillfocusStep7DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP7_DIGITAL_OUTPUT, m_strDigitalOutput[6], 16);
}

void CDlgTTB::OnKillfocusStep7InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP7_INPUT_PATTERN, m_strInputPattern[6], 16);
}

void CDlgTTB::OnKillfocusStep8CoilOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP8_COIL_OUTPUT, m_strCoilOutput[7], 8);
}

void CDlgTTB::OnKillfocusStep8CoilPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP8_COIL_PATTERN, m_strCoilPattern[7], 8);
}

void CDlgTTB::OnKillfocusStep8DigitalOutput() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP8_DIGITAL_OUTPUT, m_strDigitalOutput[7], 16);
}

void CDlgTTB::OnKillfocusStep8InputPattern() 
{
  // TODO: Add your control notification handler code here
  UpdateString(IDC_STEP8_INPUT_PATTERN, m_strInputPattern[7], 16);
}

BOOL CDlgTTB::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  // TODO: Add extra initialization here
  for ( int i = 0; i < 8; i++ ) {
    SendDlgItemMessage(m_iCoilPattern[i], WM_SETTEXT, 0L, (LPARAM)(LPSTR)m_strCoilPattern[i]);
    SendDlgItemMessage(m_iCoilOutput[i], WM_SETTEXT, 0L, (LPARAM)(LPSTR)m_strCoilOutput[i]);
    SendDlgItemMessage(m_iInputPattern[i], WM_SETTEXT, 0L, (LPARAM)(LPSTR)m_strInputPattern[i]);
    SendDlgItemMessage(m_iDigitalOutput[i], WM_SETTEXT, 0L, (LPARAM)(LPSTR)m_strDigitalOutput[i]);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
                                                        // EXCEPTION: OCX Property Pages should return FALSE
}
