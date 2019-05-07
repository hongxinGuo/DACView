// setprope.h : header file
//
#pragma once

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetComponentProperty dialog

class CDlgSetComponentProperty : public CDlgObject
{
// Construction
public:
	CDlgSetComponentProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetComponentProperty)
	enum { IDD = IDD_OBJECT_COMPOUND };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	CString	m_strBitmap;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, CString Bitmap, double eScanRate );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, CString& Bitmap, INT32& ulScanRate );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetComponentProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//  afx_msg void OnEnChangeMfceditbrowse1();
//  afx_msg void OnBnClickedOk();
  virtual void OnOK();
};

