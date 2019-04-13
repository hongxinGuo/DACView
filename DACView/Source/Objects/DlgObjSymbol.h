// setprope.h : header file
//
#pragma once

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetSymbolProperty dialog

class CDlgSetSymbolProperty : public CDlgObject
{
// Construction
public:
	CDlgSetSymbolProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetSymbolProperty)
	enum { IDD = IDD_OBJECT_SYMBOL };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	double	m_eScanRate;
	BOOL	m_fTransparent;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate, bool transparent );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate, bool& transparent );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetSymbolProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

