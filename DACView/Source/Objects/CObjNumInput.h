#pragma once

#include "cObjRectBase.h"

class CObjectNumberInput : public CObjRectBase {
	public :
	CObjectNumberInput(const CString& s, CRect r);
	CObjectNumberInput();

	DECLARE_SERIAL(CObjectNumberInput);
	
	~CObjectNumberInput();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;
  virtual bool      CanInSymbol( void ) override;
 
// Operations
public:
	virtual void      ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool      ExectiveDynLink( void ) override;
	virtual bool      InIt(POINT const pt, int iStatus) override;
  virtual bool      ProcessChar( UINT nChar, CPoint& pt ) override;
  virtual bool      AdjustCaretPos( CDC * pdc, CPoint& pt ) override;

  virtual bool      SetProperty( void ) override;					// set object's property 
  virtual bool      CheckSelf( void ) override;

// dynamic link virtual function 
	virtual ParaName* GetParaNameAddress( void ) override;
	virtual CString		GetParaName( ULONG index ) override;
	virtual INT32			GetIndex( ULONG ulIndex ) override;
	virtual ULONG 		GetDynLinkType( ULONG Index ) override;
	virtual void     	SelectParameter( ULONG ulType ) override; 	

// implementation 
private :
	virtual void      ToShow( CDC * const pdc ) override;

public :
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
  CString     m_strFontName;
  INT32        m_lHeight;
  bool        m_fBold;
  bool        m_fItalic;
  bool        m_fUnderline;
  bool        m_fStrikeOut;
  double			m_eHighLimit;
	double			m_eLowLimit;
	double			m_eStep;
		
// no serialize data
	double			m_eValue;

  ULONG       m_ulCurrentInsertPosition;
  char        m_pchChar[64];
  ULONG       m_ulTotalChar;
	CFont				m_font;

};                       
