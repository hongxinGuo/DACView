// dlgquad.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitQuad.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgQuad dialog

CDlgQuad::CDlgQuad(CWnd* pParent /*=nullptr*/)
	: CDialog(CDlgQuad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgQuad)
	m_eHiLimit = 10000;
	m_eHiRange = 10000;
	m_eK0 = 1.0;
	m_eK1 = 1,0;
	m_eK2 = 1.0;
	m_eLoLimit = 0;
	m_eLoRange = 0;
	m_strName = "";
	m_strComment = "";
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgQuad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgQuad)
	DDX_Text(pDX, IDC_EDIT_HILIMIT, m_eHiLimit);
	DDX_Text(pDX, IDC_EDIT_HIRANGE, m_eHiRange);
	DDX_Text(pDX, IDC_EDIT_K0, m_eK0);
	DDX_Text(pDX, IDC_EDIT_K1, m_eK1);
	DDX_Text(pDX, IDC_EDIT_K2, m_eK2);
	DDX_Text(pDX, IDC_EDIT_LOLIMIT, m_eLoLimit);
	DDX_Text(pDX, IDC_EDIT_LORANGE, m_eLoRange);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 32);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
	DDV_MaxChars(pDX, m_strComment, 128);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 10000000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgQuad, CDialog)
	//{{AFX_MSG_MAP(CDlgQuad)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgQuad::SetData(CString name, double K0, double K1, double K2, double HiRange,
						 					 double LowRange, double HiLimit, double LowLimit, double scanrate,
						 					 CString comment) {
	m_strName = name;
	m_eK0 = K0;
	m_eK1 = K1;
	m_eK2 = K2;
	m_eHiRange = HiRange;
	m_eLoRange = LowRange;
	m_eHiLimit = HiLimit;
	m_eLoLimit = LowLimit;
	m_eScanRate = scanrate / 1000;
	m_strComment = comment;
}

void CDlgQuad::GetData(CString& name, double& K0, double& K1, double& K2, double& HiRange,
						 					 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
						   				 CString& comment) {
	name = m_strName;
	K0 = m_eK0;
	K1 = m_eK1;
	K2 = m_eK2;
	HiRange = m_eHiRange;
	LowRange = m_eLoRange;
	HiLimit = m_eHiLimit;
	LowLimit = m_eLoLimit;
  scanrate = (INT32)(m_eScanRate * 1000);
	comment = m_strComment;
}  

/////////////////////////////////////////////////////////////////////////////
// CDlgQuad message handlers
