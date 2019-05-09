#ifndef __DLG_UNIT_MULTIPLE_H__
#define __DLG_UNIT_MULTIPLE_H__

class CDlgMultiple : public CDialog
{
// Construction
public:
	CDlgMultiple(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double K1, double K2, double HiRange,
							 double LowRange, double HiLimit, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, double& K1, double& K2, double& HiRange,
							 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgMultiple)
	enum { IDD = IDD_UNIT_MULTIPLE };
	CString	m_strComment;
	double	m_eHiLimit;
	double	m_eHiRange;
	double	m_eK1;
	double	m_eK2;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMultiple)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMultiple)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
