// dlgPuls.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPulse dialog
#pragma once

class CDlgPulse : public CDialog
{
// Construction
public:
	CDlgPulse(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, INT32 width, double scanrate, CString comment);
	void GetData(CString& name, INT32& width, INT32& scanrate, CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgPulse)
	enum { IDD = IDD_UNIT_PULSE };
	CString	m_strName;
	CString	m_strComment;
	DWORD	m_dwWidth;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPulse)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPulse)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
