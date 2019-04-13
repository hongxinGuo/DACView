// setprope.h : header file
//
#pragma once

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetGaugeProperty dialog

class CDlgSetGaugeProperty : public CDlgObject
{
// Construction
public:
	CDlgSetGaugeProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetGaugeProperty)
	enum { IDD = IDD_OBJECT_GAUGE };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	BOOL			m_fSaveToFile;
	DWORD			m_ulAngle;
	ULONG			m_ulType;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( bool fSaveFlag, INT32 ulAngle, INT32 ulType, COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate );
	void GetData( bool &fSaveFlag, INT32 &ulAngle, INT32&ulType, COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetGaugeProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	afx_msg void OnCheckRecord();
	afx_msg void OnSelchangeComboType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeObjectname();
};
