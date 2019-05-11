// CDelCpt.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgDeleteComponent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDeleteComponentPara dialog


CDlgDeleteComponentPara::CDlgDeleteComponentPara(CWnd* pParent /*=nullptr*/)
	: CDialog(CDlgDeleteComponentPara::IDD, pParent)
{

	//{{AFX_DATA_INIT(CDlgDeleteComponentPara)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgDeleteComponentPara::SetLink( CUnitComponentPtr pCpt, CUCPara * pCUC[] ) {
  m_pCpt = pCpt;
  for (int i = 0; i < 16; i++) {
    m_pCUCP[i] = pCUC[i];
  }
}

void CDlgDeleteComponentPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDeleteComponentPara)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDeleteComponentPara, CDialog)
	//{{AFX_MSG_MAP(CDlgDeleteComponentPara)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_COMPONENT_DEL, OnSelchangeListCptDel)
	ON_LBN_DBLCLK(IDC_LIST_COMPONENT_DEL, OnDblclkListCptDel)
	ON_BN_CLICKED(IDC_BUTTON_UNDELETE, OnButtonUndelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgDeleteComponentPara message handlers

void CDlgDeleteComponentPara::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	m_fDeleted[m_ulIndex] = TRUE;
	UpdateListBox();
}

void CDlgDeleteComponentPara::OnSelchangeListCptDel() 
{
	// TODO: Add your control notification handler code here
	m_ulChoiceIndex = (ULONG)SendDlgItemMessage(IDC_LIST_COMPONENT_DEL, LB_GETCURSEL, 0, 0L);

	int i;

	for ( i = 0; i < 16; i++ ) {
		if ( m_pDeletedPara[i] == m_ulChoiceIndex ) break;;
	}
	ASSERT( i < 16 );
	m_ulIndex = i;
	if ( m_fDeleted[i] ) {
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
}

void CDlgDeleteComponentPara::OnDblclkListCptDel() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgDeleteComponentPara::OnOK() 
{
	// TODO: Add extra validation here
  char buffer[20];
  CString strName;

	for( int i = 0; i < 16; i++ ) {
		if ( m_fDeleted[i] ) {
      
			// 解除参数锁.
      if ((m_pCUCP[i]->GetParaType() & (tINPUT | tOUTPUT)) & tINPUT) { //  输入型参数？
        m_pCUCP[i]->GetDestUnit()->SetParameterLock(m_pCUCP[i]->GetDestIndex(), FALSE);
        m_pCUCP[i]->GetDestUnit()->LinkFromComponent(false); // 清除从单元联入标志
        m_pCUCP[i]->SetDestUnit(nullptr);
      }
			// 清除连入部件标志.
      else if ((m_pCUCP[i]->GetParaType() & (tINPUT | tOUTPUT)) & tOUTPUT) { // 输出型参数
        m_pCUCP[i]->GetSrcUnit()->LinkToComponent(FALSE);
        m_pCUCP[i]->SetSrcUnit(nullptr);
      }
      else {
        ASSERT(0);
      }
      m_pCUCP[i]->SetParaType(0);
      m_pCUCP[i]->SetSrcIndex(-1);
      m_pCUCP[i]->SetDestIndex(-1);
      m_pCUCP[i]->SetLinkedFlag(false);
      _itoa_s(i, buffer, 10);
      strName = "Para";
      strName += buffer;
      m_pCUCP[i]->SetName(strName); // 重置名称
		}
	}
	CDialog::OnOK();
}

void CDlgDeleteComponentPara::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialog::OnCancel();
}

BOOL CDlgDeleteComponentPara::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i, j = 0;
	CString str;

	for ( i = 0; i < 16; i++ ) {
		m_fDeleted[i] = FALSE;
		if ( m_pCUCP[i]->GetSrcUnit() != nullptr ) {
			m_pDeletedPara[i] = j++;
		}
    if (m_pCUCP[i]->GetDestUnit() != nullptr) {
      m_pDeletedPara[i] = j++;
    }
	}

	for ( i = 0; i < 16; i++ ) {
		if ( m_fDeleted[i] ) break;
		}
	m_ulIndex = i;
	m_ulChoiceIndex = m_pDeletedPara[m_ulIndex];
	UpdateListBox();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDeleteComponentPara::UpdateListBox( ) {
	CString str;
	int i, j= 0;
  char buffer[20];

	SendDlgItemMessage(IDC_LIST_COMPONENT_DEL, LB_RESETCONTENT, (WPARAM)0L, (LPARAM)0L);
	for ( i = 0; i < 16; i++ ) {
		if ( m_pCUCP[i]->GetSrcUnit() != nullptr ) {
      _itoa_s(i, buffer, 10);
			str = m_pCUCP[i]->GetSrcUnit()->GetName() + '.';
			str = str + m_pCUCP[i]->GetSrcUnit()->GetParaName( m_pCUCP[i]->GetSrcIndex() ) + " -> " "Para" + buffer;
			if ( m_fDeleted[i] ) {
				str += '*';
			}
			SendDlgItemMessage(IDC_LIST_COMPONENT_DEL, LB_INSERTSTRING, (WPARAM)j++,
										 		 (LPARAM)(LPCTSTR(str)));
		}
	}
  for (i = 0; i < 16; i++) {
    if (m_pCUCP[i]->GetDestUnit() != nullptr) {
      _itoa_s(i, buffer, 10);
      str = "Para";
      str += buffer;
      str = str + " -> " + m_pCUCP[i]->GetDestUnit()->GetName() + '.';
      str += m_pCUCP[i]->GetDestUnit()->GetParaName(m_pCUCP[i]->GetDestIndex());
      if (m_fDeleted[i]) {
        str += '*';
      }
      SendDlgItemMessage(IDC_LIST_COMPONENT_DEL, LB_INSERTSTRING, (WPARAM)j++,
        (LPARAM)(LPCTSTR(str)));
    }
	}
	m_ulChoiceIndex = m_pDeletedPara[i];
	m_ulIndex = i;
	SendDlgItemMessage(IDC_LIST_COMPONENT_DEL, LB_SETCURSEL, m_ulChoiceIndex, 0L);
	
	if ( m_fDeleted[m_ulIndex] ) {
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
}

void CDlgDeleteComponentPara::OnButtonUndelete() 
{
	// TODO: Add your control notification handler code here
	m_fDeleted[m_ulIndex] = FALSE;
	UpdateListBox();
}
