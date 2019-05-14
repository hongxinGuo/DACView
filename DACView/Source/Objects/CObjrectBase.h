#ifndef __OBJECT_RECTBASE_H__
#define __OBJECT_RECTBASE_H__

#include <afxmt.h>

#include "cObjectBase.h"
#include "typedef.h"

extern COLORREF rgbArray[];

class CObjRectBase : public CObjectBase {
public :
  CObjRectBase(const CString& Name, const CRect& Area, 
  						 COLORREF BkGrd = RGB(0, 0, 0), COLORREF ForeGrd = RGB(255, 255, 255)); 
  CObjRectBase();
  
  DECLARE_SERIAL(CObjRectBase);
  
  ~CObjRectBase();

// Attributes
public:
  virtual bool            IsRectShape( void );
  
// Operations
public:          
  virtual void            Serialize( CArchive& ar ) override;

  virtual const CRect&		GetSize(void) override { return m_rectArea; }
  virtual const CRect&		GetOriginSize(void) override { return m_rectOrigin; }
  virtual const	CRect&		GetLastSize(void) override { return m_rectLastTime; }
  virtual const CRect     GetAbsoluteSize(void) override { return(m_rectArea + GetOffset()); }
	virtual CRgn *		      GetClipRgn( const CPoint& ptScrollPosition = CPoint(0, 0) ) override;
  virtual CString         GetTitle(void) { return(m_strTitle); }

	virtual void 			SetAllSize( const CRect& rectArea ) override { m_rectOrigin = m_rectArea = m_rectLastTime = rectArea; }
  virtual void      SetOriginSize(const CRect& rectOrigin) override { m_rectOrigin = rectOrigin; }
  virtual void 			SetDynamicSize(const CRect& rectArea) override { m_rectArea = rectArea; m_fDrawAll = true; }
	
	virtual bool      InIt(POINT const pt, int iStatus) override;
  virtual void      ToShowDynamic(CDC * const ) override {}	// draw myself on memory DC 
  virtual void      ToShowStatic(CDC * const , CPoint ) override {}	// draw myself on DC 
  virtual void 			AdjustInnerSize(void) override {}
	virtual void 			AdjustDynamicInnerSize( void ) override {}	// do nothing, use by CObjectSymbol
  virtual bool			SetProperty( void ) override;					// set object's property 
  virtual bool			CheckSelf( void ) override;

// dynamic link virtual function 
// below function should difinition virtually in CObjRectBase
	virtual bool 			SetRectPosition(ULONG index, LONG lValue) override;
  virtual bool      SetBool(ULONG index, bool fValue) override;
	virtual bool 			SetInteger(ULONG index, LONG lValue) override;
  virtual bool 		 	SetDouble(ULONG , double ) override { ASSERT(0); return(false); }
  virtual double		GetDouble(ULONG ) override { ASSERT(0); return(0.0); }
	virtual bool		 	SetString(ULONG index, const CString& strValue) override;
	virtual bool 			SetColor(ULONG index, DWORD lValue);
	virtual bool  		ExectiveDynLink( void ) override;
	
// implememtation 
 
protected: 
	// assist function
  virtual void 			SetFocus( CDC *pdc) override;									// this object is on focus

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif  
  
protected :
// Serialized Data
  CRect 					m_rectOrigin;	 // this object's area, 
	CString 				m_strTitle;	// title

// Not Serialized Data
	CRect						m_rectArea;  	
	CRect						m_rectLastTime;
#ifdef _DEBUG
	CRect						m_rectTest;
#endif
  CCriticalSection  m_csLock;
	bool						m_ptrfSelectIndex[6];

// Dynamic changed data	
};                        

#endif
