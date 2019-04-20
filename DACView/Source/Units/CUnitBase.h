///////////////////////////////////////////////////////////////////////////
//	CUnitBase class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo
//	Create Time									: 1999, 2, 27
//
// 基本Unit类，除了部件和复合单元，其他所有的Unit类都叫做“简单单元”。
//
//
///////////////////////////////////////////////////////////////////////////
#pragma once

#include<list>

#include "CObjectPrimitive.h"

#include "CUnitDynLink.h"
#include "cUnitDictionary.h"

class CUnitBase;
class CUnitComponent;

typedef CList<CUnitDictionary *, CUnitDictionary *> CDicList;
typedef CList<CUnitBase *, CUnitBase *> CUnitList; 

class CUnitBase : public CObjectPrimitive {
public :
  CUnitBase(const CString& Name, CPoint pt); 
  CUnitBase( void );
  
  DECLARE_SERIAL(CUnitBase);
  
  ~CUnitBase();

// Attributes
public:
  virtual void Serialize( CArchive& ar ) override;
  
// Operations
public:
  // 得到本单元的名称
  virtual CString		GetName(void) override;

	void            SetUpperUnitList(CUnitList * pUnitList);
	CUnitList *     GetUpperUnitList(void);

	// 得到本单元的位置大小.
	const CRect&			GetSize( void ) const;
	// 设置本单元的位置大小.
	void 							SetSize( const CRect& RectSize );

	// 设置注释
	void							SetComment(const CString& strComment // 注释
																);
	//得到注释字符串
	CString						GetComment(void);


	// 得到本单元的动态连接序列的指针.
	CUDLList * 				GetDynLinkList( void );

	// 是否本单元的参数被包含本单元的部件使用.
  bool							IsLinkToComponent( void );
	// 设置参数连入包含本单元的部件标志.
	void							LinkToComponent( bool fFlag // 连入为真,取消连入为假.
																		);

	// 是否本单元从Object处联入数据
	bool							IsLinkFromObject(void) { return m_lLinkFromObject; }
	// 设置从Object处联入数据数量
	void							SetLinkFromObjectFlag(bool fFlag  // 联入为真，取消联入为假
																					);
	// 有多少参数连入部件.
	ULONG							HowManyLinkToComponent( void );

  void              LinkFromComponent(bool fFlag); 
 // 是否本单元的参数被包含本单元的部件使用.
  bool							IsLinkFromComponent(void) { if (m_lLinkFromComponent > 0) return(true); else return(false); }
 // 有多少参数连入部件.
  INT32							HowManyLinkFromComponent(void) { return(m_lLinkFromComponent); }

	void 							SetComponentUpper(CUnitComponent * pUnitComponent // 指向包含本单元的复合部件.
																					);
	CUnitComponent *	  GetComponentUpper( void ) const;

	// 设置本单元的动态连接删除标志.如本单元的动态连接连入listUnit, 则设置标志.
	virtual void			SetDeleteDynLinkFlag( CUnitList& listUnit // 被删除的单元序列,
																				);
	// 清楚本单元的动态连接删除标志.
	virtual void			ClearDeleteDynLinkFlag( void );
	
	// 设置截断标志.
	void 							SetCutOff( bool fFlag // 是否截断标志.
															);
  bool							IsSetCutOff( void ) const;

  //设置单元已被编译标志
  virtual void      SetCompiledFlag(bool fFlag) { m_fCompiled = fFlag; }
  virtual bool      IsCompiled(void) { return(m_fCompiled); }
	
	// 得到本单元的执行优先值.
	INT32 						GetExectivePriority( void ) const;
	// 直接设置本单元的执行优先值为ulPriority
	void							SetExectivePriorityDirect( ULONG ulPriority );
	// 设置本单元的执行优先值.
	virtual void 			SetExectivePriority( ULONG ulPriority // 要设置的优先值.
																				);
	
  // 设置本单元的目标单元的运行优先值.
	virtual void      SetDestUnitPriority( void ); 
	
  // 重置编译标志.
	virtual void			ResetCompileFlag( void );

	// 编译自己, 被部件使用,其他单元为空
	virtual bool			Encapsulation( CUnitList & listTotalUnit );
	// 封装与否？
	virtual bool			IsEncapsulated(void);

  // 可否被封装。 简单单元不可被封装；部件允许被封装；复合单元（特殊的部件）不可被封装。
  virtual bool      IsEncapsulable(void) { return false;  }

	// 准备运行时态序列.
	virtual void			PrepareRunTimeList( void );

	// 将一个动态连接加入本单元的动态连接序列.
	void 							AddDynLink( CUnitDynLink * punitDynLink // 指向动态连接的指针.
															 ) ;
	
	// 观察并处理本单元的动态连接.
	virtual bool 			ArrangeDynLink( void );
	// 删除本单元的指向pUnit的动态连接.
	virtual bool			DeleteDynLink( CUnitBase * pUnit	// 指向被删除的unit
																	);

	// 生成独一无二的单元名称.
	virtual bool			CreateUniName( CUnitList& listUnit // 指向需要避免的unit list
																	);
	// 调节本单元动态连接的坐标位置.
  virtual void      AdjustDynLinkLinePosition( CUnitBase * pc,	// 指向被调整位置的单元
																							 CPoint ptStart,	//动态链接线的起始点
																							 CPoint ptEnd);		// 动态链接线的终止点
	
	// 设置本单元动态连接所指向的单元的参数锁.
	virtual void 			SetParaLockFlag( void );

	// 得到本单元的输入参数个数.
	ULONG							GetInputParameterNumber( void );
	// 设置本单元输入的参数个数
	void							SetInputParameterNumber(LONG lNumber // 输入参数的个数
																						);

	// 本单元是否存在从上层联入的参数
	virtual bool			IsDynLinkFromUpper(void);

	virtual bool			IsOverFlow(void);
	
	bool							IsHaveSourceUnit( void ) const;
	void							SetDynLinkToNumber(long lValue) { m_lDynLinkToNumber = lValue; }
	
	// 设置本单元的发现循环标志.
  bool              SetLoopDetectFlag( CUnitBase * pcunit // 指向被检查的单元
																			);

	// 清除本单元的发现循环标志.
	virtual void      ClearLoopDetectFlag( void );

	// 检查本单元是否有循环在pCUnitList中.
	virtual bool			LoopDetect( CUnitList * pCUnitList	// 存储当前寻找到的单元序列.
																);

  // 检查本单元的循环中是否存在截断
  virtual bool      CheckCutOff(CUnitList * pCUnitList);

  // 设置本单元是否有动态链接至包含本单元的部件参数处.
  virtual void			CheckInnerDataLink(INT64 lSrcIndex, INT64 lDestParaPos, CUnitList * pCUnitList); // 存储当前寻找到的单元序列.
  
  // 检查本单元的内部是否有错误.
	virtual bool			CheckSelf( void );
	
	virtual const CString&		GetClassNameStr( void ) override;				// 得到本类的类名
	virtual CString						GetParaName( ULONG index );			// 得到本类的参数名称
	virtual ULONG							GetParaType( ULONG ulIndex );		// 得到本类的参数类型。此函数是得到参数所有的类型，而GetDynLinkType只得到四种数据类型中的一种

	// 是否可被连入.
	virtual bool							CanLinkIn( void );

	// 是否是我.
	virtual bool 							IsMe( const CString& strName // 被检查的单元名称.
																);

	// 得到本单元的输入输出类型,输出型或输入输出型.有些单元只有输出型参数，如计数器、图形发生器等。
	virtual ULONG			GetUnitType( void );
	
	// 执行本单元.
	virtual void  		Exective( void );
	// 执行本单元的动态连接.
	virtual bool 			ExectiveDynLink( void );

	// 得到布尔值数值
	virtual bool 							GetBool( ULONG index	// 参数位置
																		);
	// 设置布尔值数据
	virtual bool 							SetBool( ULONG index,	// 参数位置
																		 bool fValue	// 标志
																		);
	// 得到整数数值
	virtual LONG 							GetInteger(ULONG index	// 参数位置
																			);
	// 设置整数数据
	virtual bool 							SetInteger(ULONG index, // 参数位置
																			LONG iValue		// 参数数值
																			);
	// 得到浮点数数值
	virtual double 						GetDouble(ULONG index		// 参数位置
																			);
	// 设置浮点数数据
	virtual bool  						SetDouble(ULONG index,	// 参数位置
																			double eValue	// 参数数值
																			);
	// 得到字符串数值
	virtual const CString 		GetString(ULONG index		// 参数位置
																			);     
	// 设置字符串数据
	virtual bool		 					SetString(ULONG index,	// 参数位置
																			const CString& strValue	// 参数数值
																			); 

	// 得到动态连接的类型：布尔型,整数型,实数型等
	virtual ULONG 		GetDynLinkType( ULONG ulIndex		// 参数位置
																		);
	// 设置或解除参数锁.
	virtual bool 			SetParameterLock( ULONG ulIndex, // 参数索引.
																			bool fSelected // 设置或解除标志.
																			) override;

  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected) override;

	// 准备参数辞典.
	virtual void			PrepareParaDictionary( CDicList &CListDic,	// 处理后的辞典.
																					 ULONG ulType					// 要匹配的参数类型.
																					 );
	// 选择合适的参数,准备参数索引.
	virtual void     	SelectParameter( ULONG ulType  // 被选择的参数类型
                                    );  
	// 得到合适类型参数的索引.
	virtual INT32			GetIndex( ULONG ulIndex				// 合适参数的索引
															);

	// 得到本单元类的参数队列地址.
	virtual ParaName* GetParaNameAddress( void );
	// 生成参数词典,内部使用.
	virtual void			CreateParaDictionary( CDicList &m_CDicList, // 处理后的辞典.
																					ULONG ulType // 要匹配的参数类型.
																					);
	
	// 是否处于本单元显示范围内.
	virtual bool 			InIt( POINT const pt // 当前点. 
												);
	// 显示本单元.
  virtual void 			ToShow(CDC * const pdc);
	// 设置焦点.
  virtual void 			SetFocus( CDC *pdc);
	// 删除焦点.
  virtual void 			ClearFocus( CDC *pdc);
	// 设置本单元的内部选项.
  virtual bool 			SetProperty( void );

	// 将本单元加入单元序列中.
	virtual void 			AddToList(CUnitList& UnitList);

	// 得到本单元类的参数选择队列的地址, 内部使用.
	virtual INT32*		GetArrayIndex( void );

  virtual bool      IsParameterLocked(ULONG ulIndex) override;

public : 
	// assist function
	void DrawBitmap( CDC * pdc, CBitmap& bitmap, CRect& rect );

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

public:			
    
protected :
// Serialized Data
  CString  		m_cstrComment;				// 本单元的描述.
  CRect    		m_rectArea;  					// 单元的坐标位置.
  CUDLList		m_listDynLink;				// 单元的动态连接序列.

	bool 				m_fCutOff;						// 是否设置截断.
	bool	 			m_fAutoExective;			// 是否自动执行.
  INT64       m_lReserved3;         // 保留，以备后用
  // 当编译后的部件存储时，最好存储这两个变量，这样就不需要再次确认这两个变量的值了。
	INT32			  m_lDynLinkToNumber;	// 本单元输入连接的个数. 
	INT32 		  m_lExectivePriority;	// 单元的执行优先值,用于确定执行顺序,1最优先.

// No Serialized Data
protected:
	CUnitComponent * m_pUnitComponentUpper;	// 指向包含本单元的复合单元,nullptr如果没有.
	CUnitList *     m_pUnitListUpper;	// 指向上层部件(或者就是本SQI文件)的单元序列

	bool			m_fOverFlow;						// 是否溢出, 运行时使用.

  bool      m_fCompiled;            // 单元本身已被编译。如果是部件，则部件内部单元序列已被编译。

	INT32			m_lCountDLToNumber;		// 输入连接计数器.

	INT32			m_lLinkToComponent;		// 本单元输出到部件的动态链接数量.仅显示时使用。
  INT32     m_lLinkFromComponent;  // 本单元从部件联入的动态链接数量，仅显示时使用。
	INT32     m_lLinkFromObject;			// 本单元从Object联入的动态链接数量，仅显示时使用。

  // 测试用变量
public:

};
