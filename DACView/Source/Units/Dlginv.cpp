// dlginv.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitInvert.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInv dialog


CDlgInv::CDlgInv(CWnd* pParent /*=nullptr*/)
        : CDialog(CDlgInv::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDlgInv)
  m_strName = "";
  m_fEnableAlarm = FALSE;
  m_strComment = "";
  m_fAlarmHigh = FALSE;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgInv::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDlgInv)
  DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
  DDX_Check(pDX, IDC_ENABLE_ALARM, m_fEnableAlarm);
  DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
  DDX_Check(pDX, IDC_ALARM_HIGH, m_fAlarmHigh);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 100000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInv, CDialog)
        //{{AFX_MSG_MAP(CDlgInv)
                // NOTE: the ClassWizard will add message map macros here
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgInv::SetData(CString name, BOOL enableAlarm, BOOL alarmHigh, double scanrate,
                      CString comment) {
  m_strName = name;
  m_fEnableAlarm = enableAlarm;
  m_fAlarmHigh = alarmHigh;
  m_eScanRate = scanrate / 1000;
  m_strComment = comment;
}

void CDlgInv::GetData(CString& name, BOOL& enableAlarm, BOOL& alarmHigh, INT32& scanrate,
                      CString& comment) {
  name = m_strName;
  enableAlarm = m_fEnableAlarm;
  alarmHigh = m_fAlarmHigh;
  scanrate = (ULONG)(m_eScanRate * 1000);
  comment = m_strComment;
}  
/////////////////////////////////////////////////////////////////////////////
// CDlgInv message handlers
