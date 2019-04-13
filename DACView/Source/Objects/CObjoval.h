#pragma once

#include "cObjRectBase.h"

class CObjectOval : public CObjRectBase {
	public :
	CObjectOval(CString s, CRect r);
	CObjectOval( void );
	
	DECLARE_SERIAL(CObjectOval);
	
	~CObjectOval();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;
  virtual bool      IsRectShape( void ) override;
  
// Operations
public:
	virtual CRgn *	  GetClipRgn( const CPoint& ptScrollPosition = CPoint(0, 0) ) override;
	virtual void      ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	
// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

protected:

};                       
