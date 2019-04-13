// dlgadd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAdd dialog

#pragma once

class CDlgAdd : public CDialog
{
// Construction
public:
	CDlgAdd(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double K1, double K2, double HiRange,
							 double LowRange, double HiLimit, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, double& K1, double& K2, double& HiRange,
							 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgAdd)
	enum { IDD = IDD_UNIT_ADD };
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
	//{{AFX_VIRTUAL(CDlgAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAdd)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
