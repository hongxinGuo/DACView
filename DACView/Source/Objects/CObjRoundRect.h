#pragma once
#include "typedef.h"

#include "cObjRectBase.h"

class CObjectRoundRect : public CObjRectBase {
	public :
	CObjectRoundRect(CString s, CRect r);
	CObjectRoundRect();
	
	DECLARE_SERIAL(CObjectRoundRect);
	
	~CObjectRoundRect();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassName( void ) override;
  virtual bool    IsRectShape( void ) override;

// Operations
public:
	virtual CRgn *	GetClipRgn( const CPoint& ptScrollPosition ) override;
	virtual void    ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void    ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool 		SetProperty( void ) override;					// set object's property 
  
	virtual ParaName* GetParaNameAddress(void) override;
	virtual CString		GetParaName(ULONG index) override;
	virtual INT32			GetIndex(ULONG ulIndex) override;
	virtual ULONG 		GetDynLinkType(ULONG Index) override;
	virtual void     	SelectParameter(ULONG ulType) override;

// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

// globe static data declaration
private:
	static INT32  sm_aulSuitable[7];

	static const CString  sm_cstrClassName;
	static ParaName 			sm_ptrParaName[7];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
	// serialized data
	INT32 m_ulRound;

};                       
