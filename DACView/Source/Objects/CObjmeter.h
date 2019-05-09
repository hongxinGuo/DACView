#ifndef __OBJECT_METER_H__
#define __OBJECT_METER_H__

#include "cObjRectBase.h"

class CObjectMeter : public CObjRectBase {
	public :
	CObjectMeter(const CString& s, CRect r);
	CObjectMeter();

	DECLARE_SERIAL(CObjectMeter);
	
	~CObjectMeter();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;

// Operations
public:
	virtual void      ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool      InIt(POINT const pt, int iStatus) override;
	virtual bool      ExectiveDynLink( void ) override;
	virtual bool      SetProperty( void ) override;					// set object's property 
  
	virtual bool CheckSelf( void ) override;

// dynamic link virtual function 
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual CString		GetParaName( ULONG index ) override;
	virtual INT32			GetIndex( ULONG ulIndex ) override;
	virtual ULONG 		GetDynLinkType( ULONG Index ) override;
	virtual void     	SelectParameter( ULONG ulType ) override; 	
	virtual bool			SetDouble( ULONG ulIndex, double eValue ) override;


// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif


// globe static data declaration
private :
  static INT32  sm_aulSuitable[2];
  
  static const CString  sm_cstrClassName;          
	static ParaName 			sm_ptrParaName[];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
// serialize data
	double 			m_eLowLimit;
	double			m_eHighLimit;
	double			m_eNormalLimit;
	double			m_eWarningLimit;
	double			m_eErrorLimit;
	COLORREF		m_clrNormal;
	COLORREF		m_clrWarn;
	COLORREF		m_clrError;
	COLORREF		m_clrNoddle;
	INT32				m_ulNumberStep;
		
// no serialize data
	double 			m_eValue;

	bool 				m_fTrackMouseMove;
	bool				m_fBeginTrack;

	ULONG				m_ulCircleSize;
};     

#endif

