// dlginv.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitPulse.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPulse dialog


CDlgPulse::CDlgPulse(CWnd* pParent /*=nullptr*/)
  : CDialog(CDlgPulse::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgPulse)
  m_strName = "";
  m_strComment = "";
  m_dwWidth = 0;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgPulse::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgPulse)
  DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
  DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
  DDX_Text(pDX, IDC_EDIT_WIDTH, m_dwWidth);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 1000000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPulse, CDialog)
  //{{AFX_MSG_MAP(CDlgPulse)
    // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgPulse::SetData(CString name, INT32 width, double scanrate, CString comment) {
  m_strName = name;
  m_dwWidth = width;
  m_eScanRate = scanrate / 1000;
  m_strComment = comment;
}

void CDlgPulse::GetData(CString& name, INT32& width, INT32& scanrate, CString& comment) {
  name = m_strName;
  width = m_dwWidth;
  scanrate = (INT32)(m_eScanRate * 1000);
  comment = m_strComment;
}  
/////////////////////////////////////////////////////////////////////////////
// CDlgPulse message handlers
