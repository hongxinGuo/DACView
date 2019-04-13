// dlgAdd.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitPID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPID dialog


CDlgPID::CDlgPID(CWnd* pParent /*=nullptr*/)
        : CDialog(CDlgPID::IDD, pParent)
{
        //{{AFX_DATA_INIT(CDlgPID)
        m_strComment = "";
        m_eHiLimit = 10000.0;
        m_eHiRange = 10000.0;
        m_ePB = 1.0;
        m_eINTG = 600;
				m_eDERV = 600;
        m_eLoLimit = 0.0;
        m_eLoRange = 0.0;
        m_strName = "";
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgPID::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDlgPID)
        DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
        DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
        DDX_Text(pDX, IDC_EDIT_HIRANGE, m_eHiRange);
        DDX_Text(pDX, IDC_EDIT_PB, m_ePB);
        DDX_Text(pDX, IDC_EDIT_INTG, m_eINTG);
        DDX_Text(pDX, IDC_EDIT_DERV, m_eDERV);
        DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
        DDX_Text(pDX, IDC_EDIT_LORANGE, m_eLoRange);
        DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPID, CDialog)
        //{{AFX_MSG_MAP(CDlgPID)
                // NOTE: the ClassWizard will add message map macros here
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgPID::SetData(CString name, double PB, double INTG, double DERV, double HiRange,
	                     double LowRange, double HiLimit, double LowLimit, double scanrate,
	                     CString comment) {
  m_strName = name;
  m_ePB = PB;
  m_eINTG = INTG;
	m_eDERV = DERV;
  m_eHiRange = HiRange;
  m_eLoRange = LowRange;
  m_eHiLimit = HiLimit;
  m_eLoLimit = LowLimit;
  m_eScanRate = scanrate / 1000;
  m_strComment = comment;
}

void CDlgPID::GetData(CString& name, double& PB, double& INTG, double& DERV, double& HiRange,
                     	double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
            					CString& comment) {
	name = m_strName;
  PB = m_ePB;
  INTG = m_eINTG;
	DERV = m_eDERV;
  HiRange = m_eHiRange;
  LowRange = m_eLoRange;
  HiLimit = m_eHiLimit;
  LowLimit = m_eLoLimit;
  scanrate = (INT32)(m_eScanRate * 1000);
  comment = m_strComment;
}  

/////////////////////////////////////////////////////////////////////////////
// CDlgPID message handlers
