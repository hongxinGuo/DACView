///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		处理SQI文件时调用
//
// CSQIFileDoc()					: initialize
// ~CSQIFileDoc()					:
// 
// CreateDicList()				: Create Dictionary list according m_CRunTimeUnitList
// ClearDicList()					:
// ClearUnitList()	
// LoadUnitList()
// SaveUnitList()
//
// GetObjNumber()         : Get Current object counter number
// GetObjNumberUpdated()  : Increase current unit counter by 1, and return this number.
// GetUnitNumber()				: Get current unit counter number
// GetUnitNumberUpdated() :	Increase current unit counter by 1, and return this number.
//
// ClearLoopDetectFlag()  : Clear all loop detect flag in m_CUnitList
// LoopDetect()           : Check weither have a loop dynamic links in m_CUnitList
// AlreadyHaveCutOff()    : Check weither loop have a CutOff
//
// CreateRunTimeUnitList(): Create runtime unit list.
//
// Serialize()						:
//
// GetUnitList()					: return &m_CUnitList
// GetRunTimeUnitList()		: return &m_CRunTimeUnitList
// GetObjectList()				: return &m_CObjectList
// GetDictionaryList()		: return &m_CDicList
//
// OnProjectCompile()     : Compile this object and unit list
// OnUpdateProjectCompile()	 : 
//
// DeleteDynLinkToMe()    : Delete all dynamic links that link to me
//
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Dacview.h"
#include "globedef.h"

#include "SQIFileDoc.h"

#include "cUnitDynLink.h"
#include "CUnitComponent.h"

#include"CompileUnitList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSQIFileDoc

IMPLEMENT_DYNCREATE(CSQIFileDoc, CDocument)

BEGIN_MESSAGE_MAP(CSQIFileDoc, CDocument)
  //{{AFX_MSG_MAP(CSQIFileDoc)
  ON_COMMAND(ID_PROJECT_COMPILE, OnProjectCompile)
  ON_UPDATE_COMMAND_UI(ID_PROJECT_COMPILE, OnUpdateProjectCompile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSQIFileDoc construction/destruction

CSQIFileDoc::CSQIFileDoc()
{
  // TODO: add one-time construction code here
  m_nCurrentObjNumber = 0;
  m_nCurrentUnitNumber = 0;
  m_pCurrentUnitList = &m_CUnitList;

	m_trackerUnit.m_nStyle &= ~CRectTracker::dottedLine;
	m_trackerUnit.m_nStyle ^= CRectTracker::solidLine;
	m_trackerUnit.m_nStyle &= ~CRectTracker::resizeOutside;
  m_trackerUnit.m_nStyle ^= CRectTracker::resizeInside;
  m_trackerUnit.m_nHandleSize = 8;
	m_trackerUnit.m_rect.left = 0;
	m_trackerUnit.m_rect.top = 0;
	m_trackerUnit.m_rect.right = 0;
	m_trackerUnit.m_rect.bottom = 0;

	m_trackerObject.m_nStyle &= ~CRectTracker::dottedLine;
	m_trackerObject.m_nStyle ^= CRectTracker::solidLine;
	m_trackerObject.m_nStyle &= ~CRectTracker::resizeOutside;
  m_trackerObject.m_nStyle ^= CRectTracker::resizeInside;
  m_trackerObject.m_nHandleSize = 8;
	m_trackerObject.m_rect.left = 0;
	m_trackerObject.m_rect.top = 0;
	m_trackerObject.m_rect.right = 0;
	m_trackerObject.m_rect.bottom = 0;

}

CSQIFileDoc::~CSQIFileDoc() {

  ReleaseSQIFile(&m_CUnitList, &m_CObjectList);

  m_CRunTimeUnitList.RemoveAll(); // 运行时单元序列也需要删除

  // 删除字典序列
  CUnitDictionary * pcDic;
  for ( auto it = m_CDicList.begin(); it != m_CDicList.end(); it++ ) {
    pcDic = *it;
    delete pcDic;
  } 
  m_CDicList.clear();
}

bool ReleaseSQIFile(CUnitList * pUnitList, CObjectList * pObjectList) {
  // release Object's memory
  POSITION Po = pObjectList->GetHeadPosition();
  CObjectBase * pcobjTemp;
  INT_PTR iTemp = pObjectList->GetCount();
  for (int i = 0; i < iTemp; i++) {
    pcobjTemp = pObjectList->GetNext(Po);
    delete pcobjTemp;
    pcobjTemp = nullptr;
  }
  // release list's memory
  pObjectList->RemoveAll();

  Po = pUnitList->GetHeadPosition();
  CUnitBase * pcunit;
  iTemp = pUnitList->GetCount();
  for (int i = 0; i < iTemp; i++) {
    pcunit = pUnitList->GetNext(Po);
    delete pcunit;
    pcunit = nullptr;
  }
  // release list's memory
  pUnitList->RemoveAll();

  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// LoadUnitList()
//
// Parameter : 
//		CArchive & ar : archive
//
// Return :
//		BOOL : TRUE if seccess, FALSE if failed
//
//////////////////////////////////////////////////////////////////////////////////////////
bool LoadUnitList( CArchive & ar, CUnitList * pUnitList, INT64 * pUnitNumber ) {
  // Load strategy file
  CString strTemp;
  static CString strStrategyFile = "DACVIEW_STRATEGY_VERSION_001_002";
  CUnitBase * pcunit;
  INT64 iTemp, iUnitNumber;

  ar >> strTemp >> iUnitNumber >> iTemp;
  *pUnitNumber = iUnitNumber;
  // test whether a strategy file
  if ( strTemp == strStrategyFile );
  else {
		ShowMessage(ID_ERROR_READ_FILE, (LPCTSTR)strStrategyFile);
    return( FALSE );
  }
  // load origin cUnitList
  for ( int i = 0; i < iTemp; i ++ ) {
    ar >> pcunit;
    pUnitList->AddTail( pcunit );
    pcunit->SetUpperUnitList(pUnitList);
  }  
  return( true );
}  

bool LoadObjectList(CArchive & ar, CObjectList * pObjectList, INT32 * pObjectNumber) {
  INT32 nCurrentObjNumber;
  INT64 iTotal;
  CObjectBase * pcobj = nullptr;
  CString strTemp;
  static CString strViewFile = "DACVIEW_VIEW_VERSION_001_002";

  ar >> strTemp >> nCurrentObjNumber >> iTotal;
  *pObjectNumber = nCurrentObjNumber;
  // test whether a Dacview file
  for (int i = 0; i < iTotal; i++) {
    ar >> pcobj;
    pObjectList->AddTail(pcobj);
  }
  return(true);
}

bool LoadSQIFile(CArchive & ar, CUnitList * pUnitList, CObjectList * pObjectList, INT64 * pUnitNumber, INT32 * pObjectNumber) {
  LoadUnitList(ar, pUnitList, pUnitNumber);
  LoadObjectList(ar, pObjectList, pObjectNumber);
  return(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// SaveUnitList()
//
// Parameter : 
//		CArchive& ar : archive
//
// return : no return
//
// Description :
//		Save m_CUnitList and m_CRunTimeUnitList
//	
////////////////////////////////////////////////////////////////////////////////////////////
void CSQIFileDoc::SaveUnitList( CArchive& ar ) {
  CUnitBase * pcunit;
  POSITION pos = m_CUnitList.GetHeadPosition();                          
  INT64 iTemp = m_CUnitList.GetCount();
  CString strStrategyFile;
  
	//存储UnitList
  VERIFY(strStrategyFile.LoadString(IDS_STRATEGY_FILE_VERSION));
  ar << strStrategyFile << iTemp;
  for ( int i = 0; i < iTemp; i ++ ) { 
    pcunit = m_CUnitList.GetNext(pos);
    ar << pcunit;
  } 

	//存储RunTimeUnitList
	pos = m_CRunTimeUnitList.GetHeadPosition();                          
  iTemp = m_CRunTimeUnitList.GetCount();
  ar << iTemp;
  for ( int i = 0; i < iTemp; i ++ ) { 
    pcunit = m_CRunTimeUnitList.GetNext(pos);
    ar << pcunit;
  } 
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// SaveRunTimeUnitList()
//
// Parameter : 
//		CArchive& ar : archive
//
// return : no return
//
// Description :
//		Save m_CRunTimeUnitList
//	
////////////////////////////////////////////////////////////////////////////////////////////
void CSQIFileDoc::SaveRunTimeUnitList(CArchive& ar) {
  CUnitBase * pcunit;
  POSITION pos = m_CRunTimeUnitList.GetHeadPosition();
  INT64 iTemp = m_CRunTimeUnitList.GetCount();

  //存储RunTimeUnitList
  ar << iTemp;
  for (int i = 0; i < iTemp; i++) {
    pcunit = m_CRunTimeUnitList.GetNext(pos);
    ar << pcunit;
  }
}

///////////////////////////////////////////////////////////////////////////////////////
//
// GetObjNumber()
//
///////////////////////////////////////////////////////////////////////////////////////
INT32 CSQIFileDoc::GetObjNumber( void ) {
  return( m_nCurrentObjNumber );
}

///////////////////////////////////////////////////////////////////////////////////////
//
// SetObjNumber()
//
///////////////////////////////////////////////////////////////////////////////////////

void CSQIFileDoc::SetObjNumber( ULONG iNumber ) {
  m_nCurrentObjNumber = iNumber;
}

///////////////////////////////////////////////////////////////////////////////////////
//
// GetUnitNumber()
//
///////////////////////////////////////////////////////////////////////////////////////
INT64 CSQIFileDoc::GetUnitNumber( void ) {
  return( m_nCurrentUnitNumber );
}

///////////////////////////////////////////////////////////////////////////////////////
//
// SetUnitNumber()
//
///////////////////////////////////////////////////////////////////////////////////////
void CSQIFileDoc::SetUnitNumber( ULONG iNumber) {
  m_nCurrentUnitNumber = iNumber;
}

BOOL CSQIFileDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;
  // TODO: add reinitialization code here
  // (SDI documents will reuse this document)
  return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// ClearLoopDetectFlag( void )
//
//
/////////////////////////////////////////////////////////////////////////////////////////
void CSQIFileDoc::ClearLoopDetectFlag( void ) {
  POSITION poUnit = m_CUnitList.GetHeadPosition();
  CUnitBase * pcunit;
  INT_PTR iCount = m_CUnitList.GetCount();

  for ( int i = 0; i < iCount; i++ ) {
    pcunit = m_CUnitList.GetNext( poUnit );
    pcunit->ClearLoopDetectFlag();
  }
}

////////////////////////////////////////////////////////////////////////
//
// CompileRunTimeUnitList()
//
// Parameter : No
//
// Return :
//    bool      : TRUE if secuess created.
//              : FALSE if not created runtime unit list
//
// Description :
//   这个函数是先编译下层部件，然后生成运行时态单元序列,
//    
////////////////////////////////////////////////////////////////////////
bool CSQIFileDoc::CompileRunTimeUnitList( void ) {
	return(CompileUnitList(&m_CUnitList, &m_CRunTimeUnitList));
}

/////////////////////////////////////////////////////////////////////////////
//
// CSQIFileDoc serialization
//
//
/////////////////////////////////////////////////////////////////////////////
void CSQIFileDoc::Serialize(CArchive& ar)
{ 
  CString strTemp, strViewFile;
	CString strStrategyFile;
  CObjectBase * pcobj;
  POSITION poObj;                          
  INT64 iTotal;
	CUnitBase * pcUnit;
                              
  VERIFY(strViewFile.LoadString(IDS_VIEW_FILE_VERSION));
  if (ar.IsStoring())
  {
    // TODO: add storing code here
		POSITION poUnit = m_CUnitList.GetHeadPosition();
		iTotal = m_CUnitList.GetCount();
		VERIFY(strStrategyFile.LoadString(IDS_STRATEGY_FILE_VERSION));
		
		ar << strStrategyFile << m_nCurrentUnitNumber << iTotal;
    for ( int i = 0; i < iTotal; i ++ ) { 
      pcUnit = m_CUnitList.GetNext(poUnit);
      pcUnit->CheckSelf();
      ar << pcUnit;
    } 

		poObj = m_CObjectList.GetHeadPosition();
		iTotal = m_CObjectList.GetCount();
    ar << strViewFile << m_nCurrentObjNumber << iTotal;
    for ( int i = 0; i < iTotal; i ++ ) { 
      pcobj = m_CObjectList.GetNext(poObj);
      ar << pcobj;
    }
  }
  else
  {
    // TODO: add loading code here
    LoadSQIFile(ar, &m_CUnitList, &m_CObjectList, &m_nCurrentUnitNumber, &m_nCurrentObjNumber);

    // 重置编译标志
    ReSetCompileFlag(&m_CUnitList);
    // 设置编译标志。
    SetParaLockFlag(&m_CUnitList, &m_CObjectList);
 
		UnitListLoopDetect(&m_CUnitList);     // check loop	
  } 
}

/////////////////////////////////////////////////////////////////////////////
// CSQIFileDoc diagnostics

#ifdef _DEBUG
void CSQIFileDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CSQIFileDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}

#endif //_DEBUG  


///////////////////////////////////////////////////////////////////////////////////////
//
// GetDictionaryList()
//
// 返回: 
//		CDicList * : 已做好的单元词典.
//
// 参数:
//    ULONG ulType					: 适合的单元类型(tMODIFIABLE, tINPUT, tOUTPUT).
//		CObjectBase * pcObj		: 当前处理动态连接的对象.
//
// 描述:
//    清除老的单元词典,生成新的单元词典, called by MAKE_DYN_LINK in CSQIObjectView.
//    此单元词典，只从当前层的单元序列中选择合适的变量，这样能够在众多单元中轻松选择，否则太多的单元会导致无法寻找恰当的参数。
//
///////////////////////////////////////////////////////////////////////////////////////
CDicList * CSQIFileDoc::GetUnitDictionaryList( ULONG ulType, CObjectBase * pcObj ) {
	CUnitDictionary * pDic = nullptr;
	
	// 清除以前的词典.
	for ( auto &pDic : m_CDicList ) {
		delete pDic;
	}
	m_CDicList.clear();
	
	INT64 iTotal = m_pCurrentUnitList->GetCount();
	POSITION po = m_pCurrentUnitList->GetHeadPosition();
	CUnitBase * pcunit;

	// 得到合适的词典.
	for ( int i = 0; i < iTotal; i++ ) {
		pcunit = m_pCurrentUnitList->GetNext( po );
    if (pcunit->IsEncapsulated()) { // 简单单元或者已封装的部件
      pcunit->PrepareParaDictionary(m_CDicList, ulType);
    }
	}

	CODLList * pODLL = pcObj->GetDynLinkList();
	po = pODLL->GetHeadPosition();
	iTotal = pODLL->GetCount();
	CObjectDynLink * pDY;
	ULONG ulIndex;

	// 将已有的动态连接置入词典中
	for ( int i = 0; i < iTotal; i++ ) {
		pDY = pODLL->GetNext( po );
		ulIndex = pDY->GetUnitIndex();
		pcunit = pDY->GetUnit();
		pDic = new CUnitDictionary( pcunit, ulIndex, pcunit->GetParaType( ulIndex ) );
		m_CDicList.push_back( pDic );
		}

	return( &m_CDicList );
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// OnProjectCompile()
//
// 编译整体源文件,生成运行文件.
//
////////////////////////////////////////////////////////////////////////////////////////
void CSQIFileDoc::OnProjectCompile()
{
  // TODO: Add your command handler code here
  CString strPathName = GetPathName();
  CString strViewFile;
  CObjectBase * pcobj;
  POSITION pos = m_CObjectList.GetHeadPosition();
  INT64 iTemp;
  ULONG iFilePoint = strPathName.Find('.');

  // FireBird Dacview's runtime file is .fbd
	strPathName.SetAt(iFilePoint+1,'f');
  strPathName.SetAt(iFilePoint+2, 'b');
  strPathName.SetAt(iFilePoint+3, 'd');

  // 重置编译标志
  ReSetCompileFlag(&m_CUnitList);
  // 设置各单元的输入动态链接数
  SetParaLockFlag(&m_CUnitList, &m_CObjectList);
  
  // 编译整体文件
  if ( !CompileRunTimeUnitList() ) {
	  ShowMessage(ID_MSG_FAIL_COMPILE);
		return;
	}

  CFile cFile; 
  char buffer[512];
  
  iTemp = m_CObjectList.GetCount();
  cFile.Open(strPathName, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyRead);
  CArchive ar(&cFile, CArchive::store, 512, buffer); 
  VERIFY(strViewFile.LoadString(IDS_VIEW_FILE_VERSION));

  // 存储运行时文件
  SaveUnitList(ar);
  ar << strViewFile << iTemp;
  for ( int i = 0; i < iTemp; i ++ ) { 
    pcobj = m_CObjectList.GetNext(pos);
    ar << pcobj;
  }

  SetModifiedFlag(true);
  
  ShowMessage(ID_MSG_SUCCESS_COMPILE);

  CScrollView *pView;
  pView = (CScrollView *)m_viewList.GetTail(); // 目前一共两个滚动窗口，第二个是SQIUnitView
  pView->Invalidate();    // 部件编译后显示状态发生变化，显示之。
}

////////////////////////////////////////////////////////////////////////////
//
// OnUpdateProjectCompile()
//
//
//////////////////////////////////////////////////////////////////////////////
void CSQIFileDoc::OnUpdateProjectCompile(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->Enable(TRUE);
}
