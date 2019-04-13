// dlgTOT.cpp : implementation file
//

#include "stdafx.h"
#include "..//resource.h"
#include "dlgUnitTOT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTOT dialog


CDlgTOT::CDlgTOT(CWnd* pParent /*=nullptr*/)
        : CDialog(CDlgTOT::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgTOT)
  m_strComment = "";
  m_eHiLimit = 10000.0;
  m_eHiRange = 10000.0;
  m_eGain = 1.0;
  m_eLoLimit = 0.0;
  m_eLoRange = 0.0;
  m_strName = "";
	m_ulNumber = 10;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgTOT::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgTOT)
  DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
  DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
  DDX_Text(pDX, IDC_EDIT_HIRANGE, m_eHiRange);
  DDX_Text(pDX, IDC_EDIT_GAIN, m_eGain);
  DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
  DDX_Text(pDX, IDC_EDIT_LORANGE, m_eLoRange);
  DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
  DDX_Text(pDX, IDC_EDIT_NUMBER, m_ulNumber);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTOT, CDialog)
  //{{AFX_MSG_MAP(CDlgTOT)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgTOT::SetData(CString name, double Gain, double HiRange, double LowRange, 
											double HiLimit, double LowLimit, INT32 ulNumber, double scanrate,
	                     CString comment) {
  m_strName = name;
  m_eGain = Gain;
  m_eHiRange = HiRange;
  m_eLoRange = LowRange;
  m_eHiLimit = HiLimit;
  m_eLoLimit = LowLimit;
	m_ulNumber = ulNumber;
  m_eScanRate = scanrate / 1000;
  m_strComment = comment;
}

void CDlgTOT::GetData(CString& name, double& Gain, double& HiRange, double &LowRange, 
											double& HiLimit, double& LowLimit, INT32& ulNumber, INT32& scanrate,
            					CString& comment) {
	name = m_strName;
  Gain = m_eGain;
  HiRange = m_eHiRange;
  LowRange = m_eLoRange;
  HiLimit = m_eHiLimit;
  LowLimit = m_eLoLimit;
	ulNumber = m_ulNumber;
  scanrate = (INT32)(m_eScanRate * 1000);
  comment = m_strComment;
}  

/////////////////////////////////////////////////////////////////////////////
// CDlgTOT message handlers
