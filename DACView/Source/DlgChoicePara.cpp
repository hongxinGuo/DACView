///////////////////////////////////////////////////////////////////////////
//
// chiocepa.cpp : implementation file
// 用于单元动态连接时选择输入输出参数.	
// 
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "..\\resource.h"
#include "DlgChoicePara.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

// data change function
//////////////////////////////////////////////////////////////////////////
//
// GetDicList
//
// return CDicList & : 
//  
//
///////////////////////////////////////////////////////////////////////////
CDicList &  CDlgChoiceParameter::GetDicList( void ) {
	return( m_CDicList );
}
	
ULONG CDlgChoiceParameter::GetIndex( void ) {
	return( m_ulIndex );
}

/////////////////////////////////////////////////////////////////////////////
// CDlgChoiceParameter dialog

CDlgChoiceParameter::CDlgChoiceParameter(CWnd* pParent /*=nullptr*/)
	: CDialog(CDlgChoiceParameter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChoiceParameter)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CDlgChoiceParameter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChoiceParameter)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgChoiceParameter, CDialog)
	//{{AFX_MSG_MAP(CDlgChoiceParameter)
	ON_CBN_SELCHANGE(IDC_COMBO_CHOICE_PARAMETER, OnSelchangeComboChoiceParameter)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChoiceParameter message handlers

BOOL CDlgChoiceParameter::OnInitDialog()
{
	CDialog::OnInitDialog();           
	
	// TODO: Add extra initialization here
	INT_PTR i, iTotal = m_CDicList.GetCount();
	POSITION po = m_CDicList.GetHeadPosition();
	CUnitDictionary * pDic;

	// 将词典中的内容加入选择框中.
	for ( i = 0; i < iTotal; i++ ) {
		pDic = m_CDicList.GetNext( po );
		SendDlgItemMessage(IDC_COMBO_CHOICE_PARAMETER,
											 CB_INSERTSTRING, (WPARAM)-1,                    
									 		 (LPARAM)((LPSTR)pDic->GetParaName().GetBuffer()));
		}
	if ( i == 0 ) { // no proper paremeter to select
    GetDlgItem(IDOK)->EnableWindow(FALSE);
  }
  else {	// 选择第一个参数.
    SendDlgItemMessage(IDC_COMBO_CHOICE_PARAMETER,
										  CB_SETCURSEL, (WPARAM)0L, (LPARAM)0L);
	  m_ulIndex = SendDlgItemMessage(IDC_COMBO_CHOICE_PARAMETER, CB_GETCURSEL, 0, 0L);
  }
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDlgChoiceParameter::OnSelchangeComboChoiceParameter()
{
	// TODO: Add your control notification handler code here
	m_ulIndex = SendDlgItemMessage(IDC_COMBO_CHOICE_PARAMETER, CB_GETCURSEL, 0, 0L);
}

void CDlgChoiceParameter::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	INT_PTR i, iTotal = m_CDicList.GetCount();
	POSITION po = m_CDicList.GetHeadPosition();
	CUnitDictionary * pDic;

	// 清除词典的内容.
	for ( i = 0; i < iTotal; i++ ) {
		pDic = m_CDicList.GetNext( po );
		delete pDic;
		}
	m_CDicList.RemoveAll();
}
