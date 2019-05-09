#ifndef __DLG_UNIT_AND_H__
#define __DLG_UNIT_AND_H__

class CDlgAnd : public CDialog
{
// Construction
public:
	CDlgAnd(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, bool enableAlarm, bool alarmHigh, 
               double scanrate, CString comment);
	void GetData(CString& name, bool& enableAlarm, bool& alarmHigh, INT32& scanrate,
			 				CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgAnd)
	enum { IDD = IDD_UNIT_AND };
	BOOL	m_fAlarmHigh;
	BOOL	m_fEnableAlarm;
	CString	m_strComment;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAnd)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
