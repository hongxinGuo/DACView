// CDlgChoiceCUCPara.cpp: 实现文件
//

#include "stdafx.h"
#include"resource.h"

#include "DlgChoiceCUCPara.h"


// CDlgChoiceCUCPara 对话框

IMPLEMENT_DYNAMIC(CDlgChoiceCUCPara, CDialogEx)

CDlgChoiceCUCPara::CDlgChoiceCUCPara(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHOICE_CUCP_LINK, pParent)
{

}

CDlgChoiceCUCPara::~CDlgChoiceCUCPara()
{
}

void CDlgChoiceCUCPara::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgChoiceCUCPara, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOICE_CUCPARAMETER, &CDlgChoiceCUCPara::OnCbnSelchangeComboChoiceParameter)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlgChoiceCUCPara 消息处理程序

// data change function
//////////////////////////////////////////////////////////////////////////
//
// GetDicList
//
// return CDicList & : 
//  
//
///////////////////////////////////////////////////////////////////////////
CDicList &  CDlgChoiceCUCPara::GetDicList(void) {
	return(m_CDicList);
}

ULONG CDlgChoiceCUCPara::GetIndex(void) {
	return(m_ulIndex);
}

void CDlgChoiceCUCPara::OnCbnSelchangeComboChoiceParameter()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ulIndex = SendDlgItemMessage(IDC_COMBO_CHOICE_CUCPARAMETER, CB_GETCURSEL, 0, 0L);
}


BOOL CDlgChoiceCUCPara::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	INT_PTR i, iTotal = m_CDicList.GetCount();
	POSITION po = m_CDicList.GetHeadPosition();
	CUnitDictionary * pDic;

	// 将词典中的内容加入选择框中.
	for (i = 0; i < iTotal; i++) {
		pDic = m_CDicList.GetNext(po);
		SendDlgItemMessage(IDC_COMBO_CHOICE_CUCPARAMETER,
			CB_INSERTSTRING, (WPARAM)-1,
			(LPARAM)((LPSTR)pDic->GetParaName().GetBuffer()));
	}
	if (i == 0) { // no proper paremeter to select
		GetDlgItem(IDOK)->EnableWindow(FALSE);
	}
	else {	// 选择第一个参数.
		SendDlgItemMessage(IDC_COMBO_CHOICE_CUCPARAMETER,
			CB_SETCURSEL, (WPARAM)0L, (LPARAM)0L);
		m_ulIndex = SendDlgItemMessage(IDC_COMBO_CHOICE_CUCPARAMETER, CB_GETCURSEL, 0, 0L);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void CDlgChoiceCUCPara::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	INT_PTR i, iTotal = m_CDicList.GetCount();
	POSITION po = m_CDicList.GetHeadPosition();
	CUnitDictionary * pDic;

	// 清除词典的内容.
	for (i = 0; i < iTotal; i++) {
		pDic = m_CDicList.GetNext(po);
		delete pDic;
	}
	m_CDicList.RemoveAll();
}
