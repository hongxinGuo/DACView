#pragma once

#include "cObjRectBase.h"

class CObjectFillBar : public CObjRectBase {
	public :
	CObjectFillBar(const CString& s, CRect r);
	CObjectFillBar( void );

	DECLARE_SERIAL(CObjectFillBar);
	
	~CObjectFillBar();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;
  
// Operations
public:
	virtual void      ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool      ExectiveDynLink( void ) override;

	virtual bool			CheckSelf( void ) override;

// dynamic link virtual function 
	virtual bool  		SetDouble(ULONG index, double eValue) override;
	
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual CString		GetParaName( ULONG index ) override;
	virtual INT32			GetIndex( ULONG ulIndex ) override;
	virtual ULONG 		GetDynLinkType( ULONG Index ) override;
	virtual void     	SelectParameter( ULONG ulType ) override;
	virtual bool 			SetProperty( void ) override;					// set object's property 
   	

// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif


// globe static data declaration
private :
  static INT32 sm_aulSuitable[2];
  
  static const CString  sm_cstrClassName;          
	static ParaName 			sm_ptrParaName[2];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
// serialize data
	double 			m_eLoLimit;
	double 			m_eHiLimit;	
	bool				m_fVertical;
	bool 				m_fMoveMark;
	bool				m_fShowRange;
	COLORREF		m_clrBar;
	CRect				m_rectFillBar;
	
// no serialize data	
	bool 				m_fPressed;
	bool 				m_fTrackMouseMove;
	bool 				m_fBeginTrack;
	
	double 			m_eMarkPosition;
};                       
