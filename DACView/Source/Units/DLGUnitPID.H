// dlgadd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPID dialog
#pragma once

class CDlgPID : public CDialog
{
// Construction
public:
	CDlgPID(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, double PB, double IntegralTime, double DERVTime, double HiRange,
							 double LowRange, double HiLimit, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, double& PB, double& IntegralTime, double &DERVTime, double& HiRange,
							 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgPID)
	enum { IDD = IDD_UNIT_PID };
	CString	m_strComment;
	double	m_eHiLimit;
	double	m_eHiRange;
	double	m_ePB;
	double	m_eINTG;
	double  m_eDERV;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPID)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
