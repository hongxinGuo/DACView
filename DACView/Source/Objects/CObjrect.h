#pragma once

#include "cObjRectBase.h"

class CObjectRect : public CObjRectBase {
	public :
	CObjectRect(CString s, CRect r);
	CObjectRect( void );
	
	DECLARE_SERIAL(CObjectRect);
	
	~CObjectRect();
	
// Attributes
public:
  virtual void  Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;
  
// Operations
public:
	virtual bool	CheckSelf( void ) override;
	virtual void  ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void  ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	
// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

protected:

};                       
