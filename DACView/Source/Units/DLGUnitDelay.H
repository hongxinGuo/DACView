// dlgdlay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDlay dialog
#pragma once

class CDlgDlay : public CDialog
{
// Construction
public:
	CDlgDlay(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString name, INT32 lDelayOn, INT32 lDelayOff, double scanrate,CString comment);
	void GetData(CString& name, INT32& lDelayOn, INT32& lDelayOff, INT32& scanrate,	CString& comment);

// Dialog Data
	//{{AFX_DATA(CDlgDlay)
	enum { IDD = IDD_UNIT_DELAY };
	CString	m_strComment;
	long	m_lDelayOff;
	long	m_lDelayOn;
	CString	m_strName;
	double	m_eScanRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDlay)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDlay)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
