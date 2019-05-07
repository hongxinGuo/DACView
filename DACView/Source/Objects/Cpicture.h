#ifndef __OBJECT_PICTURE_H__
#define __OBJECT_PICTURE_H__

#include "cObjRectBase.h"

class CObjectPicture : public CRectVirtual {
	public :
	CObjectPicture(const CString& s, CRect r);
	CObjectPicture();

	DECLARE_SERIAL(CObjectPicture);
	
	~CObjectPicture();
	
// Attributes
public:
  virtual void Serialize( CArchive& ar ); 
  virtual const CString& GetClassNameStr( void ) override;

// Operations
public:
  virtual void ToShowDynamic( CDC * const pdc, CPoint ptOffset, CPoint ptScrollPosition);		// draw myself on DC 
	virtual void ToShowStatic( CDC * const pdc, CPoint ptOffset, CPoint ptScrollPosition);		// draw myself on DC 
	virtual BOOL ExectiveLink( void );
	virtual BOOL SetProperty( void );					// set object's property 
  
	virtual BOOL CheckSelf( void );

// dynamic link virtual function 
	virtual ParaName* GetParaNameAddress( void );
	virtual char *		GetParaName( ULONG index );
	virtual ULONG			GetIndex( ULONG ulIndex );
	virtual ULONG 		GetDynLinkType( ULONG Index );
	virtual void     	SelectParameter( ULONG ulType ); 	
	virtual BOOL			SetDouble( ULONG ulIndex, double eValue );


// implementation 

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif


// globe static data declaration
private :
  static ULONG sm_ulArrayIndex[6];
  
  static const CString  sm_cstrClassName;          
	static ParaName 			sm_ptrParaName[];
	static const ULONG    sm_ulDoubleEnd;
	static const ULONG    sm_ulWordEnd;
	static const ULONG    sm_ulStringEnd;

protected:
// serialize data
	CString     m_strBitmapName;	

// no serialize data
	CDC 				m_MemoryDC;
	CBitmap			m_Bitmap;
	BOOL 				m_fCreateMemoryDC;
};                       

#endif
