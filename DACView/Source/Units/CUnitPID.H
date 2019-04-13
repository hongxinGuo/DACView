#pragma once

#include "CUnitBase.h"

class CUnitPID : public CUnitBase {
public :
  CUnitPID(const CString& Name, CPoint pt); 
  CUnitPID( void );
  
  DECLARE_SERIAL(CUnitPID);
  
  ~CUnitPID();

// Attributes
public:
  virtual void 		Serialize( CArchive& ar ) override;
  
// Operations
public:          
	virtual const CString&	GetClassNameStr( void ) override;
	virtual CString     GetParaName( ULONG index ) override;
	virtual ULONG		GetParaType( ULONG ulIndex ) override;

	virtual bool 		GetBool( ULONG index) override;
	virtual bool 		SetBool( ULONG index, bool fVaule) override;
	virtual LONG	 	GetInteger(ULONG index) override;
	virtual bool 		SetInteger(ULONG index, LONG iValue) override;
	virtual double 	GetDouble(ULONG index) override;
	virtual bool  	SetDouble(ULONG index, double eValue) override;
	
	virtual INT32   GetIndex( ULONG ulIndex ) override;
	virtual void  	Exective( void ) override;
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual ULONG 	GetDynLinkType( ULONG ulIndex ) override; 
	
  virtual void 		ToShow( CDC * const pdc ) override;		// draw myself an DC 
  virtual bool 		SetProperty( void ) override;					// set object's property 
// implememtation 
	virtual INT32*					GetArrayIndex( void ) override;

  virtual bool      IsParameterLocked(ULONG ulIndex) override;
  // 清除参数被选择索引
  virtual void      ClearParaSelectedFlag(  void ) override;


public : 
	// assist function
 	
#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

// globe class data	
private : 
	static ParaName 			sm_ptrParaName[14];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG 		sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;
  
	static INT32 					sm_aulSuitable[13];
protected :
// Serialized Data
	double m_eHiRange;
	double m_eLoRange;
	double m_eHiLimit;
	double m_eLoLimit;

	double m_eDeriveTime;
	double m_eIntegralTime;
	INT32	 m_ulSampleNumber;
	INT32	 m_ulSampleTime;
	double m_ePB;						 // factor Proportion Band
	
// No Serialized Data
	double m_eMeas;
	double m_eMeas1;
	double m_eSetPoint;
	double m_eOutput;
	double m_eUp, m_eUp1;
	double m_eUi, m_eUi1;
	double m_eUd, m_eUd1;
	double m_eKh, m_eKh1;
	double m_eEkh, m_eEkh1;
	double m_eB;

	bool m_fEnable;
	bool m_fDALM;
};                        


  
