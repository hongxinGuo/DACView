//////////////////////////////////////////////////////////////////////////
//
//	CUnitBase class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo 
//	Create Time									: 2001, 2, 27
//
//
///////////////////////////////////////////////////////////////////////////
#pragma once

using namespace std;
#include<vector>

#include "typedef.h"
#include "CObjectPrimitive.h"

#include "cObjDynLink.h"

class CObjectBase;
class CUnitBase;
typedef CList<CObjectBase *, CObjectBase *> CObjectList;
typedef vector<CObjectBase *> CObjectVector;
typedef CList<CUnitBase *, CUnitBase *> CUnitList;

class CObjectSymbol;

class CObjectBase : public CObjectPrimitive {
public :
  CObjectBase(const CString& Name, COLORREF BkGrd = RGB(0, 0, 0), COLORREF ForeGrd = RGB(255, 255, 255)); 
  CObjectBase( void );
  
  DECLARE_SERIAL(CObjectBase);
  
  ~CObjectBase();
// Attributes
	// �ܷ񱻷��Ű�������Щ�ܣ��糤���εȣ�����Щ���ܣ��簴ť���������ȣ�
  virtual bool            CanInSymbol( void );

	// �Ƿ��ǳ����� 
	virtual bool            IsRectShape( void );
	// �Ƿ�͸��  
	virtual bool            IsTransparent( void );
	// ����͸����־ 
	virtual void            SetTransparentFlag( bool fFlag );	


// Operations
public:          
	// �õ�������ĵ�ǰ��С
	virtual const CRect&		GetSize( void );			
	// �õ��������ԭ��С
	virtual const CRect&		GetOriginSize( void );
	// �õ���һ����ʾʱ�Ĵ�С
	virtual const CRect&		GetLastSize( void );

	//�õ�������
	virtual CRgn *					GetClipRgn( const CPoint& ptScrollPosition);
	CODLList*  							GetDynLinkList( void ); 
	COLORREF 								GetBkGrdColor( void ) const ;
	COLORREF 								GetForeGrdColor( void ) const ; 
	
	// �������е�����
	virtual void 			SetAllSize( const CRect& rectArea );
  virtual void      SetOriginSize( const CRect& rectOrigin );
	// ���ö�̬�����С
	virtual void			SetDynamicSize( const CRect& rectArea );

	void							SetBkGrdColor( COLORREF color );
	void							SetForeGrdColor( COLORREF color );
	
	// ���ð���������ķ���ָ��
  void              SetSymbolThatHaveMe( CObjectSymbol * pcSymbol );
  CObjectSymbol *   GetSymbolThatHaveMe( void );

  virtual bool      SetParameterSelected(void);

	virtual bool			CheckSelf( void ) override;

  virtual bool      CreateUniName( CObjectList& listObject );
  virtual void      AddToList( CObjectList& listObject );

	// ���öԻ�������̬����
	bool 				 			IsDeleteDynLink( void );
	// ɾ����̬����
	virtual bool			DeleteDynLink( CUnitBase * pUnit );

	virtual bool      IsDrawAll(void) { return m_fDrawAll; }
  virtual void      SetDrawAll( bool fFlag ) { m_fDrawAll = fFlag; }

  virtual bool      NeedShowCaret( void ) { return( m_fNeedShowCaret ); }
  virtual bool      AdjustCaretPos( CDC *pdc, CPoint& pt );

  virtual CPoint    GetOffset( void );

	virtual bool      InIt( CDC * const pdc, POINT const pt, int iStatus );
  virtual bool      ProcessChar( UINT nChar, CPoint& pt );
	virtual void      ToShowDynamic( CDC * const pdc);		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition);		// draw myself on DC 
  virtual bool 			SetProperty( void );					// set object's property 

  
// dynamic link virtual function 
// below function should difinition virtually in CObjectBase
	virtual bool 			SetRectPosition(ULONG index, LONG lValue);
  virtual bool      SetBool(ULONG index, bool fValue);
	virtual bool 			SetInteger(ULONG index, LONG lValue);
	virtual bool  		SetDouble(ULONG index, double eValue);
	virtual double		GetDouble(ULONG index);
	virtual bool	 		SetString(ULONG index, const CString& strValue); 
	virtual bool 			SetColor( ULONG index, LONG lValue);

	virtual bool  		ExectiveDynLink( void );		// ִ�ж�̬����
	
	// ����̬���ӵĺ���
	virtual ParaName* GetParaNameAddress( void );
	virtual CString		GetParaName( ULONG index );
	virtual INT32			GetIndex( ULONG ulIndex );
	virtual ULONG 		GetDynLinkType( ULONG Index );
	virtual bool 			SetParameterLock( ULONG ulIndex, bool fSelected );
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected);
	virtual void     	SelectParameter( ULONG ulType ); 

// implememtation 
 	virtual void Serialize( CArchive& ar );
  
	virtual void 			AdjustInnerSize( void );				//�����ڲ�Objects��С��CObjSymbolʹ��
	virtual void 			AdjustDynamicInnerSize( void );	// ��̬�����ڲ�Objects��С��CObjSymbolִ��ʱʹ�á�

protected:
  virtual void 			SetFocus(CDC *pdc);	 // this object is on focus

private :
	virtual void			ToShow( CDC * const pdc );
  
public: 
	// assist function

#ifdef _DEBUG
	virtual	void 			AssertValid() const;
	virtual	void 			Dump(CDumpContext& dc) const;
#endif  

// globe static data declaration
protected :

protected :
// Serialized Data
  CODLList 					m_listDynLink;	// �������������Ķ�̬����(�ӵ�Ԫ����������)
  COLORREF 					m_clrBkGrd;			// ����ɫ
  COLORREF 					m_clrForeGrd;		// ǰ��ɫ
  bool              m_fTransparent; // �Ƿ�͸��
  INT64             m_iReserved1;   // ��������
  INT64             m_iReserved2;   // ��������
  INT64             m_iReserved3;   // ��������
  INT64             m_iReserved4;   // ��������

// Not Serialized Data
	bool 							m_fChangeSize;		// �����С�ı��־
	bool              m_fDrawAll;				// ���»���־��
	bool              m_fNeedShowCaret;	// ��ʾ����־

	CRgn							m_rgnClip;				// ������

  CObjectSymbol *   m_pSymbolHaveMe;	// ����������ķ���ָ��
									
// Dynamic changed data	
	CString  					dm_strName; 		// this object's name, used for ID
  COLORREF 					dm_clrBkGrd;		// ��̬�仯����ɫ
  COLORREF 					dm_clrForeGrd;	// ��̬�仯ǰ��ɫ

  
	// ��ʾ�������������
  CDC 				m_MemoryDC;						// 
	CBitmap			m_Bitmap;							// 
	bool 				m_fCreateMemoryDC;		//

};                        

  
