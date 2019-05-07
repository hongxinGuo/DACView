#pragma once

#include "cObjRectBase.h"             

class CObjectText : public CObjRectBase {
public: 
	CObjectText( CString s, CRect r);
	CObjectText();
	
	DECLARE_SERIAL(CObjectText);
	
	~CObjectText();
	                
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassName( void ) override;
  
// Operations	 
public:               
	virtual void ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool SetProperty( void ) override;					// set object's property 
  
	CString GetShowName( void ) const;
	void SetShowName( CString const strShowName );
	             
// implementation	  

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

// globe static data declaration
private:
	static INT32 sm_aulSuitable[2];

	static const CString  sm_cstrClassName;
	static ParaName 			sm_ptrParaName[];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
	CString m_strShowName;    



	
};

