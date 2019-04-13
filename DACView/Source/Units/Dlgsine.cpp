// dlgsin.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitSine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSine dialog

CDlgSine::CDlgSine(CWnd* pParent /*=nullptr*/)
	: CDialog(CDlgSine::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSine)
	m_strComment = "";
	m_eHiLimit = 0;
	m_eHiRange = 0;
	m_eK0 = 0;
	m_eK1 = 0;
	m_eK2 = 0;
	m_eLoLimit = 0;
	m_eLoRange = 0;
	m_strName = "";
	m_eScanRate = 0.0;
	m_eK3 = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgSine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSine)
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
	DDX_Text(pDX, IDC_EDIT_HIRANGE, m_eHiRange);
	DDX_Text(pDX, IDC_EDIT_K0, m_eK0);
	DDX_Text(pDX, IDC_EDIT_K1, m_eK1);
	DDX_Text(pDX, IDC_EDIT_K2, m_eK2);
	DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
	DDX_Text(pDX, IDC_EDIT_LORANGE, m_eLoRange);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	DDX_Text(pDX, IDC_EDIT_K3, m_eK3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSine, CDialog)
	//{{AFX_MSG_MAP(CDlgSine)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgSine::SetData(CString name, double K0, double K1, double K2, double K3, 
                       double HiRange, double LowRange, double HiLimit, double LowLimit,
						 					 double scanrate, CString comment) {
	m_strName = name;
	m_eK0 = K0;
	m_eK1 = K1;
	m_eK2 = K2;
  m_eK3 = K3;
	m_eHiRange = HiRange;
	m_eLoRange = LowRange;
	m_eHiLimit = HiLimit;
	m_eLoLimit = LowLimit;
	m_eScanRate = scanrate / 1000;
	m_strComment = comment;
}

void CDlgSine::GetData(CString& name, double& K0, double& K1, double& K2, double& K3,
                       double& HiRange, double &LowRange, double& HiLimit,
                       double& LowLimit, INT32& scanrate,	CString& comment) {
	name = m_strName;
	K0 = m_eK0;
	K1 = m_eK1;
	K2 = m_eK2;
  K3 = m_eK3;
	HiRange = m_eHiRange;
	LowRange = m_eLoRange;
	HiLimit = m_eHiLimit;
	LowLimit = m_eLoLimit;
	scanrate = (INT32)(m_eScanRate * 1000);
	comment = m_strComment;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgSine message handlers
