///////////////////////////////////////////////////////////////////////////
//
// dacvidoc.h : interface of the CSQIFileDoc class
//
///////////////////////////////////////////////////////////////////////////// 
#pragma once

#include "globedef.h"			// use gl_sizeDoc

#include "cobjectbase.h"
#include "cUnitBase.h"

#include "CUnitDictionary.h"                      
#include "cUnitDynLink.h"
#include"CUnitComponent.h"

bool LoadUnitList(CArchive & ar, CUnitList * pUnitList, INT64 * pUnitNumber);
bool LoadObjectList(CArchive & ar, CObjectList * pObjectList, INT32 * pObjectNumber);
bool LoadSQIFile(CArchive & ar, CUnitList * pUnitList, CObjectList * pObjectList, INT64 * pUnitNumber, INT32 * pObjectNumber);
bool ReleaseSQIFile(CUnitList * pUnitList, CObjectList * pObjectList);

class CSQIFileDoc : public CDocument
{
protected: // create from serialization only
	CSQIFileDoc();
	DECLARE_DYNCREATE(CSQIFileDoc)
  
// Attributes
public:
  CRectTracker    m_trackerObject;
	CRectTracker		m_trackerUnit;

  // Operations
public:
	// operate an object of ObjList         
  CUnitList *       GetUnitList(void) { return m_pUnitList; }
  CUnitList *       GetCurrentUnitList(void) { return m_pCurrentUnitList; }
  void              SetCurrentUnitList(CUnitList * pCurrentUnitList) { m_pCurrentUnitList = pCurrentUnitList; }
  CUnitList *       GetRunTimeUnitList(void) { return m_pRunTimeUnitList; }
  CObjectList *     GetObjectList(void) { return &m_CObjectList; }
	CDicList *        GetUnitDictionaryList( ULONG ulType, CObjectBase * pcObj );
  
  void              ClearLoopDetectFlag( void );
	bool 							CompileRunTimeUnitList( void ); 
  INT32							GetObjNumber( void );
	void							SetObjNumber( ULONG iNumber );
	INT64							GetUnitNumber( void );
	void							SetUnitNumber( ULONG iNunmber );

private :
  void              SaveUnitList(CArchive & ar);
  void              SaveRunTimeUnitList(CArchive & ar);

// Implementation
public:
	virtual ~CSQIFileDoc();
	virtual void Serialize(CArchive& ar);	// overridden for document i/o
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;

#endif
protected:
	virtual	BOOL	OnNewDocument();
	
protected: 
// Serialized Data
	CObjectList     m_CObjectList;				// 存储对象的链表.
	INT32						m_nCurrentObjNumber;	// 当前对象的数量.
  
 	CUnitList	*			m_pUnitList;					// 存储单元的链表.
	INT64						m_nCurrentUnitNumber; // 当前单元的数量. 

// No Serialized Data
  CUnitComponent  m_ComponentTop;       // 将单元序列文件看作为在一个虚拟的部件中，简化了存储、封装和编译（尚未实现，研究几日再说05/04/2019)
	CUnitList *     m_pRunTimeUnitList;
  CUnitList *     m_pCurrentUnitList;
  CDicList        m_CDicList;

// Generated message map functions
protected:
	//{{AFX_MSG(CSQIFileDoc)
	afx_msg void OnProjectCompile();
	afx_msg void OnUpdateProjectCompile(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

                                                  
