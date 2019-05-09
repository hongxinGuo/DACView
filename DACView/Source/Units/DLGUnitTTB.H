#ifndef __DLG_UNIT_TTB_H__
#define __DLG_UNIT_TTB_H__

class CDlgTTB : public CDialog
{
// Construction
public:
	CDlgTTB(CWnd* pParent = nullptr);   // standard constructor
	void SetData(CString cstrName, INT32 ulNumberInput, INT32 ulNumberStep, 
							 BYTE bInitCoil, WORD wInitOutput, BYTE bCoilPattern[8], BYTE bCoilPatternMask[8],
							 WORD wInputPattern[8], WORD wInputPatternMask[8], BYTE bCoilOutput[8], 
							 BYTE bCoilOutputMask[8], WORD wDigitalOutput[8], WORD wDigitalOutputMask[8],
							 bool fMC[8], double ulScanRate, CString cstrComment);
	void GetData(CString& name, INT32& ulNumberInput, INT32& ulNumberStep, 
							 BYTE& bInitCoil, WORD& wInitOutput, BYTE bCoilPattern[8], BYTE bCoilPatternMask[8],
							 WORD wInputPattern[8], WORD wInputPatternMask[8], BYTE bCoilOutput[8], 
							 BYTE bCoilOutputMask[8], WORD wDigitalOutput[8], WORD wDigitalOutputMask[8],
							 bool fMC[8], INT32& ulScanRate, CString& cstrComment);

private:
	void UpdateString(int iLabel, char * strName, int iNumber);

	static int m_iCoilPattern[8];
	static int m_iCoilOutput[8];
	static int m_iInputPattern[8];
	static int m_iDigitalOutput[8];
	static int m_iMatchingCriteria[8];

public:

// Dialog Data
	//{{AFX_DATA(CDlgTTB)
	enum { IDD = IDD_UNIT_TTB };
	CString	m_strComment;
	CString	m_strName;
	DWORD	m_dwNumberInput;
	DWORD	m_dwNumberStep;
	DWORD	m_wInitOutput;
	BOOL	m_fStep1MC;
	BOOL	m_fStep2MC;
	BOOL	m_fStep3MC;
	BOOL	m_fStep4MC;
	BOOL	m_fStep5MC;
	BOOL	m_fStep6MC;
	BOOL	m_fStep7MC;
	BOOL	m_fStep8MC;
	DWORD	m_ulInitCoil;
	double	m_eScanRate;
	//}}AFX_DATA
	
	char m_strCoilPattern[8][9];
	char m_strInputPattern[8][17];
	char m_strCoilOutput[8][9];
	char m_strDigitalOutput[8][17];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTTB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTTB)
	virtual void OnOK();
	afx_msg void OnKillfocusStep1CoilOutput();
	afx_msg void OnKillfocusStep1CoilPattern();
	afx_msg void OnKillfocusStep1InputPattern();
	afx_msg void OnKillfocusStep2CoilOutput();
	afx_msg void OnKillfocusStep2CoilPattern();
	afx_msg void OnKillfocusStep2DigitalOutput();
	afx_msg void OnKillfocusStep2InputPattern();
	afx_msg void OnKillfocusStep3CoilOutput();
	afx_msg void OnKillfocusStep3CoilPattern();
	afx_msg void OnKillfocusStep3DigitalOutput();
	afx_msg void OnKillfocusStep3InputPattern();
	afx_msg void OnKillfocusStep4CoilOutput();
	afx_msg void OnKillfocusStep4CoilPattern();
	afx_msg void OnKillfocusStep4DigitalOutput();
	afx_msg void OnKillfocusStep4InputPattern();
	afx_msg void OnKillfocusStep5CoilOutput();
	afx_msg void OnKillfocusStep5CoilPattern();
	afx_msg void OnKillfocusStep5DigitalOutput();
	afx_msg void OnKillfocusStep5InputPattern();
	afx_msg void OnKillfocusStep6CoilOutput();
	afx_msg void OnKillfocusStep6CoilPattern();
	afx_msg void OnKillfocusStep6DigitalOutput();
	afx_msg void OnKillfocusStep6InputPattern();
	afx_msg void OnKillfocusStep7CoilOutput();
	afx_msg void OnKillfocusStep7CoilPattern();
	afx_msg void OnKillfocusStep7DigitalOutput();
	afx_msg void OnKillfocusStep7InputPattern();
	afx_msg void OnKillfocusStep8CoilOutput();
	afx_msg void OnKillfocusStep8CoilPattern();
	afx_msg void OnKillfocusStep8DigitalOutput();
	afx_msg void OnKillfocusStep8InputPattern();
	afx_msg void OnKillfocusStep1DigitalOutput();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

