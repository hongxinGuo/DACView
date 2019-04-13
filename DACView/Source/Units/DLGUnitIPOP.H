// dlgIPOP.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInputOutput dialog
#pragma once

class CDlgInputOutput : public CDialog
{
// Construction
public:
	CDlgInputOutput(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double HiRange,	double LowRange, 
							 double HiLimit, double LowLimit, double scanrate, CString comment);
	void GetData(CString& name, double& HiRange, double &LowRange, 
							 double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgInputOutput)
	enum { IDD = IDD_UNIT_IPOP };
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
	//{{AFX_VIRTUAL(CDlgInputOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInputOutput)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
