// dlgQFLT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgQFLT dialog
#pragma once

class CDlgQFLT : public CDialog
{
// Construction
public:
	CDlgQFLT(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double HiRange, double LowRange, double HiLimit, 
							 double FiltFactor, double DeadBand, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, double& HiRange, double &LowRange, double& HiLimit, 
							 double& FiltFactor, double& DeadBand, double& LowLimit, INT32& scanrate, CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgQFLT)
	enum { IDD = IDD_UNIT_QFLT };
	CString	m_strComment;
	double	m_eHiLimit;
	double	m_eHiRange;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	double	m_eFiltFactor;
	double	m_eDeadBand;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgQFLT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgQFLT)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
