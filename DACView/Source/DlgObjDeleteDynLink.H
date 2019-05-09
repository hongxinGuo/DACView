///////////////////////////////////////////////////////////////////////////////////////////////
//
// DlgObjDeleteDynLink.h : header file
//
// 对话窗，用于删除Object（与Unit之间）的动态链接
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __DLG_OBJECT_DELETE_DYNAMIC_LINK_H__
#define __DLG_BOJECT_DELETE_DYNAMIC_LINK_H__

#include "cObjectBase.h"

/////////////////////////////////////////////////////////////////////////////
// CODelDynLink dialog

class CODelDynLink : public CDialog
{
// Construction
public:
	CODelDynLink(CWnd* pParent = nullptr);   // standard constructor

	void SetLink( CODLList * plistObjectDynLink, CObjectBase * pcobj );

// Dialog Data
	//{{AFX_DATA(CODelDynLink)
	enum { IDD = IDD_DIALOG_DELETE_ODYNLINK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected:
	CODLList * m_plistObjectDynLink;
	CObjectBase * m_pcobj;

	ULONG 		m_ulChoiceIndex;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CODelDynLink)
	protected :
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateListBox( void );

	// Generated message map functions
	//{{AFX_MSG(CODelDynLink)
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

#endif
