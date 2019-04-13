#include "stdafx.h"

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
	m_hDIB = nullptr;
	m_palDIB = nullptr;
  m_fCreateMemoryDC = FALSE;

}

CObjectComponent::CObjectComponent( void ) : CObjectComponentBase( ) {
	m_hDIB = nullptr;
	m_palDIB = nullptr;
  m_fCreateMemoryDC = FALSE;
}
	
CObjectComponent::~CObjectComponent() {
	if (m_hDIB != nullptr)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	if (m_palDIB != nullptr)
	{
		delete m_palDIB;
    m_palDIB = nullptr;
	}
}

bool CObjectComponent::IsNeedUpdate( void ) {
	POSITION Po = m_CObjectList.GetHeadPosition();
	CObjectBase * pcobjTemp;

	for ( int i = 0; i < m_CObjectList.GetCount(); i++ ) {
		pcobjTemp = m_CObjectList.GetNext(Po);
		if ( pcobjTemp->IsNeedUpdate() ) {
			pcobjTemp->SetUpdateFlag( FALSE );
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
    switch ( ar.GetObjectSchema() ) {
    case 1 :
    case -1 :
		  ar >> m_strBitmap;
      break;
    default :
      ShowMessage(IDS_WARN_LOAD_OBJECT_VERSION, "Cpt");
      break;
    }
    
		if ( m_strBitmap.IsEmpty() ) return;

		CFile file;
		CFileException fe;
		if (!file.Open(m_strBitmap, CFile::modeRead | CFile::shareDenyWrite, &fe))
		{
			ShowMessage(ID_ERROR_FIND_FILE, (LPCTSTR)m_strName, (LPCTSTR)m_strBitmap);
			return;
		}
    TRY
		{
			m_hDIB = ReadDIBFile(file);
		}
		CATCH (CFileException, eLoad)
		{
			file.Abort(); // will not throw an exception
			m_hDIB = nullptr;
			ShowMessage(ID_ERROR_READ_FILE, (LPCTSTR)m_strBitmap);
			return;
		}
		END_CATCH
		if (m_palDIB != nullptr)
		{
			delete m_palDIB;
			m_palDIB = nullptr;
		}
		if (m_hDIB == nullptr)
		{
			return;
		}
		// Create copy of palette
		m_palDIB = new CPalette;
		if (m_palDIB == nullptr)
		{
			// we must be really low on memory
			::GlobalFree((HGLOBAL) m_hDIB);
			m_hDIB = nullptr;
			ShowMessage(ID_ERROR_OUT_OF_MEMORY, "");
			return;
		}
		if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
		{
			// DIB may not have a palette
			delete m_palDIB;
			m_palDIB = nullptr;
			return;
		}   
  } 
} 

const CString& CObjectComponent::GetClassNameStr( void ) {
  static CString s_strClassName = "Cpt";

  return(s_strClassName);
}

void CObjectComponent::ToShowStatic( CDC * const pdc, CPoint  ) {
  CRect rectArea = m_rectArea;
  CPoint pt = GetOffset();

  rectArea += pt;
  if ( pdc->RectVisible(rectArea) ) {  	// if I need to redraw ?
		if (m_hDIB != nullptr) {
			LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
			int cxDIB = (int) DIBWidth(lpDIB);         // Size of DIB - x
			int cyDIB = (int) DIBHeight(lpDIB);        // Size of DIB - y
			::GlobalUnlock((HGLOBAL) m_hDIB);
			CRect rcDIB;
			rcDIB.top = 0;
			rcDIB.left = 0;
			rcDIB.right = cxDIB;
			rcDIB.bottom = cyDIB;
			PaintDIB(pdc->GetSafeHdc(), &rectArea, m_hDIB,
				&rcDIB, m_palDIB);
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
	if (m_hDIB != nullptr) {
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
		int cxDIB = (int) DIBWidth(lpDIB);         // Size of DIB - x
		int cyDIB = (int) DIBHeight(lpDIB);        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) m_hDIB);
		CRect rcDIB;
		rcDIB.top = 0;
		rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		PaintDIB(pdc->GetSafeHdc(), &rectArea, m_hDIB,
			&rcDIB, m_palDIB);
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
	m_fDrawAll = FALSE;
  m_fNeedUpdate = FALSE;
}

bool CObjectComponent::ExectiveDynLink( ) {
  return( CObjectComponentBase::ExectiveDynLink() );
}

bool CObjectComponent::SetProperty( void ) {
	CDlgSetComponentProperty CDlg;
  CDlg.SetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strBitmap, m_lScanRate );
  if ( CDlg.DoModal() == IDOK ) {
    CDlg.GetData( m_clrForeGrd, m_clrBkGrd, m_strName, m_strBitmap, m_lScanRate );

	  CFile file;
	  CFileException fe;
	  if (!file.Open(m_strBitmap, CFile::modeRead | CFile::shareDenyWrite, &fe)) {
		  return( FALSE );
	  }
    TRY {
		  m_hDIB = ReadDIBFile(file);
	  }
	  CATCH (CFileException, eLoad) {
		  file.Abort(); // will not throw an exception
		  m_hDIB = nullptr;
		  return( FALSE );
	  }
	  END_CATCH 
	
	  if (m_palDIB != nullptr) {
		  delete m_palDIB;
		  m_palDIB = nullptr;
	  }
	  if (m_hDIB == nullptr) {
		  return( FALSE );
	  }
	  // Create copy of palette
	  m_palDIB = new CPalette;
	  if (m_palDIB == nullptr) {
		// we must be really low on memory
		  ::GlobalFree((HGLOBAL) m_hDIB);
		  m_hDIB = nullptr;
		  return( FALSE );
	  }
	  if (CreateDIBPalette(m_hDIB, m_palDIB) == NULL) {
		// DIB may not have a palette
		  delete m_palDIB;
		  m_palDIB = nullptr;
		  return( FALSE );
	  }
    return( TRUE );
  }
  return( FALSE );
}

/////////////////////////////////////////////////////////////////////////
//
// compound can't in symbol
// so return FALSE.
//
//////////////////////////////////////////////////////////////////////////
bool CObjectComponent::CanInSymbol( void ) {
  return( FALSE );
}
