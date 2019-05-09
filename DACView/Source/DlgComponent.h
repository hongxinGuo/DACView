#ifndef __DLG_COMPONENT_H__
#define __DLG_COMPONENT_H__

#include"stdafx.h"

#include"CUnitComponentPara.h"

// DlgComponent 对话框

class DlgComponent : public CDialog
{
	DECLARE_DYNAMIC(DlgComponent)

public:
	DlgComponent(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DlgComponent();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UNIT_COMPONENT
	};
#endif

	bool						SetCUCPara(shared_ptr<CUCPara> pCUCP[16]);

protected:
	shared_ptr<CUCPara>		m_pCUCP[16];			// 16个参数指针

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
  afx_msg void OnLbnSelchangeListUnitcomponent();
};

#endif
