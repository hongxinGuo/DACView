// dlgquad.h : header file
//
			
/////////////////////////////////////////////////////////////////////////////
// CDlgQuad dialog
#pragma once

class CDlgQuad : public CDialog
{
// Construction
public:
	CDlgQuad(CWnd* pParent = nullptr);	// standard constructor
	void SetData(CString name, double K0, double K1, double K2, double HiRange,
							 double LowRange, double HiLimit, double LowLimit, double scanrate,
							 CString comment);
	void GetData(CString& name, double& K0, double& K1, double& K2, double& HiRange,
							 double &LowRange, double& HiLimit, double& LowLimit, INT32& scanrate,
							 CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgQuad)
	enum { IDD = IDD_UNIT_QUADRATIC };
	double	m_eHiLimit;
	double	m_eHiRange;
	double	m_eK0;
	double	m_eK1;
	double	m_eK2;
	double	m_eLoLimit;
	double	m_eLoRange;
	CString	m_strName;
	CString	m_strComment;
	double	m_eScanRate;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgQuad)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};                           

