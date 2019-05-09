#ifndef __DLG_UNIT_TPO_H__
#define __DLG_UNIT_TPO_H__

class CDlgTPO : public CDialog
{
// Construction
public:
	CDlgTPO(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, INT32 PeriodTime, double HiRange,
							 double LowRange, double HiLimit, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, INT32& PeriodTime, double& HiRange,
							 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgTPO)
	enum { IDD = IDD_UNIT_TPO };
	CString	m_strComment;
	double	m_eHiLimit;
	double	m_eHiRange;
	UINT    m_ulPeriodTime;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTPO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTPO)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
