#ifndef __DLG_UNIT_INVERT_H__
#define __DLG_UNIT_INVERT_H__

class CDlgInv : public CDialog
{
// Construction
public:
	CDlgInv(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, BOOL enableAlarm, BOOL alarmHigh, double scanrate,
							 CString comment);
	void GetData(CString& name, BOOL& enableAlarm, BOOL& alarmHigh, INT32& scanrate,
			 				CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgInv)
	enum { IDD = IDD_UNIT_INV };
	CString	m_strName;
	BOOL	m_fEnableAlarm;
	CString	m_strComment;
	BOOL	m_fAlarmHigh;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInv)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInv)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
