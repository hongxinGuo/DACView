//////////////////////////////////////////////////////////////////////////
//
//	CUnitBase class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo 
//	Create Time									: 2001, 2, 27
//
//
///////////////////////////////////////////////////////////////////////////
#ifndef __OBJECTBASE_H__
#define __OBJECTBASE_H__

#include "typedef.h"
#include "CObjectPrimitive.h"

#include "cObjDynLink.h"

class CObjectSymbol;

class CObjectBase : public CObjectPrimitive {
public :
  CObjectBase(const CString& Name, COLORREF BkGrd = RGB(0, 0, 0), COLORREF ForeGrd = RGB(255, 255, 255)); 
  CObjectBase();
  
  DECLARE_SERIAL(CObjectBase);
  
  ~CObjectBase();
// Attributes
	// 能否被符号包含，有些能（如长方形等），有些不能（如按钮，滚动条等）
  virtual bool            CanInSymbol(void) { return true; }
	// 是否是长方形 
	virtual bool            IsRectShape( void )  { ASSERT(false); return(true); }
	// 是否透明  
	virtual bool            IsTransparent( void ) { return(m_fTransparent); }
	// 设置透明标志 
  virtual void            SetTransparentFlag(bool fFlag) { m_fTransparent = fFlag; }

// Operations
public:          
	// 得到本对象的当前大小
	virtual const CRect&		GetSize( void );			
	// 得到本对象的原大小
	virtual const CRect&		GetOriginSize( void );
	// 得到上一次显示时的大小
	virtual const CRect&		GetLastSize( void );
  // 得到本对象的绝对位置（以Rect表示）
  virtual const CRect     GetAbsoluteSize(void);

	//得到剪辑区
	virtual CRgn *					GetClipRgn( const CPoint& ) { ASSERT(false); return(&m_rgnClip); }
  CODLList*  							GetDynLinkList(void) { return &m_listDynLink; }
  COLORREF 								GetBkGrdColor(void) const { return m_clrBkGrd; }
  COLORREF 								GetForeGrdColor(void) const { return m_clrForeGrd; }
	
	// 设置所有的区域
	virtual void 			SetAllSize( const CRect& ) { ASSERT(false); }
  virtual void      SetOriginSize( const CRect& ) { ASSERT(false); }
	// 设置动态区域大小
	virtual void			SetDynamicSize( const CRect&  ) { ASSERT(false); }

  void							SetBkGrdColor(COLORREF color) { m_clrBkGrd = color; }
  void							SetForeGrdColor(COLORREF color) { m_clrForeGrd = color; }
	
	// 设置包含本对象的符号指针
  void              SetSymbolThatHaveMe( CObjectSymbol * pSymbol ) { m_pSymbolHaveMe = pSymbol; }
  CObjectSymbol *   GetSymbolThatHaveMe(void) { return m_pSymbolHaveMe; }

  virtual bool      SetParameterSelected(void);

	virtual bool			CheckSelf( void ) override;

  virtual bool      CreateUniName( CObjectList& listObject );
  virtual void      AddToList( CObjectList& listObject ) { listObject.push_back(this); }

	// 调用对话窗处理动态连接
	bool 				 			IsDeleteDynLink( void );
	// 删除动态连接
	virtual bool			DeleteDynLink( CUnitBase * pUnit );

	virtual bool      IsDrawAll(void) { return m_fDrawAll; }
  virtual void      SetDrawAll( bool fFlag ) { m_fDrawAll = fFlag; }

  virtual bool      NeedShowCaret( void ) { return( m_fNeedShowCaret ); }
  virtual bool      AdjustCaretPos( CDC * , CPoint& ) { ASSERT(false); return false; }

  virtual CPoint    GetOffset(void);

  virtual bool      InIt(POINT const , int ) { ASSERT(false); return false; }
  virtual bool      ProcessChar( UINT , CPoint& ) { ASSERT(false); return false; }
	virtual void      ToShowDynamic( CDC * const ) { ASSERT(false); }			// draw myself on memory DC 
  virtual void      ToShowStatic(CDC * const , CPoint ) { ASSERT(false); }		// draw myself on DC 
  virtual bool 			SetProperty(void) { ASSERT(false); return false; }					// set object's property 

  
// dynamic link virtual function 
// below function should difinition virtually in CObjectBase
	virtual bool 			SetRectPosition(ULONG , LONG ) { ASSERT(false); return false; }
  virtual bool      SetBool(ULONG , bool ) { ASSERT(false); return false; }
  virtual bool 			SetInteger(ULONG , LONG ) { ASSERT(false); return false; }
  virtual bool  		SetDouble(ULONG , double ) { ASSERT(false); return false; }
  virtual double		GetDouble(ULONG ) { ASSERT(false); return 0.0; }
  virtual bool	 		SetString(ULONG , const CString& ) { ASSERT(false); return false; }
	virtual bool 			SetColor( ULONG , LONG ) { ASSERT(false); return false; }

	virtual bool  		ExectiveDynLink( void ) { ASSERT(false); return false; }		// 执行动态连接
	
	// 处理动态连接的函数
  virtual ParaName* GetParaNameAddress(void) { return nullptr; }
  virtual CString		GetParaName(ULONG ) { return(""); }
  virtual INT32			GetIndex(ULONG ) { ASSERT(false); return 0; }
	virtual ULONG 		GetDynLinkType( ULONG  ) { ASSERT(false); return 0; }
	virtual bool 			SetParameterLock(ULONG , bool ) { ASSERT(false); return false; }
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected) { m_vfSelected[ulIndex] = fSelected; return true; }

  virtual void     	SelectParameter(ULONG ) { ASSERT(false); }

// implememtation 
 	virtual void Serialize( CArchive& ar );
  
	virtual void 			AdjustInnerSize( void ) { ASSERT(false); }					//调整内部Objects大小，CObjSymbol使用
	virtual void 			AdjustDynamicInnerSize( void ) { ASSERT(false); }		// 动态调整内部Objects大小，CObjSymbol执行时使用。

protected:
  virtual void 			SetFocus(CDC * ) { ASSERT(false); }	 // this object is on focus

private :
	virtual void			ToShow(CDC * const ) { ASSERT(false); }
  
public: 
	// assist function

#ifdef _DEBUG
	virtual	void 			AssertValid() const;
	virtual	void 			Dump(CDumpContext& dc) const;
#endif  

// globe static data declaration
protected :

protected :
// Serialized Data
  CODLList 					m_listDynLink;	// 本对象所包含的动态连接(从单元中联入数据)
  COLORREF 					m_clrBkGrd;			// 背景色
  COLORREF 					m_clrForeGrd;		// 前景色
  bool              m_fTransparent; // 是否透明
  INT64             m_iReserved1;   // 备用数据
  INT64             m_iReserved2;   // 备用数据
  INT64             m_iReserved3;   // 备用数据
  INT64             m_iReserved4;   // 备用数据

// Not Serialized Data
	bool 							m_fChangeSize;		// 对象大小改变标志
	bool              m_fDrawAll;				// 重新画标志。
	bool              m_fNeedShowCaret;	// 显示光标标志

	CRgn							m_rgnClip;				// 剪辑区

  CObjectSymbol *   m_pSymbolHaveMe;	// 包含本对象的符号指针
									
// Dynamic changed data	
	CString  					dm_strName; 		// this object's name, used for ID
  COLORREF 					dm_clrBkGrd;		// 动态变化背景色
  COLORREF 					dm_clrForeGrd;	// 动态变化前景色

  
	// 显示本类所需变量。
  CDC 				m_MemoryDC;						// 
	CBitmap			m_Bitmap;							// 
	bool 				m_fCreateMemoryDC;		//

};     

typedef shared_ptr<CObjectBase> ObjectPtr;

#endif
  
