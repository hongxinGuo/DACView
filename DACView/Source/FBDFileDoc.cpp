// showdoc.cpp : implementation of the CFBDFileDoc class
//
// Functions:
//
// CFBDFileDoc()               : initialize
// ~CFBDFileDoc()              : clear up
//
// ClearUnitList()          : Delete unit list and its contains.
// LoadUnitList()           : Retrive unit list from disk.
// SaveUnitList()           : Store unit list to disk.
//
// CreateRunTimeUnitList()  : Create runtime unit list.
// CreateRunTimeObjectList(): Create runtime object list.
//
// Serialize()              : Store or Retrive data.
//
// ExectiveRunTimeUnitList(): Exective unit list, Called by realtime thread.
//
// FindUnit()               : Find unit according its name.
// GetUnitList()            : Get unit list.
// GetObjectList()          : Get object list.
// 
// OnUpdateFileNew()        : Disable FileNew, don't permit new a .Shw file.
// OnRunRun()               : Run this file.
// OnRunStop()              : Stop running this file.
// OnRunPause()             : Temperary stop running.
// OnRunResume()            : Rerunning this file.
//
// OnUpdateRunRun()         : Enable or disable Run button.
// OnUpdateRunStop()        : Enable or disable Stop button.
// OnUpdateRunPause()       : Enable or disable Pause button.
// OnUpdateRunResume()      : Enable or disable Resume button.
//
/////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mmsystem.h"

#include "dacview.h"

#include "globedef.h"

#include "FBDFileDoc.h"
#include "FBDObjView.h"

#include "mainFrm.h"

#include "cObjectBase.h"
#include "CUnitBase.h"
#include "cUnitDynLink.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc

IMPLEMENT_DYNCREATE(CFBDFileDoc, CDocument)

BEGIN_MESSAGE_MAP(CFBDFileDoc, CDocument)
  //{{AFX_MSG_MAP(CFBDFileDoc)
  ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(ID_RUN_RUN, OnRunRun)
	ON_COMMAND(ID_RUN_STOP, OnRunStop)
	ON_UPDATE_COMMAND_UI(ID_RUN_RUN, OnUpdateRunRun)
	ON_UPDATE_COMMAND_UI(ID_RUN_STOP, OnUpdateRunStop)
	ON_COMMAND(ID_RUN_PAUSE, OnRunPause)
	ON_UPDATE_COMMAND_UI(ID_RUN_PAUSE, OnUpdateRunPause)
	ON_COMMAND(ID_RUN_RESUME, OnRunResume)
	ON_UPDATE_COMMAND_UI(ID_RUN_RESUME, OnUpdateRunResume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc construction/destruction

CFBDFileDoc::CFBDFileDoc()
{
  // TODO: add one-time construction code here 
  // set current process's priority to high
/*
  if ( !SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS) ) {
    TRACE("Can set process to realtime.\n");
  }
*/  
  TIMECAPS tc;

  if(timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
      /* Error; application can't continue. */
  }

  m_nTimerRes = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
	ASSERT( m_nTimerRes <= 1 );
  timeBeginPeriod(m_nTimerRes);
  
	m_lCount10MS = 0;
  m_lCount100MS = 0;
  m_lCountSecond = 0;
  m_lCountMinute = 0;
  m_lCountHour = 0;
  m_lCountShowStatus = 0;
  gl_ulRealTimeProcessTime = 0;
  m_fInterRunUnitList = false;

  m_CUnitList.clear();
  m_CRunTimeUnitList.clear();

	m_fRun = false;
  m_fInitialRun = false;

	m_pObjectShowStatus = nullptr;
}

CFBDFileDoc::~CFBDFileDoc() { 
  
  // set current process's priority to normal
  SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
  
  MMRESULT result;

  if ( m_fRun ) {
	  if ( m_nTimerID ) {      /* If timer event is pending */
	    result = timeKillEvent(m_nTimerID);  /* Cancel the event */
	    if ( result == TIMERR_NOCANDO ) {
	      ShowMessage(ID_ERROR_REALTIME_PROCESS);
	    }
      else ASSERT( result == 0 );
	  }
	  else {    // error
	    ShowMessage(ID_ERROR_REALTIME_PROCESS);
	  }
	}
  timeEndPeriod(m_nTimerRes);

  m_CRunTimeObjectList.clear();
  
  // release Object's memory
  for (auto pcobj : m_CObjectList) {
    delete pcobj;
  } 
  // release list's memory
  m_CObjectList.clear();
  
  ClearUnitList();
} 

//////////////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc private function

void CFBDFileDoc::ClearUnitList( void ) {
  // delete m_CUnitList and m_CRunTimeUnitList
  INT64 iTemp = m_CUnitList.size();
  for (auto punit : m_CUnitList) {
    delete punit;
  } 
  TRACE("%d Units deleted\n", iTemp);                  
  // release list's memory
  m_CUnitList.clear();
  
  m_CRunTimeUnitList.clear();
}

BOOL CFBDFileDoc::LoadUnitList( CArchive & ar ) {
  // Load strategy file
  CString strTemp, strStrategyFile;
  CUnitBase * punit;
  INT64 iTotal;

  VERIFY(strStrategyFile.LoadString(IDS_STRATEGY_FILE_VERSION));
  ar >> strTemp >> iTotal;
  // test whether a Dacview file
  if ( strTemp == strStrategyFile );
  else {
    ShowMessage(ID_ERROR_SHOWVIEW_STRATEGY_FILE);
    return( FALSE );
  }

	for ( int i = 0; i < iTotal; i ++ ) {
    ar >> punit;
    m_CUnitList.push_back( punit );
  }

	// load CRunTimeUnitList
  ar >> iTotal;
  for ( int i = 0; i < iTotal; i ++ ) {
    ar >> punit;
    m_CRunTimeUnitList.push_back(punit);
  }

  return( TRUE );
} 

void CFBDFileDoc::SaveUnitList( CArchive& ar ) {
  INT64 iCount = m_CUnitList.size();
  CString strStrategyFile;
  
  VERIFY(strStrategyFile.LoadString(IDS_STRATEGY_FILE_VERSION));
  ar << strStrategyFile << iCount;
  for (const auto punit : m_CUnitList) { 
    ar << punit;
  } 

  iCount = m_CRunTimeUnitList.size();
  ar << iCount;
  for (const auto punit : m_CRunTimeUnitList) { 
    ar << punit;
  } 
}

BOOL CFBDFileDoc::MakeRunTimeUnitList( void ) {
  
  // create exective list 10ms, 100ms, 1Second, 1Minute
  for (const auto punit : m_CRunTimeUnitList) {
		punit->PrepareRunTimeList();	// 生成部件的运行时态序列.
    if ( ((punit->GetScanRate()/60000)*60000) == punit->GetScanRate() ) {
      m_vCUnit1Minute.push_back(punit);
    }
    else if ( ((punit->GetScanRate()/1000)*1000) == punit->GetScanRate() ) {
      m_vCUnit1Second.push_back(punit);
    }
    else if ( ((punit->GetScanRate()/100)*100) == punit->GetScanRate() ) {
      m_vCUnit100MS.push_back(punit);
    }
    else if ( ((punit->GetScanRate()/10)*10) == punit->GetScanRate() ) {
			m_vCUnit10MS.push_back(punit);
		}
		else {
      m_vCUnit1MS.push_back(punit);
    }
  }
  ASSERT( m_CRunTimeUnitList.size() == (  m_vCUnit1Minute.size() + 
                                          m_vCUnit1Second.size() +
                                          m_vCUnit100MS.size() +
                                          m_vCUnit10MS.size() +
										                      m_vCUnit1MS.size() ) );
  return ( TRUE );
}

BOOL CFBDFileDoc::CreateRunTimeObjectList( void ) {

  CObjectList listObject;

  // create temperary list listObject  
  for (const auto pObject : m_CObjectList) {
    pObject->AddToList( listObject );
  }

  // create RunTimeObjctList from temperary list listObject
  bool done = false;
  int iCount = listObject.size();
  while ( ! done ) {
    for (const auto pObject : listObject) {
      m_CRunTimeObjectList.push_back(pObject);
    }   
    int iRunTime = m_CRunTimeObjectList.size();
    if ( iRunTime == iCount ) done = true;
  }

  // clear temperary list listObject
  listObject.clear();

  // create seperate list from RunTimeObjectList
  for (const auto pobj : m_CRunTimeObjectList) {
    if ( ((pobj->GetScanRate()/60000)*60000) == pobj->GetScanRate() ) {
      m_vCObject1Minute.push_back(pobj);
    }
    else if ( ((pobj->GetScanRate()/1000)*1000) == pobj->GetScanRate() ) {
      m_vCObject1Second.push_back(pobj);
    }
    else if ( ((pobj->GetScanRate()/100)*100) == pobj->GetScanRate() ) {
      m_vCObject100MS.push_back(pobj);
    }
    else if ( ((pobj->GetScanRate()/10)*10) == pobj->GetScanRate() ) {
      m_vCObject10MS.push_back(pobj);
    }
    else {
      m_vCObject1MS.push_back(pobj);
    }
  }
  ASSERT( m_CRunTimeObjectList.size() ==
            ( m_vCObject1Minute.size() +
              m_vCObject1Second.size() +
              m_vCObject100MS.size() +
              m_vCObject10MS.size() +
							m_vCObject1MS.size() ) );
  return ( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc serialization

void CFBDFileDoc::Serialize(CArchive& ar)
{ 
  CString str, strViewFile;
  INT64 iCount;
  
  VERIFY(strViewFile.LoadString(IDS_VIEW_FILE_VERSION));

  if ( ar.IsStoring() ) {
    SaveUnitList(ar);
  }
  else {
    LoadUnitList(ar);

		// make runtime unit list
		MakeRunTimeUnitList();
	}

	if (ar.IsStoring()) {
    // TODO: add storing code here 
    iCount = m_CObjectList.size();
    ar << strViewFile << iCount;
    for (const auto pcobj : m_CObjectList) {
      ar << pcobj;
    }
  }
  else {
    // TODO: add loading code here
    CObjectBase * pcobj;


    ar >> str >> iCount;
    // test whether a Dacview file
    if ( str == strViewFile );
    else ShowMessage(ID_ERROR_SHOWVIEW_DACVIEW_FILE);
    // load cobject list
    for ( int i = 0; i < iCount; i ++ ) {
      ar >> pcobj;
      m_CObjectList.push_back( pcobj );
    } 
    // create runtime object list
  	CreateRunTimeObjectList();
  } 
}

BOOL CFBDFileDoc::ToggleViewStatus( CObjectPrimitive * pc ) {
	if ( m_pObjectShowStatus == pc ) {
		m_pObjectShowStatus = nullptr;
		}
	else {
		m_pObjectShowStatus = pc;
		}
  return( TRUE );
}

void CFBDFileDoc::ShowStatus( void ) {
	CTime time = CTime::GetCurrentTime();
	CString strTime = time.Format(_T("%X"));
  char pch[256];

  m_fShowStatus = FALSE;
	if ( m_pObjectShowStatus != nullptr ) {
		m_pObjectShowStatus->DumpCurrentStatus( pch );
		strTime += "   ";
		strTime += pch;
		ShowMessage( ID_MESSAGE_UNIT_STATUS_VIEW, (LPCSTR)strTime );
		}
}

/////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc diagnostics

#ifdef _DEBUG
void CFBDFileDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CFBDFileDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc commands, public function

////////////////////////////////////////////////////////////////////
//
//  Exective the unitList. This function is a seperate callback function, 
//  and in a separate thread.
//  Call by system every 1ms.
//
//  dwUser为本FBDFileDoc的地址，Exective需要用到此指针。在64位版本中，需要定义为DWORD_PTR类型。
//
//
//////////////////////////////////////////////////////////////////////
void CALLBACK CFBDFileDoc::Exective( UINT IdEvent, UINT , DWORD_PTR dwUser, DWORD , DWORD  ) {
  CFBDFileDoc * pDoc = (CFBDFileDoc *)dwUser;
  ULONGLONG ulTimeTick;
  CObjectBase * pcobj;
  INT_PTR iCount, i;
	CUnitBase * punit;
      
  if ( IdEvent == pDoc->m_nTimerID ) {
    ulTimeTick = GetTickCount64();

    // exective per 1ms Task
    iCount = pDoc->m_vCUnit1MS.size();
    for ( i = 0; i < iCount; i ++ ) {                            
      punit = pDoc->m_vCUnit1MS[i];
      if ( punit->CountDown(1) ) punit->Exective();  
    }

    iCount = pDoc->m_vCObject1MS.size();
    for ( i = 0; i < iCount; i ++ ) {                            
      pcobj = pDoc->m_vCObject1MS[i];
      if ( pcobj->CountDown( 1 ) ) pcobj->ExectiveDynLink();
    }

    // exective per 10ms Task
    if ( pDoc->m_lCount10MS <= 0 ) {
			pDoc->m_lCount10MS = 9;
			iCount = pDoc->m_vCUnit10MS.size();

			for ( i = 0; i < iCount; i ++ ) {                            
				punit = pDoc->m_vCUnit10MS[i];
				if ( punit->CountDown(10) ) punit->Exective();  
			}

			iCount = pDoc->m_vCObject10MS.size();
			for ( i = 0; i < iCount; i ++ ) {                            
				pcobj = pDoc->m_vCObject10MS[i];
				if ( pcobj->CountDown(10) ) pcobj->ExectiveDynLink();
			}
		}
		else pDoc->m_lCount10MS--;

    // exective per 100 ms Task
    if ( pDoc->m_lCount100MS <= 0 ) {
      pDoc->m_lCount100MS = 99;
      iCount = pDoc->m_vCUnit100MS.size();
      for ( i = 0; i < iCount; i ++ ) {                            
        punit = pDoc->m_vCUnit100MS[i];
        if ( punit->CountDown(100) ) punit->Exective();  
      }

      iCount = pDoc->m_vCObject100MS.size();
      for ( i = 0; i < iCount; i ++ ) {                            
        pcobj = pDoc->m_vCObject100MS[i];
        if ( pcobj->CountDown(100) )	pcobj->ExectiveDynLink();  
			}
    }
    else pDoc->m_lCount100MS--;

    // exective per second task
    if ( pDoc->m_lCountSecond <= 0 ) {
      pDoc->m_lCountSecond = 999;
      // do function per socond

      iCount = pDoc->m_vCUnit1Second.size();
      for ( i = 0; i < iCount; i ++ ) {                            
        punit = pDoc->m_vCUnit1Second[i];
        if ( punit->CountDown(1000) ) punit->Exective();  
      }

      iCount = pDoc->m_vCObject1Second.size();
      for ( i = 0; i < iCount; i ++ ) {                            
        pcobj = pDoc->m_vCObject1Second[i];
        if ( pcobj->CountDown(1000) ) pcobj->ExectiveDynLink();  
		  }
    
      if ( pDoc->m_lCountShowStatus <= 1 ) {
        pDoc->m_fShowStatus = TRUE;
        pDoc->m_lCountShowStatus = gl_ulCountShowStatus;
      }
      else pDoc->m_lCountShowStatus--;
    }
    else pDoc->m_lCountSecond--;

    // exective per minute task
    if ( pDoc->m_lCountMinute <= 0 ) {
      pDoc->m_lCountMinute = 60000 - 1;
      // do function per minute
      iCount = pDoc->m_vCUnit1Minute.size();

      for ( i = 0; i < iCount; i ++ ) {                            
        punit = pDoc->m_vCUnit1Minute[i];
        if ( punit->CountDown( 60000 ) ) punit->Exective();  
      }

      iCount = pDoc->m_vCObject1Minute.size();
      for ( i = 0; i < iCount; i ++ ) {                            
        pcobj = pDoc->m_vCObject1Minute[i];
        if ( pcobj->CountDown( 60000 ) ) pcobj->ExectiveDynLink();
			}
    }
    else pDoc->m_lCountMinute--;

		// exective per hour task
    if ( pDoc->m_lCountHour <= 0 ) {
      pDoc->m_lCountHour = 60000*60 - 1;
      // do function per hour

    }
    else pDoc->m_lCountHour--;

    gl_ulRealTimeProcessTime += (GetTickCount64() - ulTimeTick);
  }
}

CUnitBase * CFBDFileDoc::FindUnit( CString TagName ) {
  for (const auto pc : m_CUnitList) {                            
    if ( pc->IsMe( TagName ) ) {
      return( pc );
    }
  }    
  return ( nullptr );
}

CUnitList * CFBDFileDoc::GetUnitList( void ) {
  return( &m_CUnitList );
}

CObjectList * CFBDFileDoc::GetObjectList( void ) {
  return( &m_CObjectList );
}

void CFBDFileDoc::OnUpdateFileNew(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->Enable(FALSE);
}

void CFBDFileDoc::OnRunRun() 
{
	// TODO: Add your command handler code here
	// set periodic callback timer function, 
  m_nTimerID = timeSetEvent(
         1,												// 1ms per time
         max(1, m_nTimerRes),			// Resolution (global variable) 
         (LPTIMECALLBACK)Exective,// Callback function   
         (DWORD_PTR)this,         // 本FBDFileDoc的地址，Exective需要用到此指针。在64位版本中，需要定义为DWORD_PTR类型。            
         TIME_PERIODIC);					// Event type ( periodic )

  if ( m_nTimerID == 0) {
		ShowMessage(ID_ERROR_SHOWVIEW_REALTIME_PROCESS_CANNOT_CREATE, "");
  }
	m_fRun = TRUE;
  m_fInitialRun = TRUE;
	gl_ulNumberOfShowViewRun++;
}

void CFBDFileDoc::OnRunStop() 
{
	// TODO: Add your command handler code here
  MMRESULT result;
  
  if ( m_nTimerID ) {                   // If timer event is pending
    result = timeKillEvent(m_nTimerID); // Cancel the event
    if ( result == TIMERR_NOCANDO ) {
      TRACE("Error kill timer\n");
    }
    ASSERT(result == 0);
  }
  else {    // error
    TRACE("Can't kill realtime event!\n");
  }
	
	gl_ulNumberOfShowViewRun--;
	m_fRun = FALSE;
  m_fInitialRun = FALSE;
}

void CFBDFileDoc::OnRunPause() 
{
	// TODO: Add your command handler code here
	MMRESULT result;
  
  if ( m_nTimerID ) {                   // If timer event is pending
    result = timeKillEvent(m_nTimerID); // Cancel the event
    if ( result == TIMERR_NOCANDO ) {
      TRACE("Error kill timer\n");
    }
    ASSERT(result == 0);
  }
  else {    // error
    TRACE("Can't kill realtime event!\n");
  }
	
	m_fRun = FALSE;
	gl_ulNumberOfShowViewRun--;
}

void CFBDFileDoc::OnRunResume() 
{
	// TODO: Add your command handler code here
	// set periodic callback timer function, 
  m_nTimerID = timeSetEvent(
         1,													// 1ms per time                        
         max(1, m_nTimerRes),				// Resolution (global variable) 
         (LPTIMECALLBACK)Exective,	// Callback function   
         (DWORD_PTR)this,						// User data             
         TIME_PERIODIC);						// Event type ( periodic )

  if ( m_nTimerID == 0) {
		ShowMessage(ID_ERROR_SHOWVIEW_REALTIME_PROCESS_CANNOT_CREATE, "");
  }
	m_fRun = TRUE;
	gl_ulNumberOfShowViewRun++;
}

void CFBDFileDoc::OnUpdateRunRun(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( (m_fRun == FALSE) && (m_fInitialRun == FALSE) ) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CFBDFileDoc::OnUpdateRunStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( (m_fRun == TRUE) && (m_fInitialRun == TRUE) ) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CFBDFileDoc::OnUpdateRunPause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( (m_fRun == TRUE) && (m_fInitialRun == TRUE) ) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CFBDFileDoc::OnUpdateRunResume(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if ( (m_fRun == FALSE) && (m_fInitialRun == TRUE) ) pCmdUI->Enable( TRUE );
  else pCmdUI->Enable( FALSE );
}
