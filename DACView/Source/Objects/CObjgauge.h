#pragma once

#include "cObjRectBase.h"

enum {	TY_GAUGE              = 0,    // gauge type 
 	      TY_FILLVERTICAL 	    = 1,    // vertical fill bar type
 	      TY_FILLHORIZON        = 2,    // horizon fill bar type
 	      TY_END                = 3, 
 	   };

class CObjectGauge : public CObjRectBase {
	public :
	CObjectGauge(const CString& s, CRect r);
	CObjectGauge();

	DECLARE_SERIAL(CObjectGauge);
	
	~CObjectGauge();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassName( void ) override;
  
// Operations
public:
	virtual void      ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual CString		GetParaName( ULONG index ) override;
	virtual INT32			GetIndex( ULONG ulIndex ) override;
	virtual ULONG 		GetDynLinkType( ULONG Index ) override;
	virtual void     	SelectParameter( ULONG ulType ) override; 	

	virtual bool 			SetProperty( void ) override;					// set object's property 
  virtual bool			CheckSelf( void ) override;

// dynamic link virtual function 
	virtual bool  	SetDouble(ULONG index, double eValue) override;
	virtual bool 		SetColor(ULONG index, LONG lValue) override;
	virtual bool  	ExectiveDynLink( void ) override;
	
#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

// implementation 
private :
	virtual void    ToShow( CDC * const pdc ) override;

// globe static data declaration
private :
  static INT32  sm_aulSuitable[4];
  
  static const CString  sm_cstrClassName;          
	static ParaName 			sm_ptrParaName[4];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
// serialize data
	INT32				m_ulGaugeType;  // gauge or fill bar.
	INT32				m_ulAngle;			// used by TY_GAUGE only
	bool				m_fSaveToFile;

// no serialize data
	double			m_eValue;
	double 			m_eUpperLimit;
	double 			m_eLowLimit;
};                       
