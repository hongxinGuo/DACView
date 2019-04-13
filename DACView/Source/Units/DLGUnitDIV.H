// dlgmulti.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDivide dialog
#pragma once

class CDlgDivide : public CDialog
{
// Construction
public:
	CDlgDivide(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double K1, double K2, double HiRange,
							 double LowRange, double HiLimit, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, double& K1, double& K2, double& HiRange,
							 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgDivide)
	enum { IDD = IDD_UNIT_DIVIDE };
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
	//{{AFX_VIRTUAL(CDlgDivide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDivide)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
