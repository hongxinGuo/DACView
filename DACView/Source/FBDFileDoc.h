// showdoc.h : interface of the CFBDFileDoc class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once
  
#include "CUnitBase.h"
#include "CObjectBase.h"

#include "CUnitDynLink.h"
#include "CUnitDictionary.h"

using namespace std;
#include<vector>


class CFBDFileDoc : public CDocument
{
protected: // create from serialization only
	CFBDFileDoc();
	DECLARE_DYNCREATE(CFBDFileDoc)

// Attributes
public:

// Operations
public:                        
	CUnitBase * 			FindUnit( CString TagName );
  CUnitList *       GetUnitList( void );
	CObjectList *     GetObjectList( void );
  
  BOOL				IsInterRunUnitList( void ) { return ( m_fInterRunUnitList ); }
  void				SetInterRunUnitList( BOOL flags ) { m_fInterRunUnitList = flags; }

	BOOL				fInRunShowView( void ) { return m_fRun; }
	void				SetRunShowViewFlag( BOOL fFlag ) { m_fRun = fFlag; }

  BOOL        ToggleViewStatus( CObjectPrimitive * pc );
  void        ShowStatus( void );
  BOOL        IsShowStatus( void ) { return( m_fShowStatus ); }

private :
  void ClearUnitList( void );
  BOOL LoadUnitList( CArchive & ar );
	void SaveUnitList( CArchive & ar );
  BOOL MakeRunTimeUnitList( void ); 
  BOOL CreateRunTimeObjectList( void );

	//dwUser为本FBDFileDoc的地址，Exective需要用到此指针。在64位版本中，需要定义为DWORD_PTR类型。
  static void CALLBACK Exective( UINT IDEvent, UINT nRes, DWORD_PTR dwUser, DWORD dwRes1, DWORD dwRes2);
  
// Implementation
public:
	virtual ~CFBDFileDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
protected:

protected:
// Serialized Data
	CUnitList			m_CUnitList;
	CUnitList 		m_CRunTimeUnitList;	// run time unit list, the sequence of unit is recalculated.
	CUnitList			m_CUnitList1MS;
	CUnitList			m_CUnitList10MS;
	CUnitList 		m_CUnitList100MS;
	CUnitList 		m_CUnitList1Second;
	CUnitList 		m_CUnitList1Minute;
	// 采用C++标准vector容器
	vector<CUnitBase *> m_vUnit;


	CObjectList 	m_CObjectList;			// object list
	CObjectList		m_CRunTimeObjectList;
	CObjectList		m_CObjectList1MS;
	CObjectList		m_CObjectList10MS;
	CObjectList 	m_CObjectList100MS;
	CObjectList   m_CObjectList1Second;
	CObjectList   m_CObjectList1Minute;
	
	
// no serialized data
	UINT          m_nTimerID;
	UINT 	        m_nTimerRes;
	BOOL          m_fRun;
  BOOL          m_fInitialRun;

  BOOL          m_fShowStatus;
	CObjectPrimitive *   m_pObjectShowStatus;

	volatile LONG m_lCount10MS;
	volatile LONG	m_lCount100MS;
	volatile LONG	m_lCountSecond;
	volatile LONG m_lCountMinute;
	volatile LONG m_lCountHour;
	volatile BOOL	m_fInterRunUnitList;

  volatile LONG m_lCountShowStatus;

// Generated message map functions
protected:
	//{{AFX_MSG(CFBDFileDoc)
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnRunRun();
	afx_msg void OnRunStop();
	afx_msg void OnUpdateRunRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRunStop(CCmdUI* pCmdUI);
	afx_msg void OnRunPause();
	afx_msg void OnUpdateRunPause(CCmdUI* pCmdUI);
	afx_msg void OnRunResume();
	afx_msg void OnUpdateRunResume(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

