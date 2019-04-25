// Odeldynl.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"

#include "CObjectBase.h"
#include "CUnitBase.h"
#include "CObjDynLink.h"

#include "DlgObjDeleteDynLink.h"

using namespace std;
#include<algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CODelDynLink dialog


CODelDynLink::CODelDynLink(CWnd* pParent /*=nullptr*/)
	: CDialog(CODelDynLink::IDD, pParent)
{
	//{{AFX_DATA_INIT(CODelDynLink)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CODelDynLink::SetLink( CODLList * plistObjectDynLink, CObjectBase * pcobj ) {
	ASSERT( plistObjectDynLink != nullptr );
	ASSERT( pcobj != nullptr );
	m_plistObjectDynLink = plistObjectDynLink;
	m_pcobj = pcobj;
}

void CODelDynLink::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CODelDynLink)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CODelDynLink, CDialog)
	//{{AFX_MSG_MAP(CODelDynLink)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_DYNLINK, OnSelchangeListDynlink)
	ON_LBN_DBLCLK(IDC_LIST_DYNLINK, OnDblclkListDynlink)
	ON_BN_CLICKED(IDC_BUTTON_UNDELETE, OnButtonUndelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CODelDynLink message handlers

void CODelDynLink::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	auto it = m_plistObjectDynLink->begin();

	for ( int i = 0; i <= m_ulChoiceIndex; i++ ) {
		it++;
	}
  auto pODL = *it;
	pODL->SetDeleteMeFlag( TRUE );
	UpdateListBox();
}

void CODelDynLink::OnSelchangeListDynlink() 
{
	// TODO: Add your control notification handler code here
	m_ulChoiceIndex = (ULONG)SendDlgItemMessage(IDC_LIST_DYNLINK, LB_GETCURSEL, 0, 0L);
	
	auto it = m_plistObjectDynLink->begin();

	for ( int i = 0; i <= m_ulChoiceIndex; i++ ) {
    it++;
	}
  auto pODL = *it;
	if ( pODL->IsDeleteMe() ) {
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
}

void CODelDynLink::OnDblclkListDynlink() 
{
	// TODO: Add your control notification handler code here
	
}

void CODelDynLink::OnOK() 
{
	// TODO: Add extra validation here

  for (auto it = m_plistObjectDynLink->begin(); it != m_plistObjectDynLink->end(); it++) {
    auto pODL = *it;
		if ( pODL->IsDeleteMe() ) {
			m_plistObjectDynLink->erase( it );
			delete pODL;
			pODL = nullptr;
		}
	}

	CDialog::OnOK();
}

void CODelDynLink::OnCancel() 
{
	// TODO: Add extra cleanup here

	for ( const auto pODL : *m_plistObjectDynLink ) {
		pODL->SetDeleteMeFlag( FALSE );
	}
	CDialog::OnCancel();
}

BOOL CODelDynLink::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateListBox();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CODelDynLink::UpdateListBox( void ) {
	INT_PTR i = 0, j;
	CString strPara;

	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	for ( const auto pDL : *m_plistObjectDynLink ) {
		strPara = m_pcobj->GetName();
		strPara += '.';
		strPara += m_pcobj->GetParaName( pDL->GetObjectIndex() );
		strPara += "-->";
		strPara += pDL->GetUnit()->GetName();
		strPara += '.';
		strPara += (pDL->GetUnit())->GetParaName( pDL->GetUnitIndex() );
		if ( pDL->IsDeleteMe() ) {
			strPara += "*";
		}
		j = SendDlgItemMessage(IDC_LIST_DYNLINK, LB_INSERTSTRING, (WPARAM)i,                    
											 		 (LPARAM)(LPCTSTR(strPara)));
    i++;
	}	
	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_SETCURSEL, m_ulChoiceIndex = 0, 0L);

	auto pDL = m_plistObjectDynLink->front();
	if ( pDL->IsDeleteMe() ) {
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(TRUE);
	}
	else {
		GetDlgItem(IDC_BUTTON_UNDELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
	}
}

void CODelDynLink::OnButtonUndelete() 
{
	// TODO: Add your control notification handler code here
  auto it = m_plistObjectDynLink->begin();

	for ( int i = 0; i <= m_ulChoiceIndex; i++ ) {
    it++;
	}
  auto pDL = *it;
	pDL->SetDeleteMeFlag( FALSE );
	UpdateListBox();
}
