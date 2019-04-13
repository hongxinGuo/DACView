// dlgQFLT.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitQFLT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQFLT dialog


CDlgQFLT::CDlgQFLT(CWnd* pParent /*=nullptr*/)
  : CDialog(CDlgQFLT::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgQFLT)
  m_strComment = "";
  m_eHiLimit = 10000.0;
  m_eHiRange = 10000.0;
  m_eLoLimit = 0.0;
  m_eLoRange = 0.0;
  m_strName = "";
  m_eFiltFactor = 0.0;
  m_eDeadBand = 0.0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgQFLT::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgQFLT)
  DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
  DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
  DDX_Text(pDX, IDC_EDIT_HIRANGE, m_eHiRange);
  DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
  DDX_Text(pDX, IDC_EDIT_LORANGE, m_eLoRange);
  DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
  DDX_Text(pDX, IDC_FiltFactor, m_eFiltFactor);
  DDV_MinMaxDouble(pDX, m_eFiltFactor, 0., 1.);
  DDX_Text(pDX, IDC_DeadBand, m_eDeadBand);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgQFLT, CDialog)
  //{{AFX_MSG_MAP(CDlgQFLT)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgQFLT::SetData(CString name, double HiRange, double LowRange, double HiLimit, 
                       double LowLimit, double FiltFactor, double DeadBand, double scanrate,
                       CString comment) {
  m_strName = name;
  m_eHiRange = HiRange;
  m_eLoRange = LowRange;
  m_eHiLimit = HiLimit;
  m_eLoLimit = LowLimit;
  m_eFiltFactor = FiltFactor;
  m_eDeadBand = DeadBand;
  m_eScanRate = scanrate / 1000;
  m_strComment = comment;
}

void CDlgQFLT::GetData(CString& name, double& HiRange, double &LowRange, double& HiLimit, double& LowLimit, 
                       double& FiltFactor, double& DeadBand, INT32& scanrate, CString& comment) {
  name = m_strName;
  HiRange = m_eHiRange;
  LowRange = m_eLoRange;
  HiLimit = m_eHiLimit;
  LowLimit = m_eLoLimit;
  FiltFactor = m_eFiltFactor;
  DeadBand = m_eDeadBand;
  scanrate = (INT32)(m_eScanRate * 1000);
  comment = m_strComment;
}  

/////////////////////////////////////////////////////////////////////////////
// CDlgQFLT message handlers
