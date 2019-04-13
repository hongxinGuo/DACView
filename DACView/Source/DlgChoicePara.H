// chiocepa.h : header file
//
#pragma once

#include "CUnitBase.h"            

/////////////////////////////////////////////////////////////////////////////
// CDlgChoiceParameter dialog

class CDlgChoiceParameter : public CDialog
{
// Construction
public:
	CDlgChoiceParameter(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChoiceParameter)
	enum { IDD = IDD_CHOICE_LINK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA 
	
// data change function 
	CDicList&  GetDicList( void );
  ULONG GetIndex( void );							// 得到被选中参数的索引.
  
// data declaration
protected :
	CDicList			m_CDicList;						// 被选择单元的参数表.
	ULONG 				m_ulIndex;						// 被选中参数的索引.
		

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgChoiceParameter)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboChoiceParameter();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
