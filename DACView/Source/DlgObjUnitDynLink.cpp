// dynamicl.cpp : implementation file
//
// SetLink()						: set up parameter when create dynamick link, 
//												call before create Dialog
// SetLinkMethodIndex()	: Choice suitable link methods, and setup IDC_LINK_METHOD list.
// GetLinkMethodIndex()	: Get link method index from sm_aulSuitable.
// ChangeLinkName()			: 从 m_pDicList 中选出适合的unit的连接参数。
// ResetDicIndex()			: Reset dictionary.
// UpdateDlg()					: 
// UpdateDynLink()			:
// 
// OnInitDialog()				:
// OnOK()								:
// OnCancel()						:
// OnClickButtonNew()		:
// OnClickedDetail()		:
// OnChangeComment()		:
// OnSelchangeLinkMethod() :
// ONSelchangeVariableParameter() :
// OnSelchangeLinkName() :
// OnClickedButtonNext() :
// OnClickButtonPrev() :
// OnClickedButtonDelete() :
//
//
//////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..//resource.h"

#include "DlgObjUnitDynLink.h"

#include "SQIFileDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif  

// 连接方法的最大值
#define MAX_LINK_METHOD_NUMBER 14

static struct { ULONG LinkMethod;
                char * StringName;
                ULONG ulType; } sm_LinkMethod[MAX_LINK_METHOD_NUMBER] =
  { { IDD_INPUT_BOOL,         "INPUT_BOOL",         tINPUT | tBOOL },
    { IDD_INPUT_DWORD,        "INPUT_DWORD",        tINPUT | tWORD },
    { IDD_INPUT_DOUBLE,       "INPUT_DOUBLE",       tINPUT | tDOUBLE },
    { IDD_INPUT_STRING,       "INPUT_STRING",       tINPUT | tSTRING },
    { IDD_OUTPUT_GRAPH,       "OUTPUT_GRAPH",       tOUTPUT | tBOOL | tWORD | tDOUBLE },
		{	IDD_OUTPUT_METER,			  "OUTPUT_METER",       tOUTPUT | tWORD | tDOUBLE },
		{ IDD_OUTPUT_FILLBAR,			"OUTPUT_FILLBAR",     tOUTPUT | tWORD | tDOUBLE },
    { IDD_OUTPUT_GAUGE,       "OUTPUT_GAUGE",       tOUTPUT | tWORD | tDOUBLE },
    { IDD_OUTPUT_STRING,      "OUTPUT_STRING",      tOUTPUT | tSTRING | tBOOL | tWORD | tDOUBLE },
    { IDD_OUTPUT_LOCATION,    "OUTPUT_LOCATION",    tOUTPUT | tWORD | tDOUBLE },
    { IDD_OUTPUT_OBJECT_SIZE, "OUTPUT_OBJECT_SIZE", tOUTPUT | tWORD | tDOUBLE },
    { IDD_OUTPUT_FILL_COLOR,  "OUTPUT_FILL_COLOR",  tOUTPUT | tWORD },
    { IDD_OUTPUT_VISIBILITY,  "OUTPUT_VISIBILITY",  tOUTPUT | tBOOL },
    { IDD_END_LINK_METHOD,    "",  0 },
  };
static INT32  sm_aulSuitable[MAX_LINK_METHOD_NUMBER] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
     
/////////////////////////////////////////////////////////////////////////////
// CDynamicLinkDlg dialog

CDynamicLinkDlg::CDynamicLinkDlg(CWnd* pParent /*=nullptr*/)
  : CDialog(CDynamicLinkDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDynamicLinkDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
} 

/////////////////////////////////////////////////////////////////////////////
//
// SetLink()
//
// Parameter :
//			CDicList * pDicList				: point to current dictionary list.
//			CObjectBase * pCObject		: point current process object.
//			CODLList * LinkList				: point to curent process object's DynLInk list.
//			ParaName * ptName					: point to current process object's ParaName address.
//
// Return : no return
//
// Description :
//		When you want to create an object's dynamic link, you must use this function to 
//  tell the dialog current process information.
//		this is an internal function. Called when want to build dynamic link.
//
//////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::SetLink( CDicList * pDicList, CObjectBase * pCObject, 
                               CODLList* LinkList, ParaName * ptName ) {
  m_pDicList				= pDicList;
  m_pCObjectCurrent = pCObject;
  m_listDynLink			= LinkList;
  m_strName					= m_pCObjectCurrent->GetName();
  m_ptParaName			= ptName;
}

////////////////////////////////////////////////////////////////////////////////////
//
// SetLinkMethodIndex()
//
// Parameter : 
//		ULONG ulType : current suitable type.
//
// Return : no return.
//
// Description :
//		Find the suitable type from the link method group. 
//	then create the mapping index array( sm_aulSuitable ) 
//	and add link method to the IDC_LINK_METHOD listbox.
//	sm_aulSuitable from 0 up.
//
/////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::SetLinkMethodIndex(ULONG ulType) {
  ULONG i = 0;
  ULONG j = 0;

  SendDlgItemMessage(IDC_LINK_METHOD, CB_RESETCONTENT, 0L, 0L);
  while ( sm_LinkMethod[i].ulType != 0 ) {
		// sm_linkMethod must have common point with ultype
    if ( ((sm_LinkMethod[i].ulType & ulType) == sm_LinkMethod[i].ulType) ||
         ((sm_LinkMethod[i].ulType & ulType) == ulType) ) {
      sm_aulSuitable[j++] = i;
    }
		i++;
  }
  sm_aulSuitable[j] = 0;

  i = 0;
  while ( sm_aulSuitable[i] != 0 ) {
    SendDlgItemMessage(IDC_LINK_METHOD, CB_INSERTSTRING, (WPARAM)-1,                    
                      (LPARAM)((LPSTR)sm_LinkMethod[sm_aulSuitable[i]].StringName));
    i++;
  };
  SendDlgItemMessage(IDC_LINK_METHOD, CB_SETCURSEL, 0, 0L);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// GetLInkMethodIndex()
//
// Parameter :
//		ULONG ulLinkMethod : current process link method that need to find index.
//
// Return :
//		ULONG	: found index of current link method.
//
// Description :
//		find the index of link method.
//
///////////////////////////////////////////////////////////////////////////////////////
ULONG CDynamicLinkDlg::GetLinkMethodIndex( ULONG ulLinkMethod ) {
  int i = 0;
  while ( sm_LinkMethod[sm_aulSuitable[i]].LinkMethod != ulLinkMethod ) {
    ASSERT( sm_aulSuitable[i] != 0 );
    i++;
  }
  return( i );
}

////////////////////////////////////////////////////////////////////////////////////
//
// ChangeLinkName()
//
// Parameter :
//		ULONG ulLinkType : current suitable dynamic link type.
//
// Return : no return.
//
// Description :
//		update dialog IDC_LINK_NAME's items. choice from dictionary's proper item
//
////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::ChangeLinkName( ULONG ulLinkType ) {
	CUnitDictionary * pDic;
  POSITION po = m_pDicList->GetHeadPosition();
  INT_PTR i, j, iCount = m_pDicList->GetCount();
  CString str;
	ULONG ulAttr;
	CUnitBase * pcunit;
	ULONG ulUnitIndex;
             
  SendDlgItemMessage(IDC_LINK_NAME, CB_RESETCONTENT, 0L, 0L); // clear former Dlg's items
  j = 0;  // reset count
  for( i = 0; i < iCount; i++ ) {
    pDic = m_pDicList->GetNext(po);
		ulAttr = pDic->GetType();
		pcunit = pDic->GetUnit();
		ulUnitIndex = pDic->GetIndex();
    // if ulLinkType is an Input type, skip the no_match pair
    if ( ulLinkType & tINPUT ) {          // Input type ?
      if ( !(ulAttr & tINPUT) ) continue; // skip output type unit parameter
    }
    // find the proper item
    if ( ulAttr & (ulLinkType & (tBOOL|tSTRING|tWORD|tDOUBLE)) ) {
			pDic->SetIndexNumber( j );  // choice this dictionary item
			j++;                        // increace count
      // add this item to dialog IDC_LINK_NAME
			str = pDic->GetUnit()->GetName();
			str += "->";
			str += pDic->GetParaName();
			SendDlgItemMessage(IDC_LINK_NAME, CB_ADDSTRING, (WPARAM)-1,                    
	                      (LPARAM)((LPCTSTR)str)); 
		}
  }
  ASSERT( SendDlgItemMessage(IDC_LINK_NAME, CB_GETCOUNT, 0L, 0L) == j );
}

//////////////////////////////////////////////////////////////////////////////////
//
// ResetDicIndex()
//
// Parameter : no
//
// Return : no 
//
// Description :
//		clear dictionary's all selected flag. set index to -1.
//
/////////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::ResetDicIndex( void ) {
	CUnitDictionary * pDic;
  POSITION po = m_pDicList->GetHeadPosition();
  INT_PTR i, iCount = m_pDicList->GetCount();
  for( i = 0; i < iCount; i++ ) {
    pDic = m_pDicList->GetNext(po);
		pDic->SetIndexNumber(-1);
  }
}

///////////////////////////////////////////////////////////////////////////////////////
// 
// UpdateDlg()
//
// Parameter :
//		CObjectDynLink * pcDyn : point  to current process objectDynLink
//
// Return : no
//
// Description :
//		Update all dialog's item.
//
///////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::UpdateDlg( CObjectDynLink * pcDyn ) {
  if ( pcDyn == nullptr ) {       // no dynamic link ?
    // disable all dialog's items
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
    GetDlgItem(IDC_VARIABLE_PARAMETER)->EnableWindow(FALSE);
    GetDlgItem(IDC_LINK_METHOD)->EnableWindow(FALSE);
    GetDlgItem(IDC_LINK_NAME)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMMENT)->EnableWindow(FALSE);
    return;
  }
  // update variables
  m_lUnitIndex						= pcDyn->GetUnitIndex();
  m_lObjectIndex					= pcDyn->GetObjectIndex();
  m_lLinkMethod					= pcDyn->GetLinkMethod();
  m_pCUnitCurrent					= pcDyn->GetUnit();
  
  m_pCObjectCurrent->SelectParameter( tMODIFIABLE );
  SendDlgItemMessage(IDC_VARIABLE_PARAMETER, CB_SETCURSEL, m_lObjectIndex, 0L);
  
	ResetDicIndex(); // clear dictionary's selected flag
	// update dictionary's selected flag
	ParaName * pp;
	INT32 lObjectDynLinkType = m_pCObjectCurrent->GetDynLinkType(m_lObjectIndex);
	pp = m_pCObjectCurrent->GetParaNameAddress();
	pp += m_lObjectIndex;
	ULONG ulAttr = pp->ulType;
  m_fInputType = ulAttr & tINPUT;
	if ( m_fInputType == FALSE ) {  // object -> unit ?
		ChangeLinkName( lObjectDynLinkType | tINPUT );	// output value can only link to unit's input
    SetDlgItemText(IDC_STATIC_UNIT_TO_OBJECT, "Object->Unit");  // update data flow
    SetLinkMethodIndex( lObjectDynLinkType | tINPUT );
	}
	else {                            // unit -> object
		ChangeLinkName( lObjectDynLinkType ); // input value can link to unit's input and output.
    SetDlgItemText(IDC_STATIC_UNIT_TO_OBJECT, "Unit->Object");
    SetLinkMethodIndex( lObjectDynLinkType | tOUTPUT );
	}

  // find current show dictionary's item
  CUnitDictionary * pDic;
  POSITION po = m_pDicList->GetHeadPosition();
	int i = 0;
  do {
  	pDic = m_pDicList->GetNext(po);
		ASSERT( i++ < m_pDicList->GetCount() );	// 不能越界,出错.
  } while ( (pDic->GetUnit() != pcDyn->GetUnit()) || (pDic->GetIndex() != m_lUnitIndex) );
  SendDlgItemMessage(IDC_LINK_NAME, CB_SETCURSEL, (WPARAM)pDic->GetIndexNumber(), (LPARAM)0L);
  SendDlgItemMessage(IDC_LINK_METHOD, CB_SETCURSEL,(WPARAM)GetLinkMethodIndex(pcDyn->GetLinkMethod()));
  // update comment
  SetDlgItemText(IDC_COMMENT, pcDyn->GetComment());

} 

//////////////////////////////////////////////////////////////////////////////////////////
//
// UpdateDynLink
//
// Parameter :
//		CObjectDynLink * pcDyn : DynLink that need update.
//
// Return : no
//
// Description :
//		Update current DynLink 
//
/////////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::UpdateDynLink( CObjectDynLink * pcDyn ) {
  char buffer[256];
  
  pcDyn->SetLinkMethod(m_lLinkMethod);
  pcDyn->SetName(m_pCObjectCurrent->GetName());
  pcDyn->SetUnit(m_pCUnitCurrent);
  pcDyn->SetObjectIndex(m_lObjectIndex);
  pcDyn->SetUnitIndex(m_lUnitIndex);
	pcDyn->SetDataFlowUnitToObject( m_fInputType );
  GetDlgItemText(IDC_COMMENT, buffer, 256);
  pcDyn->SetComment(buffer);
}

void CDynamicLinkDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDynamicLinkDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDynamicLinkDlg, CDialog)
  //{{AFX_MSG_MAP(CDynamicLinkDlg)
  ON_BN_CLICKED(IDC_BUTTON_NEW, OnClickedButtonNew)
  ON_BN_CLICKED(IDC_DATAIL, OnClickedDetail)
  ON_EN_CHANGE(IDC_COMMENT, OnChangeComment)
  ON_CBN_SELCHANGE(IDC_LINK_METHOD, OnSelchangeLinkMethod)
  ON_CBN_SELCHANGE(IDC_VARIABLE_PARAMETER, OnSelchangeVariableParameter)
  ON_CBN_SELCHANGE(IDC_LINK_NAME, OnSelchangeLinkName)
  ON_BN_CLICKED(IDC_BUTTON_NEXT, OnClickedButtonNext)
  ON_BN_CLICKED(IDC_BUTTON_PREV, OnClickedButtonPrev)
  ON_BN_CLICKED(IDC_BUTTON_DELETE, OnClickedButtonDelete)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicLinkDlg message handlers

/////////////////////////////////////////////////////////////////////////////////////////
//
// OnInitDialog()
//
// Parameter : no
//
// Return : 
//		BOOL : TRUE
//
// Description :
//		Initialize Dialog. Create temporary dynamic link list. 
//
/////////////////////////////////////////////////////////////////////////////////////////
BOOL CDynamicLinkDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  // TODO: Add extra initialization here
  int i = 0;
  LRESULT j; 
  SendDlgItemMessage(IDC_TAG_NAME, WM_SETTEXT, (WPARAM)0L, (LPARAM)((LPSTR)(m_strName.GetBuffer(m_strName.GetLength()))));

  i = 0;
  m_pCObjectCurrent->SelectParameter( tMODIFIABLE );
  while ( m_ptParaName[i].ulType != 0 ) {
    j = SendDlgItemMessage(IDC_VARIABLE_PARAMETER, CB_INSERTSTRING, (WPARAM)-1,                    
                          (LPARAM)((LPSTR)m_ptParaName[i].Name.GetBuffer()));
    i++;
  }  
  
  switch ( m_listDynLink->GetCount() ) {
  case 0 :  // no dynamic link
    m_pCTag = nullptr;
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
    GetDlgItem(IDC_VARIABLE_PARAMETER)->EnableWindow(FALSE);
    GetDlgItem(IDC_LINK_METHOD)->EnableWindow(FALSE);
    GetDlgItem(IDC_LINK_NAME)->EnableWindow(FALSE);
    GetDlgItem(IDC_COMMENT)->EnableWindow(FALSE);

    break;
  case 1 :  // only one dynamic link
    m_pCTag = m_listDynLink->GetHead();
    m_po = m_listDynLink->GetHeadPosition();
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE); 
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
    UpdateDlg( m_pCTag );
    break;
  default:  // two or more dynamic links
    m_pCTag = m_listDynLink->GetHead();
    m_po = m_listDynLink->GetHeadPosition();
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE); 
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
    UpdateDlg( m_pCTag );
    break;
  } // switch

	// Create new dynamic link list
	m_listNewDynLink = new CODLList;

	CObjectDynLink * pDynLink, * pNewDynLink;
	POSITION po = m_listDynLink->GetHeadPosition();
	for ( i = 0; i < m_listDynLink->GetCount(); i ++ ) {
		pNewDynLink = new CObjectDynLink;
		pDynLink = m_listDynLink->GetNext( po );
		pNewDynLink->Copy( pDynLink );
		m_listNewDynLink->AddTail( pNewDynLink );
	}
  
  return TRUE;  // return TRUE  unless you set the focus to a control
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// OnOK()
//
// Description:
//		Update dynamic link list.
//
///////////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::OnOK()
{
  // TODO: Add extra validation here
  if ( m_pCTag != nullptr ) UpdateDynLink(m_pCTag); // restore variable

	POSITION po, poNew;
	INT_PTR i, iTotal = m_listDynLink->GetCount();
	INT_PTR j, jTotal = m_listNewDynLink->GetCount();
	ULONG ulObjIndex, ulNewObjIndex, ulUnitIndex, ulNewUnitIndex;
	CUnitBase * pUnit, *pNewUnit;
	CObjectDynLink * pDynLink, *pNewDynLink;

	// 清除原来的输入连接标志.
	poNew = m_listNewDynLink->GetHeadPosition();
	for ( j = 0; j < jTotal; j++ ) {
		pNewDynLink = m_listNewDynLink->GetNext( poNew );
		ulNewObjIndex = pNewDynLink->GetObjectIndex();
		pNewUnit = pNewDynLink->GetUnit();
		ulNewUnitIndex = pNewDynLink->GetUnitIndex();
		if ( !pNewDynLink->IsUnitToObject() ) {
			// clear flag
			pNewUnit->SetParameterLock( ulNewUnitIndex, FALSE );
		}
	}

	// 设置新的输入连接标志.
	po = m_listDynLink->GetHeadPosition();
	for ( i = 0; i < iTotal; i++ ) {
		pDynLink = m_listDynLink->GetNext( po );
		ulObjIndex = pDynLink->GetObjectIndex();
		pUnit = pDynLink->GetUnit();
		ulUnitIndex = pDynLink->GetUnitIndex();
		if ( !pDynLink->IsUnitToObject() ) {
			pUnit->SetParameterSelected( ulUnitIndex, TRUE );
		}
	}

	poNew = m_listNewDynLink->GetHeadPosition();
	for ( j = 0; j < jTotal; j++ ) {
		pNewDynLink = m_listNewDynLink->GetNext( poNew );
		delete pNewDynLink;
	}
	m_listNewDynLink->RemoveAll();
	delete m_listNewDynLink;		
  CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////////////////
//
// OnCancel()
//
// Description:
//		Cancel updated list. Restore origin dynamic link list.
//
//////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::OnCancel()
{
  // TODO: Add extra cleanup here  
	POSITION po = m_listDynLink->GetHeadPosition();
	INT_PTR i, iTotal = m_listDynLink->GetCount();
	CObjectDynLink * pDynLink;

	// delete modified dynamic link list
	for ( i = 0; i < iTotal; i++ ) {
		pDynLink = m_listDynLink->GetNext( po );
		delete pDynLink;
	}
	m_listDynLink->RemoveAll();

	po = m_listNewDynLink->GetHeadPosition();
	iTotal = m_listNewDynLink->GetCount();

	// set dynamic link list to original
	for ( i = 0; i < iTotal; i++ ) {
		pDynLink = m_listNewDynLink->GetNext( po );
		m_listDynLink->AddTail( pDynLink );
	}
	// delete temporary dynamic link list
	m_listNewDynLink->RemoveAll();
	delete m_listNewDynLink;

	CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////////////////////////
//
// OnClickedButtonNew()
//
// Description :
//  Create new dynamic link.
//
//////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::OnClickedButtonNew()
{
  // TODO: Add your control notification handler code here         
  
  if ( m_pCTag != nullptr ) { // have dynamic link ?
    UpdateDynLink( m_pCTag );
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);  
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(TRUE);
  }
  else {                   // no dynamic link 
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);  
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
  }
  GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
    
  GetDlgItem(IDC_VARIABLE_PARAMETER)->EnableWindow(TRUE);
  GetDlgItem(IDC_LINK_METHOD)->EnableWindow(TRUE);
  GetDlgItem(IDC_LINK_NAME)->EnableWindow(TRUE);
  GetDlgItem(IDC_COMMENT)->EnableWindow(TRUE);
  
  // create new dynamic link 
  m_pCTag = new CObjectDynLink;
  m_pCTag->SetName("Untitled"); 
  m_pCTag->SetObjectIndex(0);
  m_pCUnitCurrent = m_pDicList->GetHead()->GetUnit();
  m_pCTag->SetUnit( m_pCUnitCurrent );
  m_pCTag->SetObject( m_pCObjectCurrent );
  m_pCObjectCurrent->SelectParameter( tMODIFIABLE );
  m_pCTag->SetObjectIndex(m_pCObjectCurrent->GetIndex(0));
  ParaName * para = m_pCObjectCurrent->GetParaNameAddress();
  ULONG ulAttr = para[0].ulType;
  m_fInputType = ulAttr & tINPUT;
  m_pCTag->SetDataFlowUnitToObject( m_fInputType );
  if ( m_fInputType == FALSE ) {   // 向单元的参数写入.
    m_pCUnitCurrent->SelectParameter( (ulAttr | tINPUT) & (tINPUT |tDOUBLE |tWORD |tBOOL |tSTRING |tMODIFIABLE) );
    SetLinkMethodIndex( (ulAttr | tINPUT) & (tINPUT | tDOUBLE | tWORD | tBOOL | tSTRING) ); 
  }
  else {                            // 从单元中读出.
    m_pCUnitCurrent->SelectParameter( ulAttr & (tDOUBLE | tWORD | tBOOL | tSTRING | tMODIFIABLE) );
    SetLinkMethodIndex( (ulAttr | tOUTPUT) & (tOUTPUT | tDOUBLE | tWORD | tBOOL | tSTRING) ); 
  }
  m_pCTag->SetUnitIndex(m_pCUnitCurrent->GetIndex(0));
  m_pCTag->SetLinkMethod(sm_LinkMethod[sm_aulSuitable[0]].LinkMethod);
  m_pCTag->SetComment("");
  m_listDynLink->AddTail(m_pCTag);    // add new dynamic link to list
  m_po = m_listDynLink->GetTailPosition();    // set m_po to current position
  TRACE("%d Dynamic link objects in %s\n", m_listDynLink->GetCount(), m_strName);
  UpdateDlg( m_pCTag );
}

void CDynamicLinkDlg::OnClickedDetail()
{
  // TODO: Add your control notification handler code here
  
}

////////////////////////////////////////////////////////////////////////////////////
//
// OnChangeComment()
//
//
////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::OnChangeComment()
{
  // TODO: Add your control notification handler code here
  CString strTemp;
  char buffer[256];
  GetDlgItemText(IDC_COMMENT, buffer, 256); 
  strTemp = buffer;
  m_pCTag->SetComment(strTemp);
}

void CDynamicLinkDlg::OnSelchangeLinkMethod()
{
  // TODO: Add your control notification handler code here
  ULONG a;
	a = (ULONG)SendDlgItemMessage(IDC_LINK_METHOD, CB_GETCURSEL, 0, 0L);
  m_lLinkMethod = sm_LinkMethod[sm_aulSuitable[a]].LinkMethod;
}

void CDynamicLinkDlg::OnSelchangeVariableParameter()
{
  // TODO: Add your control notification handler code here
  LRESULT i;
  
  // Get Object parameter's index
  i = SendDlgItemMessage(IDC_VARIABLE_PARAMETER, CB_GETCURSEL, 0, 0L);
  m_lObjectIndex = m_pCObjectCurrent->GetIndex(i);
	ResetDicIndex();
	ParaName * pp;
	INT32 lObjectLinkType = m_pCObjectCurrent->GetDynLinkType( m_lObjectIndex );
	pp = m_pCObjectCurrent->GetParaNameAddress();
	pp += m_lObjectIndex;
	ULONG ulAttr = pp->ulType;
  m_fInputType = ulAttr & tINPUT;
	if ( m_fInputType == FALSE ) {	
		ChangeLinkName( lObjectLinkType | tINPUT );	// output value can link to input unit only.
    SetLinkMethodIndex( lObjectLinkType | tINPUT );
	}
	else {
		ChangeLinkName( lObjectLinkType );
    SetLinkMethodIndex( lObjectLinkType | tOUTPUT );
	}	
	SendDlgItemMessage(IDC_LINK_NAME, CB_SETCURSEL, 0, 0L);
  SendDlgItemMessage(IDC_LINK_METHOD, CB_SETCURSEL, 0, 0L);
  m_lLinkMethod = sm_LinkMethod[sm_aulSuitable[0]].LinkMethod;
}

void CDynamicLinkDlg::OnSelchangeLinkName()
{
  // TODO: Add your control notification handler code here
  POSITION po;
  LRESULT i;
	CUnitDictionary * pDic;
  
  // Get Object parameter's index
  i = SendDlgItemMessage(IDC_LINK_NAME, CB_GETCURSEL, 0, 0L);
	ASSERT( i < m_pDicList->GetCount() );
  po = m_pDicList->GetHeadPosition();   
  do {
    pDic = m_pDicList->GetNext(po);
  } while ( pDic->GetIndexNumber() != i );
  m_pCUnitCurrent = pDic->GetUnit();
	m_lUnitIndex = pDic->GetIndex();
}

void CDynamicLinkDlg::OnClickedButtonNext()
{
  // TODO: Add your control notification handler code here
  UpdateDynLink( m_pCTag );
  m_po = m_listDynLink->Find(m_pCTag);
  m_pCTag = m_listDynLink->GetNext(m_po);
  m_pCTag = m_listDynLink->GetNext(m_po);
  UpdateDlg( m_pCTag );
  if ( m_pCTag == m_listDynLink->GetTail() ) {
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);  
  }
  GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(TRUE);
}

void CDynamicLinkDlg::OnClickedButtonPrev()
{
  // TODO: Add your control notification handler code here
  UpdateDynLink( m_pCTag );
  m_po = m_listDynLink->Find(m_pCTag);
  m_pCTag = m_listDynLink->GetPrev(m_po);
  m_pCTag = m_listDynLink->GetPrev(m_po);
  UpdateDlg( m_pCTag ); 
  if ( m_pCTag == m_listDynLink->GetHead() ) {
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);  
  }
  GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
}

void CDynamicLinkDlg::OnClickedButtonDelete()
{
  // TODO: Add your control notification handler code here
  CObjectDynLink * pc = nullptr;
  
  if ( m_listDynLink->GetCount() == 1 ) {
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);  
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
  }
  else {
    m_po = m_listDynLink->Find(m_pCTag);
    if ( m_po != m_listDynLink->GetTailPosition() ) {
      pc = m_listDynLink->GetNext(m_po);
      pc = m_listDynLink->GetNext(m_po);
    }
    else {
      pc = m_listDynLink->GetPrev(m_po);
      pc = m_listDynLink->GetPrev(m_po);
    }
    if ( pc == m_listDynLink->GetHead() ) {
      GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);  
    }
    if ( pc == m_listDynLink->GetTail() ) {
      GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);  
    }
  }
  
  m_po = m_listDynLink->Find(m_pCTag);
  m_listDynLink->RemoveAt(m_po);
	  
	delete m_pCTag;
  m_pCTag = pc;
  UpdateDlg( m_pCTag ); 
}
