// setprope.h : header file
//
#pragma once

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetKnobProperty dialog

class CDlgSetKnobProperty : public CDlgObject
{
// Construction
public:
	CDlgSetKnobProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetKnobProperty)
	enum { IDD = IDD_OBJECT_KNOB };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	COLORREF		m_clrNormal;
	COLORREF		m_clrWarn;
	COLORREF		m_clrError;
	COLORREF 		m_clrNoddle;
	double	m_eHiLimit;
	double	m_eLoLimit;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate,
								COLORREF colorNormal, COLORREF colorWarn, COLORREF colorError, COLORREF colorNoddle,
								double eHiLimit, double eLoLimit );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate,
								COLORREF& colorNormal, COLORREF& colorWarn, COLORREF& colorError,	COLORREF& colorNoddle,
								double& eHiLimit, double& eLoLimit );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetKnobProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	afx_msg void OnButtonnormal();
	afx_msg void OnButtonerror();
	afx_msg void OnButtonwarn();
	afx_msg void OnButtonnoddle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
