/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 对话窗：用于建立Object和Unit之间的连接。
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "typedef.h"
#include "CObjDynLink.h"
#include "CUnitDictionary.h"
#include "CObjectBase.h"

/////////////////////////////////////////////////////////////////////////////
// CDynamicLinkDlg dialog
   
class CDynamicLinkDlg : public CDialog
{
// Construction
public:
	CDynamicLinkDlg(CWnd* pParent = nullptr);	// standard constructor
  
// attribute
public:
	void SetLink(CDicList * pDicList, CObjectBase * pCObject, 
							 CODLList* LinkList, ParaName * ptName);
							 
// show function 
private :
	void    UpdateDlg( CObjectDynLink * pCTag );
	void    UpdateDynLink( CObjectDynLink * pCTag );
	void    ChangeLinkName( ULONG ulLinkType );
  void    SetLinkMethodIndex( ULONG ulType );
  ULONG   GetLinkMethodIndex( ULONG ulIndex );
	void    ResetDicIndex( void );
	
// public data
protected :
// serialized data declaration

// no serialized data declaration	
	CDicList *						m_pDicList;
	CString  							m_strName;
	CODLList * 						m_plistDynLink;
	ParaName * 						m_ptParaName;

	CODLList::iterator 		m_it;
	CObjectDynLink * 			m_pCTag;
	CODLList *						m_plistNewDynLink;

	INT32			            m_lUnitIndex;
	INT32									m_lObjectIndex;
  BOOL                  m_fInputType;
	INT32									m_lLinkMethod;
	CUnitBase * 					m_pCUnitCurrent;
	CObjectBase * 				m_pCObjectCurrent;
	BOOL									m_fUnitToObject;
		 
// Dialog Data
	//{{AFX_DATA(CDynamicLinkDlg)
	enum { IDD = IDD_DYNAMIC_LINK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDynamicLinkDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickedButtonNew();
	afx_msg void OnClickedDetail();
	afx_msg void OnChangeComment();
	afx_msg void OnSelchangeLinkMethod();
	afx_msg void OnSelchangeVariableParameter();
	afx_msg void OnSelchangeLinkName();
	afx_msg void OnClickedButtonNext();
	afx_msg void OnClickedButtonPrev();
	afx_msg void OnClickedButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

