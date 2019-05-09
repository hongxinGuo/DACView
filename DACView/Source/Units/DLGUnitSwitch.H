#ifndef __DLG_UNIT_SWITCH_H__
#define __DLG_UNIT_SWITCH_H__

class CDlgSwch : public CDialog
{
// Construction
public:
	CDlgSwch(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double HiRange, double LowRange, double HiLimit,
							 double LowLimit, double ScanRate, CString comment);
	void GetData(CString& name, double& HiRange, double &LowRange, double& HiLimit,
							 double& LowLimit, INT32& ScanRate, CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgSwch)
	enum { IDD = IDD_UNIT_SWCH };
	CString	m_strComment;
	double	m_eHiLimit;
	double	m_eHiRange;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSwch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSwch)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
