#pragma once

#include"stdafx.h"
#include "afxdialogex.h"

#include "CUnitBase.h"  
#include"CUnitDictionary.h"


// CDlgChoiceCUCPara 对话框

class CDlgChoiceCUCPara : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgChoiceCUCPara)

public:
	CDlgChoiceCUCPara(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgChoiceCUCPara();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHOICE_CUCP_LINK };
#endif

	// data change function 
	CDicList&  GetDicList(void);
	ULONG GetIndex(void);							// 得到被选中参数的索引.

// data declaration
protected:
	CDicList			m_CDicList;						// 被选择单元的参数表.
	ULONG 				m_ulIndex;						// 被选中参数的索引.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboChoiceParameter();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};
