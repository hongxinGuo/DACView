/////////////////////////////////////////////////////////////////////
//
// CUnitBase implementation file
// including function :
//
// CUnitBase()              : initialize funciton
// ~CUnitBase()             : clear up function
// Serialize()              : Store and Retrive data
//
// DrawBitmap()             : Draw bitmap on DC
// 
// SetComponentUpper()  : Set m_pUnitComponentUpper pointer
// GetComponentUpper()  : Retrive m_pUnitComponentUpper pointer
//
// GetExectivePriority()					  : Get unit's exective priority
// SetExectivePriorityDirect()	    : direct Set unit's exective priority to the parameter
// SetExectivePriority()    : if all dynamic links link to me have found,
//                            then set my exective priority.
// SetLinkToMe()            : some unit is link to me, set flag
// IsLinkToMe()             : Test weither some unit link to me.
//
// SetLinkToType()          : Set unit list's input type< tINPUT, tOUTPUT, or 0 >.
// SetDestUnitPriority() : Set my link to unit's exective priority to my priority+1.
//
// GetDynLinkList()         : Retrive my dynamic link list pointer.
// CreateUniName()          : Create a unique name for me.
// AdjustDynLinkLinePosition() : adjust dynamic link's position that link to me
//                               and my link to.
// AddDynLink()             : Add dynamic link to my dynamic link list 
// BreakDynLink()           : delete dynamic link that I have.
// DeleteDynLink()          : Delete my dynamic link that point to list's unit.
// 
// SetParaLockFlag()      : Set my link to unit's selected flag.
// 
// GetClassnameStr()        : Get class name.
// IsMe()                   : Check weither is me.
// GetParaName()            : Retrive parameter's pointer.
//
// ExectiveDynLink()        : Exective my dynamic links.
// SetParameterLock()				: set or clear selected flag.
//
// ToShow()                 : Show myself and my dynamic links.
// MoveTo()                 : Move my position.
//
// SetFocus()               :
// ClearFocus()             :
// 
// AddToList()              : Add me to unit list.
//
// GetSize()                : Get my rect area.
// SetSize()                : Set my rect area.
//
// ClearLoopDetectFlag()    : Clear all my loop detect flag.
// SetLoopDetectFlag()      : Set my dyynamic link that link to unit's loop detect flag.
// LoopDetect()             : Check weither I am in a loop.
// SetDeleteDynLinkFlag()   : Set dynamic link's delete me flag.
// ClearDeleteDynLinkFlag()  : Clear dynamic link's delete me flag.
//
// Below function are virtual function, need implement by derived class 
//
// GetCalssNamePtr()
// GetParaName() 
//
// Exective()
//
// GetUnitType()
// GetIndex()
// GetParaNameAddress()
// GetDynLinktype()
// CheckSelf()
// SetParaNsameRange()
//
// GetBool()
// GetInteger()
// GetDouble()
// GetString()
// SetBool()
// SetInteger()
// SetDouble()
// SetString()
// SetProperty()
//
////////////////////////////////////////////////////////////////
#include "stdafx.h"    

#include "..\\resource.h"

#include "globedef.h"

#include "CUnitBase.h"
#include "CUnitComponent.h" 
#include "CUnitDynLink.h"

#include "DlgUnitDeleteDynLink.h"

IMPLEMENT_SERIAL(CUnitBase, CObjectPrimitive, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif   

CUnitBase::CUnitBase(const CString& Name, CPoint pt): CObjectPrimitive(Name) {
  m_pUnitComponentUpper = nullptr;
	m_pUnitListUpper = nullptr;
  m_rectArea.left   = pt.x - 20;
  m_rectArea.right  = pt.x + 20;
  m_rectArea.top    = pt.y - 20;
  m_rectArea.bottom = pt.y + 20;
  m_cstrComment.Empty();

  m_fCutOff = false;

  m_lScanRate = 100; // Ĭ��ɨ������Ϊ100msÿ��
  m_fAutoExective = true;

  m_lDynLinkToNumber = 0;
  m_lCountDLToNumber = 0;

  m_lExectivePriority = 0;
  m_fCompiled = false;

	m_lLinkToComponent = 0;
  m_lLinkFromComponent = 0;
	m_lLinkFromObject = 0;

  m_fOverFlow = false;

  m_lReserved3 = 0;

}      

CUnitBase::CUnitBase( void ) : CObjectPrimitive() { 
  
  m_strName.Empty();   
  m_cstrComment.Empty();
  m_rectArea.SetRectEmpty();
  m_pUnitComponentUpper = nullptr;
	m_pUnitListUpper = nullptr;
  m_fCutOff = false;

  m_fAutoExective = true;

  m_lDynLinkToNumber = 0;
  m_lCountDLToNumber = 0;

  m_lExectivePriority = 0;
  m_fCompiled = false;

	m_lLinkToComponent = 0;
  m_lLinkFromComponent = 0;
	m_lLinkFromObject = 0;

  m_fOverFlow = false;

  m_lReserved3 = 0;
}

CUnitBase::~CUnitBase() {
  // delete m_listDynLink
  POSITION Po = m_listDynLink.GetHeadPosition();
  CUnitDynLink * pcDynLink;
	INT_PTR iTemp = m_listDynLink.GetCount();
  for ( int i = 0; i < iTemp; i++ ) {
    pcDynLink = m_listDynLink.GetNext(Po);
    delete pcDynLink;
    pcDynLink = nullptr;
  } 
	// TRACE("%d Dynamic Links in Unit %s deleted\n", iTemp, m_strName);                  
  // release list's memory
  m_listDynLink.RemoveAll();
} 

void CUnitBase::Serialize( CArchive& ar ) {
  CObjectPrimitive::Serialize( ar );

  INT64 a, b;

  if( ar.IsStoring() ) {
    ar << m_cstrComment << m_rectArea 
       << (INT64)m_fCutOff << (INT64)m_fAutoExective << m_lReserved3 << m_lDynLinkToNumber << m_lExectivePriority;
  }
  else {
    ar >> m_cstrComment >> m_rectArea 
       >> a >> b >> m_lReserved3 >> m_lDynLinkToNumber >> m_lExectivePriority;
    m_fCutOff = (bool)a;
    m_fAutoExective = (bool)b;
    m_fSelected = false;
  }
  
  CString strTemp;
  CUnitDynLink * pcDynLink;
  POSITION pos;                          

  INT64 iTemp;

  if (ar.IsStoring())
  {
    // TODO: add storing code here
    pos = m_listDynLink.GetHeadPosition();
		iTemp = 0;
		for ( int i = 0; i < m_listDynLink.GetCount(); i++ ) {
      pcDynLink = m_listDynLink.GetNext(pos);
      if ( !pcDynLink->IsDeleteMe() ) iTemp++;
		}
		ar << iTemp;
    pos = m_listDynLink.GetHeadPosition();
    for ( int i = 0; i < m_listDynLink.GetCount(); i ++ ) { 
      pcDynLink = m_listDynLink.GetNext(pos);
      // when cut or copy me to clipboard, some dynamic links that I have
      // can't copy to clipboard, for its link to other unit. so if its 
      // delete flag is set, I didn't store it.
      if ( !pcDynLink->IsDeleteMe() )
        ar << pcDynLink;
    }
	}
	else
	{
		// TODO: add loading code here
		ar >> iTemp;
		for (int i = 0; i < iTemp; i++) {
			ar >> pcDynLink;
			m_listDynLink.AddTail(pcDynLink);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
// ��Ԫ������Ϊ�����Լ��Ĳ���������еĻ������Ƽ����Լ�������
//
//
//
//////////////////////////////////////////////////////////////////////////
CString CUnitBase::GetName(void)
{
  // TODO: �ڴ˴����� return ���
  CString str;
  if (m_pUnitComponentUpper != nullptr) {
    str = m_pUnitComponentUpper->GetName() + "." + m_strName;
  }
  else {
    str = m_strName;
  }
  return(str);
}

void CUnitBase::SetUpperUnitList(CUnitList * pUnitList) {
	m_pUnitListUpper = pUnitList;
}

CUnitList * CUnitBase::GetUpperUnitList(void) {
	return (m_pUnitListUpper);
}

///////////////////////////////////////////////////////////////////////////
//
// SetComponentUpper()
//
// Parameter :
//    CUnitComponent * pCUnitComponent : component that have me,
// Return : No return
//
// Decription :
//    Set up pointer point to the component that have me.
//
////////////////////////////////////////////////////////////////////////////
void  CUnitBase::SetComponentUpper(CUnitComponent * pCUnitComponent) {
	m_pUnitComponentUpper = pCUnitComponent;
}

CUnitComponent * CUnitBase::GetComponentUpper(void) const {
	return(m_pUnitComponentUpper);
}

INT32  CUnitBase::GetExectivePriority(void) const {
	return (m_lExectivePriority);
}

void CUnitBase::SetExectivePriorityDirect(ULONG ulPriority) {
	m_lExectivePriority = ulPriority;
}

bool CUnitBase::IsHaveSourceUnit(void) const {
  if (m_lDynLinkToNumber > 0) return (true);
  else return(false);
}

bool CUnitBase::IsLinkToComponent(void) {
	if (m_lLinkToComponent > 0) {
		return(true);
	}
	else {
		return(false);
	}
}

void CUnitBase::LinkToComponent(bool fFlag) {
	if (fFlag == true) {
		m_lLinkToComponent++;
	}
	else {
		if (m_lLinkToComponent > 0) { //���û������ʱ����ʲô���鶼��Ҫ�����ƺ�Ӧ�ñ���
			m_lLinkToComponent--;
		}
	}
}

void CUnitBase::LinkFromComponent(bool fFlag)
{
  if (fFlag == true) {
    m_lLinkFromComponent++;
  }
  else {
    if (m_lLinkFromComponent > 0) { //���û������ʱ����ʲô���鶼��Ҫ��(�ƺ�Ӧ�ñ���
      m_lLinkFromComponent--;
    }
  }
}

void CUnitBase::SetLinkFromObjectFlag(bool fFlag) {
	if (fFlag == true) {
		m_lLinkFromObject++;
	}
	else {
		if (m_lLinkFromObject > 0) { //���û������ʱ����ʲô���鶼��Ҫ��(�ƺ�Ӧ�ñ���
			m_lLinkFromObject--;
		}
	}
}

ULONG	CUnitBase::HowManyLinkToComponent(void) {
	return(m_lLinkToComponent);
}

void CUnitBase::SetCutOff(bool fFlag) {
	m_fCutOff = fFlag;
}

bool CUnitBase::IsSetCutOff(void) const {
	return(m_fCutOff);
}

//////////////////////////////////////////////////////////////////
//
// GetInputParameterNumber()
//
// return : 
//		ULONG	: ����������ӵĸ���.
//
// ����:
//		�õ�����������ӵĸ���,�������������ָ���뱾��Ԫ����������ϵ�����.
//
/////////////////////////////////////////////////////////////////////
ULONG	CUnitBase::GetInputParameterNumber(void) {
	return(m_lDynLinkToNumber);
}

//////////////////////////////////////////////////////////////////
//
// SetInputParameterNumber()
//	
// ���� 	: ����������ӵĸ���.
// return : ��
//		
//
// ����:
//		�õ�����������ӵĸ���,�������������ָ���뱾��Ԫ����������ϵ�����.
//
/////////////////////////////////////////////////////////////////////
void	CUnitBase::SetInputParameterNumber(LONG lNumber) {
	ASSERT(lNumber >= 0);
	m_lDynLinkToNumber = lNumber;
}

///////////////////////////////////////////////////////////////////
//
// ����Ԫ�Ƿ���ڴ��ϲ�����Ĳ���
// ����ֵ��
//      true������Ԫ���ڴ��ϲ�����Ķ�̬����
//		  false������Ԫ�����ڴ��ϲ�����Ķ�̬����
//
///////////////////////////////////////////////////////////////////
bool CUnitBase::IsDynLinkFromUpper(void) {
	CUnitList * pList;
	INT_PTR iCount, iDynLinkNum = 0, jCount;
	POSITION po, po2;
	CUnitBase* pUnit;
	CUDLList * pDynLinkList;
	CUnitDynLink * pcunitDynLink;
	if ( m_pUnitComponentUpper != nullptr ) { // ����Ԫ�������ڲ�����
    // ͳ�Ʋ����ڲ���Ԫ�������������������Ԫ������
		pList = m_pUnitComponentUpper->GetUnitList();
		po = pList->GetHeadPosition();
		iCount = pList->GetCount();
		for (int i = 0; i < iCount; i++) {
			pUnit = pList->GetNext(po);
			pDynLinkList = pUnit->GetDynLinkList();
			jCount = pDynLinkList->GetCount();
			po2 = pDynLinkList->GetHeadPosition();
			for (int j = 0; j < jCount; j++) {
				pcunitDynLink = pDynLinkList->GetNext( po2 );
				if (pcunitDynLink->GetDestUnit() == this) {
					iDynLinkNum++;
				}
			}
		}
    // ���㲿�����������뱾��Ԫ������
    for (int i = 0; i < 16; i++) {
      if (m_pUnitComponentUpper->IsParaLinked(i)) {
        if (m_pUnitComponentUpper->GetParaDestUnit(i) != nullptr) { // ����������ڲ���Ԫ
          if (m_pUnitComponentUpper->GetParaDestUnit(i) == this) { // ���������Ԫ��
            iDynLinkNum++;
          }
        }
      }
    }
		if (iDynLinkNum < m_lDynLinkToNumber) { // ����Ԫ�ڱ����������������С�ڱ���Ԫ�����������������
			return(true); // �Ǳ���Ԫ���д��ϲ�����Ķ�̬����
		}
		else {
			ASSERT(iDynLinkNum == m_lDynLinkToNumber);	// ��������붯̬���Ӳ��ܴ��������붯̬����
			return(false);
		}
	}
	else return(false);
}

///////////////////////////////////////////////////////////////////////////
// 
// SetDestUnitPriority()
//
// Description :
//    Set all my link to unit's priority to my priority+1.
//
///////////////////////////////////////////////////////////////////////////
void CUnitBase::SetDestUnitPriority( void ) {
	INT_PTR i, iTemp = m_listDynLink.GetCount();
  CUnitDynLink * pcunitDynLink;
  POSITION po = m_listDynLink.GetHeadPosition();
  CUnitBase * punit = nullptr;
  
  for ( i = 0; i < iTemp; i++ ) {
    pcunitDynLink = m_listDynLink.GetNext(po);
    punit = pcunitDynLink->GetDestUnit();
    if (!punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // ���ʱ�򵥵�Ԫ
      punit->SetExectivePriority(m_lExectivePriority + 1);
    }
    else if ((CUnitComponent *)punit != m_pUnitComponentUpper) { // �����Ŀ�Ĳ������ǰ����ҵ��ϼ�����
      punit->SetExectivePriority(m_lExectivePriority + 1);
    }
  }                                                                                  
}

/////////////////////////////////////////////////////////////////////////////
//
// SetExectivePriority()
//
// Parameter :
//    ULONG ulPriority : current set priority
//
// Description :
//    �������Ԫ����������, �����ñ���Ԫ������ֵΪ1.
//  �������е�����������Ӷ��ҵ���,�����ñ���Ԫ������ֵ.
//
///////////////////////////////////////////////////////////////////////////
void CUnitBase::SetExectivePriority( ULONG ulPriority ) {
	ULONG ulUnitType = GetUnitType();
	ASSERT( ulUnitType != tOUTPUT );			// ��֤����ԪΪ���������Ԫ. 
  if ( m_lDynLinkToNumber == 0 ) {  // �������Ԫû�����뵥Ԫ.
    m_lExectivePriority = 1; // ����ִ�б���Ԫ.
    return;
  }
	if ( m_fCutOff ) return;	// �ضϵ�Ԫ��������������ֵ.
  // ֱ���������뱾��Ԫ�ĵ�Ԫ�������,����Ԫ�Ĵ�������ֵ�ſ�������.
  if ( ++m_lCountDLToNumber == m_lDynLinkToNumber ) m_lExectivePriority = ulPriority;
	ASSERT( m_lCountDLToNumber <= m_lDynLinkToNumber );
}

void CUnitBase::ResetCompileFlag( void ) {
	m_lCountDLToNumber = 0;
  m_lDynLinkToNumber = 0;
	m_lExectivePriority = 0;
  m_fCompiled = false;
  ClearParaSelectedFlag();
}

CUDLList * CUnitBase::GetDynLinkList( void ) {
  return( &m_listDynLink );
}

////////////////////////////////////////////////////////////////////////
//
// CreateUniName() 
//
// Parameter :
//    CUnitList& listUnit : contain unit list that my name is prevent to same as them.
//
// Description :
//    Create unique name for myself that different from listUnit.
//		����Ψһ������
//
///////////////////////////////////////////////////////////////////////////
bool CUnitBase::CreateUniName( CUnitList& listUnit ) {
  bool fFind = false;
	INT_PTR iTemp = 1, i, iCount = listUnit.GetCount();
  POSITION po = listUnit.GetHeadPosition();
  CUnitBase * pcunit;
  CString strNumber;
  char s[10];

  for ( i = 0; i < iCount; i ++ ) {
    pcunit = listUnit.GetNext( po );
    if ( m_strName == pcunit->GetName() ) {
      fFind = true;
      break;
    }
  }

  bool fDone = false;
  if ( fFind ) {
    while ( !fDone ) {
      _itoa(iTemp++, s, 10);
      m_strName = GetClassNameStr() + s;
      iCount = listUnit.GetCount();
      po = listUnit.GetHeadPosition();
      fDone = true;
      for ( i = 0; i < iCount; i ++ ) {
        pcunit = listUnit.GetNext( po );
        if ( m_strName == pcunit->GetName() ) {
          fDone = false;
          break;
        }
      }
    }
  }
  return( true );
}

/////////////////////////////////////////////////////////////////////////
//
// AdjustDynLinkLinePosition()
//
// Parameter :
//    CUnitBase * pcSrc : point to current moved unit
//    CPoint ptStart    : origin unit position.
//    CPoint ptEnd      : current unit position.
//
// Return : No return
//
// Description :
//    Adjust my dynamic link's position according changed position��
// �������̬���ӵ�����Ϊ���������򵥵�Ԫ���߲���������Ҫ������̬�����ߵ�λ�á�
// 
//
////////////////////////////////////////////////////////////////////////
void CUnitBase::AdjustDynLinkLinePosition(CUnitBase * pcSrc, CPoint ptStart, CPoint ptEnd) {
  CUnitDynLink * pDL;
  POSITION poLine, po = m_listDynLink.GetHeadPosition();
	INT_PTR i, iCount = m_listDynLink.GetCount();
  CPoint *ppt1, * ppt2;
  CPointList * plist;

  if ( pcSrc == this ) {  // if I was been changed size
    iCount = m_listDynLink.GetCount();
    po = m_listDynLink.GetHeadPosition();
    for ( i = 0; i < iCount; i++ ) {
      pDL = m_listDynLink.GetNext( po );
			if ((pDL->GetDynLinkClass() == COMPONENT_TO_UNIT) || (pDL->GetDynLinkClass() == COMPONENT_TO_UNIT))
				break; //����������Ԫ���ڵĸ��ϵ�Ԫʱ������Ҫ������̬�����ߵ�λ�ã������˾ͳ����ˣ�
      plist = pDL->GetLinkPointList();
      poLine = plist->GetHeadPosition();
      ppt1 = plist->GetNext( poLine );
      ppt2 = plist->GetNext( poLine );
      if ( ppt1->x == ppt2->x ) {	// �ӵ�Ԫ�����Ķ�̬����������ֱ��.
        if ( (ppt1->y < ppt2->y) && (m_rectArea.top > ppt2->y) ) {// 
          ppt1->x += ptEnd.x - ptStart.x;
          ppt1->y = m_rectArea.top;
          ppt2->x += ptEnd.x - ptStart.x;
        }
        else if ( (ppt1->y > ppt2->y) && (m_rectArea.bottom < ppt2->y) ) {
          ppt1->x += ptEnd.x - ptStart.x;
          ppt1->y = m_rectArea.bottom;
          ppt2->x += ptEnd.x - ptStart.x;
        }
        else {
          *ppt1 += ptEnd - ptStart;
          ppt2->x += ptEnd.x - ptStart.x;
        }
      }
      else if ( ppt1->y == ppt2->y ) {
        if ( (ppt1->x < ppt2->x) && (m_rectArea.left > ppt2->x) ) {
          ppt1->y += ptEnd.y - ptStart.y;
          ppt1->x = m_rectArea.left;
          ppt2->y += ptEnd.y - ptStart.y;
        }
        else if ( (ppt1->x > ppt2->x) && (m_rectArea.right < ppt2->x) ) {
          ppt1->y += ptEnd.y - ptStart.y;
          ppt1->x = m_rectArea.right;
          ppt2->y += ptEnd.y - ptStart.y;
        }
        else {
          *ppt1 += ptEnd - ptStart;
          ppt2->y += ptEnd.y - ptStart.y;
        }
      }
      else {
        *ppt1 += ptEnd - ptStart;
      }
			// �����Ԫ�ı��˴�С,��Ҫ����ppt1.
			if ( ppt1->x == ppt2->x ) {	// ��ֱ��.
				if ( ppt2->y > m_rectArea.bottom ) {
					ppt1->y = m_rectArea.bottom;
				}
				else if ( ppt2->y < m_rectArea.top ) {
					ppt1->y = m_rectArea.top;
				}
			}
			else if ( ppt1->y == ppt2->y ) {	// ˮƽ��.
				if ( ppt2->x > m_rectArea.right ) {
					ppt1->x = m_rectArea.right;
				}
				else if ( ppt2->x < m_rectArea.left ) {
					ppt1->x = m_rectArea.left;
				}
			}
			else ASSERT( 0 );
    }
  }

  // if my link to unit was been moved
  po = m_listDynLink.GetHeadPosition();
  iCount = m_listDynLink.GetCount();
  bool fDo;
  CUnitBase * pcunit;
  CRect rect, rectSrc;
  for ( i = 0; i < iCount; i++ ) {
    pDL = m_listDynLink.GetNext( po );
    if ( (pcunit = pDL->GetDestUnit()->GetComponentUpper()) == pcSrc ) {
      fDo = true;
    }
    else if ( (pcunit = pDL->GetDestUnit()) == pcSrc ) {
      fDo = true;
    }
    else {
      fDo = false;
    }
    if ( fDo ) {
      rect = pcunit->GetSize();
      plist = pDL->GetLinkPointList();
      poLine = plist->GetTailPosition();
      ppt1 = plist->GetPrev( poLine );
      ppt2 = plist->GetPrev( poLine );
			rectSrc = pcSrc->GetSize();
      if ( ppt1->x == ppt2->x ) {
        if ( (ppt1->y < ppt2->y) && (rect.top > ppt2->y) ) {
          ppt1->x += ptEnd.x - ptStart.x;
          ppt1->y = rect.top;
          ppt2->x += ptEnd.x - ptStart.x;
        }
        else if ( (ppt1->y > ppt2->y) && (rect.bottom < ppt2->y) ) {
          ppt1->x += ptEnd.x - ptStart.x;
          ppt1->y = rect.bottom;
          ppt2->x += ptEnd.x - ptStart.x;
        }
        else {
          *ppt1 += ptEnd - ptStart;
          ppt2->x += ptEnd.x - ptStart.x;
        }
      }
      else if ( ppt1->y == ppt2->y ) {
        if ( (ppt1->x < ppt2->x) && (rect.left > ppt2->x) ) {
          ppt1->y += ptEnd.y - ptStart.y;
          ppt1->x = rect.left;
          ppt2->y += ptEnd.y - ptStart.y;
        }
        else if ( (ppt1->x > ppt2->x) && (rect.right < ppt2->x) ) {
          ppt1->y += ptEnd.y - ptStart.y;
          ppt1->x = rect.right;
          ppt2->y += ptEnd.y - ptStart.y;
        }
        else {
          *ppt1 += ptEnd - ptStart;
          ppt2->y += ptEnd.y - ptStart.y;
        }
      }
      else {
        *ppt1 += ptEnd - ptStart;
      }

			// �����Ԫ�ı��˴�С,��Ҫ����ppt1.
			if ( ppt1->x == ppt2->x ) {	// ��ֱ��.
				if ( ppt2->y > rectSrc.bottom ) {
					ppt1->y = rectSrc.bottom;
				}
				else if ( ppt2->y < rectSrc.top ) {
					ppt1->y = rectSrc.top;
				}
			}
			else if ( ppt1->y == ppt2->y ) {	// ˮƽ��.
				if ( ppt2->x > rectSrc.right ) {
					ppt1->x = rectSrc.right;
				}
				else if ( ppt2->x < rectSrc.left ) {
					ppt1->x = rectSrc.left;
				}
			}
			else ASSERT( 0 );
    }
  }
}

void CUnitBase::AddDynLink( CUnitDynLink * punitDynLink ) {
  ASSERT( punitDynLink != nullptr );
  m_listDynLink.AddTail(punitDynLink); 
}  

/////////////////////////////////////////////////////////////////////////
//
// BreakDynLink()
//
//return : ��ɾ���˶�̬����ʱ����true
//  			 ��ûɾ����̬����ʱ����false
//
// Description :
//    Call Dialog delete dynamic link to delete my dynamic links.
//
/////////////////////////////////////////////////////////////////////////
bool CUnitBase::ArrangeDynLink( void ) {
  CDelDynLink cDialogDel;
	INT_PTR iTotal = m_listDynLink.GetCount();

  cDialogDel.SetLink( &m_listDynLink );
  cDialogDel.DoModal();
	if ( iTotal != m_listDynLink.GetCount() ) {
		return( true );
		}
	else return( false );
}

////////////////////////////////////////////////////////////////////////
//
// DeleteDynlink()
//
// Parameter : 
//    CUnitBase * pUnit : point to  need deleted unit
//
// Return :
//    bool          : always true.
//
// Description :
//    If my dynamic link to unit is pUnit, then delete it.
//		
/////////////////////////////////////////////////////////////////////////
bool CUnitBase::DeleteDynLink( CUnitBase * pUnit ) {
	INT_PTR iTemp = m_listDynLink.GetCount();
  CUnitDynLink * pcunitDynLink;
  POSITION pos, po = m_listDynLink.GetHeadPosition();
  CUnitBase * pcunit;
  
  for ( int i = 0; i < iTemp; i++ ) {
    pcunitDynLink = m_listDynLink.GetNext(po);
    pcunit = pcunitDynLink->GetDestUnit();
    if ( pUnit == pcunit ) {
      pos = m_listDynLink.Find( pcunitDynLink );
      ASSERT( pos != nullptr );
      pcunit->SetParameterLock( pcunitDynLink->GetDestIndex(), false );
      m_listDynLink.RemoveAt( pos );
      delete pcunitDynLink;
      pcunitDynLink = nullptr;
    }
  }
  return( true );
}

///////////////////////////////////////////////////////////////////////////
//
// SetParaLockFlag()
//
// Description :
//    Set my dynamic link to unit's index selected flag.
//
////////////////////////////////////////////////////////////////////////
void CUnitBase::SetParaLockFlag( void ) {
  CUnitDynLink * pUnitDynLink;
  CUnitBase * pUnit;
  POSITION poDynLink;
  int iDynLinkUnitCount;

  poDynLink = m_listDynLink.GetHeadPosition();
  iDynLinkUnitCount = m_listDynLink.GetCount();
  CString strName;

  for ( int i = 0; i < iDynLinkUnitCount; i++ ) {
    pUnitDynLink = m_listDynLink.GetNext(poDynLink);
    pUnit = pUnitDynLink->GetDestUnit();
    pUnit->SetParameterLock( pUnitDynLink->GetDestIndex(), true );  // set Index selected
  }
} 

// virtual function declaretion
const CString& CUnitBase::GetClassNameStr( void ) {
  static CString str = "UnitBase";      
  return( str );
}

CString CUnitBase::GetParaName( ULONG ) {
	ASSERT( 0 );
  return("");
}

ULONG CUnitBase::GetParaType( ULONG ) {
	ASSERT( 0 );
  return(0);
}

bool CUnitBase::IsMe( const CString& strName ) {
  if ( strName == m_strName ) {
    return( true );
  }
  else {
    return( false );
  }
}

bool CUnitBase::IsOverFlow(void) {
	if (m_fOverFlow) {
		return(true);
	}
	else return(false);
}
  
void CUnitBase::Exective( void ) {
	ASSERT( 0 );
}

////////////////////////////////////////////////////////////////////////
//
// ExectiveDynLink() ִ�ж�̬��������
//
// Return : 
//    bool    : always true.
//
// Description :
//    Exective my dynamic links.
//
///////////////////////////////////////////////////////////////////////////
bool CUnitBase::ExectiveDynLink( void ) {
  POSITION po = m_listDynLink.GetHeadPosition();
	INT_PTR iTemp = m_listDynLink.GetCount();
  CUnitDynLink * pcunitDynLink;
  ULONG ulSourceIndex, ulDestIndex;
  double eTemp;
  LONG lTemp;
  bool fTemp;
	CString strTemp;

  for ( int i = 0; i < iTemp; i++ ) {
    pcunitDynLink = m_listDynLink.GetNext(po);
    ulSourceIndex = pcunitDynLink->GetSrcIndex();
    ulDestIndex = pcunitDynLink->GetDestIndex();
    ASSERT((pcunitDynLink->GetSrcUnit()->GetDynLinkType(ulSourceIndex) & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
    switch ( pcunitDynLink->GetSrcUnit()->GetDynLinkType(ulSourceIndex) & (tDOUBLE | tWORD | tBOOL | tSTRING) ) {
    case tBOOL :
      fTemp = pcunitDynLink->GetSrcUnit()->GetBool(ulSourceIndex);
      ASSERT((pcunitDynLink->GetDynLinkType() & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
      switch ( pcunitDynLink->GetDynLinkType() & (tDOUBLE | tWORD | tBOOL | tSTRING)) {
      case tBOOL :
        (pcunitDynLink->GetDestUnit())->SetBool(ulDestIndex, fTemp);
        break;
      case tWORD :
        (pcunitDynLink->GetDestUnit())->SetInteger(ulDestIndex, (LONG)fTemp);
        break;
      case tDOUBLE :
        (pcunitDynLink->GetDestUnit())->SetDouble(ulDestIndex, (double)fTemp);
        break;
      case tSTRING :
        (pcunitDynLink->GetDestUnit())->SetString(ulDestIndex, GetString(ulSourceIndex));
        break;
			default: // something wrong
				TRACE("Warning: DynLink's Dest type outof range\n");
				break;
      } // switch ulDynLinkType                                          
      break;
    case tWORD :
      lTemp = pcunitDynLink->GetSrcUnit()->GetInteger(ulSourceIndex);
      ASSERT((pcunitDynLink->GetDynLinkType() & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
      switch ( pcunitDynLink->GetDynLinkType() | (tDOUBLE | tWORD | tBOOL | tSTRING) ) {
      case tBOOL :
        (pcunitDynLink->GetDestUnit())->SetBool(ulDestIndex, (bool)lTemp);
        break;
      case tWORD :
        (pcunitDynLink->GetDestUnit())->SetInteger(ulDestIndex, lTemp);
        break;
      case tDOUBLE :
        (pcunitDynLink->GetDestUnit())->SetDouble(ulDestIndex, (double)lTemp);
        break;
      case tSTRING :
        (pcunitDynLink->GetDestUnit())->SetString(ulDestIndex, GetString(ulSourceIndex));
        break;
			default: // something wrong
				TRACE("Warning: DynLink's Dest type outof range\n");
				break;
      } // switch ulDynLinkType                                          
      break;
    case tDOUBLE :
      eTemp = pcunitDynLink->GetSrcUnit()->GetDouble(ulSourceIndex);
      ASSERT((pcunitDynLink->GetDynLinkType() & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
      switch ( pcunitDynLink->GetDynLinkType() & (tDOUBLE | tWORD | tBOOL | tSTRING)) {
      case tBOOL :
        (pcunitDynLink->GetDestUnit())->SetBool(ulDestIndex, (bool)eTemp);
        break;
      case tWORD :
        (pcunitDynLink->GetDestUnit())->SetInteger(ulDestIndex, (LONG)eTemp);
        break;
      case tDOUBLE :
        (pcunitDynLink->GetDestUnit())->SetDouble(ulDestIndex, eTemp);
        break;
      case tSTRING :
        (pcunitDynLink->GetDestUnit())->SetString(ulDestIndex, GetString(ulSourceIndex));
        break;
			default: // something wrong
				TRACE("Warning: DynLink's Dest type outof range\n");
				break;
      } // switch ulDynLinkType                                          
      break;
		case tSTRING:
			strTemp = pcunitDynLink->GetSrcUnit()->GetString(ulSourceIndex);
      ASSERT((pcunitDynLink->GetDynLinkType() & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
			switch (pcunitDynLink->GetDynLinkType() | (tDOUBLE | tWORD | tBOOL | tSTRING)) {
			case tSTRING:
				(pcunitDynLink->GetDestUnit())->SetString(ulDestIndex, strTemp);
				break;
			default: // something wrong
				TRACE("Warning: DynLink's dest type outof range\n");
				break;
			} // switch ulDynLinkType                          
		default : // something wrong
			TRACE("Warning: DynLink's source type outof range\n");
			break;
    } // 
  }   // for int i = 0
  SetUpdateFlag(true);

  return(true);
}

//////////////////////////////////////////////////////////////////////////////////
//
// CUnitBase�༰��󲿷���������������������̬���ӣ������������������Դ��ڡ�
// �����������ࣨĿǰֻ��CUnitSine��CUnitInputOutput��CUnitQuad�������ֻࣩ��������������������������������������������������������
// 
//
/////////////////////////////////////////////////////////////////////////////////
ULONG CUnitBase::GetUnitType( void ) {
  return( tINPUT | tOUTPUT );
}     

INT32 CUnitBase::GetIndex( ULONG ) {
	ASSERT( 0 );
  return( 0 );
}                  

ParaName* CUnitBase::GetParaNameAddress( void ) {
	ASSERT( 0 );
  return( nullptr );
}     

INT32* CUnitBase::GetArrayIndex( void ) {
	ASSERT( 0 );
  return( nullptr );
}

bool CUnitBase::IsParameterLocked(ULONG ulIndex)
{
  ASSERT(0);
  return false;
}


ULONG CUnitBase::GetDynLinkType(ULONG ) {
	ASSERT( 0 );
  return( 0 );
} 

/////////////////////////////////////////////////////////////////////////
//
// SetParameterLock()
//
// Parameter :
//    ULONG ulIndex     : index of my programable parameter.
//    bool fSelected    : current process index is set or clear.
//
// Description :
//    �ı�ulIndexָ��Ĳ�������״̬. �����û����������.
//  �����ɻ�ɾ����̬����ʱ,����Ҫ���ô˺���. 
//
//////////////////////////////////////////////////////////////////////////
bool CUnitBase::SetParameterLock( ULONG ulIndex, bool fSelected ) {
  ParaName * pParaName = GetParaNameAddress();
	// �˺���ֻ�����������.
  if (pParaName[ulIndex].ulType & tINPUT) {
    if (fSelected == true) {		// ������ѡ���־
      ASSERT(m_pfSelected[ulIndex] == false); // ������������ã������Ͳ���ֻ������һ��
      m_pfSelected[ulIndex] = true;
      m_lDynLinkToNumber++;
    }
    else {		// �����ѡ���־
      ASSERT(m_pfSelected[ulIndex] == true);
      m_pfSelected[ulIndex] = false;
      m_lDynLinkToNumber--;
    }
    return(true);
  }
  else return(false);
}

///////////////////////////////////////////////////////////////////////////////////////
//
// ���������Ͳ����Ƿ�ѡ��ı�־��
//
// �˺���ֻ������Object->Unit�Ķ�̬����ʱ�ű����ã��������ݷ���Ϊ��������Ԫ����ʱ��������m_lDynLinkToNumber
// ����ֵ�������޷��������루��δ���ԣ���
//
//
////////////////////////////////////////////////////////////////////////////////////////
bool CUnitBase::SetParameterSelected(ULONG ulIndex, bool fSelected) {
  ParaName * pParaName = GetParaNameAddress();
  // �˺���ֻ�����������.
  if (pParaName[ulIndex].ulType & tINPUT) {
    if (fSelected == true) {		// ������ѡ���־
      ASSERT(m_pfSelected[ulIndex] == false); // ������������ã������Ͳ���ֻ������һ��
      m_pfSelected[ulIndex] = true;
			SetLinkFromObjectFlag(true);
    }
    else {		// �����ѡ���־
      ASSERT(m_pfSelected[ulIndex] == true);
      m_pfSelected[ulIndex] = false;
			SetLinkFromObjectFlag(false);
    }
    return(true);
  }
  else return(false);
}


////////////////////////////////////////////////////////////////////////////////////////////
//
// ���Ҽ�鱾��Ԫ���ݵ������ԣ���Ҫʱ��̬���������Ƿ�ƥ��
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitBase::CheckSelf( void ) {
	CUnitDynLink * pUDL;
	POSITION po = m_listDynLink.GetHeadPosition();
	CUnitBase * pSrc, * pDest;
	ULONG ulSrcType, ulDestType, ulType;

	for ( int i = 0; i < m_listDynLink.GetCount(); i++ ) {
		pUDL = m_listDynLink.GetNext( po );
		pSrc = pUDL->GetSrcUnit();
		pDest = pUDL->GetDestUnit();
		ulSrcType = pSrc->GetParaType( pUDL->GetSrcIndex() );
		ulDestType = pDest->GetParaType( pUDL->GetDestIndex() );
		ulType = pUDL->GetDynLinkType();
		if ( (ulType & ulDestType) == false ) {
			ShowMessage(ID_ERROR_DYNAMIC_LINK_TYPE_MISMATCH, 
									(LPCTSTR)m_strName,
									(LPCTSTR)pSrc->GetParaName(pUDL->GetSrcIndex()),
									(LPCTSTR)pDest->GetName(),
									(LPCTSTR)pDest->GetParaName(pUDL->GetDestIndex()));
		}
	}
  return( true );
}

bool CUnitBase::Encapsulation(CUnitList &) {
	// do nothing, use for CUnitComponent
	return( true );
}

////////////////////////////////////////////////////////////////////
//
// �˺���ֻ�Բ�������Ч�������඼����true��
//
//
////////////////////////////////////////////////////////////////////
bool CUnitBase::IsEncapsulated(void) {
	return(true);
}

void CUnitBase::PrepareRunTimeList( void ) {
	// do nothing, use for CUnitComponent
	TRACE("CUnitBase's prepareRunTimeList been called\n");
}

bool CUnitBase::GetBool(ULONG ) {
  ASSERT( false );
  return(0);
}
    
LONG CUnitBase::GetInteger(ULONG ) {
  ASSERT( false );
  return(0);
}

double CUnitBase::GetDouble(ULONG ) {
  ASSERT( false );
  return(0.0);
} 

const CString CUnitBase::GetString(ULONG ) {
  ASSERT( false );
  return(m_strName);
}

bool CUnitBase::SetBool(ULONG , bool ) {
  ASSERT( false );
  return(false);
}

bool CUnitBase::SetInteger(ULONG , LONG ) {
  ASSERT( false );
  return(false);
}

bool CUnitBase::SetDouble(ULONG , double ) {
  ASSERT( false );
  return(false);
} 

bool CUnitBase::SetString(ULONG , const CString& ) {
  ASSERT( false );
  return(false);
}   
   
bool CUnitBase::InIt( POINT const pt) {
	return ( m_rectArea.PtInRect(pt) );
}

void CUnitBase::ToShow( CDC * const pdc ) {
  // show status 
  CPen cp, cp1, cp2, cp3, * pcp;
  CRect rect = m_rectArea;
  
  cp.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
  pcp = (CPen *)pdc->SelectObject(&cp);
  if ( m_fOverFlow ) { // �����Ԫ�ı߿�Ϊ��ɫ
    cp2.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));	// ����ĵ�ԪΪ��ɫ.
    pdc->SelectObject( &cp2 );
    pdc->MoveTo( m_rectArea.left, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.top );
  }
  else if ( m_fCutOff  ) { // �нضϵĵ�Ԫ�ı߿�Ϊ��ɫ
    cp3.CreatePen(PS_SOLID, 2, RGB(255, 255, 0));	// �нضϵĵ�ԪΪ��ɫ.
    pdc->SelectObject( &cp3 );
    pdc->MoveTo( m_rectArea.left, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.top );
  }
  else {
    cp2.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));	// �����ĵ�ԪΪ��ɫ.
    pdc->SelectObject( &cp2 );
    pdc->MoveTo( m_rectArea.left, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.top );
  }

  // ����Ӳ�����������������,����ʾһ����ɫ���������Ϸ�.
  if (m_lLinkFromComponent > 0) {
    CBrush brhBlue(RGB(0, 0, 255));
    rect.top = m_rectArea.top;
    rect.left = m_rectArea.left;
    rect.bottom = rect.top + 8;
    rect.right = rect.left + 8;
    pdc->FillRect(rect, &brhBlue);
  }

	// �������������������Ĳ�����,����ʾһ����ɫ���������Ϸ�.
	if ( m_lLinkToComponent > 0 ) {
		CBrush brhBlue( RGB(0, 0, 255) );
		rect.top = m_rectArea.top;
		rect.left = m_rectArea.right - 8;
		rect.bottom = rect.top + 8;
		rect.right = m_rectArea.right;
		pdc->FillRect( rect, &brhBlue );
	}

	// �������COMPONENT_TO_UNIT����COMPONENT_TO_COMPONENT������ʾһ����ɫ���������·�
	CUnitDynLink* pUnitDynLink = nullptr;
	POSITION poUnitDynLink = m_listDynLink.GetTailPosition();
	INT_PTR iTemp = m_listDynLink.GetCount();
	bool fFind = false;
	for (int i = 0; i < iTemp; i++) {
		pUnitDynLink = m_listDynLink.GetPrev(poUnitDynLink);
		switch (pUnitDynLink->GetDynLinkClass()) {
		case COMPONENT_TO_UNIT:
		case COMPONENT_TO_COMPONENT:
			fFind = true;
			break;
		case UNIT_TO_UNIT:
		case UNIT_TO_COMPONENT:
			// DO NOTHING
			break;
    case UNIT_TO_CptPARAMETER:
      // ĿǰʲôҲ�������о�֮
      break;
		default:
			ASSERT(false);
		} // switch ( pUnitDynLink->GetType() );
	}
	if ( fFind ) {
		CBrush brhBlue(RGB(0, 0, 255));
		rect.top = m_rectArea.bottom - 8;
		rect.left = m_rectArea.right - 8;
		rect.bottom = m_rectArea.bottom;
		rect.right = m_rectArea.right;
		pdc->FillRect(rect, &brhBlue);
	}

	// �������UNIT_TO_COMPONENT��COMPONENT_TO_COMPONENT���ߴӲ��������������ݣ�����ʾһ����ɫ���������·�
	if (IsDynLinkFromUpper()) {
		CBrush brhBlue(RGB(0, 0, 255));
		rect.top = m_rectArea.bottom - 8;
		rect.left = m_rectArea.left;
		rect.bottom = m_rectArea.bottom;
		rect.right = m_rectArea.left + 8;
		pdc->FillRect(rect, &brhBlue);
	}

	// ������ڴ�Object���������ݣ�����ʾһ����ɫ����������в�
	if (IsLinkFromObject()) {
		CBrush brhBlue(RGB(0, 0, 255));
		rect.top = (m_rectArea.bottom + m_rectArea.top)/2 - 4;
		rect.left = m_rectArea.left;
		rect.bottom = rect.top + 8;
		rect.right = m_rectArea.left + 8;
		pdc->FillRect(rect, &brhBlue);
	}

	// �����ѡ���ˣ�����ʾѡ���־
  if ( m_fSelected ) {
		int left, top, bottom,  right;
		CRect arrayRect[8];

		left   = m_rectArea.left;
		right = m_rectArea.right;
		top    = m_rectArea.top;
		bottom = m_rectArea.bottom;
                          
		arrayRect[0].top    = top;
		arrayRect[0].left   = left;     
  
		arrayRect[2].top    = top;
		arrayRect[2].left   = right-8; 
  
		arrayRect[1].top    = top;
		arrayRect[1].left   = left + (right - left)/2 - 4;  
  
		arrayRect[3].top    = top + (bottom - top)/2 - 4;
		arrayRect[3].left   = left;     
  
		arrayRect[4].top    = top + (bottom - top)/2 - 4;
		arrayRect[4].left   = right -8;                 
  
		arrayRect[5].top    = bottom - 8;
		arrayRect[5].left   = left;     
  
		arrayRect[7].top    = bottom - 8;
		arrayRect[7].left   = right - 8; 
  
		arrayRect[6].top    = bottom - 8 ;
		arrayRect[6].left   = left + (right - left)/2 - 4; 
  
		for ( int i = 0; i < 8; i ++ ) {
			pdc->PatBlt(arrayRect[i].left, arrayRect[i].top, 8, 8, DSTINVERT);
		}
  }
  pdc->SelectObject(pcp);
  
  // show the dynamic link 
	iTemp = m_listDynLink.GetCount();
  poUnitDynLink = m_listDynLink.GetTailPosition();
  for ( int i = 0; i < iTemp; i++ ) { 
    pUnitDynLink = m_listDynLink.GetPrev(poUnitDynLink);
    switch ( pUnitDynLink->GetDynLinkClass() ) {
    case UNIT_TO_UNIT :
    case UNIT_TO_COMPONENT :
      pUnitDynLink->ToShow( pdc );
      break;
    case COMPONENT_TO_UNIT :
    case COMPONENT_TO_COMPONENT :
      // don't show COMPONENT_TO_UNIT & COMPONENT_TO_COMPONENT
      // they are show in CUnitComponent that have me
      break;
    case UNIT_TO_CptPARAMETER : // �������ڷ�װ������Ż�������ѷ�װ�������ڲ���Ԫ���еĶ�̬�����У�������ʾ�䶯̬�����ߡ������䶯̬������Ӧ���ǿյġ�
      ASSERT(pUnitDynLink->GetLinkPointList()->IsEmpty()); // �����͵Ķ�̬�����������Ѿ��Ƶ��˰�����Ĳ�������
      break;
    default :
      ASSERT( false );
    } // switch ( pUnitDynLink->GetDynLinkClass() );
  }  
}            

void CUnitBase::SetFocus( CDC *pdc) { 
  ToShow( pdc );
}
 
void CUnitBase::ClearFocus( CDC *pdc) {
  ToShow( pdc );
}

bool CUnitBase::SetProperty( void ) {
  return( false );
}

void CUnitBase::AddToList( CUnitList& UnitList) {
  UnitList.AddTail(this);
}

/////////////////////////////////////////////////////////////////////////////
// CUnitBase diagnostics

#ifdef _DEBUG
void CUnitBase::AssertValid() const
{
  CObjectPrimitive::AssertValid();
}

void CUnitBase::Dump(CDumpContext& dc) const
{
  CObjectPrimitive::Dump(dc);
  CUnitDynLink * pc;
  POSITION po = m_listDynLink.GetHeadPosition();
  for ( int i = 0; i < m_listDynLink.GetCount(); i++ ) {
    pc = m_listDynLink.GetNext(po);
    dc << pc << "\n";
  }
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// COjbectbase member function

const CRect& CUnitBase::GetSize( void ) const {
  return( m_rectArea );
}  

void CUnitBase::SetSize( const CRect& rectArea ) {
  m_rectArea = rectArea;
}

// ����ע��
void CUnitBase::SetComment(const CString& strComment) {
	m_cstrComment = strComment;
}

//�õ�ע���ַ���
CString	CUnitBase::GetComment(void) {
	return(m_cstrComment);
}

/////////////////////////////////////////////////////////////////////////
//
// ClearLoopDetectFlag()
//
// Description :
//    Clear my link to unit's loop detect flag.
//
////////////////////////////////////////////////////////////////
void CUnitBase::ClearLoopDetectFlag( void ) {
  CUnitDynLink * pcunitDL;
  POSITION po = m_listDynLink.GetHeadPosition();
	INT_PTR i, iCount = m_listDynLink.GetCount();

  for ( i = 0; i < iCount; i++ ) {
    pcunitDL = m_listDynLink.GetNext( po );
    pcunitDL->SetLoopDetectFlag( false );
  }
}

/////////////////////////////////////////////////////////////////////////
//
// SetLoopDetectFlag()
//
// Parameter :
//    CUnitBase * pcunit  : current process unit
//
// Return :
//    bool        : true if seccess set. else some thing was wrong.
//
// Description :
//   ����Ҷ�̬���ӵ�Ŀ�ĵ�Ԫ���Ǳ����Եĵ�Ԫ��������Ŀ�ĵ�Ԫ�ķ���ѭ����־
//   �������Ԫ���ڷ�װ��Ĳ������������Ƿ���ڶ�̬���ӣ���Զ���سɹ���
//
/////////////////////////////////////////////////////////////////////////
bool CUnitBase::SetLoopDetectFlag( CUnitBase * pcunit ) {
  CUnitDynLink * pcunitDL;
  POSITION po = m_listDynLink.GetHeadPosition();
	INT_PTR iCount = m_listDynLink.GetCount();

  for ( int i = 0; i < iCount; i++ ) {
    pcunitDL = m_listDynLink.GetNext( po );
    if ( pcunitDL->GetDestUnit() == pcunit ) {
      pcunitDL->SetLoopDetectFlag( true );
      return( true );
    }
  }
  if (this->IsEncapsulated()) {
    // ����Ƿ�װ��Ĳ�������������û�ж�̬����
    return(true);
  }
  ASSERT(false);  // �����⡣
  return( false );
}

////////////////////////////////////////////////////////////////
//
// LoopDetect()
//
// Parameter :
//    CUnitList * plistUnit : �����Եĵ�Ԫ���У����Ϊ�����С�������ֶ�̬���ӵ�ѭ������˵�Ԫ���а����γɵĵ�Ԫѭ������
//
// Return :
//    bool      : true if find loop or find cutoff.
//                false if not find loop or not find cutoff.
//
// Decription :
//    Check weither my dynamic link's link to unit is in a loop list
// 
//		���������̬���ӵ�Ŀ�ĵ�Ԫ�Ƿ��Ա���Ԫ��ΪĿ�ĵ�Ԫ������У��򷵻ط���ѭ����־�����û�У�����Ŀ�ĵ�Ԫ��Ŀ�ĵ�Ԫ�Ƿ��Ա���ԪΪĿ�ĵ�Ԫ��
//		����һ���ݹ���̡�
//		Ŀǰ�������ֻ����򵥵�Ԫ����δ�����װ��Ĳ�����(���ܴ���
//		��װ��Ĳ�������̳д˺����Ƚϼ����ˣ���ʵ�֣���
//
////////////////////////////////////////////////////////////////////////////
bool CUnitBase::LoopDetect(CUnitList * pCUnitList) {
  CUnitDynLink * pcunitDL;
  POSITION po = m_listDynLink.GetHeadPosition();
	INT_PTR i, iCount = m_listDynLink.GetCount();
  CUnitBase * pcunit;

  pCUnitList->AddTail( this );
  for ( i = 0; i < iCount; i++ ) {
    pcunitDL = m_listDynLink.GetNext(po);
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if (pcunit->GetUnitType() != tOUTPUT) {   // ����Ԫ�Ƿ�ֻ����������ӣ����������
      if (!pcunit->IsSetCutOff()) {   // ���û�����ýضϱ�־�������˽ضϱ�־�Ļ��Ͳ����ˣ�����һ����̬�����м����ң�
        if (pCUnitList->Find(pcunit)) {	// �ҵ���ѭ����
          // ������Ԫ���뵥Ԫ���У����ұ��淢����ѭ��
          pCUnitList->AddTail(pcunit);
          return(true);
        }
        else {
          pcunit = pcunitDL->GetDestUnit();
          if (pcunit->LoopDetect(pCUnitList)) { // �������������̬���ӵ�Ŀ�ĵ�Ԫ�Ƿ���ѭ������
            return(true);
          }
        }
      }
    }
  }
	// ���е���������û�з���ѭ����־
  pcunit = pCUnitList->RemoveTail(); // remove me from tail
  ASSERT( pcunit == this ); //���������ʱ������,�����ǵݹ���ɵģ����Ҳ����ˡ�(�ҵ������ˡ�������ά�������ȶ��ԵĹ��ߣ���������������
  return( false );
}

////////////////////////////////////////////////////////////////////////////
//
// ��鵥Ԫ�Ķ�̬ѭ�����Ƿ���ڽضϣ��Ѿ��ҵ���ѭ����
//
/////////////////////////////////////////////////////////////////////////////
bool CUnitBase::CheckCutOff(CUnitList * pCUnitList) {
  CUnitDynLink * pcunitDL;
  POSITION po2, po = m_listDynLink.GetHeadPosition();
  INT_PTR i, iCount = m_listDynLink.GetCount();
  CUnitBase * pcunit, *pcUnit4;
  long lTotal;

  pCUnitList->AddTail(this);
  for (i = 0; i < iCount; i++) {
    pcunitDL = m_listDynLink.GetNext(po);
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if (pcunit->GetUnitType() != tOUTPUT) {   // ����Ԫ�Ƿ�ֻ����������ӣ����������
      if (pCUnitList->Find(pcunit)) { // �ҵ��˶�̬����ѭ����
        po2 = pCUnitList->GetHeadPosition();
        lTotal = pCUnitList->GetCount();
        for (int j = 0; j < lTotal; j++) {
          pcUnit4 = pCUnitList->GetNext(po2);
          if (pcUnit4->IsSetCutOff()) {
            // ������Ԫ���뵥Ԫ���У����ұ��淢���˽ض�
            pCUnitList->AddTail(pcunit);
            return(true);
          }
        }
      }
      else {
        if (pcunitDL->GetDestUnit()->CheckCutOff(pCUnitList)) { // ����������
          return(true);
        }
      }
    }
  }
  // ���е���������û�з��ֽضϱ�־
  pcunit = pCUnitList->RemoveTail(); // remove me from tail
  ASSERT( pcunit == this ); //���������ʱ������,�����ǵݹ���ɵģ����Ҳ����ˡ�
  return(false);
}

////////////////////////////////////////////////////////////////
//
// CheckInnerDataLink()
//
// Parameter :
//    INT64  lSrcParaPos : �ڲ������������ӵ�Դλ�á�
//    CUnitList * plistUnit : �����Եĵ�Ԫ���У����Ϊ�����С�������ֶ�̬���ӵ�ѭ������˵�Ԫ���а����γɵĵ�Ԫѭ������
//
// Return :
//    bool      : true if find loop or find cutoff.
//                false if not find loop or not find cutoff.
//
// Decription :
//    ��鱾��Ԫ�Ķ�̬�����Ƿ����ӵ���������Ԫ�Ĳ������������еĻ����ñ�־�������з񣬶������������еĶ�̬���ӣ�ֱ�������ꡣ
// 
//		���������̬���ӵ�Ŀ�ĵ�Ԫ�Ƿ��������Ԫ�Ĳ���������У������ò����ڲ����ӱ�־��
//    �����з񣬶�����Ѱ��ֱ�����������еĶ�̬���ӡ�
//		����һ���ݹ���̡�
//		��װ��Ĳ�������̳д˺����Ƚϼ����ˣ���ʵ�֣���
//
//    ������ֻ�ڷ�װ����ʱ������,�������ñ������ᵼ�´��󡣵��ñ�����ʱ��������װ�Ѿ��������һ�����ʶ����������Ķ�̬�����Ѿ�������˴��벿������Ĳ����У�
//    �ʶ��������еĵ�Ԫ�����Ѿ������������������Ķ�̬���ӡ�
//
////////////////////////////////////////////////////////////////////////////
void CUnitBase::CheckInnerDataLink(INT64 lSrcParaPos, INT64 , CUnitList * pCUnitList) {
  CUnitDynLink * pcunitDL;
  POSITION po = m_listDynLink.GetHeadPosition();
  INT_PTR iCount = m_listDynLink.GetCount();
  CUnitBase * pcunit;
  CUnitBase *pFirstUnit = pCUnitList->GetHead();

	ASSERT(pCUnitList->GetCount() > 0); // ������ֱ�ӵ��ñ������������ɲ����෢���顣
  ASSERT(pFirstUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))); // �������������ɱ���Ĳ������õģ���һ����Ԫ���Ǳ���Ĳ�������
  for (int i = 0; i < iCount; i++) {
    pcunitDL = m_listDynLink.GetNext(po);
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if ((CUnitComponent *)pcunit == pFirstUnit) { // �ҵ���
      ((CUnitComponent *)pcunit)->SetInnerDataLinked(lSrcParaPos, pcunitDL->GetDestIndex(), true); // �����ڲ����ӱ�־
    }
    else {
      ASSERT((CUnitComponent *)pFirstUnit == pcunit->GetComponentUpper()); // �²㲿���Ѿ���װ����������������
      if (pcunit->GetUnitType() != tOUTPUT) {   // ����Ԫ�Ƿ�ֻ����������ӣ����������
        if (!pcunit->IsSetCutOff()) {   // ���û�����ýضϱ�־�������˽ضϱ�־�Ļ��Ͳ����ˣ�����һ����̬�����м����ң�
          //����������
          pCUnitList->AddTail(pcunit);
          pcunit->CheckInnerDataLink(lSrcParaPos, pcunitDL->GetDestIndex(), pCUnitList); // ����Ѱ��
        }
      }
    }
  }
  // ���е��������������걾��Ԫ������Ԫ�������еĶ�̬������
  pcunit = pCUnitList->RemoveTail(); // remove me from tail
}
  
////////////////////////////////////////////////////////////////////////
//
// SetDeleteDynLinkFlag()
//
// Parameter :
//    CUnitList & listUnit : list of destination units
// Return :
//    No return.
//
// Description :
//    if my DynLink->DestUnit is in listUnit, do nothing. else set DynLink's delete flag.
//    so when cut or copy this unit to clipboard, don't copy dynamic link that link to outer 
//    unit. call by EditCopy or EditCut.
//
///////////////////////////////////////////////////////////////////////////
void CUnitBase::SetDeleteDynLinkFlag( CUnitList& listUnit ) {
  CUnitDynLink * pcunitDL;
  POSITION po = m_listDynLink.GetHeadPosition();
	INT_PTR i, iCount = m_listDynLink.GetCount();
  CUnitBase * pcunit;

  for ( i = 0; i < iCount; i++ ) {
    pcunitDL = m_listDynLink.GetNext(po);
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if ( listUnit.Find( pcunit ) == nullptr ) { // ���Ŀ�ĵ�Ԫ������ѡ��ĵ�Ԫ������
      pcunitDL->SetDeleteMeFlag( true );			// �򱾵�Ԫ�Ķ�̬���������ӵ����ⲿ��Ԫ����Ҫɾ��
    }
  }
}

//////////////////////////////////////////////////////////////////////////
//
// ClearDeleteDynLinkFlag()
//
// Parameter :
//    No parameter
//
// Return :
//    No return.
//
// Description :
//    Clear unit dynamic link's delete flag. Call by EditCopy and EditCut.
//
///////////////////////////////////////////////////////////////////////////
void CUnitBase::ClearDeleteDynLinkFlag( void ) {
  CUnitDynLink * pcunitDL;
  POSITION po = m_listDynLink.GetHeadPosition();
	INT_PTR i, iCount = m_listDynLink.GetCount();

  for ( i = 0; i < iCount; i++ ) {
    pcunitDL = m_listDynLink.GetNext(po);
    pcunitDL->SetDeleteMeFlag( false );
  }
}

/////////////////////////////////////////////////////////////////
//
// DrawBitmap()
//
//
//
//
/////////////////////////////////////////////////////////////////
void CUnitBase::DrawBitmap( CDC * pdc, CBitmap& bitmap, CRect& rect ) {
  CDC dcMem;
  CBitmap * pbitmap;

  dcMem.CreateCompatibleDC(nullptr);
  pbitmap = dcMem.SelectObject( &bitmap );
  pdc->BitBlt(rect.left, 
              rect.top, 
              rect.Width(), 
              rect.Height(),
              &dcMem,
              0,
              0,
              SRCCOPY);
  dcMem.SelectObject( pbitmap );
  dcMem.DeleteDC();
}

/////////////////////////////////////////////////////////////////////////////
//
//
//
/////////////////////////////////////////////////////////////////////////////
void CUnitBase::PrepareParaDictionary( CDicList &CListDic, ULONG ulType ) {
	SelectParameter( ulType );
	CreateParaDictionary( CListDic, ulType );
}

void CUnitBase::CreateParaDictionary( CDicList &CListDic, ULONG ulType ) {
	CUnitDictionary * pDic;
	ParaName * pParaName = GetParaNameAddress();
	int i = 0;
  ULONG ulTypeLowPart, ulTypeHighPart;

  ulTypeLowPart = ulType & 0x00FF;
  ulTypeHighPart = ulType & 0xFF00;

  ASSERT(ulType & (tMODIFIABLE | tINPUT | tOUTPUT));
  while (pParaName[i].ulType != 0) {  //���в���
    if (((pParaName[i].ulType | ulTypeHighPart) == pParaName[i].ulType) && (ulTypeLowPart & GetDynLinkType(i))) { //��������?
      if ( (ulType & (tINPUT | tOUTPUT)) == tINPUT ) {		// ѡ��д��(INPUT)����? 
				if ( (m_pfSelected[i] == false) && (pParaName[i].ulType & tINPUT) ) { // ��д���������û�б�ѡ���.
					pDic = new CUnitDictionary( this, pParaName[i].ulIndex, pParaName[i].ulType );
					CListDic.AddTail( pDic );
				}
			}
			else if ( (ulType & (tINPUT | tOUTPUT)) == tOUTPUT ) {	// ѡ�񱻶���(OUTPUT)����, ���������������������.
				if ( pParaName[i].ulType & tOUTPUT ) { 
					pDic = new CUnitDictionary( this, pParaName[i].ulIndex, pParaName[i].ulType );
					CListDic.AddTail( pDic );
				}
			}
			else if ( (ulType & tMODIFIABLE) == tMODIFIABLE ) {	// ѡ�����еĲ���,����������������.
				if ( (pParaName[i].ulType & tMODIFIABLE) && (m_pfSelected[i] == false) ) { // ��д���������û�б�ѡ���.
					pDic = new CUnitDictionary( this, pParaName[i].ulIndex, pParaName[i].ulType );
					CListDic.AddTail( pDic );
				}
			}
			else { // ����.
				ASSERT( 0 );
			}
		}
		i++;
	}
}

void CUnitBase::SelectParameter(ULONG ulType) {
  ULONG i = 0;
  ULONG j = 0;
  ParaName * pParaName = GetParaNameAddress();
  INT32 * pulIndex = GetArrayIndex();
  ULONG ulTypeLowPart, ulTypeHighPart;

  ulTypeLowPart = ulType & 0x00FF;
  ulTypeHighPart = ulType & 0xFF00;

  int k = 0;
  while (pParaName[k].ulType != 0) {
    pulIndex[k++] = -1;
  }
  ASSERT(ulType & (tMODIFIABLE | tINPUT | tOUTPUT));
  while (pParaName[i].ulType != 0) {  //���в���
    if ( ((pParaName[i].ulType | ulTypeHighPart) == pParaName[i].ulType) && (ulTypeLowPart & GetDynLinkType(i)) ) { //��������?
      if ((ulType & (tINPUT | tOUTPUT)) == tINPUT) {		// ѡ��д��(INPUT)����? 
				if ( (m_pfSelected[i] == false) && (pParaName[i].ulType & tINPUT) ) { // ��д���������û�б�ѡ���.
      		pulIndex[j++] = pParaName[i].ulIndex;
				}
			}
			else if ( (ulType & (tINPUT | tOUTPUT)) == tOUTPUT ) {	// ѡ�񱻶���(OUTPUT)����, ���������������������.
				if ( pParaName[i].ulType & tOUTPUT ) {
					pulIndex[j++] = pParaName[i].ulIndex;
				}
			}
			else if ( (ulType & tMODIFIABLE) == tMODIFIABLE ) { 		// ѡ�����еĲ���, ����������������.
				if ( (pParaName[i].ulType & tMODIFIABLE) && (m_pfSelected[i] == false) ) { // ��д���������û�б�ѡ���.
					pulIndex[j++] = pParaName[i].ulIndex;
				}
			}
			else { // ����.
        ASSERT(0); // ѡ��Խ��
        return;
			}
		}
		i++;
	}
}

bool CUnitBase::CanLinkIn( void ) {
	return( false );
}


