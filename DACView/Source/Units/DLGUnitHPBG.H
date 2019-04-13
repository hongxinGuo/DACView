// dlgHPBG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgHighPassBargin dialog
#pragma once

class CDlgHighPassBargin : public CDialog
{
// Construction
public:
	CDlgHighPassBargin(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double bargin, bool enableAlarm, bool alarmHigh, bool HighPass, 
							 double scanrate, CString comment);
	void GetData(CString& name, double& bargin, bool& enableAlarm, bool& alarmHigh, bool& HighPass,
							 INT32& scanrate, CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgHighPassBargin)
	enum { IDD = IDD_UNIT_HPBG };
	CString	m_strName;
	BOOL	m_fEnableAlarm;
	CString	m_strComment;
	BOOL	m_fAlarmHigh;
	double	m_eBargin;
	BOOL	m_fHighPass;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHighPassBargin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHighPassBargin)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
