#ifndef __DLG_OBJECT_RECT_H__
#define __DLG_OBJECT_RECT_H__

#include "..\\resource.h"
#include "cobjectbase.h"
#include "dlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetProperty dialog

class CDlgSetProperty : public CDlgObject
{
// Construction
public:
	CDlgSetProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetProperty)
	enum { IDD = IDD_OBJECT_VIRTUAL_RECT };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif


