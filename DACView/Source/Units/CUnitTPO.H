#pragma once

#include "CUnitBase.h"

class CUnitTPO : public CUnitBase {
public :
  CUnitTPO(const CString& Name, CPoint pt); 
  CUnitTPO();
  
  DECLARE_SERIAL(CUnitTPO);
  
  ~CUnitTPO();

// Attributes
public:
  virtual void 		Serialize( CArchive& ar ) override;
  
// Operations
public:          
	virtual const CString&	GetClassName( void ) override;
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
	static ParaName 			sm_ptrParaName[9];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG 		sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;
  
	static INT32 					sm_aulSuitable[8];
protected :
// Serialized Data
	double m_eHiRange;
	double m_eLoRange;
	double m_eHiLimit;
	double m_eLoLimit;
	
	INT32  m_ulPeriodTime;
	
// No Serialized Data
	double m_eInput;
	bool 	 m_fOutput;

	ULONG  m_ulCountHighTime, m_ulHighTime;
	ULONG	 m_ulCountLowTime, m_ulLowTime;
	
	bool 	 m_fEnable;
	bool 	 m_fDo;
	
};                        

  
