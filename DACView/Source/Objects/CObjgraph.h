#pragma once

#include "cObjRectBase.h"

class CObjectGraph : public CObjRectBase {
	public :
	CObjectGraph(const CString& s, CRect r);
	CObjectGraph();

	DECLARE_SERIAL(CObjectGraph);
	
	~CObjectGraph();
	
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
	virtual bool  	ExectiveDynLink( void ) override;
	
	
// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif


// class static data declaration
private :
  static INT32  sm_aulSuitable[5];
  
  static const CString  sm_cstrClassName;          
	static ParaName 			sm_ptrParaName[];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
// serialize data
	COLORREF 		m_penColor[4];    // maxium four pens per graph
  INT32       m_ulInputDataType;
	INT32				m_ulDataLength;   // graph length

// no serialize data
	CPen * 			m_ppenGraph[4];
	double *		m_eptrData[4];		// store display data, used by ShowView
	double * 		m_ptrBegin[4];
	ULONG				m_pScrollLength[4];
	
	double 			m_eUpperLimit;
	double 			m_eLowLimit;
	ULONG 			m_ulScrollLength;
  CRect       m_rectGraph;
	
};                       
