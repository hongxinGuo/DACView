#ifndef __UNIT_TTB_H__
#define __UNIT_TTB_H__

#include "CUnitBase.h"

// save current warning state
#pragma warning( push )
// disable warning nonstandard extension used: nameless struct/union
#pragma warning( disable : 4201 )

class CUnitTTB : public CUnitBase {
public :
  CUnitTTB(const CString& Name, CPoint pt); 
  CUnitTTB();
  
  DECLARE_SERIAL(CUnitTTB);
  
  ~CUnitTTB();

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
	static ParaName 			sm_ptrParaName[43];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG 		sm_ulBoolEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;
  
	static INT32 					sm_aulSuitable[42];
protected :
// Serialized Data
	INT32 m_ulNumberInput;
	INT32 m_ulNumberStep;
	BYTE  m_bInitCoil;
	WORD  m_wInitOutput;
	BYTE  m_bCoilPattern[8];
	BYTE  m_bCoilPatternMask[8];
	WORD  m_wInputPattern[8];
	WORD  m_wInputPatternMask[8];
	BYTE  m_bCoilOutput[8];
	BYTE  m_bCoilOutputMask[8];
	WORD  m_wDigitalOutput[8];
	WORD	m_wDigitalOutputMask[8];
	bool  m_fMatchingCriteria[8];
	
// No Serialized Data
	union {
		WORD m_wInput;
		struct {
			bool m_fDI1 :1;
			bool m_fDI2 :1;
			bool m_fDI3 :1;
			bool m_fDI4 :1;
			bool m_fDI5 :1;
			bool m_fDI6 :1;
			bool m_fDI7 :1;
			bool m_fDI8 :1;
			bool m_fDI9 :1;
			bool m_fDI10:1;
			bool m_fDI11:1;
			bool m_fDI12:1;
			bool m_fDI13:1;
			bool m_fDI14:1;
			bool m_fDI15:1;
			bool m_fDI16:1;
		};
	};

	union {
		WORD m_wOutput;
		struct {
			bool m_fDO1 :1;
			bool m_fDO2 :1;
			bool m_fDO3 :1;
			bool m_fDO4 :1;
			bool m_fDO5 :1;
			bool m_fDO6 :1;
			bool m_fDO7 :1;
			bool m_fDO8 :1;
			bool m_fDO9 :1;
			bool m_fDO10:1;
			bool m_fDO11:1;
			bool m_fDO12:1;
			bool m_fDO13:1;
			bool m_fDO14:1;
			bool m_fDO15:1;
			bool m_fDO16:1;
		};
	};

	union {
		BYTE m_bTotalStep;
		struct {
			bool m_fStep1 :1;
			bool m_fStep2 :1;
			bool m_fStep3 :1;
			bool m_fStep4 :1;
			bool m_fStep5 :1;
			bool m_fStep6 :1;
			bool m_fStep7 :1;
			bool m_fStep8 :1;
		};
	};

	BYTE m_bInnerRegister;
	bool m_fEnable;
};                        
// restore warning state
#pragma warning( pop )

#endif
