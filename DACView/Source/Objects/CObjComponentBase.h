#pragma once

#include "cObjRectBase.h"

class CObjectComponentBase : public CObjRectBase {
public: 
	CObjectComponentBase( const CString& s, CRect r);
	CObjectComponentBase();
	
	DECLARE_SERIAL(CObjectComponentBase);
	
	~CObjectComponentBase();
	                
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override;
  
  virtual void SetUpdateFlag( bool fFlag ) override;

  virtual bool SetParameterSelected(void) override;


  virtual bool CanViewIn( void ) const override;
  virtual void ViewOut( void );

	virtual bool ExectiveDynLink( void ) override;

  virtual bool CreateUniName( CObjectList& listObject ) override;
  virtual void AddToList( CObjectList& listObject );

// Operations	 
public:               
	void 							SetUpperObjectList( CObjectList * pObjectList );
  CObjectList *     GetUpperObjectList( void );
  void 							SetUpperObjectComponent( CObjectComponentBase * pObjectComponent );
  void							SetUpperScrollPosition( CPoint pt );         
  CPoint						GetUpperScrollPosition( void );
  CObjectComponentBase * GetUpperObjectComponent( void );
  CObjectList *     GetObjectList( void );

	virtual void      ToShowDynamic( CDC * const pdc);		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition);		// draw myself on DC 

// dynamic link virtual function 
	virtual bool 		SetInteger(ULONG index, LONG lValue);

// implementation	  

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

protected:
	// serialize data
	CObjectList 	m_CObjectList;
	
	// noserialize data          
	// ��3���������ڼ�¼���״̬
	CObjectList *         m_pObjectListUpper;				// ���Objects��
	CObjectComponentBase * m_pObjectComponentUpper;		// ����CObjectComponentָ��
	CPoint				  	    m_ptScrollPositionUpper;	// ��㵱ʱ�Ĺ�����λ��

};
