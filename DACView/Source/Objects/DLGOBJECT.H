#ifndef __DLG_OBJECT__H__
#define __DLG_OBJECT__H__

class CDlgObject : public CDialog
{
// Construction
public:
	CDlgObject(UINT IDD, CWnd* pParent = nullptr);   // standard constructor

  void DrawColor( LPDRAWITEMSTRUCT lpDraw, COLORREF clr );
  COLORREF ChooseColorRef( COLORREF clr );

// Dialog Data
	//{{AFX_DATA(CDlgObject)
	enum { IDD = IDD_UNIT_BASE_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

protected :
	UINT_PTR		m_ulTimerId;
	BOOL				m_fDynUpdate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObject)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgObject)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
