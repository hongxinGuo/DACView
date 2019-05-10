///////////////////////////////////////////////////////////////////////
//
// ��˾								: FireBird software Inc.
// ����								: guo
// ��������						: 1999, 3, 11
//
// Object��Unit֮������ӣ�
//
////////////////////////////////////////////////////////////////////////
#ifndef __OBJECT_DYANMIC_LINK_H__
#define __OBJECT_DYANMIC_LINK_H__

using namespace std;
#include<list>

#include "cObjectPrimitive.h"

class CUnitBase;
class CObjectBase;

class CObjectDynLink : public CObjectPrimitive {
public :
  CObjectDynLink(CString Name);
  CObjectDynLink( void );
  
  DECLARE_SERIAL(CObjectDynLink);
  
  ~CObjectDynLink();

// Attributes
public:
  virtual void Serialize( CArchive& ar );

	void Copy( CObjectDynLink * pDynLink );
  
// Operations
public:          
	CString 				GetUnitName( void ) const;
	CUnitBasePtr		GetUnit( void ) const ;
  CObjectBase *		GetObject( void ) const ;
	INT32						GetUnitIndex( void ) const ;
	INT32						GetObjectIndex( void ) const ;
	ULONG						GetLinkMethod( void ) const ;
	ULONG						GetObjectDynLinkType( void ) const ;
  ULONG						GetUnitDynLinkType(void) const;
  ULONG						GetUnitParaType(void) const;
  bool						IsUnitToObject( void ) const ;
	CString&				GetComment( void );
	
	void 						SetDeleteMeFlag( BOOL fFlag );
	bool						IsDeleteMe( void ) const ;

	void 						SetUnit( CUnitBasePtr pc);
  void						SetObject( CObjectBase * pc);
	void 						SetObjectIndex( ULONG ulIndex );
	void 						SetUnitIndex( ULONG ulIndex );
	void 						SetLinkMethod( ULONG ulIndex );
	void						SetDataFlowUnitToObject( bool flag );
	void 						SetComment( CString strComment );
	
// implememtation 
 
public : 
	// assist function
	
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif
 
protected :
// Serialized Data
	CUnitBasePtr    m_pCUnit;						// ָ��Ԫ��ָ��.
  INT32 			    m_lUnitIndex;			  // ��Ԫ�Ĳ�������.
	INT32 			    m_lObjectIndex;		  // ����Ĳ�������.
	INT32				    m_lLinkMethod;			// ���ݵ����ӷ���.
	bool				    m_fUnitToObject;		// ���ݵ�����:��Ԫ->����,�����->��Ԫ.
	CString			    m_strComment;				// ����̬���ӵ�����.
	  
// Not Serialized Data
	CString			    m_strUnitName;			// ��Ԫ������.
  CObjectBase *   m_pCObject;				// ָ������ָ��.	
	bool 				    m_fDeleteMe;				// �Ƿ�ɾ���ı�־.
};                        

typedef list<CObjectDynLink *> CODLList;

#endif

