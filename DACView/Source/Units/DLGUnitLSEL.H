// dlgLSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLSel dialog
#pragma once

class CDlgLSel : public CDialog
{
// Construction
public:
	CDlgLSel(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double HiRange, double LowRange, double HiLimit,
							 double LowLimit, double scanrate, CString comment);
	void GetData(CString& name, double& HiRange, double &LowRange, double& HiLimit,
							 double& LowLimit, INT32& scanrate, CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgLSel)
	enum { IDD = IDD_UNIT_LSEL };
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
	//{{AFX_VIRTUAL(CDlgLSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLSel)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
