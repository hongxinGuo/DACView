/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// DlgUnitDeleteDynLink.h : header file
//
// 对话窗：用于删除Unit之间的动态链接。
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDelDynLink dialog
#pragma once

#include "CUnitDynLink.h"

class CDelDynLink : public CDialog
{
// Construction
public:
	CDelDynLink(CWnd* pParent = nullptr);   // standard constructor

	void SetLink( CUDLList * plistUnitDynLink );

// Dialog Data
	//{{AFX_DATA(CDelDynLink)
	enum { IDD = IDD_DIALOG_DELETE_DYNLINK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	CUDLList * m_plistUnitDynLink;

	ULONG 		m_ulChoiceIndex;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDelDynLink)
	protected :
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateListBox( void );

	// Generated message map functions
	//{{AFX_MSG(CDelDynLink)
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeListDynlink();
	afx_msg void OnDblclkListDynlink();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUndelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

