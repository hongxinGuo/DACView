///////////////////////////////////////////////////////////////////////
//
// 公司								: FireBird software Inc.
// 作者								: guo
// 生成日期						: 1999, 3, 11
//
// Object和Unit之间的连接，
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
	CUnitBasePtr    m_pCUnit;						// 指向单元的指针.
  INT32 			    m_lUnitIndex;			  // 单元的参数索引.
	INT32 			    m_lObjectIndex;		  // 对象的参数索引.
	INT32				    m_lLinkMethod;			// 数据的连接方法.
	bool				    m_fUnitToObject;		// 数据的流向:单元->对象,或对象->单元.
	CString			    m_strComment;				// 本动态连接的描述.
	  
// Not Serialized Data
	CString			    m_strUnitName;			// 单元的名称.
  CObjectBase *   m_pCObject;				// 指向对象的指针.	
	bool 				    m_fDeleteMe;				// 是否删除的标志.
};                        

typedef list<CObjectDynLink *> CODLList;

#endif

