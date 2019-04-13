// setprope.h : header file
//
#pragma once

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGraphProperty dialog

class CDlgSetGraphProperty : public CDlgObject
{
// Construction
public:
	CDlgSetGraphProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetGraphProperty)
	enum { IDD = IDD_OBJECT_GRAPH };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	DWORD	m_ulDataLength;
  DWORD m_ulInputDataType;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( BOOL fDisableSelectType, INT32 ulDataLength, INT32 ulInputType, COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate );
	void GetData( INT32& ulDataLength, INT32& ulInputType, COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate );
  
  BOOL m_fDisableSelectType;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetGraphProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	afx_msg void OnSelchangeComboInputType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

