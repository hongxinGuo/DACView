// Odeldynl.cpp : implementation file
//

#include "stdafx.h"
#include "..\\resource.h"

#include "CObjectBase.h"
#include "CUnitBase.h"
#include "CObjDynLink.h"

#include "DlgObjDeleteDynLink.h"

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
	CObjectDynLink * pDL = m_plistObjectDynLink->GetHead();
	POSITION po = m_plistObjectDynLink->GetHeadPosition();
	ULONG i;

	for ( i = 0; i <= m_ulChoiceIndex; i++ ) {
		pDL = m_plistObjectDynLink->GetNext(po);
	}
	pDL->SetDeleteMeFlag( TRUE );
	UpdateListBox();
}

void CODelDynLink::OnSelchangeListDynlink() 
{
	// TODO: Add your control notification handler code here
	m_ulChoiceIndex = (ULONG)SendDlgItemMessage(IDC_LIST_DYNLINK, LB_GETCURSEL, 0, 0L);
	
	CObjectDynLink * pDL = m_plistObjectDynLink->GetHead();
	POSITION po = m_plistObjectDynLink->GetHeadPosition();
	ULONG i;

	for ( i = 0; i <= m_ulChoiceIndex; i++ ) {
		pDL = m_plistObjectDynLink->GetNext(po);
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

void CODelDynLink::OnDblclkListDynlink() 
{
	// TODO: Add your control notification handler code here
	
}

void CODelDynLink::OnOK() 
{
	// TODO: Add extra validation here
	CObjectDynLink * pDL;
	POSITION po1, po = m_plistObjectDynLink->GetHeadPosition();
	INT_PTR i, iCount = m_plistObjectDynLink->GetCount();


	for ( i = 0; i < iCount; i++ ) {
		pDL = m_plistObjectDynLink->GetNext(po);
		if ( pDL->IsDeleteMe() ) {
			po1 = m_plistObjectDynLink->Find( pDL );
			m_plistObjectDynLink->RemoveAt( po1 );
			delete pDL;
			pDL = nullptr;
		}
	}

	CDialog::OnOK();
}

void CODelDynLink::OnCancel() 
{
	// TODO: Add extra cleanup here
	CObjectDynLink * pDL;
	POSITION po = m_plistObjectDynLink->GetHeadPosition();
	INT_PTR i, iCount = m_plistObjectDynLink->GetCount();


	for ( i = 0; i < iCount; i++ ) {
		pDL = m_plistObjectDynLink->GetNext(po);
		pDL->SetDeleteMeFlag( FALSE );
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
	CObjectDynLink * pDL;
	POSITION po = m_plistObjectDynLink->GetHeadPosition();
	INT_PTR i, j, iCount = m_plistObjectDynLink->GetCount();
	CString strPara;

	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	for ( i = 0; i < iCount; i++ ) {
		pDL = m_plistObjectDynLink->GetNext(po);
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
	}	
	SendDlgItemMessage(IDC_LIST_DYNLINK, LB_SETCURSEL, m_ulChoiceIndex = 0, 0L);

	pDL = m_plistObjectDynLink->GetHead();
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
	CObjectDynLink * pDL = m_plistObjectDynLink->GetHead();
	POSITION po = m_plistObjectDynLink->GetHeadPosition();
	ULONG i;

	for ( i = 0; i <= m_ulChoiceIndex; i++ ) {
		pDL = m_plistObjectDynLink->GetNext(po);
	}
	pDL->SetDeleteMeFlag( FALSE );
	UpdateListBox();
}
