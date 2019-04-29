// deldynli.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"
#include "DlgUnitDeleteDynLink.h"

#include "CUnitDynLink.h"
#include "CUnitBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDelDynLink dialog


CDelDynLink::CDelDynLink(CWnd* pParent /*=nullptr*/)
	: CDialog(CDelDynLink::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDelDynLink)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDelDynLink::SetLink( CUDLList * plistUnitDynLink ) {
	ASSERT( plistUnitDynLink != nullptr );
	m_plistUnitDynLink = plistUnitDynLink;
}

void CDelDynLink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDelDynLink)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDelDynLink, CDialog)
	//{{AFX_MSG_MAP(CDelDynLink)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_DYNLINK, OnSelchangeListDynlink)
	ON_LBN_DBLCLK(IDC_LIST_DYNLINK, OnDblclkListDynlink)
	ON_BN_CLICKED(IDC_BUTTON_UNDELETE, OnButtonUndelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDelDynLink message handlers

void CDelDynLink::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	auto it = m_plistUnitDynLink->begin();

	for ( int i = 0; i < m_ulChoiceIndex; i++ ) {
    it++;
	}
  ASSERT(it != m_plistUnitDynLink->end());
  auto pDL = *it;
  pDL->SetDeleteMeFlag(true);
	UpdateListBox();
}

void CDelDynLink::OnSelchangeListDynlink() 
{
	// TODO: Add your control notification handler code here
	m_ulChoiceIndex = (ULONG)SendDlgItemMessage(IDC_LIST_DYNLINK, LB_GETCURSEL, 0, 0L);

  auto it = m_plistUnitDynLink->begin();
	for ( int i = 0; i < m_ulChoiceIndex; i++ ) {
    it++;
	}
  ASSERT(it != m_plistUnitDynLink->end());
  auto pDL = *it;
	if ( pDL->IsDeleteMe() ) {
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
}

void CDelDynLink::OnDblclkListDynlink() 
{
	// TODO: Add your control notification handler code here
	
}

void CDelDynLink::OnOK() 
{
	// TODO: Add extra validation here
  auto it = m_plistUnitDynLink->begin();
  do {
    auto pDL = *it;
		if ( pDL->IsDeleteMe() ) {
			pDL->GetDestUnit()->SetParameterLock(pDL->GetDestIndex(), false);	// clear selected Flag
			it = m_plistUnitDynLink->erase(it);
		}
    else it++;
  } while (it != m_plistUnitDynLink->end());

	CDialog::OnOK();
}

void CDelDynLink::OnCancel() 
{
	// TODO: Add extra cleanup here
	for ( const auto pDL : *m_plistUnitDynLink ) {
		pDL->SetDeleteMeFlag(false);
	}
	CDialog::OnCancel();
}

BOOL CDelDynLink::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateListBox();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDelDynLink::UpdateListBox( ) {
	CString strPara;

  int i = 0, j;

	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_RESETCONTENT, (WPARAM)0L, (LPARAM)0L);
	for ( const auto pDL : *m_plistUnitDynLink ) {
		strPara = pDL->GetSrcUnit()->GetName();
		strPara += '.';
		strPara += (pDL->GetSrcUnit())->GetParaName( pDL->GetSrcIndex() );
		strPara += "-->";
		strPara += pDL->GetDestUnit()->GetName();
		strPara += '.';
		strPara += (pDL->GetDestUnit())->GetParaName( pDL->GetDestIndex() );
		if ( pDL->IsDeleteMe() ) {
			strPara += "*";
		}
		j = SendDlgItemMessage(IDC_LIST_DYNLINK, LB_INSERTSTRING, (WPARAM)i++,                    
											 		 (LPARAM)(LPCTSTR(strPara)));
	}	
	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_SETCURSEL, m_ulChoiceIndex = 0, 0L);
	
	shared_ptr<CUnitDynLink> pDL = m_plistUnitDynLink->front();
	if ( pDL->IsDeleteMe() ) {
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
}

void CDelDynLink::OnButtonUndelete() 
{
	// TODO: Add your control notification handler code here
  auto it = m_plistUnitDynLink->begin();
  for (int i = 0; i < m_ulChoiceIndex; i++) {
    it++;
	}
  auto pDL = *it;
  pDL->SetDeleteMeFlag(false);
	UpdateListBox();
}
