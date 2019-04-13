// dlgDTim.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDelayTime dialog
#pragma once

class CDlgDelayTime : public CDialog
{
// Construction
public:
	CDlgDelayTime(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double DelayTime, double HiRange,
							 double LowRange, double HiLimit, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, double& DelayTime, double& HiRange,
							 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgDelayTime)
	enum { IDD = IDD_UNIT_DELAY_TIME };
	CString	m_strComment;
	double	m_eHiLimit;
	double	m_eHiRange;
	double	m_eDelayTime;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDelayTime)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDelayTime)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
