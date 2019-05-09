#ifndef __DLG_UNIT_TOT_H__
#define __DLG_UNIT_TOT_H__

class CDlgTOT : public CDialog
{
// Construction
public:
	CDlgTOT(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double Gain, double HiRange,	double LowRange, 
							 double HiLimit, double LowLimit, INT32 ulNumber, double scanrate,
							 CString comment);
	void GetData(CString& name, double& Gain, double& HiRange, double &LowRange, 
							 double& HiLimit, double& LowLimit, INT32& ulNumber, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgTOT)
	enum { IDD = IDD_UNIT_TOT };
	CString	m_strComment;
	double	m_eHiLimit;
	double	m_eHiRange;
	double	m_eGain;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	DWORD  m_ulNumber;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTOT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTOT)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
