//////////////////////////////////////////////////////////////////////////////////////
//
//	CObjSymbol.h
//
//	符号类，由Objects复合而成一符号，复合的Objects可以被外部看见，只有一层，无法进入，。
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////
#ifndef __OBJECT_SYMBOL_H__
#define __OBJECT_SYMBOL_H__

#include "cObjComponentBase.h"

class CObjectSymbol : public CObjectComponentBase {
	public :
	CObjectSymbol(CString s, CRect r);
	CObjectSymbol();
	
	DECLARE_SERIAL(CObjectSymbol);
	
	~CObjectSymbol();
	
// Attributes
public:
  virtual void    Serialize( CArchive& ar ) override; 
  virtual const   CString& GetClassNameStr( void ) override;

	virtual bool    IsNeedUpdate( void ) override;
  virtual bool    IsDrawAll( void ) override;
  virtual void    SetDrawAll( bool fFlag ) override;
	virtual CRgn *	GetClipRgn( const CPoint& ptOffset) override;

  virtual CPoint  GetOffset(void) override;

  virtual void    ViewOut( void ) override;
  virtual bool    DumpCurrentStatus( char * pch ) const override;
// Operations
public:
	void            InsertObject( CObjectBase * pcobj );
  void            BreakSymbol( CObjectList * pObjList );
	virtual void    ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void    ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
  virtual void    AdjustInnerSize( void ) override;
	virtual void    AdjustDynamicInnerSize( void ) override;
	virtual bool    ExectiveDynLink( void ) override;
	
  virtual bool    SetProperty( void ) override;	// set object's property 
  virtual bool    CheckSelf( void ) override;

// dynamic link virtual function 
	virtual bool    SetInteger(ULONG index, LONG lValue) override;
	virtual bool    SetRectPosition(ULONG index, LONG lValue) override;

  // implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

protected:
// serialized data

// no serialized data
  list<shared_ptr<CRect>> m_CRectList;
  CRect                   m_rectSymbolOrigin;

};     

typedef shared_ptr<CObjectSymbol> CObjectSymbolPtr;

#endif

