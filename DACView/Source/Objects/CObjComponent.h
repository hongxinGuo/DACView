///////////////////////////////////////////////////////////////////////////////////////////////////
//
// CObjectComponent.h
//
// ����Objects�࣬�ڲ��޷�������ȥ���ڲ��洢Objects���ɰ���������CObjcompound�ࣩ�����ṹ����Ƕ�ס�
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include<boost/gil/gil_all.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
using namespace boost::gil;

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
  virtual bool      CanInSymbol( void ) override;
	

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
  rgb8_image_t      m_img;
  bool              m_fLoadBitmap;

};