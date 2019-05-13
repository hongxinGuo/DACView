//////////////////////////////////////////////////////////////////////////
//
//	CUnitBase class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo 
//	Create Time									: 2001, 2, 27
//
//
///////////////////////////////////////////////////////////////////////////
#ifndef __OBJECTBASE_H__
#define __OBJECTBASE_H__

#include "typedef.h"
#include "CObjectPrimitive.h"

#include "cObjDynLink.h"

class CObjectSymbol;

class CObjectBase : public CObjectPrimitive {
public :
  CObjectBase(const CString& Name, COLORREF BkGrd = RGB(0, 0, 0), COLORREF ForeGrd = RGB(255, 255, 255)); 
  CObjectBase();
  
  DECLARE_SERIAL(CObjectBase);
  
  ~CObjectBase();
// Attributes
	// �ܷ񱻷��Ű�������Щ�ܣ��糤���εȣ�����Щ���ܣ��簴ť���������ȣ�
  virtual bool            CanInSymbol(void) { return true; }
	// �Ƿ��ǳ����� 
	virtual bool            IsRectShape( void )  { ASSERT(false); return(true); }
	// �Ƿ�͸��  
	virtual bool            IsTransparent( void ) { return(m_fTransparent); }
	// ����͸����־ 
  virtual void            SetTransparentFlag(bool fFlag) { m_fTransparent = fFlag; }

// Operations
public:          
	// �õ�������ĵ�ǰ��С
	virtual const CRect&		GetSize( void );			
	// �õ��������ԭ��С
	virtual const CRect&		GetOriginSize( void );
	// �õ���һ����ʾʱ�Ĵ�С
	virtual const CRect&		GetLastSize( void );

	//�õ�������
	virtual CRgn *					GetClipRgn( const CPoint& ) { ASSERT(false); return(&m_rgnClip); }
  CODLList*  							GetDynLinkList(void) { return &m_listDynLink; }
  COLORREF 								GetBkGrdColor(void) const { return m_clrBkGrd; }
  COLORREF 								GetForeGrdColor(void) const { return m_clrForeGrd; }
	
	// �������е�����
	virtual void 			SetAllSize( const CRect& ) { ASSERT(false); }
  virtual void      SetOriginSize( const CRect& ) { ASSERT(false); }
	// ���ö�̬�����С
	virtual void			SetDynamicSize( const CRect&  ) { ASSERT(false); }

  void							SetBkGrdColor(COLORREF color) { m_clrBkGrd = color; }
  void							SetForeGrdColor(COLORREF color) { m_clrForeGrd = color; }
	
	// ���ð���������ķ���ָ��
  void              SetSymbolThatHaveMe( CObjectSymbol * pSymbol ) { m_pSymbolHaveMe = pSymbol; }
  CObjectSymbol *   GetSymbolThatHaveMe(void) { return m_pSymbolHaveMe; }

  virtual bool      SetParameterSelected(void);

	virtual bool			CheckSelf( void ) override;

  virtual bool      CreateUniName( CObjectList& listObject );
  virtual void      AddToList( CObjectList& listObject ) { listObject.push_back(this); }

	// ���öԻ�������̬����
	bool 				 			IsDeleteDynLink( void );
	// ɾ����̬����
	virtual bool			DeleteDynLink( CUnitBase * pUnit );

	virtual bool      IsDrawAll(void) { return m_fDrawAll; }
  virtual void      SetDrawAll( bool fFlag ) { m_fDrawAll = fFlag; }

  virtual bool      NeedShowCaret( void ) { return( m_fNeedShowCaret ); }
  virtual bool      AdjustCaretPos( CDC * , CPoint& ) { ASSERT(false); return false; }

  virtual CPoint    GetOffset(void);

  virtual bool      InIt(POINT const , int ) { ASSERT(false); return false; }
  virtual bool      ProcessChar( UINT , CPoint& ) { ASSERT(false); return false; }
	virtual void      ToShowDynamic( CDC * const ) { ASSERT(false); }			// draw myself on memory DC 
  virtual void      ToShowStatic(CDC * const , CPoint ) { ASSERT(false); }		// draw myself on DC 
  virtual bool 			SetProperty(void) { ASSERT(false); return false; }					// set object's property 

  
// dynamic link virtual function 
// below function should difinition virtually in CObjectBase
	virtual bool 			SetRectPosition(ULONG , LONG ) { ASSERT(false); return false; }
  virtual bool      SetBool(ULONG , bool ) { ASSERT(false); return false; }
  virtual bool 			SetInteger(ULONG , LONG ) { ASSERT(false); return false; }
  virtual bool  		SetDouble(ULONG , double ) { ASSERT(false); return false; }
  virtual double		GetDouble(ULONG ) { ASSERT(false); return 0.0; }
  virtual bool	 		SetString(ULONG , const CString& ) { ASSERT(false); return false; }
	virtual bool 			SetColor( ULONG , LONG ) { ASSERT(false); return false; }

	virtual bool  		ExectiveDynLink( void ) { ASSERT(false); return false; }		// ִ�ж�̬����
	
	// ����̬���ӵĺ���
  virtual ParaName* GetParaNameAddress(void) { return nullptr; }
  virtual CString		GetParaName(ULONG ) { return(""); }
  virtual INT32			GetIndex(ULONG ) { ASSERT(false); return 0; }
	virtual ULONG 		GetDynLinkType( ULONG  ) { ASSERT(false); return 0; }
	virtual bool 			SetParameterLock(ULONG , bool ) { ASSERT(false); return false; }
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected) { m_vfSelected[ulIndex] = fSelected; return true; }

  virtual void     	SelectParameter(ULONG ) { ASSERT(false); }

// implememtation 
 	virtual void Serialize( CArchive& ar );
  
	virtual void 			AdjustInnerSize( void ) { ASSERT(false); }					//�����ڲ�Objects��С��CObjSymbolʹ��
	virtual void 			AdjustDynamicInnerSize( void ) { ASSERT(false); }		// ��̬�����ڲ�Objects��С��CObjSymbolִ��ʱʹ�á�

protected:
  virtual void 			SetFocus(CDC * ) { ASSERT(false); }	 // this object is on focus

private :
	virtual void			ToShow(CDC * const ) { ASSERT(false); }
  
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

typedef shared_ptr<CObjectBase> ObjectPtr;

#endif
  
