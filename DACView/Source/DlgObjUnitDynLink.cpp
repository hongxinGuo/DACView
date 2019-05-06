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

// 连接方法的最大值，目前为14种
#define MAX_LINK_METHOD_NUMBER 14

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此处的输入输出方向是已单元为源的，即tINPUT对单元来说是（从对象处）输入数据。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
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
//
//////////////////////////////////////////////////////////////////////////////////////
void CDynamicLinkDlg::SetLink( CDicList * pDicList, CObjectBase * pCObject, 
                               CODLList* LinkList, ParaName * ptName ) {
  m_pDicList				= pDicList;
  m_pCObjectCurrent = pCObject;
  m_plistDynLink			= LinkList;
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
  shared_ptr<CUnitDictionary> pDic;
  CString str;
	ULONG ulAttr;
	CUnitBase * pcunit;
	ULONG ulUnitIndex;
             
  SendDlgItemMessage(IDC_LINK_NAME, CB_RESETCONTENT, 0L, 0L); // clear former Dlg's items
  int j = 0;  // reset count
  for( auto it = m_pDicList->begin(); it != m_pDicList->end(); it++ ) {
    pDic = *it;
		ulAttr = pDic->GetType();
		pcunit = pDic->GetUnit();
		ulUnitIndex = pDic->GetUnitIndex();
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
  for( auto& pDic : *m_pDicList ) {
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
  m_lUnitIndex = pcDyn->GetUnitIndex();
  m_lObjectIndex = pcDyn->GetObjectIndex();
  m_lLinkMethod = pcDyn->GetLinkMethod();
  m_pCUnitCurrent = pcDyn->GetUnit();
  
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
  shared_ptr<CUnitDictionary> pDic;
  auto it = m_pDicList->begin();
	int i = 0;
  do {
 		ASSERT(it != m_pDicList->end());	// 不能越界,出错.
 	  pDic = *it++;
  } while ( (pDic->GetUnit() != pcDyn->GetUnit()) || (pDic->GetUnitIndex() != m_lUnitIndex) );
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
  m_pCObjectCurrent->SelectParameter( tMODIFIABLE ); // 选择当前对象中所有的变量
  while ( m_ptParaName[i].ulType != 0 ) {
    j = SendDlgItemMessage(IDC_VARIABLE_PARAMETER, CB_INSERTSTRING, (WPARAM)-1,                    
                          (LPARAM)((LPSTR)m_ptParaName[i].Name.GetBuffer()));
    i++;
  }  
  
  switch ( m_plistDynLink->size() ) {
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
    m_pCTag = m_plistDynLink->front();
    m_it = m_plistDynLink->begin();
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE); 
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
    UpdateDlg( m_pCTag );
    break;
  default:  // two or more dynamic links
    m_pCTag = m_plistDynLink->front();
    m_it = m_plistDynLink->begin();
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE); 
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(TRUE);
    UpdateDlg( m_pCTag );
    break;
  } // switch

	// Create new dynamic link list
	m_plistNewDynLink = new CODLList;

	CObjectDynLink * pNewDynLink;
	for (const auto pDynLink : *m_plistDynLink ) {
		pNewDynLink = new CObjectDynLink;
		pNewDynLink->Copy( pDynLink );
		m_plistNewDynLink->push_back( pNewDynLink );
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

	ULONG ulObjIndex, ulNewObjIndex, ulUnitIndex, ulNewUnitIndex;
	CUnitBase * pUnit, *pNewUnit;

	// 清除原来的输入连接标志.
	for ( const auto pNewDynLink : *m_plistNewDynLink ) {
		ulNewObjIndex = pNewDynLink->GetObjectIndex();
		pNewUnit = pNewDynLink->GetUnit();
		ulNewUnitIndex = pNewDynLink->GetUnitIndex();
		if ( !pNewDynLink->IsUnitToObject() ) {
// clear flag
pNewUnit->SetParameterLock(ulNewUnitIndex, FALSE);
    }
  }

  // 设置新的输入连接标志.
  for (const auto pDynLink : *m_plistDynLink) {
    ulObjIndex = pDynLink->GetObjectIndex();
    pUnit = pDynLink->GetUnit();
    ulUnitIndex = pDynLink->GetUnitIndex();
    if (!pDynLink->IsUnitToObject()) {
      pUnit->SetParameterSelected(ulUnitIndex, TRUE);
    }
  }

  for (auto pNewDynLink : *m_plistNewDynLink) {
    delete pNewDynLink;
  }
  m_plistNewDynLink->clear();
  delete m_plistNewDynLink;
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
  // delete modified dynamic link list
  for (auto pDynLink : *m_plistDynLink) {
    delete pDynLink;
  }
  m_plistDynLink->clear();

  // set dynamic link list to original
  for (const auto pDynLink : *m_plistNewDynLink) {
    m_plistDynLink->push_back(pDynLink);
  }
  // delete temporary dynamic link list
  for (auto pDynLink : *m_plistNewDynLink) {
    delete pDynLink;
  }
  m_plistNewDynLink->clear();
  delete m_plistNewDynLink;

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

  if (m_pCTag != nullptr) { // have dynamic link ?
    UpdateDynLink(m_pCTag);
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
  m_pCTag->SetObject(m_pCObjectCurrent);
  m_pCObjectCurrent->SelectParameter(tMODIFIABLE);
  m_pCTag->SetObjectIndex(m_pCObjectCurrent->GetIndex(0));
  ParaName * para = m_pCObjectCurrent->GetParaNameAddress();
  ULONG ulAttr = para[0].ulType;
  m_fInputType = ulAttr & tINPUT;
  ULONG ulTypeLowPart = ulAttr & (tDOUBLE | tWORD | tBOOL | tSTRING);
  auto it = m_pDicList->begin();
  bool found = false;
  do {
    auto pDic = *it++;
    ULONG lType = pDic->GetType();
    ULONG lLowPart = lType & (tDOUBLE | tWORD | tBOOL | tSTRING);
    ULONG lHighPart = lType & (tINPUT | tOUTPUT);
    if (m_fInputType) { // 输入型参数
      if (lHighPart == tOUTPUT) { // 则选择单元的输出型参数
        if ((lLowPart & ulTypeLowPart) == lLowPart) { // 找到了，单元的输出参数类型包含于对象的参数类型中
          m_pCUnitCurrent = pDic->GetUnit();
          m_lUnitIndex = pDic->GetUnitIndex();
          m_pCTag->SetUnit(m_pCUnitCurrent);
          m_pCTag->SetUnitIndex(m_lUnitIndex);
          found = true;
        }
      }
    }
    else { // 输出型参数
      if (lHighPart == tINPUT) { //则选择单元的输入型参数
        if ((lLowPart & ulTypeLowPart) == ulTypeLowPart) { // 找到了，对象的参数类型包含于单元的参数类型中
          m_pCUnitCurrent = pDic->GetUnit();
          m_lUnitIndex = pDic->GetUnitIndex();
          m_pCTag->SetUnit(m_pCUnitCurrent);
          m_pCTag->SetUnitIndex(m_lUnitIndex);
          found = true;
        }
      }
    }
  } while (!found && (it != m_pDicList->end()));
  ASSERT(found);

  m_pCTag->SetDataFlowUnitToObject( m_fInputType );
  if (m_fInputType == false) {   // 向单元的参数写入.
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
  m_plistDynLink->push_back(m_pCTag);    // add new dynamic link to list
  m_it = m_plistDynLink->end()--;    // set m_po to current position
  TRACE("%d Dynamic link objects in %s\n", m_plistDynLink->size(), m_strName);
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
  LRESULT i;
  shared_ptr<CUnitDictionary> pDic;

  // Get Object parameter's index
  i = SendDlgItemMessage(IDC_LINK_NAME, CB_GETCURSEL, 0, 0L);
	ASSERT( i < m_pDicList->size() );
  auto it = m_pDicList->begin();   
  do {
    pDic = *it++;
  } while ( pDic->GetIndexNumber() != i );
  m_pCUnitCurrent = pDic->GetUnit();
	m_lUnitIndex = pDic->GetUnitIndex();
}

void CDynamicLinkDlg::OnClickedButtonNext()
{
  // TODO: Add your control notification handler code here
  UpdateDynLink( m_pCTag );
  m_it = find(m_plistDynLink->begin(), m_plistDynLink->end(), m_pCTag);
  m_pCTag = *++m_it;
  UpdateDlg( m_pCTag );
  auto it = m_it;
  if ( ++it == m_plistDynLink->end() ) {
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);  
  }
  GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(TRUE);
}

void CDynamicLinkDlg::OnClickedButtonPrev()
{
  // TODO: Add your control notification handler code here
  UpdateDynLink( m_pCTag );
  m_it = find(m_plistDynLink->begin(), m_plistDynLink->end(), m_pCTag);
  m_pCTag = *--m_it;
  UpdateDlg( m_pCTag ); 
  if ( m_it == m_plistDynLink->begin() ) {
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);  
  }
  GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(TRUE);
}

void CDynamicLinkDlg::OnClickedButtonDelete()
{
  // TODO: Add your control notification handler code here
  CObjectDynLink * pc = nullptr;
  
  if ( m_plistDynLink->size() == 1 ) { // 只有一个元素
    GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);  
    GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BUTTON_DELETE)->EnableWindow(FALSE);
  }
  else {
    m_it = find(m_plistDynLink->begin(), m_plistDynLink->end(), m_pCTag);
    if ( m_it == m_plistDynLink->begin() ) {
      m_plistDynLink->erase(m_it);
      m_it = m_plistNewDynLink->begin();
      
    }
    if ( pc == m_plistDynLink->front() ) {
      GetDlgItem(IDC_BUTTON_PREV)->EnableWindow(FALSE);  
    }
    if ( pc == m_plistDynLink->back() ) {
      GetDlgItem(IDC_BUTTON_NEXT)->EnableWindow(FALSE);  
    }
  }
  
  m_it = find(m_plistDynLink->begin(), m_plistDynLink->end(), m_pCTag);
  m_plistDynLink->erase(m_it);
	  
	delete m_pCTag;
  m_pCTag = pc;
  UpdateDlg( m_pCTag ); 
}
