#ifndef __UNIT_DELAY_H__
#define __UNIT_DELAY_H__

#include "CUnitBase.h"

class CUnitDelay : public CUnitBase {
public :
  CUnitDelay(const CString& Name, CPoint pt); 
  CUnitDelay();
  
  DECLARE_SERIAL(CUnitDelay);
  
  ~CUnitDelay();

// Attributes
public:
  virtual void 		Serialize( CArchive& ar ) override;
  
// Operations
public:          
	virtual const CString&	GetClassNameStr( void ) override;
	virtual CString	GetParaName( ULONG index ) override;
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
	static ParaName 			sm_ptrParaName[5];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG 		sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;
  
	static INT32 					sm_aulSuitable[4];
protected :
// Serialized Data
	INT32	m_lDelayOn;
	INT32	m_lDelayOff;
	
// No Serialized Data
	bool 	m_fInput;
	bool 	m_fOutput;

	LONG m_lCountDelayOn;
	LONG m_lCountDelayOff;
	bool m_fCountDelayOn;
	bool m_fCountDelayOff;
	bool m_fInputBefore;

	bool m_fEnable;
};                        

#endif

  
