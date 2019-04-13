// dlgdlay.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "dlgUnitDelay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDlay dialog


CDlgDlay::CDlgDlay(CWnd* pParent /*=nullptr*/)
        : CDialog(CDlgDlay::IDD, pParent)
{
        //{{AFX_DATA_INIT(CDlgDlay)
        m_strComment = "";
        m_lDelayOff = 0;
        m_lDelayOn = 0;
        m_strName = "";
	m_eScanRate = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgDlay::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CDlgDlay)
        DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
        DDX_Text(pDX, IDC_EDIT_DELAY_OFF, m_lDelayOff);
        DDX_Text(pDX, IDC_EDIT_DELAY_ON, m_lDelayOn);
        DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SCANRATE, m_eScanRate);
	DDV_MinMaxDouble(pDX, m_eScanRate, 1.e-003, 100000000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDlay, CDialog)
        //{{AFX_MSG_MAP(CDlgDlay)
                // NOTE: the ClassWizard will add message map macros here
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgDlay::SetData(CString name, INT32 lDelayOn, INT32 lDelayOff, double scanrate,CString comment) {
  m_lDelayOn = lDelayOn;
  m_lDelayOff = lDelayOff;
  m_eScanRate = scanrate / 1000;
  m_strComment = comment;
  m_strName = name;
}

void CDlgDlay::GetData(CString& name, INT32& lDelayOn, INT32& lDelayOff, INT32& scanrate, CString& comment) {
  name = m_strName;
  lDelayOn = m_lDelayOn;
  lDelayOff = m_lDelayOff;
  scanrate = (INT32)(m_eScanRate * 1000);
  comment = m_strComment;
}

/////////////////////////////////////////////////////////////////////////////
// CDlgDlay message handlers
