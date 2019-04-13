// dlgand.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFFLP dialog
#pragma once

class CDlgFFLP : public CDialog
{
// Construction
public:
	CDlgFFLP(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, bool enableAlarm, bool alarmHigh, double scanrate,
							 CString comment);
	void GetData(CString& name, bool& enableAlarm, bool& alarmHigh, INT32& scanrate,
			 				CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgFFLP)
	enum { IDD = IDD_UNIT_FFLP };
	BOOL	m_fAlarmHigh;
	BOOL	m_fEnableAlarm;
	CString	m_strComment;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFFLP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFFLP)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
