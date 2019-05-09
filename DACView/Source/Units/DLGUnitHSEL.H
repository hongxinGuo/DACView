#ifndef __DLG_UNIT_HSEL_H__
#define __DLG_UNIT_HSEL_H__

class CDlgHSel : public CDialog
{
// Construction
public:
	CDlgHSel(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double HiRange, double LowRange, double HiLimit,
							 double LowLimit, double scanrate, CString comment);
	void GetData(CString& name, double& HiRange, double &LowRange, double& HiLimit,
							 double& LowLimit, INT32& scanrate, CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgHSel)
	enum { IDD = IDD_UNIT_HSEL };
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
	//{{AFX_VIRTUAL(CDlgHSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHSel)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
