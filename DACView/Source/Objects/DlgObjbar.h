#pragma once

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetFillBarProperty dialog

class CDlgSetFillBarProperty : public CDlgObject
{
// Construction
public:
	CDlgSetFillBarProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetFillBarProperty)
	enum { IDD = IDD_OBJECT_FILLBAR };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	COLORREF	m_clrBar;
	BOOL			m_fVertical;
	BOOL  		m_fMoveMark;
	double		m_eHiLimit;
	double		m_eLoLimit;
	BOOL			m_fShowRange;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, 
								COLORREF clrBar, double eScanRate, BOOL fMask, BOOL fShowRange, 
								double eHiLimit, double eLoLimit, BOOL fVertical );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, 
								COLORREF& clrBar, INT32& ulScanRate, bool& fMask, bool& fShowRange, 
								double& eHiLimit, double& eLoLimit, bool& fVertical );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetFillBarProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnButtonBarColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
