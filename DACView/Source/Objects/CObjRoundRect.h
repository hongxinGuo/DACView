#pragma once

#include "cObjRectBase.h"

class CObjectRoundRect : public CObjRectBase {
	public :
	CObjectRoundRect(CString s, CRect r);
	CObjectRoundRect( void );
	
	DECLARE_SERIAL(CObjectRoundRect);
	
	~CObjectRoundRect();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;
  virtual bool    IsRectShape( void ) override;

// Operations
public:
	virtual CRgn *	GetClipRgn( const CPoint& ptScrollPosition ) override;
	virtual void    ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void    ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool 		SetProperty( void ) override;					// set object's property 
  
// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

protected:
	// serialized data
	INT32 m_ulRound;

};                       
