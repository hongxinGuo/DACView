/////////////////////////////////////////////////////////////////////////////
//
// DrawInvertRect()				: draw invert color on rect
// DrawInvertLine()				: draw invert color on line
//
//////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include"CUnitComponent.h"

#include"accesory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

///////////////////////////////////////////////////////////////////////////////////////
//
// DrawInvertRect()
//
///////////////////////////////////////////////////////////////////////////////////////
void DrawInvertRect(CDC  * pdc, CRect const rect) {
  CPen * oldPen, newPen;
   
  if ( (rect.left == rect.right) && ( rect.bottom == rect.top) ) { 
  }
  else {
    newPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    oldPen = pdc->SelectObject(&newPen);
    ASSERT( oldPen != nullptr );
    pdc->MoveTo(rect.left, rect.top);
    const int nTemp = pdc->SetROP2(R2_NOT);
    pdc->LineTo(rect.left, rect.bottom-1);
    pdc->LineTo(rect.right-1, rect.bottom-1);
    pdc->LineTo(rect.right-1, rect.top);
    pdc->LineTo(rect.left, rect.top);
    pdc->SetROP2(nTemp);        
  
    pdc->SelectObject(oldPen);
  }

}   

///////////////////////////////////////////////////////////////////////////////////////
//
// DrawInvertLine()
//
///////////////////////////////////////////////////////////////////////////////////////
void DrawInvertLine(CDC * pdc, ULONG ulWidth, CPoint ptStart, CPoint ptEnd) {
  CPen * oldPen, newPen;
   
  newPen.CreatePen(PS_SOLID, ulWidth, RGB(0, 0, 0));
  oldPen = pdc->SelectObject(&newPen);
  ASSERT( oldPen != nullptr );
  pdc->MoveTo(ptStart);
  int nTemp = pdc->SetROP2(R2_NOT);
  pdc->LineTo(ptEnd);
  pdc->SetROP2(nTemp);        
  pdc->SelectObject(oldPen);
}

CUnitBase * FindUnit(CUnitList * pUnitList, CString strUnitName, bool fFind) {
	for (const auto punit : *pUnitList) {
		CString str = punit->GetName();
		if (str.Compare(strUnitName) == 0) {
			fFind = true;
			return punit;
		}
		if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
			CUnitComponent * pCpt = dynamic_cast<CUnitComponent *>(punit);
			if ( !pCpt->IsEncapsulated()) {
				return FindUnit(pCpt->GetUnitList(), strUnitName, fFind);
			}
		}
	}
	fFind = false;
	return nullptr;
}

