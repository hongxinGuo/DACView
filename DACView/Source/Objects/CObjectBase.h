//////////////////////////////////////////////////////////////////////////
//
//	CUnitBase class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo 
//	Create Time									: 2001, 2, 27
//
//
///////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<vector>

#include "typedef.h"
#include "CObjectPrimitive.h"

#include "cObjDynLink.h"

class CObjectBase;
class CUnitBase;
typedef CList<CObjectBase *, CObjectBase *> CObjectList;
typedef vector<CObjectBase *> CObjectVector;
typedef CList<CUnitBase *, CUnitBase *> CUnitList;

class CObjectSymbol;

class CObjectBase : public CObjectPrimitive {
public :
  CObjectBase(const CString& Name, COLORREF BkGrd = RGB(0, 0, 0), COLORREF ForeGrd = RGB(255, 255, 255)); 
  CObjectBase( void );
  
  DECLARE_SERIAL(CObjectBase);
  
  ~CObjectBase();
// Attributes
	// 能否被符号包含，有些能（如长方形等），有些不能（如按钮，滚动条等）
  virtual bool            CanInSymbol( void );

	// 是否是长方形 
	virtual bool            IsRectShape( void );
	// 是否透明  
	virtual bool            IsTransparent( void );
	// 设置透明标志 
	virtual void            SetTransparentFlag( bool fFlag );	


// Operations
public:          
	// 得到本对象的当前大小
	virtual const CRect&		GetSize( void );			
	// 得到本对象的原大小
	virtual const CRect&		GetOriginSize( void );
	// 得到上一次显示时的大小
	virtual const CRect&		GetLastSize( void );

	//得到剪辑区
	virtual CRgn *					GetClipRgn( const CPoint& ptScrollPosition);
	CODLList*  							GetDynLinkList( void ); 
	COLORREF 								GetBkGrdColor( void ) const ;
	COLORREF 								GetForeGrdColor( void ) const ; 
	
	// 设置所有的区域
	virtual void 			SetAllSize( const CRect& rectArea );
  virtual void      SetOriginSize( const CRect& rectOrigin );
	// 设置动态区域大小
	virtual void			SetDynamicSize( const CRect& rectArea );

	void							SetBkGrdColor( COLORREF color );
	void							SetForeGrdColor( COLORREF color );
	
	// 设置包含本对象的符号指针
  void              SetSymbolThatHaveMe( CObjectSymbol * pcSymbol );
  CObjectSymbol *   GetSymbolThatHaveMe( void );

  virtual bool      SetParameterSelected(void);

	virtual bool			CheckSelf( void ) override;

  virtual bool      CreateUniName( CObjectList& listObject );
  virtual void      AddToList( CObjectList& listObject );

	// 调用对话窗处理动态连接
	bool 				 			IsDeleteDynLink( void );
	// 删除动态连接
	virtual bool			DeleteDynLink( CUnitBase * pUnit );

	virtual bool      IsDrawAll(void) { return m_fDrawAll; }
  virtual void      SetDrawAll( bool fFlag ) { m_fDrawAll = fFlag; }

  virtual bool      NeedShowCaret( void ) { return( m_fNeedShowCaret ); }
  virtual bool      AdjustCaretPos( CDC *pdc, CPoint& pt );

  virtual CPoint    GetOffset( void );

	virtual bool      InIt( CDC * const pdc, POINT const pt, int iStatus );
  virtual bool      ProcessChar( UINT nChar, CPoint& pt );
	virtual void      ToShowDynamic( CDC * const pdc);		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition);		// draw myself on DC 
  virtual bool 			SetProperty( void );					// set object's property 

  
// dynamic link virtual function 
// below function should difinition virtually in CObjectBase
	virtual bool 			SetRectPosition(ULONG index, LONG lValue);
  virtual bool      SetBool(ULONG index, bool fValue);
	virtual bool 			SetInteger(ULONG index, LONG lValue);
	virtual bool  		SetDouble(ULONG index, double eValue);
	virtual double		GetDouble(ULONG index);
	virtual bool	 		SetString(ULONG index, const CString& strValue); 
	virtual bool 			SetColor( ULONG index, LONG lValue);

	virtual bool  		ExectiveDynLink( void );		// 执行动态连接
	
	// 处理动态连接的函数
	virtual ParaName* GetParaNameAddress( void );
	virtual CString		GetParaName( ULONG index );
	virtual INT32			GetIndex( ULONG ulIndex );
	virtual ULONG 		GetDynLinkType( ULONG Index );
	virtual bool 			SetParameterLock( ULONG ulIndex, bool fSelected );
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected);
	virtual void     	SelectParameter( ULONG ulType ); 

// implememtation 
 	virtual void Serialize( CArchive& ar );
  
	virtual void 			AdjustInnerSize( void );				//调整内部Objects大小，CObjSymbol使用
	virtual void 			AdjustDynamicInnerSize( void );	// 动态调整内部Objects大小，CObjSymbol执行时使用。

protected:
  virtual void 			SetFocus(CDC *pdc);	 // this object is on focus

private :
	virtual void			ToShow( CDC * const pdc );
  
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

  
