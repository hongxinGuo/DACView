#pragma once

#include "cObjRectBase.h"

class CObjectButton : public CObjRectBase {
	public :
	CObjectButton(const CString& s, CRect r);
	CObjectButton( void );

	DECLARE_SERIAL(CObjectButton);
	
	~CObjectButton();
	
// Attributes
public:
  virtual void      Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;

  virtual bool      CanInSymbol( void ) override;
// Operations
public:
	virtual void      ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool      ExectiveDynLink( void ) override;
	virtual bool      InIt( CDC * const pdc, POINT const pt, int iStatus ) override;

// dynamic link virtual function 
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual CString		GetParaName( ULONG index ) override;
	virtual INT32			GetIndex( ULONG ulIndex ) override;
	virtual ULONG 		GetDynLinkType( ULONG Index ) override;
	virtual void     	SelectParameter( ULONG ulType ) override; 	
	virtual bool 			SetProperty( void ) override;					// set object's property 
  
	virtual bool			CheckSelf( void ) override;
// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif


// globe static data declaration
private :
  static INT32 sm_aulSuitable[2];
  
  static const CString  sm_cstrClassName;          
	static ParaName 			sm_ptrParaName[];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
// serialize data
	COLORREF	 	m_clrOn;
	COLORREF		m_clrOff;
	CString			m_strOn;
	CString			m_strOff;
		
// no serialize data
	bool 				m_fPressed;
	bool 				m_fTrackMouseMove;
	bool 				m_fBeginTrack;
};                       
