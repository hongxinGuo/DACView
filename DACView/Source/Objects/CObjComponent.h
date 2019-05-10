///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CObjectComponent.h
//
// 复合Objects类，内部无法被看进去，内部存储Objects（可包括其他的CObjcompound类），多层结构，可嵌套。
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __OBJECT_COMPONENT_H__
#define __OBJECT_COMPONENT_H__

using namespace std;
#include<memory>

#include "cObjComponentBase.h"
#include "DlgObjComponent.h"

#include "dibapi.h"

class CObjectComponent : public CObjectComponentBase {
public: 
	CObjectComponent( CString s, CRect r);
	CObjectComponent();
	
	DECLARE_SERIAL(CObjectComponent);
	
	~CObjectComponent();
	                
// Attributes
public:
  virtual void Serialize( CArchive& ar ) override; 
  virtual const CString& GetClassNameStr( void ) override;
  
  virtual bool      IsNeedUpdate( void ) override;
  virtual bool      CanInSymbol(void) override { return false; } //部件类对象不允许包含于符号对象中
	

// Operations	 
public:               
	virtual void      ToShowDynamic( CDC * const pdc) override;		// draw myself on memory DC 
	virtual void      ToShowStatic( CDC * const pdc, CPoint ptScrollPosition) override;		// draw myself on DC 
	virtual bool      ExectiveDynLink( void ) override;
	virtual bool      SetProperty( void ) override;	// set object's property 
  
// implementation	  

#ifdef _DEBUG
	virtual	void AssertValid() const override;
	virtual	void Dump(CDumpContext& dc) const override;
#endif

protected:
	// serialize data
	CString 					m_strTitle;
	CString						m_strBitmap;
	
	// noserialize data   
  HBITMAP *         m_hBitmap;
  char *            m_pJpegFile;
  int               m_iFileSize;
  bool              m_fLoadBitmap;

};

typedef shared_ptr<CObjectComponent> CObjectComponentPtr;

#endif