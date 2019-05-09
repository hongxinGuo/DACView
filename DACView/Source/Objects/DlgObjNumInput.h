#ifndef __DLG_OBJECT_NUMBER_INPUT_H__
#define __DLG_OBJECT_NUMBER_INPUT_H__

#include "..\\resource.h"
#include "cobjectbase.h"
#include "DlgObject.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSetNumberInputProperty dialog

class CDlgSetNumberInputProperty : public CDlgObject
{
// Construction
public:
	CDlgSetNumberInputProperty(CWnd* pParent = nullptr);	// standard constructor
  void Init( CObjectBase * pcobj );
  
// Dialog Data
	//{{AFX_DATA(CDlgSetNumberInputProperty)
	enum { IDD = IDD_OBJECT_NUMBER_INPUT };
	CString		m_ObjectName;
	COLORREF 	m_clrBkGrd;		// background color
  COLORREF 	m_clrForeGrd;	// foreground color 
	double	m_eScanRate;
	//}}AFX_DATA
  
  CString m_strFontName;
  LONG    m_lHeight;
  BOOL    m_fBold;
  BOOL    m_fItalic;
  BOOL    m_fUnderline;
	BOOL    m_fStrikeOut;

// data change function, inline
public:
	void SetData( CString FontName, INT32 lHeight, bool fBold,
    bool fItalic, bool fUnderline, bool fStrikeOut, COLORREF colorFore, COLORREF colorBk, CString Name, double eScanRate );
	void GetData( CString& FontName, INT32& lHeight, bool& fBold,
    bool& fItalic, bool& fUnderline, bool& fStrikeOut, COLORREF &colorFore, COLORREF &colorBk, CString &Name, INT32& ulScanRate );

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
  
	// Generated message map functions
	//{{AFX_MSG(CDlgSetNumberInputProperty)
	virtual BOOL OnInitDialog();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnClickedButtonbackcolor();
	afx_msg void OnClickedButtonforecolor();
	afx_msg void OnButtonChooseFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

