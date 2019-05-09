#ifndef __DLG_UNIT_OR_H__
#define __DLG_UNIT_OR_H__

class CDlgOr : public CDialog
{
// Construction
public:
	CDlgOr(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, bool enableAlarm, bool alarmHigh, double scanrate,
							 CString comment);
	void GetData(CString& name, bool& enableAlarm, bool& alarmHigh, INT32& scanrate,
			 				CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgOr)
	enum { IDD = IDD_UNIT_OR };
	BOOL	m_fAlarmHigh;
	CString	m_strComment;
	CString	m_strName;
	BOOL	m_fEnableAlarm;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOr)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
