////////////////////////////////////////////////////////////////////
//
//	CUnitDynamicLink class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo
//	Create Time									: 1996, 6, 12
//
//  First modified              : 1996, 12, 6
//    Add m_ptSrc, m_ptDest and m_plistLinkPoint->
//	
//	Second modified							: 1999, 2, 12
//		将动态连接类型中的OBJECT该为UNIT.
//
//  第三次更改:									: 1999, 3, 11
//		取消m_fSelected及相关的函数.
//
///////////////////////////////////////////////////////////////////////
#ifndef __UNIT_DYNAMIC_LINK_H__
#define __UNIT_DYNAMIC_LINK_H__

enum { 	UNIT_TO_UNIT		 			  = 0X0001,	// 简单单元（或者已封装的部件）与简单单元（或者已封装的部件）之间互联
				UNIT_TO_COMPONENT	 		  = 0X0002, // 简单单元动态链接入部件
				COMPONENT_TO_UNIT	 		  = 0X0004, // 部件内的简单单元（或者已封装的部件）动态链接入与部件在同一层的简单单元（或者已封装的部件）
				COMPONENT_TO_COMPONENT 	= 0X0008, // 部件内的简单单元（或者已封装的部件）动态链接入同层的另一个部件内的简单单元（或者已封装的部件）
        UNIT_TO_CptPARAMETER    = 0X0010, // 部件内的简单单元（或者已封装的部件）动态链接入此部件的参数处
			};

#include "typedef.h"

#include "CObjectPrimitive.h"

class CUnitDynLink : public CObjectPrimitive {
public :
  CUnitDynLink( void );
  
  DECLARE_SERIAL(CUnitDynLink);
  
  ~CUnitDynLink();

private :
	
// Attributes
public:
  virtual void Serialize( CArchive& ar );
  
// Operations
public:          
	CUnitBase * GetSrcUnit( void ) const ;
	CUnitBase * GetDestUnit( void ) const ;
	INT32 			GetDestIndex( void ) const;
	INT32 			GetSrcIndex( void ) const;
	INT32 			GetDynLinkType( void ) const;
	INT32				GetDynLinkClass( void ) const;
  CPointListPtr GetLinkPointList( void );
	BOOL				IsDeleteMe( void ) const;
	BOOL				IsSetLoopDetectFlag( void ) const;

	void 				SetLoopDetectFlag( BOOL fFlag );
	void 				SetDeleteMeFlag( BOOL fFlag );
	void 				SetSrcUnit( CUnitBase * pc);
	void 				SetDestUnit( CUnitBase * pc);
	void 				SetSrcIndex( INT32 ulIndex );
	void 				SetDestIndex( INT32 ulIndex );
	void 				SetDynLinkType( INT32 ulType );
	void 				SetDynLinkClass( INT32 ulClass );
  void        SetLinkPointList( CPointListPtr plist );

	virtual void ToShow(CDC * const pdc);// draw myself on DC 

// implememtation 
 
public : 
	// assist function

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

protected :
// Serialized Data
  CUnitBase * m_pSrcUnit;					// 指向源单元.
  CUnitBase * m_pDestUnit;				// 指向目标单元.
	INT32 			m_lSrcIndex;		    // 源单元的参数索引.
	INT32 			m_lDestIndex;			  // 目标单元的参数索引.
	INT32				m_ulDynLinkType;    // BOOL, WORD, DOUBLE, or STRING
	INT32				m_ulDynLinkClass;		// 四种连接类型之一, 单元->单元,单元->复合单元,复合单元->单元,复合单元->复合单元.
  CPointListPtr m_plistLinkPoint;		// 从源单元到目标单元的动态连接的点序列（用于显示）。
	  
// Not Serialized Data
  CUnitBasePtr   m_punitSrc;
	BOOL 				m_fDeleteMe;        // 是否删除标志.
	BOOL				m_fLoopDetect;			// 是否本动态连接位于一个循环中（循环连接的必须设置截断点，否则会出现重复计算）。

// 测试用变量
public:
  CUnitBase * mTest_pDestUnitSaved;   // 当封装部件的输出型参数时，用于保存原动态链接的目的单元
};                        

typedef list<shared_ptr<CUnitDynLink>> CUDLList;

#endif

