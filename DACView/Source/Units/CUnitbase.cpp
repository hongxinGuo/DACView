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

using namespace std;
#include<algorithm>

#include "..\\resource.h"

#include "globedef.h"

#include "CUnitBase.h"
#include "CUnitComponent.h" 
#include "CUnitDynLink.h"

#include "DlgUnitDeleteDynLink.h"

using namespace std;
#include<memory>

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

  m_lScanRate = 100; // 默认扫描速率为100ms每次
  m_fAutoExective = true;

  m_lDynLinkToNumber = 0;
  m_lCountDLToNumber = 0;

  m_lExectivePriority = 0;
  m_fCompiled = false;
  m_fEncapsulating = false;

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
  m_fEncapsulating = false;

	m_lLinkToComponent = 0;
  m_lLinkFromComponent = 0;
	m_lLinkFromObject = 0;

  m_fOverFlow = false;

  m_lReserved3 = 0;
}

CUnitBase::~CUnitBase() {
  // release list's memory
  m_listDynLink.clear();
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
  CUnitDynLink * pcDynLinkTag;

  INT64 iTemp;

  if (ar.IsStoring())
  {
    // TODO: add storing code here
		iTemp = 0;
		for ( auto pcDynLink : m_listDynLink ) {
      if ( !pcDynLink->IsDeleteMe() ) iTemp++;
		}
		ar << iTemp;
    for ( auto pcDynLink : m_listDynLink ) { 
      // when cut or copy me to clipboard, some dynamic links that I have
      // can't copy to clipboard, for its link to other unit. so if its 
      // delete flag is set, I didn't store it.
      if (!pcDynLink->IsDeleteMe()) {
        pcDynLinkTag = pcDynLink.get();
        ar << pcDynLinkTag;
      }
    }
	}
	else
	{
		// TODO: add loading code here
		ar >> iTemp;
		for (int i = 0; i < iTemp; i++) {
      shared_ptr<CUnitDynLink> pcDynLink;
			ar >> pcDynLinkTag;
      pcDynLink.reset(pcDynLinkTag);
			m_listDynLink.push_back(pcDynLink);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
// 单元的名称为包含自己的部件（如果有的话）名称加上自己的名字
//
//
//
//////////////////////////////////////////////////////////////////////////
CString CUnitBase::GetName(void)
{
  // TODO: 在此处插入 return 语句
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
		if (m_lLinkToComponent > 0) { //如果没有链接时，则什么事情都不要做（似乎应该报错）
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
    if (m_lLinkFromComponent > 0) { //如果没有链接时，则什么事情都不要做(似乎应该报错）
      m_lLinkFromComponent--;
    }
  }
}

void CUnitBase::SetLinkFromObjectFlag(bool fFlag) {
	if (fFlag == true) {
		m_lLinkFromObject++;
	}
	else {
		if (m_lLinkFromObject > 0) { //如果没有链接时，则什么事情都不要做(似乎应该报错）
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

bool CUnitBase::IsCutoff(void) const {
	return(m_fCutOff);
}

//////////////////////////////////////////////////////////////////
//
// GetInputParameterNumber()
//
// return : 
//		ULONG	: 输入参数连接的个数.
//
// 描述:
//		得到输入参数连接的个数,输入参数连接是指连入本单元的输入参数上的连接.
//
/////////////////////////////////////////////////////////////////////
ULONG	CUnitBase::GetInputParameterNumber(void) {
	return(m_lDynLinkToNumber);
}

//////////////////////////////////////////////////////////////////
//
// SetInputParameterNumber()
//	
// 参数 	: 输入参数连接的个数.
// return : 无
//		
//
// 描述:
//		得到输入参数连接的个数,输入参数连接是指连入本单元的输入参数上的连接.
//
/////////////////////////////////////////////////////////////////////
void	CUnitBase::SetInputParameterNumber(LONG lNumber) {
	ASSERT(lNumber >= 0);
	m_lDynLinkToNumber = lNumber;
}

///////////////////////////////////////////////////////////////////
//
// 本单元是否存在从上层联入的参数
// 返回值：
//      true：本单元存在从上层输入的动态链接
//		  false：本单元不存在从上层输入的动态链接
//
///////////////////////////////////////////////////////////////////
bool CUnitBase::IsDynLinkFromUpper(void) {
	CUnitList * pList;
	INT_PTR iDynLinkNum = 0;
	CUDLList * pDynLinkList;
	if ( m_pUnitComponentUpper != nullptr ) { // 本单元被包含于部件中
    // 统计部件内部单元序列中数据输出至本单元的数量
		pList = m_pUnitComponentUpper->GetUnitList();
		for (const auto pUnit : *pList) {
			pDynLinkList = pUnit->GetDynLinkList();
			for ( auto pcunitDynLink : *pDynLinkList ) {
				if (pcunitDynLink->GetDestUnit() == this) {
					iDynLinkNum++;
				}
			}
		}
    // 计算部件参数处链入本单元的数量
    for (int i = 0; i < 16; i++) {
      if (m_pUnitComponentUpper->IsParaLinked(i)) {
        if (m_pUnitComponentUpper->GetParaDestUnit(i) != nullptr) { // 数据输出至内部单元
          if (m_pUnitComponentUpper->GetParaDestUnit(i) == this) { // 输出至本单元？
            iDynLinkNum++;
          }
        }
      }
    }
		if (iDynLinkNum < m_lDynLinkToNumber) { // 本单元在本层的输入链接数量小于本单元的总输入参数数量？
			return(true); // 那本单元就有从上层输入的动态链接
		}
		else {
			ASSERT(iDynLinkNum == m_lDynLinkToNumber);	// 本层的输入动态链接不能大于总输入动态链接
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
  CUnitBase * punit = nullptr;
  
  for ( auto pcunitDynLink : m_listDynLink ) {
    punit = pcunitDynLink->GetDestUnit();
    if (!punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // 如果是简单单元
      punit->SetExectivePriority(m_lExectivePriority + 1); // 设置此单元的执行优先级至少为m_lExectivePriority+1
    }
    else if ((CUnitComponent *)punit != m_pUnitComponentUpper) { // 如果此目的部件不是包含我的上级部件
      punit->SetExectivePriority(m_lExectivePriority + 1);
    }
  }                                                                                  
}

/////////////////////////////////////////////////////////////////////////////
//
// SetExectivePriority()
//
// Parameter :
//    ULONG ulPriority : 当前优先级的值
//
// 返回值：
//    bool : 如果设置了本单元的执行优先级则为真，否则为假
//
// Description :
//   如果本单元无输入连接, 则设置本单元的优先值为1.
// 否则当所有的输入参数连接都找到后,才设置本单元的优先值.
// 截断单元不允许使用此函数设置运行优先值.设置截断单元的执行优先级需要调用SetExectivePriorityDirect()函数。
//
///////////////////////////////////////////////////////////////////////////
bool CUnitBase::SetExectivePriority( ULONG ulPriority ) {
	ULONG ulUnitType = GetUnitType();
	ASSERT( ulUnitType != tOUTPUT );			// 保证本单元为输入输出单元. 
  if ( m_lDynLinkToNumber == 0 ) {  // 如果本单元没有联入单元.
    m_lExectivePriority = 1; // 优先执行本单元.
    return true;
  }
	if ( m_fCutOff ) return false;	// 截断单元不允许使用此函数设置运行优先值.设置截断单元的执行优先级需要调用SetExectivePriorityDirect()函数。
  // 直到所有联入本单元的单元处理完后,本单元的处理优先值才可以设置.
  if (++m_lCountDLToNumber == m_lDynLinkToNumber) {
    m_lExectivePriority = ulPriority;
    return true;
  }
	ASSERT( m_lCountDLToNumber < m_lDynLinkToNumber );
  ASSERT(m_lDynLinkToNumber > 0);
  return false;
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
//		生成唯一的名称
//
///////////////////////////////////////////////////////////////////////////
bool CUnitBase::CreateUniName( CUnitList& listUnit ) {
  bool fFind = false;
	INT_PTR iTemp = 1;
  CString strNumber;
  char s[10];

  for (const auto pcunit : listUnit) {
    if ( m_strName == pcunit->GetName() ) {
      fFind = true;
      break;
    }
  }

  bool fDone = false;
  if ( fFind ) {
    while ( !fDone ) {
      _itoa_s(iTemp++, s, 10);
      m_strName = GetClassNameStr() + s;
      fDone = true;
      for (const auto pcunit : listUnit) {
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
//    Adjust my dynamic link's position according changed position，
// 如果本动态链接的类型为部件联至简单单元或者部件，则不需要调整动态链接线的位置。
// 
//
////////////////////////////////////////////////////////////////////////
void CUnitBase::AdjustDynLinkLinePosition(CUnitBase * pcSrc, CPoint ptStart, CPoint ptEnd) {
  shared_ptr<CPoint> ppt1, ppt2;
  CPointList * plist;

  if (pcSrc == this) {  // if I was been changed size
    for (auto pDL : m_listDynLink) {
			if ((pDL->GetDynLinkClass() == COMPONENT_TO_UNIT) || (pDL->GetDynLinkClass() == COMPONENT_TO_COMPONENT))
				break; //当联出本单元所在的复合单元时，不需要调整动态链接线的位置（调整了就出错了）
      plist = pDL->GetLinkPointList();
      auto itPoint = plist->begin();
      ppt1 = *itPoint++;
      ppt2 = *itPoint++;
      if ( ppt1->x == ppt2->x ) {	// 从单元引出的动态连接线是竖直的.
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
			// 如果单元改变了大小,则还要处理ppt1.
			if ( ppt1->x == ppt2->x ) {	// 竖直线.
				if ( ppt2->y > m_rectArea.bottom ) {
					ppt1->y = m_rectArea.bottom;
				}
				else if ( ppt2->y < m_rectArea.top ) {
					ppt1->y = m_rectArea.top;
				}
			}
			else if ( ppt1->y == ppt2->y ) {	// 水平线.
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
  bool fDo;
  CUnitBase * pcunit;
  CRect rect, rectSrc;
  for ( auto pDL : m_listDynLink ) {
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
      auto it = plist->end();
      it--;
      ppt1 = *it--;
      ppt2 = *it--;
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

			// 如果单元改变了大小,则还要处理ppt1.
			if ( ppt1->x == ppt2->x ) {	// 竖直线.
				if ( ppt2->y > rectSrc.bottom ) {
					ppt1->y = rectSrc.bottom;
				}
				else if ( ppt2->y < rectSrc.top ) {
					ppt1->y = rectSrc.top;
				}
			}
			else if ( ppt1->y == ppt2->y ) {	// 水平线.
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

void CUnitBase::AddDynLink( shared_ptr<CUnitDynLink> punitDynLink ) {
  m_listDynLink.push_back(punitDynLink); 
}  

/////////////////////////////////////////////////////////////////////////
//
// BreakDynLink()
//
//return : 当删除了动态连接时返回true
//  			 当没删除动态连接时返回false
//
// Description :
//    Call Dialog delete dynamic link to delete my dynamic links.
//
/////////////////////////////////////////////////////////////////////////
bool CUnitBase::ArrangeDynLink( void ) {
  CDelDynLink cDialogDel;
	INT_PTR iTotal = m_listDynLink.size();

  cDialogDel.SetLink( &m_listDynLink );
  cDialogDel.DoModal();
	if ( iTotal != m_listDynLink.size() ) {
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
  
  if (m_listDynLink.size() == 0) return true;

  auto it = m_listDynLink.begin();
  do {
    auto pDL = *it;
    CUnitBase * pcunit = pDL->GetDestUnit();
    if ( pUnit == pcunit ) {
      pcunit->SetParameterLock( pDL->GetDestIndex(), false );
     it = m_listDynLink.erase(it);
    }
    else it++;
  } while (it != m_listDynLink.end());
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
  CUnitBase * pUnit;

  for ( const auto pUnitDynLink : m_listDynLink ) {
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
// ExectiveDynLink() 执行动态链接序列
//
// Return : 
//    bool    : always true.
//
// Description :
//    Exective my dynamic links.
//
///////////////////////////////////////////////////////////////////////////
bool CUnitBase::ExectiveDynLink( void ) {
  ULONG ulSourceIndex, ulDestIndex;
  double eTemp;
  LONG lTemp;
  bool fTemp;
	CString strTemp;

  for ( const auto pcunitDynLink : m_listDynLink ) {
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
// CUnitBase类及其大部分衍生类允许联入联出动态链接，输入参数和输出参数皆存在。
// 有少数衍生类（目前只有CUnitSine、CUnitInputOutput、CUnitQuad这三个类）只存在输出参数，而不存在输入参数，即不允许链接至这三个函数。
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

bool CUnitBase::IsParameterLocked(ULONG )
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
//    改变ulIndex指向的参数连接状态. 并设置或清除参数锁.
//  当生成或删除动态连接时,总是要调用此函数. 
//
//////////////////////////////////////////////////////////////////////////
bool CUnitBase::SetParameterLock( ULONG ulIndex, bool fSelected ) {
  ParaName * pParaName = GetParaNameAddress();
	// 此函数只用于输入参数.
  if (pParaName[ulIndex].ulType & tINPUT) {
    if (fSelected == true) {		// 设置已选择标志
      ASSERT(m_vfSelected[ulIndex] == false); // 不允许二次设置，输入型参数只能链接一次
      m_vfSelected[ulIndex] = true;
      m_lDynLinkToNumber++;
    }
    else {		// 清除已选择标志
      ASSERT(m_vfSelected[ulIndex] == true);
      m_vfSelected[ulIndex] = false;
      m_lDynLinkToNumber--;
    }
    return(true);
  }
  else return(false);
}

///////////////////////////////////////////////////////////////////////////////////////
//
// 设置输入型参数是否被选择的标志。
//
// 此函数只在生成Object->Unit的动态链接时才被调用，而且数据方向为输入至单元。此时不能增加m_lDynLinkToNumber
// 的数值，否则无法正常编译（尚未测试）。
//
//
////////////////////////////////////////////////////////////////////////////////////////
bool CUnitBase::SetParameterSelected(ULONG ulIndex, bool fSelected) {
  ParaName * pParaName = GetParaNameAddress();
  // 此函数只用于输入参数.
  if (pParaName[ulIndex].ulType & tINPUT) {
    if (fSelected == true) {		// 设置已选择标志
      ASSERT(m_vfSelected[ulIndex] == false); // 不允许二次设置，输入型参数只能链接一次
      m_vfSelected[ulIndex] = true;
			SetLinkFromObjectFlag(true);
    }
    else {		// 清除已选择标志
      ASSERT(m_vfSelected[ulIndex] == true);
      m_vfSelected[ulIndex] = false;
			SetLinkFromObjectFlag(false);
    }
    return(true);
  }
  else return(false);
}


////////////////////////////////////////////////////////////////////////////////////////////
//
// 自我检查本单元数据的完整性，主要时动态链接类型是否匹配
//
//
///////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitBase::CheckSelf( void ) {
	CUnitBase * pSrc, * pDest;
	ULONG ulSrcType, ulDestType, ulType;

	for ( const auto pUDL : m_listDynLink ) {
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

//////////////////////////////////////////////////////////////////////////////////////
//
// 编译自己。此函数用于编译部件本身，其他单元只是设置编译标志
//
//
//////////////////////////////////////////////////////////////////////////////////////
bool CUnitBase::Compilation(void)
{
  m_fEncapsulating = false;
  m_fCompiled = true;

  return false;
}

////////////////////////////////////////////////////////////////////
//
// 此函数只对部件类有效，其他类都报告true。
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
  if ( m_fOverFlow ) { // 溢出单元的边框为红色
    cp2.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));	// 溢出的单元为红色.
    pdc->SelectObject( &cp2 );
    pdc->MoveTo( m_rectArea.left, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.top );
  }
  else if ( m_fCutOff  ) { // 有截断的单元的边框为黄色
    cp3.CreatePen(PS_SOLID, 2, RGB(255, 255, 0));	// 有截断的单元为黄色.
    pdc->SelectObject( &cp3 );
    pdc->MoveTo( m_rectArea.left, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.top );
  }
  else {
    cp2.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));	// 正常的单元为兰色.
    pdc->SelectObject( &cp2 );
    pdc->MoveTo( m_rectArea.left, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.top);
    pdc->LineTo( m_rectArea.right-1, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.bottom-1 );
    pdc->LineTo( m_rectArea.left, m_rectArea.top );
  }

  // 如果从部件参数处输入数据,则显示一个蓝色方块于左上方.
  if (m_lLinkFromComponent > 0) {
    CBrush brhBlue(RGB(0, 0, 255));
    rect.top = m_rectArea.top;
    rect.left = m_rectArea.left;
    rect.bottom = rect.top + 8;
    rect.right = rect.left + 8;
    pdc->FillRect(rect, &brhBlue);
  }

	// 如果将数据输出至部件的参数处,则显示一个蓝色方块于右上方.
	if ( m_lLinkToComponent > 0 ) {
		CBrush brhBlue( RGB(0, 0, 255) );
		rect.top = m_rectArea.top;
		rect.left = m_rectArea.right - 8;
		rect.bottom = rect.top + 8;
		rect.right = m_rectArea.right;
		pdc->FillRect( rect, &brhBlue );
	}

	// 如果存在COMPONENT_TO_UNIT或者COMPONENT_TO_COMPONENT，则显示一个蓝色方块于右下方
	bool fFind = false;
  for (const auto pUnitDynLink : m_listDynLink) {
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
      // 目前什么也不做，研究之
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

	// 如果存在UNIT_TO_COMPONENT、COMPONENT_TO_COMPONENT或者从部件参数联入数据，则显示一个蓝色方块于左下方
	if (IsDynLinkFromUpper()) {
		CBrush brhBlue(RGB(0, 0, 255));
		rect.top = m_rectArea.bottom - 8;
		rect.left = m_rectArea.left;
		rect.bottom = m_rectArea.bottom;
		rect.right = m_rectArea.left + 8;
		pdc->FillRect(rect, &brhBlue);
	}

	// 如果存在从Object处联入数据，则显示一个蓝色方块于左边中部
	if (IsLinkFromObject()) {
		CBrush brhBlue(RGB(0, 0, 255));
		rect.top = (m_rectArea.bottom + m_rectArea.top)/2 - 4;
		rect.left = m_rectArea.left;
		rect.bottom = rect.top + 8;
		rect.right = m_rectArea.left + 8;
		pdc->FillRect(rect, &brhBlue);
	}

	// 如果被选择了，则显示选择标志
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
  for ( const auto pUnitDynLink : m_listDynLink ) { 
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
    case UNIT_TO_CptPARAMETER : // 此类型在封装部件后才会出现于已封装部件的内部单元序列的动态链接中，不用显示其动态链接线。而且其动态链接线应该是空的。
      ASSERT(pUnitDynLink->GetLinkPointList()->size() == 0); // 此类型的动态连接线序列已经移到了包含其的部件处。
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
  UnitList.push_back(this);
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
  for ( const auto pc : m_listDynLink  ) {
    dc << pc.get() << "\n";
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

// 设置注释
void CUnitBase::SetComment(const CString& strComment) {
	m_cstrComment = strComment;
}

//得到注释字符串
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
  for ( const auto pcunitDL : m_listDynLink ) {
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
//   如果我动态链接的目的单元就是被测试的单元，就设置目的单元的发现循环标志
//   如果本单元属于封装后的部件，则无视是否存在动态链接，永远返回成功。
//
/////////////////////////////////////////////////////////////////////////
bool CUnitBase::SetLoopDetectFlag( CUnitBase * pcunit ) {
  for ( const auto pcunitDL : m_listDynLink ) {
    if ( pcunitDL->GetDestUnit() == pcunit ) {
      pcunitDL->SetLoopDetectFlag( true );
      return( true );
    }
  }
  if (this->IsEncapsulated()) {
    // 如果是封装后的部件，则无视有没有动态链接
    return(true);
  }
  ASSERT(false);  // 有问题。
  return( false );
}

////////////////////////////////////////////////////////////////
//
// LoopDetect()
//
// Parameter :
//    CUnitList * plistUnit : 被测试的单元序列，最初为空序列。如果发现动态链接的循环，则此单元序列包含形成的单元循环序列
//
// Return :
//    bool      : true if find loop or find cutoff.
//                false if not find loop or not find cutoff.
//
// Decription :
//    Check weither my dynamic link's link to unit is in a loop list
// 
//		检查我所动态链接的目的单元是否以本单元做为目的单元，如果有，则返回发现循环标志；如果没有，则检查目的单元的目的单元是否以本单元为目的单元。
//		这是一个递归过程。
//		目前这个函数只处理简单单元，尚未处理封装后的部件。(已能处理）
//		封装后的部件虚拟继承此函数比较简单明了（已实现）。
//
////////////////////////////////////////////////////////////////////////////
bool CUnitBase::LoopDetect(CUnitList * pCUnitList) {
  CUnitBase * pcunit;

  pCUnitList->push_back( this );
  for ( const auto pcunitDL : m_listDynLink ) {
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if (pcunit->GetUnitType() != tOUTPUT) {   // 本单元是否只允许输出链接？如有则忽略
      if (!pcunit->IsCutoff()) {   // 如果没有设置截断标志（设置了截断标志的话就不找了，在下一个动态链接中继续找）
        if (find(pCUnitList->begin(), pCUnitList->end(), pcunit) != pCUnitList->end()) {	// 找到了循环？
          // 将本单元加入单元序列，并且报告发现了循环
          pCUnitList->push_back(pcunit);
          return(true);
        }
        else {
          pcunit = pcunitDL->GetDestUnit();
          if (pcunit->LoopDetect(pCUnitList)) { // 继续检查我所动态链接的目的单元是否有循环链接
            return(true);
          }
        }
      }
    }
  }
	// 运行到这里，则表明没有发现循环标志
  pcunit = pCUnitList->back();  // 
  pCUnitList->pop_back();       // remove me from tail
  ASSERT( pcunit == this ); //这个断言有时出问题,估计是递归造成的，暂且不用了。(找到问题了。断言是维护程序稳定性的工具，不能随便清除）。
  return( false );
}

////////////////////////////////////////////////////////////////////////////
//
// 检查单元的动态循环中是否存在截断（已经找到了循环）
//
/////////////////////////////////////////////////////////////////////////////
bool CUnitBase::CheckCutOff(CUnitList * pCUnitList) {
  CUnitBase * pcunit;

  pCUnitList->push_back(this);
  for ( const auto pcunitDL : m_listDynLink ) {
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if (pcunit->GetUnitType() != tOUTPUT) {   // 本单元是否只允许输出链接？如有则忽略
      if (find(pCUnitList->begin(), pCUnitList->end(), pcunit) != pCUnitList->end()) { // 找到了动态链接循环？
        for (const auto pcUnit3 : *pCUnitList) {
          if (pcUnit3->IsCutoff()) { //如果此循环中存在截断，
            // 将本单元加入单元序列，并且报告发现了截断
            pCUnitList->push_back(pcunit);
            return(true);
          }
        }
      }
      else {
        if (pcunitDL->GetDestUnit()->CheckCutOff(pCUnitList)) { // 继续往下找
          return(true);
        }
      }
    }
  }
  // 运行到这里，则表明没有发现截断标志
  pcunit = pCUnitList->back();
  pCUnitList->pop_back(); // remove me from tail
  ASSERT( pcunit == this ); //这个断言有时出问题,估计是递归造成的，暂且不用了。
  return(false);
}

////////////////////////////////////////////////////////////////
//
// CheckInnerDataLink()
//
// Parameter :
//    INT64  lSrcParaPos : 内部参数数据链接的源位置。
//    CUnitList * plistUnit : 被测试的单元序列，最初为空序列。如果发现动态链接的循环，则此单元序列包含形成的单元循环序列
//
// Return :
//    bool      : true if find loop or find cutoff.
//                false if not find loop or not find cutoff.
//
// Decription :
//    检查本单元的动态链接是否链接到包含本单元的部件参数处，有的话设置标志。无论有否，都继续遍历所有的动态链接，直到遍历完。
// 
//		检查我所动态链接的目的单元是否包含本单元的部件，如果有，则设置参数内部链接标志。
//    无论有否，都继续寻找直到遍历完所有的动态链接。
//		这是一个递归过程。
//		封装后的部件虚拟继承此函数比较简单明了（已实现）。
//
//    本函数只在封装部件时被调用,单独调用本函数会导致错误。调用本函数时，部件封装已经到达最后一步，故而联入联出的动态链接已经被打断了并且存入部件本身的参数中，
//    故而本部件中的单元序列已经不存在跳出本部件的动态链接。
//
////////////////////////////////////////////////////////////////////////////
void CUnitBase::CheckInnerDataLink(INT64 lSrcParaPos, INT64 , CUnitList * pCUnitList) {
  CUnitBase * pcunit;
  CUnitBase *pFirstUnit = pCUnitList->front();

	ASSERT(pCUnitList->size() >> 0); // 不允许直接调用本函数，必须由部件类发起检查。
  ASSERT(pFirstUnit->IsKindOf(RUNTIME_CLASS(CUnitComponent))); // 这个函数最初是由被查的部件调用的，第一个单元就是被查的部件本身。
  for ( const auto pcunitDL : m_listDynLink ) {
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if (pcunit == pFirstUnit) { // 找到了
      ((CUnitComponent *)pcunit)->SetInnerDataLinked(lSrcParaPos, pcunitDL->GetDestIndex(), true); // 设置内部链接标志
    }
    else {
      ASSERT((CUnitComponent *)pFirstUnit == pcunit->GetComponentUpper()); // 下层部件已经封装，不允许跳出本层
      if (pcunit->GetUnitType() != tOUTPUT) {   // 本单元是否只允许输出链接？如有则忽略
        if (!pcunit->IsCutoff()) {   // 如果没有设置截断标志（设置了截断标志的话就不找了，在下一个动态链接中继续找）
          //接着往下找
          pCUnitList->push_back(pcunit);
          pcunit->CheckInnerDataLink(lSrcParaPos, pcunitDL->GetDestIndex(), pCUnitList); // 继续寻找
        }
      }
    }
  }
  // 运行到这里，则表明遍历完本单元及本单元以下所有的动态链接了
  pCUnitList->pop_back(); // remove me from tail
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
  CUnitBase * pcunit;

  for ( const auto pcunitDL : m_listDynLink ) {
    pcunit = pcunitDL->GetDestUnit();    // get destination unit
    if (find(listUnit.begin(), listUnit.end(), pcunit) == listUnit.end()) { // 如果目的单元不在已选择的单元序列中
      pcunitDL->SetDeleteMeFlag( true );			// 则本单元的动态链接是链接到了外部单元，需要删除
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
  for ( const auto pcunitDL : m_listDynLink ) {
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
	shared_ptr<CUnitDictionary> pDic;
	ParaName * pParaName = GetParaNameAddress();
	int i = 0;
  ULONG ulTypeLowPart, ulTypeHighPart;

  ulTypeLowPart = ulType & 0x00FF;
  ulTypeHighPart = ulType & 0xFF00;

  ASSERT(ulType & (tMODIFIABLE | tINPUT | tOUTPUT));
  while (pParaName[i].ulType != 0) {  //仍有参数
    if (((pParaName[i].ulType | ulTypeHighPart) == pParaName[i].ulType) && (ulTypeLowPart & GetDynLinkType(i))) { //参数合适?
      if ( (ulType & (tINPUT | tOUTPUT)) == tINPUT ) {		// 选择被写入(INPUT)参数? 
				if ( (m_vfSelected[i] == false) && (pParaName[i].ulType & tINPUT) ) { // 被写入参数必须没有被选择过.
					pDic = make_shared<CUnitDictionary>( this, pParaName[i].ulIndex, pParaName[i].ulType );
					CListDic.push_back( pDic );
				}
			}
			else if ( (ulType & (tINPUT | tOUTPUT)) == tOUTPUT ) {	// 选择被读出(OUTPUT)参数, 被读出参数可输出无数次.
				if ( pParaName[i].ulType & tOUTPUT ) { 
					pDic = make_shared<CUnitDictionary>( this, pParaName[i].ulIndex, pParaName[i].ulType );
					CListDic.push_back( pDic );
				}
			}
			else if ( (ulType & tMODIFIABLE) == tMODIFIABLE ) {	// 选择所有的参数,包括输入和输出类型.
				if ( (pParaName[i].ulType & tMODIFIABLE) && (m_vfSelected[i] == false) ) { // 被写入参数必须没有被选择过.
					pDic = make_shared<CUnitDictionary>( this, pParaName[i].ulIndex, pParaName[i].ulType );
					CListDic.push_back( pDic );
				}
			}
			else { // 出错.
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
  while (pParaName[i].ulType != 0) {  //仍有参数
    if ( ((pParaName[i].ulType | ulTypeHighPart) == pParaName[i].ulType) && (ulTypeLowPart & GetDynLinkType(i)) ) { //参数合适?
      if ((ulType & (tINPUT | tOUTPUT)) == tINPUT) {		// 选择被写入(INPUT)参数? 
				if ( (m_vfSelected[i] == false) && (pParaName[i].ulType & tINPUT) ) { // 被写入参数必须没有被选择过.
      		pulIndex[j++] = pParaName[i].ulIndex;
				}
			}
			else if ( (ulType & (tINPUT | tOUTPUT)) == tOUTPUT ) {	// 选择被读出(OUTPUT)参数, 被读出参数可输出无数次.
				if ( pParaName[i].ulType & tOUTPUT ) {
					pulIndex[j++] = pParaName[i].ulIndex;
				}
			}
			else if ( (ulType & tMODIFIABLE) == tMODIFIABLE ) { 		// 选择所有的参数, 包括输入和输出类型.
				if ( (pParaName[i].ulType & tMODIFIABLE) && (m_vfSelected[i] == false) ) { // 被写入参数必须没有被选择过.
					pulIndex[j++] = pParaName[i].ulIndex;
				}
			}
			else { // 出错.
        ASSERT(0); // 选择越界
        return;
			}
		}
		i++;
	}
}

bool CUnitBase::CanLinkIn( void ) {
	return( false );
}


