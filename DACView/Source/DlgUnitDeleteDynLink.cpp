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
	CUnitDynLink * pDL = m_plistUnitDynLink->GetHead();
	POSITION po = m_plistUnitDynLink->GetHeadPosition();
	ULONG i;

	for ( i = 0; i <= m_ulChoiceIndex; i++ ) {
		pDL = m_plistUnitDynLink->GetNext(po);
	}
	pDL->SetDeleteMeFlag( TRUE );
	UpdateListBox();
}

void CDelDynLink::OnSelchangeListDynlink() 
{
	// TODO: Add your control notification handler code here
	m_ulChoiceIndex = (ULONG)SendDlgItemMessage(IDC_LIST_DYNLINK, LB_GETCURSEL, 0, 0L);

	CUnitDynLink * pDL = m_plistUnitDynLink->GetHead();
	POSITION po = m_plistUnitDynLink->GetHeadPosition();
	ULONG i;

	for ( i = 0; i <= m_ulChoiceIndex; i++ ) {
		pDL = m_plistUnitDynLink->GetNext(po);
	}
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
	CUnitDynLink * pDL;
	POSITION po1, po = m_plistUnitDynLink->GetHeadPosition();
	INT_PTR i, iCount = m_plistUnitDynLink->GetCount();


	for ( i = 0; i < iCount; i++ ) {
		pDL = m_plistUnitDynLink->GetNext(po);
		if ( pDL->IsDeleteMe() ) {
			po1 = m_plistUnitDynLink->Find( pDL );
			m_plistUnitDynLink->RemoveAt( po1 );
			pDL->GetDestUnit()->SetParameterLock(pDL->GetDestIndex(), FALSE);	// clear selected Flag
			delete pDL;
			pDL = nullptr;
		}
	}

	CDialog::OnOK();
}

void CDelDynLink::OnCancel() 
{
	// TODO: Add extra cleanup here
	CUnitDynLink * pDL;
	POSITION po = m_plistUnitDynLink->GetHeadPosition();
	INT_PTR i, iCount = m_plistUnitDynLink->GetCount();


	for ( i = 0; i < iCount; i++ ) {
		pDL = m_plistUnitDynLink->GetNext(po);
		pDL->SetDeleteMeFlag( FALSE );
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
	CUnitDynLink * pDL;
	POSITION po = m_plistUnitDynLink->GetHeadPosition();
	INT_PTR i, j, iCount = m_plistUnitDynLink->GetCount();
	CString strPara;

	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_RESETCONTENT, (WPARAM)0L, (LPARAM)0L);
	for ( i = 0; i < iCount; i++ ) {
		pDL = m_plistUnitDynLink->GetNext(po);
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
		j = SendDlgItemMessage(IDC_LIST_DYNLINK, LB_INSERTSTRING, (WPARAM)i,                    
											 		 (LPARAM)(LPCTSTR(strPara)));
	}	
	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_SETCURSEL, m_ulChoiceIndex = 0, 0L);
	
	pDL = m_plistUnitDynLink->GetHead();
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
	CUnitDynLink * pDL = m_plistUnitDynLink->GetHead();
	POSITION po = m_plistUnitDynLink->GetHeadPosition();
	ULONG i;

	for ( i = 0; i <= m_ulChoiceIndex; i++ ) {
		pDL = m_plistUnitDynLink->GetNext(po);
	}
	pDL->SetDeleteMeFlag( FALSE );
	UpdateListBox();
}
