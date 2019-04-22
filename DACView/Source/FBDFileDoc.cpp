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
  m_fInterRunUnitList = FALSE;

  m_CUnitList.RemoveAll();
  m_CRunTimeUnitList.RemoveAll();
  m_CUnitList1MS.RemoveAll();
  m_CUnitList10MS.RemoveAll();
  m_CUnitList100MS.RemoveAll();
  m_CUnitList1Second.RemoveAll();
  m_CUnitList1Minute.RemoveAll();

	m_fRun = FALSE;
  m_fInitialRun = FALSE;

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

  m_CRunTimeObjectList.RemoveAll();
  m_CObjectList1MS.RemoveAll();
  m_CObjectList10MS.RemoveAll();
  m_CObjectList100MS.RemoveAll();
  m_CObjectList1Second.RemoveAll();
  m_CObjectList1Minute.RemoveAll();
  
  // release Object's memory
  POSITION Po = m_CObjectList.GetHeadPosition();
  CObjectBase * pcobjTemp;
	INT_PTR iTemp = m_CObjectList.GetCount();

  for ( int i = 0; i < iTemp; i++ ) {
    pcobjTemp = m_CObjectList.GetNext(Po);
    delete pcobjTemp;
    pcobjTemp = nullptr;
  } 
  TRACE("%d objects deleted\n", iTemp);                  
  // release list's memory
  m_CObjectList.RemoveAll();
  
  m_CUnitList1MS.RemoveAll();
  m_CUnitList10MS.RemoveAll();
  m_CUnitList100MS.RemoveAll();
  m_CUnitList1Second.RemoveAll();
  m_CUnitList1Minute.RemoveAll();
  
  ClearUnitList();
} 

//////////////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc private function

void CFBDFileDoc::ClearUnitList( void ) {
  // delete m_CUnitList and m_CRunTimeUnitList
  POSITION Po = m_CUnitList.GetHeadPosition();
  CUnitBase * pcunitTemp;
  INT_PTR iTemp = m_CUnitList.GetCount();
  for ( int i = 0; i < iTemp; i++ ) {
    pcunitTemp = m_CUnitList.GetNext(Po);
    delete pcunitTemp;
    pcunitTemp = nullptr;
  } 
  TRACE("%d Units deleted\n", iTemp);                  
  // release list's memory
  m_CUnitList.RemoveAll();
  
  m_CRunTimeUnitList.RemoveAll();
}

BOOL CFBDFileDoc::LoadUnitList( CArchive & ar ) {
  // Load strategy file
  CString strTemp, strStrategyFile;
  CUnitBase * pcunit;
  INT64 iTotal;

  VERIFY(strStrategyFile.LoadString(IDS_STRATEGY_FILE_VERSION));
  ar >> strTemp >> iTotal;
  // test whether a Dacview file
  if ( strTemp == strStrategyFile );
  else {
    ShowMessage(ID_ERROR_SHOWVIEW_STRATEGY_FILE);
    return( FALSE );
  }
	m_vUnit.reserve(iTotal);
  for ( int i = 0; i < iTotal; i ++ ) {
    ar >> pcunit;
    m_CUnitList.AddTail( pcunit );
		m_vUnit.push_back(pcunit);
  }

	// load CRunTimeUnitList
  ar >> iTotal;
  for ( int i = 0; i < iTotal; i ++ ) {
    ar >> pcunit;
    m_CRunTimeUnitList.AddTail( pcunit );
  }

  return( TRUE );
} 

void CFBDFileDoc::SaveUnitList( CArchive& ar ) {
  CUnitBase * pcunit;
  POSITION pos = m_CUnitList.GetHeadPosition();                          
  INT64 iCount = m_CUnitList.GetCount();
  CString strStrategyFile;
  
  VERIFY(strStrategyFile.LoadString(IDS_STRATEGY_FILE_VERSION));
  ar << strStrategyFile << iCount;
  for ( int i = 0; i < iCount; i ++ ) { 
    pcunit = m_CUnitList.GetNext(pos);
    ar << pcunit;
  } 

	pos = m_CRunTimeUnitList.GetHeadPosition();                          
  iCount = m_CRunTimeUnitList.GetCount();
  
  ar << iCount;
  for ( int i = 0; i < iCount; i ++ ) { 
    pcunit = m_CRunTimeUnitList.GetNext(pos);
    ar << pcunit;
  } 
}

BOOL CFBDFileDoc::MakeRunTimeUnitList( void ) {
  
  // create exective list 10ms, 100ms, 1Second, 1Minute
  POSITION po;
  CUnitBase * pcunit;
  INT_PTR iCount;

  po = m_CRunTimeUnitList.GetHeadPosition();
  iCount = m_CRunTimeUnitList.GetCount();
  for ( int i = 0; i < iCount; i++ ) {
    pcunit = m_CRunTimeUnitList.GetNext( po );
		pcunit->PrepareRunTimeList();	// ���ɲ���������ʱ̬����.
    if ( ((pcunit->GetScanRate()/60000)*60000) == pcunit->GetScanRate() ) {
      m_CUnitList1Minute.AddTail( pcunit );
    }
    else if ( ((pcunit->GetScanRate()/1000)*1000) == pcunit->GetScanRate() ) {
      m_CUnitList1Second.AddTail( pcunit );
    }
    else if ( ((pcunit->GetScanRate()/100)*100) == pcunit->GetScanRate() ) {
      m_CUnitList100MS.AddTail( pcunit );
    }
    else if ( ((pcunit->GetScanRate()/10)*10) == pcunit->GetScanRate() ) {
			m_CUnitList10MS.AddTail( pcunit );
		}
		else {
      m_CUnitList1MS.AddTail( pcunit );
    }
  }
  ASSERT( iCount == ( m_CUnitList1Minute.GetCount() + 
                     m_CUnitList1Second.GetCount() +
                     m_CUnitList100MS.GetCount() +
                     m_CUnitList10MS.GetCount() +
										 m_CUnitList1MS.GetCount() ) );
  return ( TRUE );
}

BOOL CFBDFileDoc::CreateRunTimeObjectList( void ) {

  CObjectList listObject;

  // create temperary list listObject
  POSITION poObject = m_CObjectList.GetHeadPosition();
  CObjectBase * pcObject;
	INT_PTR iRunTime, iCount = m_CObjectList.GetCount();
  
  for ( int i = 0; i < iCount; i++ ) {
    pcObject = m_CObjectList.GetNext(poObject);
    pcObject->AddToList( listObject );
  }

  // create RunTimeObjctList from temperary list listObject
  poObject = listObject.GetHeadPosition();
  iCount =  listObject.GetCount();
  BOOL done = FALSE;
  while ( ! done ) {
    poObject = listObject.GetHeadPosition();
    iCount = listObject.GetCount();
    for ( int i = 0; i < iCount; i++ ) {
      pcObject = listObject.GetNext(poObject);
      m_CRunTimeObjectList.AddTail(pcObject);
    }   
    iRunTime = m_CRunTimeObjectList.GetCount();
    if ( iRunTime == iCount ) done = TRUE;
  }

  // clear temperary list listObject
  listObject.RemoveAll();
  
  POSITION po;
  CObjectBase * pcobj;

  // create seperate list from RunTimeObjectList
  po = m_CRunTimeObjectList.GetHeadPosition();
  iCount = m_CRunTimeObjectList.GetCount();
  for ( int i = 0; i < iCount; i++ ) {
    pcobj = m_CRunTimeObjectList.GetNext( po );
    if ( ((pcobj->GetScanRate()/60000)*60000) == pcobj->GetScanRate() ) {
      m_CObjectList1Minute.AddTail( pcobj );
    }
    else if ( ((pcobj->GetScanRate()/1000)*1000) == pcobj->GetScanRate() ) {
      m_CObjectList1Second.AddTail( pcobj );
    }
    else if ( ((pcobj->GetScanRate()/100)*100) == pcobj->GetScanRate() ) {
      m_CObjectList100MS.AddTail( pcobj );
    }
    else if ( ((pcobj->GetScanRate()/10)*10) == pcobj->GetScanRate() ) {
      m_CObjectList10MS.AddTail( pcobj );
    }
    else {
      m_CObjectList1MS.AddTail( pcobj );
    }
  }
  ASSERT( m_CRunTimeObjectList.GetCount() ==
            ( m_CObjectList1Minute.GetCount() +
              m_CObjectList1Second.GetCount() +
              m_CObjectList100MS.GetCount() +
              m_CObjectList10MS.GetCount() +
							m_CObjectList1MS.GetCount() ) );
  return ( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
// CFBDFileDoc serialization

void CFBDFileDoc::Serialize(CArchive& ar)
{ 
  CString str, strViewFile;
  CObjectBase * pcobj;
  POSITION poObj = m_CObjectList.GetHeadPosition();                          
  INT64 iCount = m_CObjectList.GetCount();
  
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
    ar << strViewFile << iCount;
    for ( int i = 0; i < iCount; i ++ ) { 
      pcobj = m_CObjectList.GetNext(poObj);
      ar << pcobj;
    }
  }
  else {
    // TODO: add loading code here
    ar >> str >> iCount;
    // test whether a Dacview file
    if ( str == strViewFile );
    else ShowMessage(ID_ERROR_SHOWVIEW_DACVIEW_FILE);
    // load cobject list
    for ( int i = 0; i < iCount; i ++ ) {
      ar >> pcobj;
      pcobj->SetParameterSelected(); // ���õ�Ԫ�����Ͳ�����ѡ���־��
      m_CObjectList.AddTail( pcobj );
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
//  dwUserΪ��FBDFileDoc�ĵ�ַ��Exective��Ҫ�õ���ָ�롣��64λ�汾�У���Ҫ����ΪDWORD_PTR���͡�
//
//
//////////////////////////////////////////////////////////////////////
void CALLBACK CFBDFileDoc::Exective( UINT IdEvent, UINT , DWORD_PTR dwUser, DWORD , DWORD  ) {
  CFBDFileDoc * pDoc = (CFBDFileDoc *)dwUser;
  ULONGLONG ulTimeTick;
  CUnitBase * pc;
  CObjectBase * pcobj;
  POSITION pos; 
  INT_PTR iCount, i;
      
  if ( IdEvent == pDoc->m_nTimerID ) {
    ulTimeTick = GetTickCount64();

    // exective per 1ms Task
    pos = pDoc->m_CUnitList1MS.GetHeadPosition(); 
    iCount = pDoc->m_CUnitList1MS.GetCount();

    for ( i = 0; i < iCount; i ++ ) {                            
      pc = pDoc->m_CUnitList1MS.GetNext(pos);
      if ( pc->CountDown(1) ) pc->Exective();  
    }

    pos = pDoc->m_CObjectList1MS.GetHeadPosition(); 
    iCount = pDoc->m_CObjectList1MS.GetCount();

    for ( i = 0; i < iCount; i ++ ) {                            
      pcobj = pDoc->m_CObjectList1MS.GetNext(pos);
      if ( pcobj->CountDown( 1 ) ) pcobj->ExectiveDynLink();
    }

    // exective per 10ms Task
    if ( pDoc->m_lCount10MS <= 0 ) {
			pDoc->m_lCount10MS = 9;
			pos = pDoc->m_CUnitList10MS.GetHeadPosition(); 
			iCount = pDoc->m_CUnitList10MS.GetCount();

			for ( i = 0; i < iCount; i ++ ) {                            
				pc = pDoc->m_CUnitList10MS.GetNext(pos);
				if ( pc->CountDown(10) ) pc->Exective();  
			}

			pos = pDoc->m_CObjectList10MS.GetHeadPosition(); 
			iCount = pDoc->m_CObjectList10MS.GetCount();

			for ( i = 0; i < iCount; i ++ ) {                            
				pcobj = pDoc->m_CObjectList10MS.GetNext(pos);
				if ( pcobj->CountDown(10) ) pcobj->ExectiveDynLink();
			}
		}
		else pDoc->m_lCount10MS--;

    // exective per 100 ms Task
    if ( pDoc->m_lCount100MS <= 0 ) {
      pDoc->m_lCount100MS = 99;
      pos = pDoc->m_CUnitList100MS.GetHeadPosition(); 
      iCount = pDoc->m_CUnitList100MS.GetCount();
      for ( i = 0; i < iCount; i ++ ) {                            
        pc = pDoc->m_CUnitList100MS.GetNext(pos);
        if ( pc->CountDown(100) ) pc->Exective();  
      }

      pos = pDoc->m_CObjectList100MS.GetHeadPosition(); 
      iCount = pDoc->m_CObjectList100MS.GetCount();

      for ( i = 0; i < iCount; i ++ ) {                            
        pcobj = pDoc->m_CObjectList100MS.GetNext(pos);
        if ( pcobj->CountDown(100) )	pcobj->ExectiveDynLink();  
			}
    }
    else pDoc->m_lCount100MS--;

    // exective per second task
    if ( pDoc->m_lCountSecond <= 0 ) {
      pDoc->m_lCountSecond = 999;
      // do function per socond

      pos = pDoc->m_CUnitList1Second.GetHeadPosition(); 
      iCount = pDoc->m_CUnitList1Second.GetCount();

      for ( i = 0; i < iCount; i ++ ) {                            
        pc = pDoc->m_CUnitList1Second.GetNext(pos);
        if ( pc->CountDown(1000) ) pc->Exective();  
      }

      pos = pDoc->m_CObjectList1Second.GetHeadPosition(); 
      iCount = pDoc->m_CObjectList1Second.GetCount();

      for ( i = 0; i < iCount; i ++ ) {                            
        pcobj = pDoc->m_CObjectList1Second.GetNext(pos);
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
      pos = pDoc->m_CUnitList1Minute.GetHeadPosition(); 
      iCount = pDoc->m_CUnitList1Minute.GetCount();

      for ( i = 0; i < iCount; i ++ ) {                            
        pc = pDoc->m_CUnitList1Minute.GetNext(pos);
        if ( pc->CountDown( 60000 ) ) pc->Exective();  
      }

      pos = pDoc->m_CObjectList1Minute.GetHeadPosition(); 
      iCount = pDoc->m_CObjectList1Minute.GetCount();

      for ( i = 0; i < iCount; i ++ ) {                            
        pcobj = pDoc->m_CObjectList1Minute.GetNext(pos);
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
  CUnitBase * pc;
  POSITION pos = m_CUnitList.GetHeadPosition(); 
	INT_PTR iCount = m_CUnitList.GetCount();

  for ( int i = 0; i < iCount; i ++ ) {                            
    pc = m_CUnitList.GetNext(pos);
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
         (DWORD_PTR)this,         // ��FBDFileDoc�ĵ�ַ��Exective��Ҫ�õ���ָ�롣��64λ�汾�У���Ҫ����ΪDWORD_PTR���͡�            
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
