#ifndef __DLG_OBJECT_BUTTON_H__
#define __DLG_OBJECT_BUTTON_H__

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetButtonProperty dialog

class CDlgSetButtonProperty : public CDlgObject
{
// Construction
public:
	CDlgSetButtonProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetButtonProperty)
	enum { IDD = IDD_OBJECT_BUTTON };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	CString	m_strOff;
	CString	m_strOn;
	double	m_eScanRate;
	//}}AFX_DATA

// data change function, inline
public:
	void SetData( COLORREF colorFore, COLORREF colorBk, CString Name, CString strOn, CString strOff, double ulScanRate );
	void GetData( COLORREF &colorFore, COLORREF &colorBk, CString &Name, CString& strOn, CString& strOff, INT32& ulScanRate );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDlgSetButtonProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif


