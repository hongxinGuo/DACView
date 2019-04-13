// setprope.h : header file
//
#pragma once

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetSliderProperty dialog

class CDlgSetSliderProperty : public CDlgObject
{
// Construction
public:
	CDlgSetSliderProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetSliderProperty)
	enum { IDD = IDD_OBJECT_SLIDER };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	BOOL	m_fVertical;
	double	m_eHiLimit;
	double	m_eLoLimit;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate,
								double eHiLimit, double eLoLimit, bool fVertical );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate,
								double& eHiLimit, double& eLoLimit, bool& fVertical );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetSliderProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

