#pragma once

#include "CUnitBase.h"

class CUnitFFLP : public CUnitBase {
public :
  CUnitFFLP(const CString& Name, CPoint pt); 
  CUnitFFLP( void );
  
  DECLARE_SERIAL(CUnitFFLP);
  
  ~CUnitFFLP();

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
	
	virtual INT32		GetIndex( ULONG ulIndex ) override;
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
	static ParaName 			sm_ptrParaName[8];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG 		sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;
  
	static INT32 					sm_aulSuitable[7];
protected :
// Serialized Data
	bool m_fEnableAlarm;
	bool m_fAlarmHigh;
	
// No Serialized Data
	bool m_fInputStatus;
	bool m_fInputFlip;
	bool m_fInputFlipBefore;    
	bool m_fOutput;
	bool m_fAlarm;

	bool m_fEnable;
	
};             

  
