#ifndef __DLG_OBJECT_TEXT_H__
#define __DLG_OBJECT_TEXT_H__

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetTextProperty dialog

class CDlgSetTextProperty : public CDlgObject
{
// Construction
public:
	CDlgSetTextProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetTextProperty)
	enum { IDD = IDD_OBJECT_TEXT };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	CString	m_strShowName;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, CString ShowName, double eScanRate );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, CString& ShowName, INT32& ulScanRate );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetTextProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
