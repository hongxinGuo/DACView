#ifndef __DLG_DELETE_COMPONENT_H__
#define __DLG_DELETE_COMPONENT_H__

#include "CUnitComponent.h"
#include "CUnitDictionary.h"

class CDlgDeleteComponentPara : public CDialog
{
// Construction
public:
	CDlgDeleteComponentPara(CWnd* pParent = nullptr);   // standard constructor

	void SetLink( CUnitComponentPtr pCpt, CUCPara * pCUC[] );

// Dialog Data
	//{{AFX_DATA(CDelCptPara)
	enum { IDD = IDD_DEL_COMPONENT_PARAMETER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
  CUnitComponentPtr m_pCpt;
	CUCPara *   m_pCUCP[16];
	BOOL				m_fDeleted[16];
	ULONG				m_pDeletedPara[16];

	ULONG				m_ulIndex;
	ULONG 			m_ulChoiceIndex;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelCptPara)
	protected :
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateListBox( void );

	// Generated message map functions
	//{{AFX_MSG(CDelCptPara)
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeListCptDel();
	afx_msg void OnDblclkListCptDel();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUndelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

