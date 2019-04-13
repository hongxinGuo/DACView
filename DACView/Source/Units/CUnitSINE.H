#pragma once

#include "cUnitBase.h"

class CUnitSine : public CUnitBase {
public :
  CUnitSine(const CString& Name, CPoint pt); 
  CUnitSine( void );
  
  DECLARE_SERIAL(CUnitSine);
  
  ~CUnitSine();

// Attributes
public:
  virtual void 		Serialize( CArchive& ar ) override;
  
// Operations
public:          
	virtual const CString& GetClassNameStr( void ) override;
	virtual CString     GetParaName( ULONG index ) override;
	virtual ULONG		GetParaType( ULONG ulIndex ) override;

	virtual bool 		GetBool( ULONG index) override;
	virtual bool 		SetBool( ULONG index, bool fVaule) override;
	virtual LONG	 	GetInteger(ULONG index) override;
	virtual bool 		SetInteger(ULONG index, LONG lValue) override;
	virtual double 	GetDouble(ULONG index) override;
	virtual bool  	SetDouble(ULONG index, double eValue) override;
	
	virtual ULONG 	GetUnitType( void ) override;
	virtual void 		SetExectivePriority( ULONG ulPriority ) override;
	virtual INT32   GetIndex( ULONG ulIndex ) override;
	virtual void  	Exective( void ) override;
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual ULONG 	GetDynLinkType( ULONG ulIndex ) override; 
	
  virtual void 		ToShow( CDC * const pdc ) override;		// draw myself an DC 
  virtual bool 		SetProperty( void ) override;					// set object's property 

  virtual bool    DumpCurrentStatus( char * pch ) const;

// implememtation 
	virtual INT32*					GetArrayIndex( void );

  virtual bool      IsParameterLocked(ULONG ulIndex);
  // 清除参数被选择索引
  virtual void      ClearParaSelectedFlag(  void );


public : 
	// assist function
 	
	
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

// globe class data	
private : 
	static ParaName 			sm_ptrParaName[12];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG 		sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;
  
	static INT32  sm_aulSuitable[11];
protected :
// Serialized Data
	double m_eOutput;  // m_eOutput = K0 * sin(K1 + K2* t/(2*3.14)) + K3 
	double m_eK0;
	double m_eK1;
	double m_eK2;
  double m_eK3;
	double m_eHiRange;
	double m_eLoRange;
	double m_eHiLimit;
	double m_eLoLimit;
	
// No Serialized Data 
	ULONG m_ulCounter;
};                        
