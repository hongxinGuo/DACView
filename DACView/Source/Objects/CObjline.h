#pragma once

#include "cObjectBase.h"

class CObjectLine : public CObjectBase {
public :
  CObjectLine(const CString& s, const CPoint& ptStart, const CPoint& ptEnd);
  CObjectLine( void );
        
  DECLARE_SERIAL(CObjectLine);
       
  ~CObjectLine();
        
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;
  virtual bool          IsRectShape( void ) override;
  
// Operations
public:  // derive from CObjectBase
	virtual const CRect&  GetSize( void ) override;
  virtual const CRect&  GetLastSize( void ) override;
	virtual CRgn *        GetClipRgn( const CPoint& ptScrollPosition = CPoint(0, 0) ) override;
	virtual void          ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void          ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool          ExectiveDynLink( void ) override;
	virtual bool          InIt( CDC * const pdc, POINT const pt, int iStatus ) override;
  virtual bool          SetProperty( void ) override;                                    // set object's property 
  virtual bool          CheckSelf( void ) override;

// implementation 
protected:
  virtual void          SetFocus( CDC *pdc) override; 

#ifdef _DEBUG
  virtual void AssertValid() const override;
  virtual void Dump(CDumpContext& dc) const override;
#endif

protected:
// Serialize data
  INT32 m_ulWidth;      // Width of line  
  CPoint m_ptEnd;
  CPoint m_ptStart;

// no serialize data
  CRect m_rectArea;
  CRect m_rectAreaLast;
  
};                       