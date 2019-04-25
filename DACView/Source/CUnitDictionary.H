////////////////////////////////////////////////////////////////////
//
// ´æ´¢Unit×ÖµäµÄ½á¹¹.
// 
// 
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

class CUnitBase;

class CUnitDictionary : public CObject {
public :
	CUnitDictionary( CUnitBase * pcunit, INT64 ulIndex, INT64 ulType ); 
	CUnitDictionary( void );
  
  DECLARE_SERIAL(CUnitDictionary);
  
  ~CUnitDictionary();
// Attributes
  
// Operations
	CUnitBase * GetUnit( void );
	CString GetUnitName( void );
	INT64 GetIndex( void );
	INT64 GetType( void );
	INT64 	GetIndexNumber( void ) { return m_iIndexNumber; }
	void  SetIndexNumber( INT64 index ) { m_iIndexNumber = index; }
	CString GetParaName( void );

// implememtation 
 	virtual void Serialize( CArchive& ar );
  
protected: 
	// assist function

#ifdef _DEBUG
	virtual	void 			AssertValid() const;
	virtual	void 			Dump(CDumpContext& dc) const;
#endif  

protected :
// Serialized Data
	INT64					m_lNumber;       // index of dictionary
	CUnitBase *		m_pCUnit;         // point to unit
	INT64 				m_lIndex;        // unit's index
	INT64					m_lType;					// tMODIFIALBE, tINPUT or tOUTPUT, tDOUBLE tWORD etc.
// Not Serialized Data
	INT64						m_iIndexNumber;   // index of SelectChange

};                        

