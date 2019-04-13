#pragma once

#include "CUnitBase.h"

class CUnitInvert : public CUnitBase {
public :
  CUnitInvert(const CString& Name, CPoint pt); 
  CUnitInvert( void );
  
  DECLARE_SERIAL(CUnitInvert);
  
  ~CUnitInvert();

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
	
	virtual INT32   GetIndex( ULONG ulIndex ) override;
	virtual void  	Exective( void ) override;
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual ULONG 	GetDynLinkType( ULONG ulIndex ) override;
	
  virtual void 		ToShow( CDC * const pdc ) override;		// draw myself an DC 
  virtual bool 		SetProperty( void ) override;					// set object's property 
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
	static ParaName 			sm_ptrParaName[7];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG 		sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;
  
	static INT32 					sm_aulSuitable[6];

protected :
// Serialized Data
	BOOL m_fAlarmHigh;
	BOOL m_fEnableAlarm;
	
// No Serialized Data
	BOOL  m_fInput;
	BOOL  m_fOutput;
	BOOL  m_fAlarm;
	
	BOOL m_fEnable;
	
};                        
