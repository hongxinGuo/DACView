// dlgHPBG.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitHPBG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHighPassBargin dialog


CDlgHighPassBargin::CDlgHighPassBargin(CWnd* pParent /*=nullptr*/)
        : CDialog(CDlgHighPassBargin::IDD, pParent)
{
        //{{AFX_DATA_INIT(CDlgHighPassBargin)
        m_strName = "";
        m_fEnableAlarm = FALSE;
        m_strComment = "";
        m_fAlarmHigh = FALSE;
        m_eBargin = 0.0;
        m_fHighPass = FALSE;
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgHighPassBargin::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDlgHighPassBargin)
        DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
        DDX_Check(pDX, IDC_ENABLE_ALARM, m_fEnableAlarm);
        DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
        DDX_Check(pDX, IDC_ALARM_HIGH, m_fAlarmHigh);
        DDX_Text(pDX, IDC_EDIT_BARGIN, m_eBargin);
        DDX_Check(pDX, IDC_HighPass, m_fHighPass);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 1000000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHighPassBargin, CDialog)
        //{{AFX_MSG_MAP(CDlgHighPassBargin)
                // NOTE: the ClassWizard will add message map macros here
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDlgHighPassBargin::SetData(CString name, double bargin, bool enableAlarm, bool alarmHigh,
                                 bool HighPass, double scanrate, CString comment) {
  m_strName = name;
  m_eBargin = bargin;
  m_fEnableAlarm = enableAlarm;
  m_fAlarmHigh = alarmHigh;
  m_fHighPass = HighPass;
  m_eScanRate = scanrate / 1000;
  m_strComment = comment;
}

void CDlgHighPassBargin::GetData(CString& name, double & bargin, bool& enableAlarm, bool& alarmHigh,
                                 bool& HighPass, INT32& scanrate, CString& comment) {
  name = m_strName;
  bargin = m_eBargin;
  enableAlarm = m_fEnableAlarm;
  alarmHigh = m_fAlarmHigh;
  HighPass = m_fHighPass;
  scanrate = (INT32)(m_eScanRate * 1000);
  comment = m_strComment;
}  
/////////////////////////////////////////////////////////////////////////////
// CDlgHighPassBargin message handlers
