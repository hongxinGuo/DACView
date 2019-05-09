// DlgComponent.cpp: 实现文件
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgComponent.h"
#include "afxdialogex.h"


// DlgComponent 对话框

IMPLEMENT_DYNAMIC(DlgComponent, CDialog)

DlgComponent::DlgComponent(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_UNIT_COMPONENT, pParent)
{

}

DlgComponent::~DlgComponent()
{
}

bool DlgComponent::SetCUCPara(shared_ptr<CUCPara> pCUCP[16])
{
  for (int i = 0; i < 16; i++) {
    m_pCUCP[i] = pCUCP[i];
  }

	return true;
}

void DlgComponent::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgComponent, CDialog)
  ON_LBN_SELCHANGE(IDC_LIST_UNITCOMPONENT, &DlgComponent::OnLbnSelchangeListUnitcomponent)
END_MESSAGE_MAP()


// DlgComponent 消息处理程序


BOOL DlgComponent::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	int i, j = 0, k;
	CString str;
	// 将词典中的内容加入选择框中.
	SendDlgItemMessage(IDC_LIST_UNITCOMPONENT, LB_RESETCONTENT, (WPARAM)0L, (LPARAM)0L);
	for (i = 0; i < 16; i++) {
    if (m_pCUCP[i]->IsLinked()) {
      j++;
      if ((m_pCUCP[i]->GetParaType() & (tINPUT | tOUTPUT)) == tINPUT) { // 输入型参数，没有源单元（源单元就是本部件，不设）
        str = m_pCUCP[i]->GetName() + " -> " + m_pCUCP[i]->GetDestUnit()->GetName();
        str += "'s " + m_pCUCP[i]->GetDestUnit()->GetParaName(m_pCUCP[i]->GetDestIndex());
        k = SendDlgItemMessage(IDC_LIST_UNITCOMPONENT, LB_INSERTSTRING, (WPARAM)i, (LPARAM)(LPCTSTR(str)));
      }
      else if ((m_pCUCP[i]->GetParaType() & (tINPUT | tOUTPUT)) == tOUTPUT) { // 输出型参数没有目的单元（目的单元极为本部件，不设）
        str = m_pCUCP[i]->GetName() + ": " + (m_pCUCP[i]->GetSrcUnit())->GetName();
        str += "'s " + m_pCUCP[i]->GetSrcUnit()->GetParaName(m_pCUCP[i]->GetSrcIndex());
        k = SendDlgItemMessage(IDC_LIST_UNITCOMPONENT, LB_INSERTSTRING, (WPARAM)i, (LPARAM)(LPCTSTR(str)));
      }
      else {
        // do nothing
      }
    }
	}
	SendDlgItemMessage(IDC_LIST_UNITCOMPONENT, LB_SETCURSEL, 0, 0L);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void DlgComponent::OnLbnSelchangeListUnitcomponent()
{
  // TODO: 在此添加控件通知处理程序代码
}
