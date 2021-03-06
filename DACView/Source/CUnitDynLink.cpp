////////////////////////////////////////////////////////////////////////
//
// ToShow()							: Show dynamic link on DC.
//
// GetSrcUnit()			: 
// GetDestUnit()				:
// GetDestIndex()				:
// GetSrcIndex()			:
// GetType()						: Get dynamic link's type.
// GetLinkPOintList()		:
// SetDeleteMeFlag()		:
// IsDeleteMe()					:
// IsSetLoopDetectFlag():
// SetLoopDetectFlag()	:
// SetDynLinkType()						:
//
// GetDynLinkClass()		:
// SetDynLinkClass()		:
// SetSrcUnit()		:
// SetDestUnit()			:
// SetSrcIndex()			:
// SetDestIndex()				:
// SetLinkPointList()		:
//
////////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "..\\resource.h"
#include "cUnitBase.h"
#include "cUnitDynLink.h"

#include "globedef.h"       

IMPLEMENT_SERIAL(CUnitDynLink, CObjectPrimitive, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

static ParaName Type[] = { {"Bool", tBOOL, 0 },
                           {"Word", tWORD, 1 },
                           {"Double", tDOUBLE, 2 },
                           {"String", tSTRING, 3 },
                         };
static CString Class[] = { "UNIT_TO_UNIT",
                          "UNIT_TO_COMPONENT",
                          "COMPONENT_TO_UNIT",
                          "COMPONENT_TO_COMPONENT",
                        };

CUnitDynLink::CUnitDynLink( void ) : CObjectPrimitive() { 
  m_strName.Empty();
	m_lScanRate = 100000000;
  m_ulDynLinkClass = 0;
  m_ulDynLinkType = 0;
	m_pDestUnit = nullptr;
  m_pSrcUnit = nullptr;
  m_lSrcIndex = -1;   // 索引位置不能为负值，故设置初始值为-1
  m_lDestIndex = -1;     // 索引位置不能为负值，故设置初始值为-1
  m_fDeleteMe = FALSE;
  m_fLoopDetect = FALSE;
  m_plistLinkPoint = make_shared<CPointList>();

  mTest_pDestUnitSaved = nullptr;
}


CUnitDynLink::~CUnitDynLink() {
  m_plistLinkPoint->clear();
} 

/////////////////////////////////////////////////////////////////////////////
//
//  Save or Retrieve Dynamic Link List.
//
// <<和>>的实现模式在某些时候不像看上去那样。如下面的 ar << m_pSrcUnit,并不是存储了一个CUnitBase类的指针，
// 而是调用其实际类的Serialize,故而实际上会存储很多内容。
//
//
/////////////////////////////////////////////////////////////////////////////
void CUnitDynLink::Serialize( CArchive& ar ) {
  CObjectPrimitive::Serialize( ar );

  INT64 iCount = m_plistLinkPoint->size();
  CPoint pt;
  shared_ptr<CPoint> ppt;

  if( ar.IsStoring() ) {
    ar << m_pSrcUnit << m_pDestUnit
       << m_lSrcIndex << m_lDestIndex 
       << m_ulDynLinkType << m_ulDynLinkClass
       << iCount;
    for (auto it = m_plistLinkPoint->begin(); it != m_plistLinkPoint->end(); ++it) {
      ppt = *it;
      ar << *ppt;
    }
  }
  else {
		ar >> m_pSrcUnit >> m_pDestUnit
			>> m_lSrcIndex >> m_lDestIndex
			>> m_ulDynLinkType >> m_ulDynLinkClass
			>> iCount;
		for (int i = 0; i < iCount; i++) {
			ar >> pt;
			ppt = make_shared<CPoint>(pt);
			m_plistLinkPoint->push_back(ppt);
		}
  } 
}               

/////////////////////////////////////////////////////////////////////////////////////
//
// ToShow()
//
// Parameter :
//		CDC * const pdc : point to current DC.
//
// Return : no return
//
// Description :
//		To show the link of two units on DC.
//
//////////////////////////////////////////////////////////////////////////////////////
void CUnitDynLink::ToShow( CDC * const pdc ) {
  CPoint pt1, pt2;
  shared_ptr<CPoint> ppt;

  CPen cp, * pcp;
  INT_PTR iCount = m_plistLinkPoint->size();
  auto it = m_plistLinkPoint->begin();

  if (iCount == 0) return; // 如果没有可显示点，则返回。
    
  if ( m_fLoopDetect ) {
    switch ( m_ulDynLinkType ) {
    case tWORD :
      cp.CreatePen(PS_DASHDOTDOT, 1, RGB(255, 0, 0));
      break;
    case tBOOL :
      cp.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
      break;
    case tDOUBLE :
      cp.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
      break;
    case tSTRING :
      cp.CreatePen(PS_DASH, 1, RGB(255, 0, 0));
      break;
    default :
      ASSERT(FALSE);
    } // switch    
  }
  else {
    switch ( m_ulDynLinkType ) {
    case tWORD :
      cp.CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 255));
      break;
    case tBOOL :
      cp.CreatePen(PS_DOT, 1, RGB(0, 0, 255));
      break;
    case tDOUBLE :
      cp.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
      break;
    case tSTRING :
      cp.CreatePen(PS_DASH, 1, RGB(0, 0, 255));
      break;
    default :
      ASSERT(FALSE);
    } // switch
  }

  int x = 0, y = 0; // initialize
  pcp = pdc->SelectObject( &cp );
  ppt = *it++;
  pt1 = *ppt;
  for ( int i = 1; i < iCount; i++ ) {
    pdc->MoveTo( pt1 );  
    ppt = *it++;
    pdc->LineTo( *ppt );
    x = ppt->x;
    y = ppt->y;
    if ( i < iCount-1 ) {
      pdc->SetPixel(x-1, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y+1, RGB(0, 0, 0));
    }
    else pt2 = pt1;
    pt1 = *ppt;
  }
  if ( pt1.x == pt2.x ) {
    if ( pt1.y < pt2.y ) { // arrow direction is up
      pdc->SetPixel(x, y, RGB(0, 0, 0));
      pdc->SetPixel(x, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x, y+2, RGB(0, 0, 0));
      pdc->SetPixel(x, y+3, RGB(0, 0, 0));
      pdc->SetPixel(x, y+4, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y+2, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y+2, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y+3, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y+3, RGB(0, 0, 0));
      pdc->SetPixel(x-2, y+3, RGB(0, 0, 0));
      pdc->SetPixel(x+2, y+3, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y+4, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y+4, RGB(0, 0, 0));
      pdc->SetPixel(x-2, y+4, RGB(0, 0, 0));
      pdc->SetPixel(x+2, y+4, RGB(0, 0, 0));
    }
    else {  // down arrow
      pdc->SetPixel(x, y, RGB(0, 0, 0));
      pdc->SetPixel(x, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x, y-2, RGB(0, 0, 0));
      pdc->SetPixel(x, y-3, RGB(0, 0, 0));
      pdc->SetPixel(x, y-4, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y-2, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y-2, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y-3, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y-3, RGB(0, 0, 0));
      pdc->SetPixel(x-2, y-3, RGB(0, 0, 0));
      pdc->SetPixel(x+2, y-3, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y-4, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y-4, RGB(0, 0, 0));
      pdc->SetPixel(x-2, y-4, RGB(0, 0, 0));
      pdc->SetPixel(x+2, y-4, RGB(0, 0, 0));
    }
  }
  else {
    if ( pt1.x < pt2.x ) { // left arrow
      pdc->SetPixel(x, y, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y, RGB(0, 0, 0));
      pdc->SetPixel(x+2, y, RGB(0, 0, 0));
      pdc->SetPixel(x+3, y, RGB(0, 0, 0));
      pdc->SetPixel(x+4, y, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x+1, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x+2, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x+2, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x+3, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x+3, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x+3, y+2, RGB(0, 0, 0));
      pdc->SetPixel(x+3, y-2, RGB(0, 0, 0));
      pdc->SetPixel(x+4, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x+4, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x+4, y+2, RGB(0, 0, 0));
      pdc->SetPixel(x+4, y-2, RGB(0, 0, 0));
    }
    else {  // right arrow
      pdc->SetPixel(x, y, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y, RGB(0, 0, 0));
      pdc->SetPixel(x-2, y, RGB(0, 0, 0));
      pdc->SetPixel(x-3, y, RGB(0, 0, 0));
      pdc->SetPixel(x-4, y, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x-1, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x-2, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x-2, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x-3, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x-3, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x-3, y+2, RGB(0, 0, 0));
      pdc->SetPixel(x-3, y-2, RGB(0, 0, 0));
      pdc->SetPixel(x-4, y+1, RGB(0, 0, 0));
      pdc->SetPixel(x-4, y-1, RGB(0, 0, 0));
      pdc->SetPixel(x-4, y+2, RGB(0, 0, 0));
      pdc->SetPixel(x-4, y-2, RGB(0, 0, 0));
    }
  }
  pdc->SelectObject( pcp );
}                 

/////////////////////////////////////////////////////////////////////////////
// CUnitDynLink diagnostics

#ifdef _DEBUG
void CUnitDynLink::AssertValid() const
{
  CObjectPrimitive::AssertValid();
}

void CUnitDynLink::Dump(CDumpContext& dc) const
{
  CObjectPrimitive::Dump(dc);
  dc << "m_pSrcUnit = " << m_pSrcUnit->GetName() ;
  dc << "\nm_pDestUnit = " << m_pDestUnit->GetName();
  dc << "\nm_lSrcIndex = " << m_pSrcUnit->GetParaName(m_lSrcIndex);
  dc << "\nm_lDestIndex = " << m_pDestUnit->GetParaName(m_lDestIndex);
  dc << "\nm_ulDynLinktype = " << m_ulDynLinkType;
  dc << "\nm_ulDynLinkClass = " << Class[m_ulDynLinkClass] << "\n";
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function


///////////////////////////////////////////////////////////////////////////////////////
//
// SetLinkPointList()
//
///////////////////////////////////////////////////////////////////////////////////////
void CUnitDynLink::SetLinkPointList( CPointListPtr plist ) {
  INT_PTR iCount;
  shared_ptr<CPoint> ppt;

  iCount = plist->size();
	ASSERT( iCount >= 3 );			//至少三个点.
  m_plistLinkPoint->clear();
  for (auto it = plist->begin(); it != plist->end(); it++) {
    ppt = *it;
    m_plistLinkPoint->push_back(ppt);
  }
}

// 检查动态链接线是否有误
bool TestDynLinkList(CPointListPtr pLitLinkPoint) {
  shared_ptr<CPoint> pt1, pt2, pt3, pt4;
  auto it = pLitLinkPoint->begin();
  if (pLitLinkPoint->size() == 3) { // 只有三个点？
    pt1 = *it++;
    pt2 = *it++;
    pt3 = *it++;
    ASSERT(pt1->x != pt3->x);
    ASSERT(pt1->y != pt3->y);
    if (pt1->x != pt2->x) {
      ASSERT(pt1->y == pt2->y);
    }
    else {
      ASSERT(pt1->y != pt2->y);
    }
    if (pt3->x != pt2->x) {
      ASSERT(pt3->y == pt2->y);
    }
    else {
      ASSERT(pt3->y != pt2->y);
    }
    return(true);
  }
  if (pLitLinkPoint->size() == 4) {
    pt1 = *it++;
    pt2 = *it++;
    pt3 = *it++;
    pt4 = *it++;
    if ((pt2->x == pt3->x) && (pt2->y == pt3->y)) {
      ASSERT((pt1->x == pt4->x) || (pt1->y == pt4->y));
    }
    return(true);
  }

  bool fX = true;
  bool fStart = false;
  pt1 = *it++;
  do {
    pt2 = *it;
    if (pt1->x == pt2->x) {
      if ( fStart ) ASSERT( fX == false);
      ASSERT(pt1->y != pt2->y);
      fX = true;
    }
    else {
      if ( fStart ) ASSERT(fX);
      ASSERT(pt1->y == pt2->y);
      fX = false;
    }
    fStart = true;
    pt1 = pt2;
  } while (++it != pLitLinkPoint->end());
  return(true);
}
