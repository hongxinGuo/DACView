#include "stdafx.h"

using namespace std;
#include<memory>
#include<algorithm>

#include "globedef.h"

#include "cObjComponent.h"

IMPLEMENT_SERIAL(CObjectComponent, CObjectComponentBase, 1 | VERSIONABLE_SCHEMA)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CObjectComponent::CObjectComponent(CString s, CRect r) 
  : CObjectComponentBase(s, r) {
	m_strBitmap = "";
  m_fCreateMemoryDC = false;
  m_fLoadBitmap = false;

}

CObjectComponent::CObjectComponent( void ) : CObjectComponentBase( ) {
  m_fCreateMemoryDC = false;
  m_fLoadBitmap = false;
}
	
CObjectComponent::~CObjectComponent() {
}

bool CObjectComponent::IsNeedUpdate( void ) {
	for (const auto pobj : m_CObjectList) {
		if ( pobj->IsNeedUpdate() ) {
			pobj->SetUpdateFlag(false);
		}
	}  
	return( m_fNeedUpdate );
} 
	               
/////////////////////////////////////////////////////////////////////////////
// CObjectComponent diagnostics

#ifdef _DEBUG
void CObjectComponent::AssertValid() const
{
	CObjectComponentBase::AssertValid();
}

void CObjectComponent::Dump(CDumpContext& dc) const
{
	CObjectComponentBase::Dump(dc);
  dc << "\nm_strBitmap = " << m_strBitmap;
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectComponent member function

void CObjectComponent::Serialize( CArchive& ar ) {
	CObjectComponentBase::Serialize( ar );
  
  if (ar.IsStoring())
  {
    // TODO: add storing code here
		ar << m_strBitmap;
  }
  else {
    // TODO: add loading code here
    switch (ar.GetObjectSchema()) {
    case 1:
    case -1:
      ar >> m_strBitmap;
      break;
    default:
      ShowMessage(IDS_WARN_LOAD_OBJECT_VERSION, "Cpt");
      break;
    }

    if (m_strBitmap.IsEmpty()) return;

    string str;
    str = m_strBitmap;
    jpeg_read_image(str, m_img);
    m_fLoadBitmap = true;
  }
} 

const CString& CObjectComponent::GetClassNameStr( void ) {
  static CString s_strClassName = "Cpt";

  return(s_strClassName);
}

void CObjectComponent::ToShowStatic( CDC * const pdc, CPoint  ) {
  CRect rectArea = m_rectArea;
  const CPoint pt = GetOffset();

  rectArea += pt;
  if ( pdc->RectVisible(rectArea) ) {  	// if I need to redraw ?
		if (m_fLoadBitmap) {
      
		}
		else {
  		CBrush cbb, *pcb;
  		CPen cpf, * pcp;
			cpf.CreatePen( PS_SOLID, 1, m_clrForeGrd );
  		cbb.CreateSolidBrush( m_clrBkGrd );
			pcp = pdc->SelectObject( &cpf );
			pcb = pdc->SelectObject( &cbb );
			pdc->Rectangle( rectArea );
			pdc->SelectObject( pcb );
			pdc->SelectObject( pcp ); 
		}
  }
	if ( m_fSelected ) SetFocus( pdc );
}  

void CObjectComponent::ToShowDynamic( CDC * const pdc ) {
  CRect rectArea = m_rectArea;

  m_rectLastTime = m_rectArea;
	if (m_fLoadBitmap) {
	}
	else {
  	CBrush cbb, *pcb;
  	CPen cpf, * pcp;
		cpf.CreatePen( PS_SOLID, 1, m_clrForeGrd );
  	cbb.CreateSolidBrush( m_clrBkGrd );
		pcp = pdc->SelectObject( &cpf );
		pcb = pdc->SelectObject( &cbb );
		pdc->Rectangle( rectArea );
		pdc->SelectObject( pcb );
		pdc->SelectObject( pcp ); 
	}
	m_fDrawAll = false;
  m_fNeedUpdate = false;
}

bool CObjectComponent::ExectiveDynLink( ) {
  return( CObjectComponentBase::ExectiveDynLink() );
}

bool CObjectComponent::SetProperty( void ) {
	CDlgSetComponentProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strBitmap, m_lScanRate );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strBitmap, m_lScanRate );

    string str;
    str = m_strBitmap;
    jpeg_read_image(str, m_img);
    m_fLoadBitmap = true;

    return(false);
  }
  return(false);
}

/////////////////////////////////////////////////////////////////////////
//
// compound can't in symbol
// so return FALSE.
//
//////////////////////////////////////////////////////////////////////////
bool CObjectComponent::CanInSymbol( void ) {
  return(false);
}
