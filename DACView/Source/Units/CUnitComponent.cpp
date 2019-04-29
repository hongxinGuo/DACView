//////////////////////////////////////////////////////////////////////////
//
// CUnitComponent implement file.
//
// 部件类是封装类，完成一定的功能。无法被看进去，只有外特性。部件数据的方向，向内部单元输出数据的为输入型，向外部单元输出数据的为输出型。
//
// 部件是构筑策略的基础,部件封装了内部细节,将功能和实现分成两部分, 使构筑层次结构成为可能.
//
//	部件与外界的联系，通过而且只通过其接口提供的参数数组来实现，内部的数据链接到接口参数数组中，提供给外界使用；内部需要的信息，也由外部通过接口参数数组
//  传递到部件内部。
//
//
// 部件的封装，是其最重要的一项功能。提供了部件化实现和调试的能力。
//
// 描述:
//    单独编译自己的单元序列.将链接入和链接出的动态链接提到此部件的参数上来，然后禁止外界看进去。
//
// 动作：
// 1.检验本Unitlist包含的动态链接中是否存在循环，存在的话退出并报告。
// 2.将所有的单元组成一个单独的单元序列.编译后的部件视同简单单元，尚未编译的或不允许编译的部件把自己的子单元和部件本身加入单元序列中。
// 3.每个单元封装前，都预先把自己下层的部件封装起来。
// 4.清除目标单元序列的内部编译标志后（从外部Paste过来的部件内部含有先前的编译标志，需要先清除掉），再次设置目标单元的有源标志.
// 5.前四个步骤是准备工作，在CompileUnitList函数中完成。
// 6.找到联入和联出本部件的动态链接，都链接到部件的参数数组上去。所有输出型的动态链接，都在部件的动态链接序列中新生成一个动态链接。
// 7.部件的联出联入方向，从内部看是输入性的，从外部看就是输出型，故而定义参数的输出方向，与从外部看的方向相同，即从外部看是输出型，则此参数为输出型参数。
// 8.封装后的部件天然具有数据截断效应，故而其m_fCutOff为真。
//   如果其内部单元序列的动态链接导致其接口的两个参数之间出现了数据链接，则部件就丧失了其固有的截断效应。
// 9.固件本身的运行优先级，封装后由上层单元序列决定。固件执行时，首先执行输入参数的读取（已经完成，可不做）；
//   然后将输入型数据存储到内部相关单元的参数中；
//   接着执行内部单元序列。固件内部单元序列的执行优先级，独立确定，不受上层单元序列的影响。
//   最后执行输出型参数的数据输出以及部件本身的动态链接（只有输出类型，是封装后的部件再生成的动态链接）。
//10.封装后的部件在进行动态链接时，输入型的修改相关的参数内容（因为输入型只允许一个动态链接）；
//   输出型则建立新的动态链接，存储与部件本省的动态链接序列中。
//
//  实现:
//  1. 每个部件最多允许有16个输入或输出参数.
//
//
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

using namespace std;
#include<memory>
#include<algorithm>

#include"CUnitBase.h"
#include "cUnitComponent.h"
#include "CUnitDynLink.h"
#include"DlgComponent.h"

#include"CompileUnitList.h"

IMPLEMENT_SERIAL(CUnitComponent, CUnitBase, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUnitComponent::CUnitComponent(const CString& Name, CPoint pt, bool fEncapsualtionPermitted): CUnitBase(Name, pt) {
	m_fEncapsulationPermitted = fEncapsualtionPermitted;
	m_lScanRate = 100;		// 初始扫描速率为每次100ms。
	m_fCanViewIn = true;	// 初始时可以被看进去，编译后则无法看进去了。
	m_fEncapsulated = false;		// 是否被编译过。

	m_rectArea.left = pt.x - 40;
	m_rectArea.right = pt.x + 40;
	m_rectArea.top = pt.y - 20;
	m_rectArea.bottom = pt.y + 20;
	m_lScanRate = 10000000;

  m_lReserved11 = m_lReserved12 = 0;
  CString strName = "Para";
  char buffer[50];

	for ( int i = 0; i < 16; i++ ) {
		m_aulSuitable[i] = false;
	  m_pInterfacePara[i] = new CUCPara;
    _itoa_s(i, buffer, 10);
    m_pInterfacePara[i]->SetName(strName + buffer);
	}

  m_lInputParaNumber = m_lOutputParaNumber = 0;

	for (int i = 0; i < 16; i++) {
		m_vfSelected.push_back(false);
	}

}

CUnitComponent::CUnitComponent(bool fEncapsualtionPermitted) : CUnitBase() {
	m_fEncapsulationPermitted = fEncapsualtionPermitted;
	m_lScanRate = 100; // 100毫秒每次
	m_fCanViewIn = true;
	m_fEncapsulated = false;
  
  CString strName = "Para";
  char buffer[50];
	for (int i = 0; i < 16; i++) {
		m_aulSuitable[i] = false;
	  m_pInterfacePara[i] = new CUCPara;
    _itoa_s(i, buffer, 10);
    m_pInterfacePara[i]->SetName(strName + buffer);
	}
  m_lReserved11 = m_lReserved12 = 0;

  m_lInputParaNumber = m_lOutputParaNumber = 0;

	for (int i = 0; i < 16; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitComponent::CUnitComponent( void ) : CUnitBase() {
	m_fEncapsulationPermitted = true;
	m_lScanRate = 100; // 100毫秒每次
	m_fCanViewIn = true;
	m_fEncapsulated = false;

  CString strName = "Para";
  char buffer[50];
	for ( int i = 0; i < 16; i++ ) {
		m_aulSuitable[i] = false;
	  m_pInterfacePara[i] = new CUCPara;
    _itoa_s(i, buffer, 10);
    m_pInterfacePara[i]->SetName(strName + buffer);
	}
  m_lReserved11 = m_lReserved12 = 0;

  m_lInputParaNumber = m_lOutputParaNumber = 0;

	for (int i = 0; i < 16; i++) {
		m_vfSelected.push_back(false);
	}
}

CUnitComponent::~CUnitComponent() {
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i] != nullptr) {
      delete m_pInterfacePara[i];
    }
  }

	for (auto pcunit : m_CUnitList) {
		delete pcunit;
	}
	//  TRACE("%d units in %s %s deleted\n", i, this->GetClassNameStr(), m_strName);                  
		// release list's memory
	m_CUnitList.clear();
	m_CRunTimeUnitList.clear();
	
	ASSERT(m_vfSelected.size() == 16);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 存储格式：
//   （INT64)m_fEncapsulated  << (INT64)iTotal << m_CRunTimeUnitList << (INT64)m_fCanViewIn
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::Serialize( CArchive& ar ) {
  CUnitBase::Serialize( ar );
  
	CString string;
	CUnitBase * pcunit;
	INT64 iTotal;
	INT64 iTemp = m_CUnitList.size();

	if (ar.IsStoring())
  {
    // TODO: add storing code here
		ar << iTemp;
		for (const auto pcunit1 : m_CUnitList) {
			ar << pcunit1;
		}

		ar << (INT64)m_fEncapsulated << (INT64)m_fEncapsulationPermitted;
		if ( m_fEncapsulated ) {
			iTotal = m_CRunTimeUnitList.size();
			ar << iTotal;
			for (const auto pcunit1 : m_CRunTimeUnitList) {
				ar << pcunit1;
			}
		}
		ar << (INT64)m_fCanViewIn << m_lReserved11 << m_lReserved12;
    for (int i = 0; i < 16; i++) {
      ar << m_pInterfacePara[i];
    }
  }
  else {
    // TODO: add loading code here
    ar >> iTemp;
    for (int i = 0; i < iTemp; i++) {
      ar >> pcunit;
      pcunit->SetComponentUpper(this);
      m_CUnitList.push_back(pcunit);
      pcunit->SetUpperUnitList(&m_CUnitList); // 设置本单元的上层部件
    }

    INT64 a, b;
    // 读入运行序列,
    ar >> a >> b;
    m_fEncapsulated = (bool)a;
    m_fEncapsulationPermitted = (bool)b;
    if (m_fEncapsulated) {
      ar >> iTotal;
      for (int i = 0; i < iTotal; i++) {
        ar >> pcunit;
        m_CRunTimeUnitList.push_back(pcunit);
      }
    }

    ar >> a >> m_lReserved11 >> m_lReserved12;
    m_fCanViewIn = (bool)a;

    for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i] != nullptr) delete m_pInterfacePara[i];
      ar >> m_pInterfacePara[i];
    }

  }
}

/////////////////////////////////////////////////////////////////////////////////////////
// virtual function declaration

bool CUnitComponent::IsParameterLocked(ULONG ulIndex) {
  ASSERT((ulIndex >= 0) && (ulIndex < 16));
  return(m_vfSelected[ulIndex]);
}

void CUnitComponent::ClearParaSelectedFlag(void)
{
  for (int i = 0; i < 16; i++) {
    m_vfSelected[i] = false;
  }
}


////////////////////////////////////////////////////////////////
//
// LoopDetect()
//
// Parameter :
//    CUnitList * plistUnit : 被测试的单元序列，如果发现动态链接的循环，则此单元序列包含形成的单元循环序列
//
// Return :
//    bool      : 找到循环时为真，否则为假.
//
// Decription :
//		检查我所动态链接的目的单元是否以本部件做为目的单元，如果有，则返回发现循环标志；如果没有，则检查目的单元的目的单元是否以本单元为目的单元。
//		这是一个递归过程。
//		
//    如果没有封装，则检查内部单元序列；如果封装了，则检查本部件参数和本部件的动态链接序列
//    
//
////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::LoopDetect(CUnitList * pCUnitList) {
  CUnitBase *pcunit;
  CUDLList * pDLList;   

  if (m_fEncapsulated) { // 部件封装了？
    //检查封装后的部件
  pCUnitList->push_back(this); 
  for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked() && (m_pInterfacePara[i]->GetParaType() & tINPUT)) { // 如果是输入型参数
        for (int j = 0; j < 16; j++) {
          if (m_pInterfacePara[i]->IsInnerDataLinked(j)) { // 如果此两参数之间有内部数据链接（通过内部单元序列的动态链接而产生）
            // 在本部件的动态链接序列中寻找相应的动态链接
            pDLList = this->GetDynLinkList();
            for (const auto pDL : *pDLList) {
              if ( pDL->GetSrcIndex() == j) { // 如果动态链接的源参数位置与检查的参数位置相同
                pcunit = pDL->GetDestUnit(); // 
                if (!pcunit->IsSetCutOff()) {   // 如果是寻找循环并且没有设置截断标志
                  if (find(pCUnitList->begin(), pCUnitList->end(), pcunit) != pCUnitList->end()) {	// 找到了循环？
                    // 将本单元加入单元序列，并且报告发现了循环
                    pCUnitList->push_back(pcunit);
                    return(true);
                  }
                  else {
                    if (pcunit->LoopDetect(pCUnitList)) { // 继续检查我所动态链接的目的单元是否有循环链接
                      return(true);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  pcunit = pCUnitList->back();
  pCUnitList->pop_back(); // remove me from tail
  ASSERT(pcunit == this);    
  return(false); // 没有找到 
  }
  else { // 部件尚未封装，检查内部单元序列是否存在循环
    for (const auto pcunit1 : m_CUnitList) {
      if (pcunit1->LoopDetect(pCUnitList)) {
        return(true);
      }
    }
    return(false); // 整个内部单元序列都没有找到
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 检查动态链接循环中是否存在截断.如果存在的话，参数pCUnitList为存在循环截断的单元序列
//
//
// 检查完这十六个参数数组后，不用再去检查部件本身的动态链接，因为检查参数时，如果发现有输入型参数，则与之相链接的输出型参数对应的所有动态链接都已经检查过了。
// 如果尚存有未被检查的动态链接，那么这个动态链接没有与之对应的输入，故而不会产生循环链接。
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::CheckCutOff(CUnitList * pCUnitList) {
  CUnitBase *pcunit;
  CUDLList * pDLList;

  if (m_fEncapsulated) { // 部件封装了？
    //检查封装后的部件
    pCUnitList->push_back(this);
    for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked() && (m_pInterfacePara[i]->GetParaType() & tINPUT)) { // 如果是输入型参数
        for (int j = 0; j < 16; j++) {
          if (m_pInterfacePara[i]->IsInnerDataLinked(j)) { // 如果此两参数之间有内部数据链接（通过内部单元序列的动态链接）
            pDLList = GetDynLinkList();
            for (const auto pDL : *pDLList) {
              if (pDL->GetSrcIndex() == j) { // 如果动态链接的源参数位置与检查的参数位置相同
                pcunit = pDL->GetDestUnit(); // 
                if (!pcunit->IsSetCutOff()) {   // 如果是寻找循环并且没有设置截断标志
                  if (find(pCUnitList->begin(), pCUnitList->end(), pcunit) != pCUnitList->end()) {	// 找到了循环？
                    // 将本单元加入单元序列，并且报告发现了截断
                    pCUnitList->push_back(pcunit);
                    return(true);
                  }
                  else {
                    if (pcunit->CheckCutOff(pCUnitList)) { // 继续检查我所动态链接的目的单元是否有截断标志
                      return(true);
                    }
                  }
                }
              }
            }
          }

        }
      }
    } 
    // 检查完这十六个参数数组后，不用再去检查部件本身的动态链接，因为检查参数时，如果发现有输入型参数，则与之相链接的输出型参数对应的所有动态链接都已经检查过了。
    // 如果尚存有未被检查的动态链接，那么这个动态链接没有与之对应的输入，故而不会产生循环链接。

    // 运行到这里，则表明没有发现截断标志。
    pcunit = pCUnitList->back();
    pCUnitList->pop_back(); // remove me from tail
    ASSERT(pcunit == this);
    return(false);     // 返回假的只有这个位置，其他返回的点必须为真。
  }
  else { // 部件尚未封装，则检查其内部单元序列
    for (const auto pcunit1 : m_CUnitList) {
      if (pcunit1->CheckCutOff(pCUnitList)) {
        return(true);
      }
    }
    return(false);
  }
}

////////////////////////////////////////////////////////////////
//
// CheckInnerDataLink()
//
// Parameter :
//    INT64 lSrcIndex       : 被检查部件的参数位置（内部数据链接的源位置）
//    INT64 lDestParaPos    : 被检查目的单元的参数位置（如果就是被检查的部件，则就是内部数据链接的目的位置）
//    CUnitList * plistUnit : 被测试的单元序列，如果发现动态链接的循环，则此单元序列包含形成的单元循环序列
//
// Return :
//    bool      : 找到循环时为真，否则为假.
//
// Decription :
//    Check weither my dynamic link's link to unit is in a loop list
//		检查我所动态链接的目的单元是否以本部件做为目的单元，如果有，则返回发现循环标志；如果没有，则检查目的单元的目的单元是否以本单元为目的单元。
//		这是一个递归过程。
//		
//    如果没有封装，则检查内部单元序列；如果封装了，则检查本部件参数和本部件的动态链接序列
//    
//
////////////////////////////////////////////////////////////////////////////
void CUnitComponent::CheckInnerDataLink(INT64 lSrcIndex, INT64 lDestParaPos, CUnitList * pCUnitList) {
  CUnitBase *pcunit, *pcunit2;
  CUDLList * pDLList, pDLList2;

  ASSERT(m_fEncapsulated); // 本部件必须被封装了
  ASSERT(m_fCutOff == false); // 本部件的截断标志已经在调用本函数之前就判断为假了
  for (int i = 0; i < 16; i++) {
    if (IsInnerDataLinked(lDestParaPos, i)) { // 如果参数lDestParaPos有内部数据链接至参数i
      ASSERT((m_pInterfacePara[i]->GetParaType() & (tINPUT | tOUTPUT)) == tOUTPUT);//
      ASSERT((m_pInterfacePara[lDestParaPos]->GetParaType() & (tINPUT | tOUTPUT)) == tINPUT);
      pCUnitList->push_back(this);
      pDLList = this->GetDynLinkList();
      for (const auto pDL2 : *pDLList) {
        if ((pDL2->GetSrcIndex() == i) && (pDL2->GetSrcUnit() == this)) { // 找到从本部件相应参数位置处联出的动态链接
          pcunit2 = pDL2->GetDestUnit();
          if (((CUnitComponent *)pcunit2) == (this->GetComponentUpper())) { // 找到了
            ((CUnitComponent *)pcunit2)->SetInnerDataLinked(lSrcIndex, pDL2->GetDestIndex(), true);
          }
          else { // 接着往下找
            pCUnitList->push_back(pcunit2);
            pcunit2->CheckInnerDataLink(lSrcIndex, pDL2->GetDestIndex(), pCUnitList);
          }
        }
      }
    }
  }
  // 运行到这里，则表明没有发现循环或者截断标志
  pcunit = pCUnitList->back();
  pCUnitList->pop_back(); // remove me from tail
  ASSERT(pcunit == this);
}

const CString& CUnitComponent::GetClassNameStr( void ) {
  static CString str = "Cpt";
  return(str);
}

bool CUnitComponent::IsMe( const CString& strName ) {
	bool fFound;
	if (m_fEncapsulated) { //如果封装了，
		return(CUnitBase::IsMe(strName));
	}
	else {	// 没有封装？
		fFound = CUnitBase::IsMe(strName);
		if (fFound) return(true);

    // 在内部单元序列里找
		for (const auto pcunit : m_CUnitList) {
			if (pcunit->IsMe(strName)) {
				return(true);
			}
		}
		return(false);
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// 显示本部件及内部单元联出本部件的动态链接（具有COMPONENT_TO_UNIT或者COMPONENT_TO_COMPONENT类型的）。
//
 //////////////////////////////////////////////////////////////////////////
void CUnitComponent::ToShow( CDC * const pdc ) {
	if (pdc->RectVisible(m_rectArea)) {   // if I need to redraw ?
		CBrush cb, *pcb;
		CPen   cp;
		CRect rect;
		int i, j;

		if (m_fEncapsulationPermitted) {
      if (m_fEncapsulated) {
        cb.CreateSolidBrush(RGB(128, 128, 128)); // 封装后的部件颜色加深
      }
      else {
        cb.CreateSolidBrush(RGB(192, 192, 192));
      }
      pcb = (CBrush *)pdc->SelectObject(&cb);
			ASSERT(pcb != nullptr);
			pdc->Rectangle(m_rectArea);           // draw myself
			if (m_fEncapsulated) { // 如果已经封装了，则画禁止入内标志及参数个数
				pdc->MoveTo(m_rectArea.left, m_rectArea.top);
				pdc->LineTo(m_rectArea.right, m_rectArea.bottom);
				pdc->MoveTo(m_rectArea.right - 1, m_rectArea.top);
				pdc->LineTo(m_rectArea.left, m_rectArea.bottom);
				j = 0;
				for (i = 0; i < 16; i++) {
					if (m_pInterfacePara[i]->IsLinked()) { j++; }
				}
        switch (j) {
        case 16:
          pdc->MoveTo(m_rectArea.right - 20, m_rectArea.top + 20);
          pdc->LineTo(m_rectArea.right - 20, m_rectArea.bottom - 20);
        case 15:
          pdc->MoveTo(m_rectArea.left + 20, m_rectArea.top + 20);
          pdc->LineTo(m_rectArea.left + 20, m_rectArea.bottom - 20);
        case 14:
          pdc->MoveTo(m_rectArea.left + 20, m_rectArea.bottom - 20);
          pdc->LineTo(m_rectArea.right - 20, m_rectArea.bottom - 20);
        case 13:
          pdc->MoveTo(m_rectArea.left + 20, m_rectArea.top + 20);
          pdc->LineTo(m_rectArea.right - 20, m_rectArea.top + 20);
        case 12:
          pdc->MoveTo(m_rectArea.right - 15, m_rectArea.top + 15);
          pdc->LineTo(m_rectArea.right - 15, m_rectArea.bottom - 15);
        case 11:
          pdc->MoveTo(m_rectArea.left + 15, m_rectArea.top + 15);
          pdc->LineTo(m_rectArea.left + 15, m_rectArea.bottom - 15);
        case 10:
          pdc->MoveTo(m_rectArea.left + 15, m_rectArea.bottom - 15);
          pdc->LineTo(m_rectArea.right - 15, m_rectArea.bottom - 15);
        case 9:
          pdc->MoveTo(m_rectArea.left + 15, m_rectArea.top + 15);
          pdc->LineTo(m_rectArea.right - 15, m_rectArea.top + 15);
        case 8:
          pdc->MoveTo(m_rectArea.right - 10, m_rectArea.top + 10);
          pdc->LineTo(m_rectArea.right - 10, m_rectArea.bottom - 10);
        case 7:
          pdc->MoveTo(m_rectArea.left + 10, m_rectArea.top + 10);
          pdc->LineTo(m_rectArea.left + 10, m_rectArea.bottom - 10);
        case 6:
          pdc->MoveTo(m_rectArea.left + 10, m_rectArea.bottom - 10);
          pdc->LineTo(m_rectArea.right - 10, m_rectArea.bottom - 10);
        case 5:
          pdc->MoveTo(m_rectArea.left + 10, m_rectArea.top + 10);
          pdc->LineTo(m_rectArea.right - 10, m_rectArea.top + 10);
        case 4:
          pdc->MoveTo(m_rectArea.right - 5, m_rectArea.top + 5);
          pdc->LineTo(m_rectArea.right - 5, m_rectArea.bottom - 5);
        case 3:
          pdc->MoveTo(m_rectArea.left + 5, m_rectArea.top + 5);
          pdc->LineTo(m_rectArea.left + 5, m_rectArea.bottom - 5);
        case 2:
          pdc->MoveTo(m_rectArea.left + 5, m_rectArea.bottom - 5);
          pdc->LineTo(m_rectArea.right - 5, m_rectArea.bottom - 5);
        case 1:
          pdc->MoveTo(m_rectArea.left + 5, m_rectArea.top + 5);
          pdc->LineTo(m_rectArea.right - 5, m_rectArea.top + 5);
          break;
        case 0: // do nothing
          break;
				default:
					ASSERT(0);
				}
				pdc->SelectObject(pcb);
			}
		}
		else {  // 不允许封装的显示两个嵌套长方形,底色为白色。
			cb.CreateSolidBrush(RGB(255, 255, 255));
			pcb = (CBrush *)pdc->SelectObject(&cb);
			ASSERT(pcb != nullptr);
			pdc->Rectangle(m_rectArea);           // draw myself
			rect.top = m_rectArea.top + 10;
			rect.bottom = m_rectArea.bottom - 10;
			rect.left = m_rectArea.left + 10;
			rect.right = m_rectArea.right - 10;
			pdc->Rectangle(rect);
			pdc->SelectObject(pcb);
		}
	}

	CUDLList * pDynLinkList;

  // 调用基类函数，显示部件本身的动态链接线
  CUnitBase::ToShow(pdc);   // show status

  // 显示部件内部单元序列的动态链接线（如果类型为 COMPONENT_TO_UNIT和COMPONENT_TO_COMPONENT，即联出本部件）
  for (const auto pUnit : m_CUnitList) {
    pDynLinkList = pUnit->GetDynLinkList();
    // 下面这行代码，本来使用的是m_listDynLink而不是pDynLinkList。在使用Position寻址时，虽然代码错误，但执行确是正确的，估计是编译器的问题
    // 使用范围循环语句，就检验出了这个错误，故而将m_listDynLink替换成了正确的pDynLinkList
    // 我去查了一下，发现这个错误一直存在。
    for (const auto pUnitDynLink : *pDynLinkList) {
      switch (pUnitDynLink->GetDynLinkClass()) {
      case COMPONENT_TO_UNIT:
      case COMPONENT_TO_COMPONENT:
        pUnitDynLink->ToShow(pdc);
        break;
      default:	// don't show dynamic link 
        break;
      } 
    }
  }

} 

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 首先将输入型参数的数据注入内部相应的单元；然后运行内部的单元序列；最后将输出型参数的数据返回上层相应的单元参数中。
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::Exective( void ) {
	CUnitBase * punit;
	INT_PTR iCount;

  ASSERT(m_fEncapsulated); // 封装后的部件才允许执行此函数

	// 将参数中的输入型数据写入本部件中的单元内（优先执行输入型数据）
	for (int i = 0; i < 16; i++) {
		if (m_pInterfacePara[i]->GetParaType() & tINPUT) { // 先将外部输入的数据读入内部相应的单元参数中
      ASSERT((m_pInterfacePara[i]->GetDynLinkType() & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
			switch (m_pInterfacePara[i]->GetDynLinkType() & (tBOOL | tWORD | tDOUBLE | tSTRING)) {
			case tBOOL:
				(m_pInterfacePara[i]->GetDestUnit())->SetBool(m_pInterfacePara[i]->GetDestIndex(),m_pInterfacePara[i]->GetBoolValue());
				break;
			case tWORD:
				(m_pInterfacePara[i]->GetDestUnit())->SetInteger(m_pInterfacePara[i]->GetDestIndex(), m_pInterfacePara[i]->GetIntegerValue());
				break;
			case tDOUBLE:
				(m_pInterfacePara[i]->GetDestUnit())->SetDouble(m_pInterfacePara[i]->GetDestIndex(), m_pInterfacePara[i]->GetDoubleValue());
				break;
			case tSTRING:
				(m_pInterfacePara[i]->GetDestUnit())->SetString(m_pInterfacePara[i]->GetDestIndex(), m_pInterfacePara[i]->GetStringValue());
				break;
			default:
				TRACE("Error in 部件参数类型\n");
				break;
			}
		}
	}

  // 执行各运行时单元序列
  // exective per 1ms Task，目前将其归入每10毫秒执行序列中。
  iCount = m_vCUnit1MS.size();
  for ( int i = 0; i < iCount; i ++ ) {                            
    punit = m_vCUnit1MS[i];
    if ( punit->CountDown(10) ) punit->Exective();  
  }

  // exective per 10ms Task
	if ( m_lCount10MS <= 0 ) {
		m_lCount10MS = 9;
		iCount = m_vCUnit10MS.size();
		for ( int i = 0; i < iCount; i ++ ) {                            
			punit = m_vCUnit10MS[i];
			if ( punit->CountDown(10) ) punit->Exective();  
		}
	}
	else m_lCount10MS--;

  // exective per 100 ms Task
  if ( m_lCount100MS <= 0 ) {
    m_lCount100MS = 99;
    iCount = m_vCUnit100MS.size();
    for ( int i = 0; i < iCount; i ++ ) {                            
      punit = m_vCUnit100MS[i];
      if ( punit->CountDown(100) ) punit->Exective();  
    }
  }
  else m_lCount100MS--;

  // exective per second task
  if ( m_lCountSecond <= 0 ) {
    m_lCountSecond = 999;
    // do function per socond
    iCount = m_vCUnit1Second.size();
    for ( int i = 0; i < iCount; i ++ ) {                            
      punit = m_vCUnit1Second[i];
      if ( punit->CountDown( 1000 ) ) punit->Exective();  
    }
  }
  else m_lCountSecond--;

  // exective per minute task
  if ( m_lCountMinute <= 0 ) {
    m_lCountMinute = 60000 - 1;
    // do function per minute
    iCount = m_vCUnit1Minute.size();
    for ( int i = 0; i < iCount; i ++ ) {                            
      punit = m_vCUnit1Minute[i];
      if ( punit->CountDown( 60000 ) ) punit->Exective();  
    }
  }
  else m_lCountMinute--;

  if ( m_lCountHour <= 0 ) {
    m_lCountHour = 3600000 - 1;
    // do function per hour
  }
  else m_lCountHour--;

	// 将参数中的输出型数据写入上层单元（最后执行输出型数据）
	for (int i = 0; i < 16; i++) {
		if (m_pInterfacePara[i]->GetParaType() & tOUTPUT) { // 最后将内部输出的数据写入上层相应的单元参数中
			switch (m_pInterfacePara[i]->GetDynLinkType() & (tBOOL | tWORD | tDOUBLE | tSTRING)) {
			case tBOOL:
				(m_pInterfacePara[i]->GetDestUnit())->SetBool(m_pInterfacePara[i]->GetDestIndex(), m_pInterfacePara[i]->GetBoolValue());
				break;
			case tWORD:
				(m_pInterfacePara[i]->GetDestUnit())->SetInteger(m_pInterfacePara[i]->GetDestIndex(), m_pInterfacePara[i]->GetIntegerValue());
				break;
			case tDOUBLE:
				(m_pInterfacePara[i]->GetDestUnit())->SetDouble(m_pInterfacePara[i]->GetDestIndex(), m_pInterfacePara[i]->GetDoubleValue());
				break;
			case tSTRING:
				(m_pInterfacePara[i]->GetDestUnit())->SetString(m_pInterfacePara[i]->GetDestIndex(), m_pInterfacePara[i]->GetStringValue());
				break;
			default:
				TRACE("Error in 部件参数类型\n");
				break;
			}
		}
	}
	//更新显示标志
	SetUpdateFlag(true);
}

////////////////////////////////////////////////////////////////////////
//
// ExectiveDynLink()
//
// Return : 
//    bool    : always true.
//
// Description :
// 部件的动态链接，是在部件封装后才可能执行的。封装前，动态链接通过部件联入联出，但与部件无关；
// 封装后，之前通过部件的动态链接在部件处被打断，信息在部件处开始中继。此后部件就被视为一个简单单元，如果再与部件发生动态链接，
// 部件的16个参数就是输出输入参数，在其中选择合适的参数，没有使用的参数，则不可见了。
// 如果此时需要增加新的参数，则只能先将部件内单元的参数链接至部件，然后才可以从外部与之相链接。
//
// 部件封装后，部件输出型参数与外部单元产生的动态链接，存储于部件本身的动态链接序列中，由此函数执行。
// 
// 目前此函数的执行与基类的函数完全一样，故而调用基类函数即可。
//
///////////////////////////////////////////////////////////////////////////
bool CUnitComponent::ExectiveDynLink( void ) {
  return(CUnitBase::ExectiveDynLink()); //调用基类的函数
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 未封装时的部件，如果其拥有输入型参数，则此参数所关联的内部单元，其优先级最高为2，且需要部件本身对其进行设置。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::SetDestUnitPriority(void) {
  if (!m_fEncapsulated) { // 如果尚未被编译
    ASSERT(m_lExectivePriority == 1);
    for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetParaType() & (tINPUT | tOUTPUT)) == tINPUT) {
          m_pInterfacePara[i]->GetDestUnit()->SetExectivePriority(2); // 从上层部件输入数据的单元，其优先级最高为2.
        }
      }
    }
  }
  else { // 否则视同为简单单元
    CUnitBase::SetDestUnitPriority();
  }
}

void CUnitComponent::SetCompiledFlag(bool fFlag) {
  for (const auto punit : m_CUnitList) {
    punit->SetCompiledFlag(fFlag);
  }
  m_fCompiled = fFlag;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 检测部件是否被编译了
//
// 检测部件的内部单元序列是否都被编译了，然后检测部件本身是否被编译了：不可编译的部件永远为真，
// 可编译的部件返回其m_fCompiled标志。
//
//
/////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::IsCompiled(void) {
  
  if ( m_fEncapsulated ) return(true); // 封装后的部件都视为编译过的

  for (const auto punit : m_CUnitList) {
    if (!punit->IsCompiled()) return(false);
    if (punit->IsEncapsulable()) { // 可编译的部件，还需要测试本身是否编译了
      if (!m_fCompiled) return false;
    }
  }
  return m_fCompiled;
}

bool CUnitComponent::DeleteDynLink( CUnitBase * pUnit ) {
  if (m_fEncapsulated) { // 如果封装了，则只删除本部件自身与pUnit之间的动态链接（如果有的话）
    return(CUnitBase::DeleteDynLink(pUnit));// 本部件也可能有自己的动态链接，故而也需要删除之（如果pUnit在其中的话）。
  }
  else { // 未封装的部件在其单元序列中检查
    for (const auto pcunit : m_CUnitList) {
      pcunit->DeleteDynLink(pUnit);
    }
  }
  return( true );
}

bool CUnitComponent::CreateUniName( CUnitList& listTotalUnit ) {
  if (m_fEncapsulated) {
    return(CUnitBase::CreateUniName(listTotalUnit));
  }
  else {
    INT_PTR iTemp = 1;

    for (const auto pcunit1 : listTotalUnit) {
      pcunit1->CreateUniName(listTotalUnit);
    }

    bool fFind = false;
    CString strNumber;
    char s[10];

    for (const auto pcunit1 : listTotalUnit) {
      if (m_strName == pcunit1->GetName()) {
        fFind = true;
        break;
      }
    }

    bool fDone = false;
    if (fFind) {
      while (!fDone) {
        _itoa_s(iTemp++, s, 10);
        m_strName += s;
        fDone = true;
        for (const auto pcunit : listTotalUnit) {
          if (m_strName == pcunit->GetName()) {
            fDone = false;
            break;
          }
        }
      }
    }
  }
	return(true);
}

/////////////////////////////////////////////////////////////////////////
//
// SetParameterLock()
//
// Parameter :
//    ULONG ulIndex     : 参数位置.
//    bool fSelected    : 当前需设置的状态.
//
// Description :
//   改变ulIndex指向的参数连接状态. 并设置或清除参数锁.
//  当生成或删除动态连接时,总是要调用此函数. 
//
//////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetParameterLock(ULONG ulIndex, bool fSelected) {
  if ((ulIndex < 0) || (ulIndex > 15)) {
    throw "index out of range";
  }
  // 此函数只用于输入参数.
  ASSERT(m_pInterfacePara[ulIndex]->IsLinked());
  if ((m_pInterfacePara[ulIndex]->GetParaType() & tINPUT)) { // 如果此参数为数据输入类型，则设置参数锁
    if (fSelected == true) {		// 设置已选择标志
      ASSERT(m_vfSelected[ulIndex] == false);
      m_vfSelected[ulIndex] = true;
      m_lDynLinkToNumber++; // 本单元做为目的单元的数量增一
    }
    else {		// 清除已选择标志
      ASSERT(m_vfSelected[ulIndex] == true);
      m_vfSelected[ulIndex] = false;
      m_lDynLinkToNumber--; // 本单元做为目的单元的数量减一
      ASSERT(m_lDynLinkToNumber >= 0); 
    }
  }
  return(true);
}

/////////////////////////////////////////////////////////////////////////
//
// SetParameterSelected()
//
// Parameter :
//    ULONG ulIndex     : 参数位置.
//    bool fSelected    : 当前需设置的状态.
//
// Description :
//   改变ulIndex指向的参数连接状态. 并设置或清除参数锁.
//  此函数只用于Object->Unit之间的动态链接，且数据的方向为输入至单元。
//
// Object链接至Unit的动态链接，无法确定执行优先级，故而不能更改m_ulDynLinkToNumber，
// 但当单元做为输入型时，又必须设置参数被选择了的标志，以防止再次选择此输入型参数。
//////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetParameterSelected(ULONG ulIndex, bool fSelected) {
  if ((ulIndex < 0) || (ulIndex > 15)) {
    throw "index out of range";
  }
  // 此函数只用于输入参数.
  ASSERT(m_pInterfacePara[ulIndex]->IsLinked());
  if ((m_pInterfacePara[ulIndex]->GetParaType() & tINPUT)) { // 如果此参数为数据输入类型，则设置参数锁
    if (fSelected == true) {		// 设置已选择标志
      ASSERT(m_vfSelected[ulIndex] == false);
      m_vfSelected[ulIndex] = true;
    }
    else {		// 清除已选择标志
      ASSERT(m_vfSelected[ulIndex] == true);
      m_vfSelected[ulIndex] = false;
    }
    return(true);
  }
  else return(false);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  部件封装前，需要处理其内部单元序列的标志设置，和部件本身联入参数的标志设置
//  部件封装后，其内部单元不再需要处理
//
//
//////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::SetParaLockFlag( void ) {

	// 如果连入部件的参数中有输入参数,则不允许再连入此参数.
  if (!m_fEncapsulated) { // 如果没有封装，则执行内部单元序列的设置工作
    for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetParaType() & (tINPUT | tOUTPUT)) == tINPUT) { // 输入型参数，需要设置参数锁
          ASSERT(m_pInterfacePara[i]->GetDestUnit() != nullptr);
          ASSERT(m_pInterfacePara[i]->GetDestIndex() != -1);
          ASSERT(m_pInterfacePara[i]->GetSrcUnit() == nullptr);
          // ASSERT(m_pInterfacePara->GetSrcIndex(i) == -1); // 暂时不用，因为有些旧数据还是0
          m_pInterfacePara[i]->GetDestUnit()->SetParameterLock(m_pInterfacePara[i]->GetDestIndex(), true); // 设置目的单元的参数锁
          m_pInterfacePara[i]->GetDestUnit()->LinkFromComponent(true);  // 设置联入部件标志
        }
        else { // 输出型参数，设置源单元联入部件标志
          ASSERT(m_pInterfacePara[i]->GetSrcUnit() != nullptr);
          ASSERT(m_pInterfacePara[i]->GetSrcIndex() != -1);
          ASSERT(m_pInterfacePara[i]->GetDestUnit() == nullptr);
          // ASSERT(m_pInterfacePara->GetDestIndex(i) == -1); // 暂时不用，因为有些旧数据还是0
          m_pInterfacePara[i]->GetSrcUnit()->LinkToComponent(true);  // 设置联入部件标志
        }
      }
    }
    // 设置内部单元序列的标志
    for (const auto pUnit : m_CUnitList) {
      pUnit->SetParaLockFlag();
    }
  }

  // 设置封装后产生的动态链接参数锁（封装后再生成动态链接只能是输出型，输出型动态链接位于部件自身的动态链接序列中，故而调用基类函数即可。
	CUnitBase::SetParaLockFlag();
}

/////////////////////////////////////////////////////////////////////
//
//	AddToList
//
//  return : no
//
//  参数:
//			CUnitList& UnitList : 被加入的单元序列. 
//
//	描述:
//		如果部件已经封装，则将本部件本身加入单元序列中；否则将本部件内的单元和本部件本身都加入单元序列中。
// 使用此函数的共有两处：一处是createUniName，另一处是CompileUnitList时生成运行时单元序列。
// 不允许封装的部件无动作，在不在序列中无所谓；允许封装的部件可能拥有输入型参数，必须在此单元序列中。
//
/////////////////////////////////////////////////////////////////////////////   
void CUnitComponent::AddToList( CUnitList& UnitList) {
	if (!m_fEncapsulated) { // 本部件没有封装，则将本部件的内部单元加入list，否则将本部件本身加入序列
    for (const auto pUnit : m_CUnitList) {
			pUnit->AddToList(UnitList);
		}
    // 把部件本身也添加进单元序列
    UnitList.push_back(this);
	}
  else {
    UnitList.push_back(this);
  }
}

bool CUnitComponent::SetProperty( void ) {
  // 等待实现.
	if (m_fEncapsulated) { // 如果封装了，则显示目前16个参数的状态
		DlgComponent Dlg;

		Dlg.SetCUCPara(m_pInterfacePara);

		if (Dlg.DoModal() == IDOK) {
			return true;
		}

	}
  return( false );
}

/////////////////////////////////////////////////////////////////////////////
// CUnitComponent diagnostics

#ifdef _DEBUG
void CUnitComponent::AssertValid() const
{
  CUnitBase::AssertValid();
}

void CUnitComponent::Dump(CDumpContext& dc) const
{
  CUnitBase::Dump(dc);
}

#endif //_DEBUG
         
//////////////////////////////////////////////////////////////////////////////
// CUnitComponent member function

void CUnitComponent::SetDeleteDynLinkFlag( CUnitList& listUnit ) {
  if (m_fEncapsulated) {
    // 封装后只设置部件本身动态链接的标志
    CUnitBase::SetDeleteDynLinkFlag(listUnit);
  }
  else { // 未封装的部件设置其单元序列的动态链接的标志
    for (const auto pcunit : m_CUnitList) {
      pcunit->SetDeleteDynLinkFlag(listUnit);
    }
  }
}

void CUnitComponent::ClearDeleteDynLinkFlag( void ) {
  if (m_fEncapsulated) {
    CUnitBase::ClearDeleteDynLinkFlag();
  }
  else {
    for (const auto pcunit : m_CUnitList) {
      pcunit->ClearLoopDetectFlag();
    }
  }
}

void CUnitComponent::ClearLoopDetectFlag( void ) {
  if (m_fEncapsulated) {
    CUnitBase::ClearLoopDetectFlag();
  }
  else {
    for (const auto pcunit : m_CUnitList) {
      pcunit->ClearLoopDetectFlag();
    }
  }
}

void CUnitComponent::AdjustDynLinkLinePosition(CUnitBase * pcSrc, CPoint ptStart, CPoint ptEnd) {
  if (m_fEncapsulated) { // 如果封装了，则视其为简单单元
    CUnitBase::AdjustDynLinkLinePosition(pcSrc, ptStart, ptEnd);
  }
  else { // 未封装的部件同时处理自身和内部单元序列
    shared_ptr<CPoint> ppt1, ppt2;
    CPointList * plist;

    CUDLList * plistDynLink;

    // 如果部件本身位置发生了移动
    if (pcSrc == this) {
      for (const auto pUnit : m_CUnitList) {
        plistDynLink = pUnit->GetDynLinkList();
        for (const auto pDL : *plistDynLink) {
          plist = pDL->GetLinkPointList();
          auto itLine = plist->begin();
          switch (pDL->GetDynLinkClass()) {
          case COMPONENT_TO_UNIT:
          case COMPONENT_TO_COMPONENT:
            ASSERT(plist->size() > 2);
            ppt1 = *itLine++;
            ppt2 = *itLine++;
            if (ppt1->x == ppt2->x) {
              if ((ppt1->y < ppt2->y) && (m_rectArea.top > ppt2->y)) {
                ppt1->x += ptEnd.x - ptStart.x;
                ppt1->y = m_rectArea.top;
                ppt2->x += ptEnd.x - ptStart.x;
              }
              else if ((ppt1->y > ppt2->y) && (m_rectArea.bottom < ppt2->y)) {
                ppt1->x += ptEnd.x - ptStart.x;
                ppt1->y = m_rectArea.bottom;
                ppt2->x += ptEnd.x - ptStart.x;
              }
              else {
                *ppt1 += ptEnd - ptStart;
                ppt2->x += ptEnd.x - ptStart.x;
              }
            }
            else if (ppt1->y == ppt2->y) {
              if ((ppt1->x < ppt2->x) && (m_rectArea.left > ppt2->x)) {
                ppt1->y += ptEnd.y - ptStart.y;
                ppt1->x = m_rectArea.left;
                ppt2->y += ptEnd.y - ptStart.y;
              }
              else if ((ppt1->x > ppt2->x) && (m_rectArea.right < ppt2->x)) {
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
            if (ppt1->x == ppt2->x) {	// 竖直线.
              if (ppt2->y > m_rectArea.bottom) {
                ppt1->y = m_rectArea.bottom;
              }
              else if (ppt2->y < m_rectArea.top) {
                ppt1->y = m_rectArea.top;
              }
            }
            else if (ppt1->y == ppt2->y) {	// 水平线.
              if (ppt2->x > m_rectArea.right) {
                ppt1->x = m_rectArea.right;
              }
              else if (ppt2->x < m_rectArea.left) {
                ppt1->x = m_rectArea.left;
              }
            }
            else ASSERT(0);
            break;
          case UNIT_TO_COMPONENT:
          case UNIT_TO_UNIT:
            // not process this kind of dynamic link class
            break;
          default:
            ASSERT(0);
          }
        }
      }
    }

    // if my children's link to unit was been moved
    bool fDo;
    CUnitBase * pcunit;
    CRect rect, rectSrc;
    ULONG ulLinkClass;
    for (const auto pUnit : m_CUnitList) {
      plistDynLink = pUnit->GetDynLinkList();
      for (const auto pDL : *plistDynLink) {
        ulLinkClass = pDL->GetDynLinkClass();
        if ((ulLinkClass == UNIT_TO_UNIT)
          || (ulLinkClass == UNIT_TO_COMPONENT)) continue;
        if ((pcunit = pDL->GetDestUnit()->GetComponentUpper()) == pcSrc) { // 我的子单元指向的单元的母单元改变了位置。
          fDo = true;
        }
        else if ((pcunit = pDL->GetDestUnit()) == pcSrc) { // 我的子单元指向的单元改变了位置。
          fDo = true;
        }
        else {
          fDo = false;
        }
        if (fDo) {
          rect = pcunit->GetSize();
          plist = pDL->GetLinkPointList();
          ASSERT(plist->size() > 2);
          auto itLine = plist->end();
          ppt1 = *--itLine;   // 迭代器位于最后一个元素之后，故而需要先递减再赋值。
          ppt2 = *--itLine;
          rectSrc = pcSrc->GetSize();
          if (ppt1->x == ppt2->x) {
            if ((ppt1->y < ppt2->y) && (rect.top > ppt2->y)) {
              ppt1->x += ptEnd.x - ptStart.x;
              ppt1->y = rect.top;
              ppt2->x += ptEnd.x - ptStart.x;
            }
            else if ((ppt1->y > ppt2->y) && (rect.bottom < ppt2->y)) {
              ppt1->x += ptEnd.x - ptStart.x;
              ppt1->y = rect.bottom;
              ppt2->x += ptEnd.x - ptStart.x;
            }
            else {
              *ppt1 += ptEnd - ptStart;
              ppt2->x += ptEnd.x - ptStart.x;
            }
          }
          else if (ppt1->y == ppt2->y) {
            if ((ppt1->x < ppt2->x) && (rect.left > ppt2->x)) {
              ppt1->y += ptEnd.y - ptStart.y;
              ppt1->x = rect.left;
              ppt2->y += ptEnd.y - ptStart.y;
            }
            else if ((ppt1->x > ppt2->x) && (rect.right < ppt2->x)) {
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
          if (ppt1->x == ppt2->x) {	// 竖直线.
            if (ppt2->y > rectSrc.bottom) {
              ppt1->y = rectSrc.bottom;
            }
            else if (ppt2->y < rectSrc.top) {
              ppt1->y = rectSrc.top;
            }
          }
          else if (ppt1->y == ppt2->y) {	// 水平线.
            if (ppt2->x > rectSrc.right) {
              ppt1->x = rectSrc.right;
            }
            else if (ppt2->x < rectSrc.left) {
              ppt1->x = rectSrc.left;
            }
          }
          else ASSERT(0);
        }
      }
    }
  }
}

bool CUnitComponent::CanViewIn(void) const {
	if (!m_fEncapsulationPermitted) return true;
  return( m_fCanViewIn );
}

////////////////////////////////////////////////////////////////////
//
// 设置是否允许看进去标志.
// 此函数只存在于部件类中。复合单元类永远允许看进去，简单单元类永远不允许看进去。
//
///////////////////////////////////////////////////////////////////
void CUnitComponent::SetViewInFlag(bool fFlag) {
	m_fCanViewIn = fFlag;
}


////////////////////////////////////////////////////////////////////////////
//
// CUnitComponent类如果封装过了则不允许动态连接进内部的Units, 未封装的部件允许动态链接
// 目前调试状态下允许链接进内部
//
///////////////////////////////////////////////////////////////////////////
bool CUnitComponent::CanLinkIn( void ) {
	if (!m_fEncapsulationPermitted) return true;
	if (m_fEncapsulated) {
		return(false);
	}
	else {
		return( true );
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// 这是部件未封装时使用的准备参数函数
//
//
// 参数：
//		CDicList &CListDic  // 存储合适的参数字典
//    ULONG ulType				// 需要选择的链接类型
//    int iLinked					// 需要选择的参数类型（0为选择没有动态链接过的参数；1为选择有动态链接过的参数；-1为选择所有适合的参数，无论是否有动态链接）
//
// 生成可用参数字典。
//  有两种形式，一种是没有被使用的参数字典，用于设置联入联出数据的位置；
//             一种是被使用过的参数字典，用于生成输出型的动态链接
// 此两种字典，分别被用于：
//  1.将部件内部单元的一个输出型参数链接到部件参数中。这种需要选择没有链接的参数数组，并且设置源单元为此内部单元。
//  2.将部件参数链接到部件内部单元的一个输如型参数中。这种需要选择有链接的参数数组，并且设置目的单元为此内部单元。
//  3.封装部件时，找到部件的联入联出，同时设置源单元和目的单元。
//
//  不允许在未封装时从外部单元与部件的参数相链接，与部件相链接的都是内部单元。
//
//	目前有两个地方需要用到这个函数，故而似乎不需要处理三种情况了（待研究）
//      1.处理联入部件参数。联入部件参数可以选择没有链接的参数或者有输入型参数链接的位置（联入型参数是不存在的，因为封装前不允许外部单元与部件本身相链接）
//      2.处理联出部件参数。联出部件参数只能选择没有链接的参数位置。不存在输出型参数，而且输入型参数不允许与输出型数据链接。
// 所有的链接都是一种情况，就是要找到未链接的参数位置。
// 
// 
//
//////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::PrepareParameterDictionary(CDicList &CListDic, ULONG ulType, int iLinked) {
  shared_ptr<CUnitDictionary> pDic;

	ULONG j = 0;

	switch (iLinked) {
	case 0: // 选择没有动态链接的参数
  case 1:
  case -1:
		for (int i = 0; i < 16; i++) {
			if (!m_pInterfacePara[i]->IsLinked()) { // 没有动态链接？
				ASSERT((m_pInterfacePara[i]->GetDestUnit() == nullptr) && (m_pInterfacePara[i]->GetSrcUnit() == nullptr));
				m_aulSuitable[j++] = i;
				pDic = make_shared<CUnitDictionary>(this, i, ulType);
				CListDic.push_back(pDic);
			}
		}
		break;
	}
}


bool CUnitComponent::SetParaSrcUnit(LONG index, CUnitBase * pUnit)
{
  if ((index < 0) || (index > 15)) {
    throw "index out of range";
  }
	m_pInterfacePara[index]->SetSrcUnit(pUnit);
	return true;
}

CUnitBase * CUnitComponent::GetParaSrcUnit(LONG index)
{
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->GetSrcUnit());
}

bool CUnitComponent::SetParaDestUnit(LONG index, CUnitBase * pUnit)
{
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	m_pInterfacePara[index]->SetDestUnit(pUnit);
	return true;
}

CUnitBase * CUnitComponent::GetParaDestUnit(LONG index)
{
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->GetDestUnit());
}

bool CUnitComponent::SetParaExectivePriority(LONG lIndex, LONG lPriority)
{
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }
  m_pInterfacePara[lIndex]->SetExectivePriority(lPriority);
  return true;
}

bool CUnitComponent::GetBool( ULONG index) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return( m_pInterfacePara[index]->GetBoolValue() );
}

bool CUnitComponent::SetBool( ULONG index, bool fValue) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->SetBoolValue(fValue));
}

LONG CUnitComponent::GetInteger( ULONG index) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->GetIntegerValue());
}

bool CUnitComponent::SetInteger( ULONG index, LONG lValue) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->SetIntegerValue(lValue));
}

double CUnitComponent::GetDouble(ULONG index) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->GetDoubleValue());
}


bool CUnitComponent::SetDouble( ULONG index, double dValue) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->SetDoubleValue(dValue));
}

const CString CUnitComponent::GetString(ULONG index) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->GetStringValue());
}

bool CUnitComponent::SetString(ULONG index, const CString& strValue) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[index]->SetStringValue(strValue));
}

ULONG CUnitComponent::GetDynLinkType( ULONG ulIndex ) {
  if ((ulIndex < 0) || (ulIndex > 15)) { // out of range
    throw "index out fo range";
  }
	return( m_pInterfacePara[ulIndex]->GetDynLinkType() );
}

INT32	CUnitComponent::GetIndex( ULONG ulIndex ) {
  if ((ulIndex < 0) || (ulIndex > 15)) { // out of range
    throw "index out fo range";
  }
	return( m_aulSuitable[ulIndex] );
}

/////////////////////////////////////////////////////////////////////////////////
//
// 这个函数只在部件封装时调用。此函数的测试只能完全依赖编译单元序列。
//
//动作：
// 1.设置部件参数之间的内部链接标志
// 2.设置本部件是否具有动态链接的截断功能。
//
/////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetInnerDataLinkFlag(void)
{
  CUnitList listUnit;
  bool fFindInnerDataLink = false;

  if (m_fCutOff) { // 如果已经设置了截断标志，则退出。
    return true;
  }

  // 递归检测是否内部数据之间有链接
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (m_pInterfacePara[i]->GetParaType() & tINPUT) { // 存在输入型参数
        listUnit.clear();
        listUnit.push_front(this);
        (m_pInterfacePara[i]->GetDestUnit())->CheckInnerDataLink(i, m_pInterfacePara[i]->GetDestIndex(), &listUnit); /// 发现了内部数据链接
      }
    }
  }
  
  // 设置是否有内部数据链接标志，以用于设置截断标志
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (m_pInterfacePara[i]->GetParaType() & tINPUT) { // 存在输入型参数
        for (int j = 0; j < 16; j++) {
          if (m_pInterfacePara[i]->IsInnerDataLinked(j)) {
            fFindInnerDataLink = true;
          }
        }
      }
    }
  }

  if (fFindInnerDataLink) { // 如果发现内部数据之间有动态链接，则设置截断为假
    m_fCutOff = false;
  }
  else { // 如果没有发现内部数据之间有动态链接，则设置截断为真。
    m_fCutOff = true;
  }
  return m_fCutOff;
}

bool CUnitComponent::SetInnerDataLinked(INT64 lSrcIndex, INT64 lDestIndex, bool fFlag)
{
  if ((lSrcIndex < 0) || (lSrcIndex > 15)) { // out of range
    throw "index out fo range";
  }
  return( m_pInterfacePara[lSrcIndex]->SetInnerDataLinked(lDestIndex, fFlag));
}

bool CUnitComponent::IsInnerDataLinked(ULONG ulIndex, ULONG ulDestIndex)
{
  if ((ulIndex < 0) || (ulIndex > 15)) { // out of range
    throw "index out fo range";
  }

  return(m_pInterfacePara[ulIndex]->IsInnerDataLinked(ulDestIndex));
}

bool CUnitComponent::HaveParameter( void ) {
	for ( int i = 0; i < 16; i++ ) {
		if ( m_pInterfacePara[i]->IsLinked() ) return( true );
	}
	return( false );
}


CUCPara ** CUnitComponent::GetParaInterface( void ) {
	return( (CUCPara **)m_pInterfacePara );
}

CString CUnitComponent::GetParaName(ULONG lIndex ) {
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }

	return(m_pInterfacePara[lIndex]->GetName() );
}

bool CUnitComponent::SetParaName(LONG index, CString strName)
{
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }

	m_pInterfacePara[index]->SetName(strName);
	return true;
}

ULONG CUnitComponent::GetParaType( ULONG index ) {
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }

	return( m_pInterfacePara[index]->GetParaType() );
}

bool CUnitComponent::SetParaType(LONG index, LONG lType)
{
  if ((index < 0) || (index > 15)) { // out of range
    throw "index out fo range";
  }

	m_pInterfacePara[index]->SetParaType(lType);
	return true;
}

bool CUnitComponent::SetParaLinkedFlag(LONG lIndex, bool fFlag)
{
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }

	m_pInterfacePara[lIndex]->SetLinkedFlag(fFlag);
	return true;
}

bool CUnitComponent::IsParaLinked(LONG lIndex) {
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }

  return(m_pInterfacePara[lIndex]->IsLinked());
}

INT64 CUnitComponent::GetParaSrcIndex(LONG lIndex)
{
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }

	return(m_pInterfacePara[lIndex]->GetSrcIndex());
}

bool CUnitComponent::SetParaSrcIndex(LONG lIndex, LONG lValue)
{
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }
	m_pInterfacePara[lIndex]->SetSrcIndex(lValue);
	return true;
}

INT64 CUnitComponent::GetParaDestIndex(LONG lIndex)
{
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }
	return(m_pInterfacePara[lIndex]->GetDestIndex());
}

bool CUnitComponent::SetParaDestIndex(LONG lIndex, LONG lValue)
{
  if ((lIndex < 0) || (lIndex > 15)) { // out of range
    throw "index out fo range";
  }
	m_pInterfacePara[lIndex]->SetDestIndex(lValue);
	return true;
}

///////////////////////////////////////////////////////////////////////
//
//  内部函数，只用于Encapsulation函数
//
//  封装本部件包含的下层部件
//
//////////////////////////////////////////////////////////////////////
bool CUnitComponent::EncapsulateBelowComponent(CUnitList & listTotalUnit) {
  // 封装下层部件（如果有的话）	
  for (const auto punit : m_CUnitList) {
    ASSERT(punit->IsEncapsulating());     // 
    punit->Encapsulation(listTotalUnit);
    punit->SetCompiledFlag(true);
  }
  return(true);
}

bool CUnitComponent::CheckComponentSelf() {
  // 检查部件状态是否正确
  bool fGood = true;
  for (const auto punit : m_CUnitList) {
    if (!punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      ASSERT(((CUnitComponent *)punit)->GetExectivePriority() == 0); // 确保所有部件都尚未被编译
    }
    else { // 是下层部件
      if (punit->IsEncapsulable()) {
        ASSERT(punit->IsEncapsulated());
      }
      fGood = ((CUnitComponent *)punit)->CheckComponentSelf(); // 继续往下查
    }
  }
  return(fGood);
}

/////////////////////////////////////////////////////////////////////////
//
// 生成本部件的运行时单元序列
//
//
//
/////////////////////////////////////////////////////////////////////////
bool CUnitComponent::CreateRunTimeUnitList() {
  // 生成内部单元序列的运行时单元序列
  bool fDone = false;
  ULONG ulCurrentExectivePriority = 0, iUnitNum = 0;
  INT64 iTotal = m_CUnitList.size();
  ASSERT(m_CRunTimeUnitList.size() == 0);
  while (!fDone) {
    ulCurrentExectivePriority++;
    for (const auto punit : m_CUnitList) {
      if (punit->GetExectivePriority() == ulCurrentExectivePriority) {
        m_CRunTimeUnitList.push_back(punit);
        iUnitNum++;
      }
    }
    if (iUnitNum == iTotal) fDone = true;
  }
  return(true);
}

//////////////////////////////////////////////////////////////////////////////
//
// 当部件参数中存在输出型链接时，在内部相应单元中生成一个新的动态链接。
// 这是封装部件中的一个过程，不能单独调用。
//
//
///////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::CreateNewDynLinkFromInterfaceOutputTypePara() {
  CUnitBase * pcUnit;
  CUDLList *pDLList;
  shared_ptr<CUnitDynLink> pDL;
  // 将部件参数中输出型的在内部源单元处建立一个新的动态链接，目的单元为本部件。
  // 此步骤要先于处理联入联出的过程执行。
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (m_pInterfacePara[i]->GetParaType() & tOUTPUT) { // 存在输出型参数
        //在源单元中加入一新的动态链接
        pDL = make_shared<CUnitDynLink>();
        pDL->SetDestUnit(this);
        pDL->SetDestIndex(i);
        pDL->SetSrcUnit(pcUnit = m_pInterfacePara[i]->GetSrcUnit());
        pDL->SetSrcIndex(m_pInterfacePara[i]->GetSrcIndex());
        pDL->SetDynLinkClass(UNIT_TO_UNIT); // 不能是COMPONENT_TO_UNIT和COMPONENT_TO_COMPONENT那两种，否则会被错误的处理。
        pDL->SetDynLinkType(m_pInterfacePara[i]->GetDynLinkType());
        pDLList = pcUnit->GetDynLinkList();
        pDLList->push_back(pDL);
      }
    }
  }
  return(true);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理联入的动态链接
// 
// 如果动态链接类型是UNIT_TO_COMPONENT，则源单元位于上层单元序列中；
// 如果动态链接类型为COMPONENT_TO_COMPONENT,则源单元位于上层单元序列中的部件的内部单元序列中。
// 
// COMPONENT_TO_COMPONENT寻找的方法，是从传入的参数unitTotalUist中，找到以本单元为目的单元的源单元，
// 然后确认此源单元不位于本部件的单元序列中，就可以处理了。此动态链接必然是COMPONENT_TO_COMPONENT。
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::HandleTheDynLinkedInComponent(CUnitList & listTotalUnit) {
  CUnitComponent * pUpperCpt = nullptr;
  pUpperCpt = (CUnitComponent *)this;
  ASSERT(pUpperCpt != nullptr);

  // 寻找是否存在联入本部件的动态链接

  CString strParaName;
  CUDLList *pDLList = nullptr;
  int iPo = 16;

  ASSERT(m_lDynLinkToNumber == 0); // 封装前部件自身的联入动态链接为零
  for (const auto pcunit : listTotalUnit) { // 在所有需编译的单元序列中寻找
    pDLList = pcunit->GetDynLinkList();
    for (const auto pDL : *pDLList) {
      switch (pDL->GetDynLinkClass()) {
      case COMPONENT_TO_COMPONENT: // 源单元位于上层单元序列中部件的内部单元序列      
      case UNIT_TO_COMPONENT: // 源单元位于上层单元序列中
        if (find(m_CUnitList.begin(), m_CUnitList.end(), pDL->GetDestUnit()) != m_CUnitList.end()) { // 找到了联入单元
          if ((pDL->GetSrcUnit()->GetComponentUpper()) != this) { // 如果源单元不位于本部件内，则找到了
            iPo = FindNextAvailableParaPosition();
            ASSERT((iPo >= 0) && (iPo < 16));
            strParaName = this->GetParaName(iPo) + "-(" + (pDL->GetSrcUnit())->GetName() + "." + pDL->GetSrcUnit()->GetParaName(pDL->GetSrcIndex()) + ")";
            this->SetParaName(iPo, strParaName); // 将参数名称后加上输入数据单元的名称
            this->SetParaSrcUnit(iPo, pDL->GetSrcUnit());       // 输入型的参数其源单元是上层的输出数据单元
            this->SetParaDestUnit(iPo, pDL->GetDestUnit());
            this->SetParaDestIndex(iPo, pDL->GetDestIndex());
            this->SetParaSrcIndex(iPo, pDL->GetSrcIndex());
            this->SetParaType(iPo, pDL->GetDynLinkType() | tINPUT | tMODIFIABLE); // 此参数是输入型参数，链接方向为外部单元联入至部件内部的单元
            ASSERT((this->GetParaType(iPo) & tOUTPUT) == 0); // 不允许同时存在输出型标志
            this->SetParaLinkedFlag(iPo, true);
            this->SetParameterLock(iPo, true); // 链接入本部件的动态链接数加一
            // 将此动态链接的目的单元设置为本部件
            pDL->SetDestUnit(this); // 目的单元为本部件
            pDL->SetDestIndex(iPo);	// 参数索引为选择的参数位置
            if (pDL->GetDynLinkClass() == COMPONENT_TO_COMPONENT) {
              pDL->SetDynLinkClass(COMPONENT_TO_UNIT); // 部件封装后，视同为简单单元
            }
            else {
              pDL->SetDynLinkClass(UNIT_TO_UNIT);// 部件封装后，视同为简单单元
            }
          }
        }
        break;
      default:
        // do nothing
        break;
      }
    }
  }
  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////
//
// 设置本部件的执行优先级
//
//
//
/////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetMyselfExectivePriority( void ) {
  ASSERT(m_fCompiled); // 如果是编译上层部件或者整体文件，则需要决定本部件的执行优先级（永远为真)
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (((m_pInterfacePara[i]->GetParaType()) & tINPUT) && (m_vfSelected[i] == true)) { // 输入型参数且存在源单元：只从上层单元序列中找
        if ((m_pInterfacePara[i]->GetSrcUnit())->GetExectivePriority() > m_lExectivePriority) {
          m_lExectivePriority = (m_pInterfacePara[i]->GetSrcUnit())->GetExectivePriority();    
          m_lExectivePriority++; //本部件的执行优先级要比最大的输入单元的执行优先级多一级(如果部件为没有联入源单元，则执行优先级为2）。

        }
      }
    }
  }
  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理联出本部件的动态链接。
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::HandleTheDynLinkedfromComponent( void ) {
  // 在本部件中生成一个新的动态链接，将联出的动态链接赋予此新的动态链接，其源单元改为本部件，
  // 并将内部源单元的目的单元设置为本部件，重置部件参数中的目的单元指针和目的参数索引。
  CString strParaName;
  CUDLList *pDLList = nullptr;
  int iPo = 16;
  shared_ptr<CUnitDynLink> pDLNew;

  for (const auto pcunit : m_CUnitList) {
    pDLList = pcunit->GetDynLinkList();
    for (const auto pDL : *pDLList) {
      switch (pDL->GetDynLinkClass()) {
      case COMPONENT_TO_UNIT:
      case COMPONENT_TO_COMPONENT:
        iPo = FindNextAvailableParaPosition();
        ASSERT((iPo >= 0) && (iPo < 16));
        // 设置参数的相关内容
        strParaName = this->GetParaName(iPo) + "-(" + (pDL->GetSrcUnit())->GetName()
          + "." + pDL->GetSrcUnit()->GetParaName(pDL->GetSrcIndex()) + ")";
        this->SetParaName(iPo, strParaName); // 将参数名称后加上输入数据单元的名称
        this->SetParaSrcUnit(iPo, pDL->GetSrcUnit());
        this->SetParaDestUnit(iPo, nullptr);  // 清除目的单元指针
        this->SetParaDestIndex(iPo, -1);      // 清除目的单元参数索引
        this->SetParaSrcIndex(iPo, pDL->GetSrcIndex());
        this->SetParaType(iPo, pDL->GetDynLinkType() | tOUTPUT | tMODIFIABLE); // 此参数是输出型参数，链接方向为从部件内部联出至部件外部
        ASSERT((this->GetParaType(iPo) & tINPUT) == 0); // 不允许同时存在输入型标志
        this->SetParaLinkedFlag(iPo, true);
        //生成一个新的动态链接，加入本部件的动态链接序列中
        pDLNew = make_shared<CUnitDynLink>();
        pDLNew->SetDestUnit(pDL->GetDestUnit());  // 目的单元存贮于此动态链接处
        pDLNew->SetDestIndex(pDL->GetDestIndex());// 目的单元参数索引
        pDLNew->SetSrcUnit(this);
        pDLNew->SetSrcIndex(iPo);
        pDLNew->SetDynLinkType(pDL->GetDynLinkType());
        if (pDL->GetDynLinkClass() == COMPONENT_TO_UNIT) {
          pDLNew->SetDynLinkClass(UNIT_TO_UNIT); // 更改动态链接类型
        }
        else pDLNew->SetDynLinkClass(UNIT_TO_COMPONENT); // 更改动态链接类型
        pDLNew->SetLinkPointList(pDL->GetLinkPointList());  // 将动态链接线序列放到部件处
        this->m_listDynLink.push_back(pDLNew);
        pDL->GetLinkPointList()->clear();   // 不再用内部单元来显示链接线
        // 将原动态链接的目的单元设置为本部件，参数索引也改为相关的参数位置
        pDL->mTest_pDestUnitSaved = pDL->GetDestUnit(); // 测试用：保存当前目的单元。
        pDL->SetDestUnit(this); // 目的单元为本部件
        pDL->SetDestIndex(iPo);	// 参数索引为选择的参数位置
        // 目前先不用这个设置
        //pDL->SetDynLinkClass(UNIT_TO_CptPARAMETER); // 此动态链接的类型为UNIT_TO_CptPARAMETER.只有此处能设置这个类型。
        break;
      default:
        // do nothing
        break;
      }
    }
  }
  return(true);
}

////////////////////////////////////////////////////////////////////
//
// Encapsulation() 部件封装自身
//
// 参数：
//    CUnitList &listTotalUnit  此次编译的所有单元的总和。
//
// 返回值:
//		bool : 如果成功为真, 否则为假.
//
// 描述:
//    单独编译自己的单元序列.将链接入和链接出的动态链接提到此部件的参数上来，然后禁止外界看进去。
//    封装有两种情况：一是编译整体文件时发生封装部件的动作，此时部件内部单元已经被编译过了；
//                  二是单独执行封装部件动作，此时其内部单元没有编译（此功能目前未实现）。
//
// 重要：   
//    封装部件的动作，必须发生在部件编译之前。
// 动作：
// 1.每个单元都调用Encapsulation，先把下层的部件封装起来。
// 2.找到联入和联出本部件的动态链接，都链接到部件的参数数组上去
// 3.部件的联出联入方向，与从外部看的方向相同。
//   即从外部看是输出型，则此参数为输出型参数。这样封装后的部件即视同为一简单单元，其行为与简单单元相类似。
// 4.检查是否存在参数之间的内部数据链接（即通过内部单元序列的动态链接，导致两个参数有数据链接），如不存在则设置本部件的截断标志。
//
////////////////////////////////////////////////////////////////////
bool CUnitComponent::Encapsulation(CUnitList & listTotalUnit) {
  if (m_fEncapsulated) { // 已经封装了则返回
    return(true);
  }
  ASSERT(!m_fCompiled); // 部件本身必须没有被编译过。

  if (m_fEncapsulationPermitted) { // 如果是可封装部件
    // 封装下层部件（如果有的话）	
    EncapsulateBelowComponent(listTotalUnit);

    // 检查部件状态是否正确
    CheckComponentSelf();

    // 将部件参数中输出型的在内部源单元处建立一个新的动态链接，目的单元为本部件。
    // 此步骤要先于处理联入联出的过程执行。
    CreateNewDynLinkFromInterfaceOutputTypePara();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // 处理接口参数，将内部联出的和外部联入的动态链接连接到部件参数中

    // 处理联入本部件的动态链接
    HandleTheDynLinkedInComponent(listTotalUnit);

    // 处理联出本部件的动态链接。
    // 在本部件中生成一个新的动态链接，将联出的动态链接赋予此新的动态链接，其源单元改为本部件，
    // 并将内部源单元的目的单元设置为本部件，重置部件参数中的目的单元指针和目的参数索引。
    HandleTheDynLinkedfromComponent();

    // 寻找是否内部两参数之间有数据链接，并相应设置部件本身的截断标志.此时尚未设置封装标志
    // 这个函数调用必须是最后一步，否则内部动态链接尚未设置好，结果不对。
    SetInnerDataLinkFlag();

    m_fEncapsulated = true; // 最后设置封装标志
  }
  else { // 不可封装部件，则只封装其下层部件。其内部单元序列要加入其上层的部件（或者就是总体文件）的单元序列中。
    // 封装下层部件（如果有的话）	
    EncapsulateBelowComponent(listTotalUnit);

    // 检查部件状态是否正确
    CheckComponentSelf();
    ASSERT(m_fEncapsulated == false);
  }
  return (true);
}

bool CUnitComponent::Compilation(void)
{
  // 编译本部件的内部单元序列。如果是部件，则往下找.不可封装的部件也要往下找。
  for (const auto punit : m_CUnitList) {
    if (punit->IsEncapsulating()) { // 排除已封装的部件。
      ASSERT(!punit->IsCompiled());
      ASSERT(!punit->IsEncapsulated());
      punit->Compilation();
    }
  }

  // 此时下层部件都已被编译了,确认之。
  for (const auto punit : m_CUnitList) {
    if (punit->IsKindOf(RUNTIME_CLASS(CUnitComponent)) && punit->IsEncapsulable()) {
      CUnitList * pUnitList = ((CUnitComponent *)punit)->GetUnitList();
      for (const auto punitInner : *pUnitList) {
        ASSERT(punit->IsEncapsulated());
        ASSERT(!punit->IsEncapsulating());
        ASSERT(punit->IsCompiled());
      }
    }
  }
 
  // 开始编译
  CompileUnitList(&m_CUnitList, &m_CRunTimeUnitList); // 编译此单元序列

  return true;
}

///////////////////////////////////////////////////////////////////////
//
// 如果是可封装的部件，则返回是否封装标志；如果是不允许封装的部件，则永远为假
//
///////////////////////////////////////////////////////////////////////
bool CUnitComponent::IsEncapsulated( void ) {
	if( m_fEncapsulationPermitted )	return (m_fEncapsulated);
	else return(false);
}

INT32 * CUnitComponent::GetArrayIndex(void)
{
	return m_aulSuitable;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// 根据运行时单元序列，建立相应的执行序列
// create exective list 1ms, 10ms, 100ms, 1Second, 1Minute
// 提供最快频率为每毫秒执行一次的运行单元。目前将一毫秒每次的归入10毫秒序列中执行。
//
////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::PrepareRunTimeList(void) {	

  for (const auto pcunit : m_CRunTimeUnitList) {
		pcunit->PrepareRunTimeList();		// 生成部件的运行时态序列.
    if ( ((pcunit->GetScanRate()/60000)*60000) == pcunit->GetScanRate() ) {
      m_vCUnit1Minute.push_back( pcunit );
    }
    else if ( ((pcunit->GetScanRate()/1000)*1000) == pcunit->GetScanRate() ) {
      m_vCUnit1Second.push_back( pcunit );
    }
    else if ( ((pcunit->GetScanRate()/100)*100) == pcunit->GetScanRate() ) {
      m_vCUnit100MS.push_back( pcunit );
    }
    else if ( ((pcunit->GetScanRate()/10)*10) == pcunit->GetScanRate() ) {
      m_vCUnit10MS.push_back( pcunit );
    }
    else {
      m_vCUnit1MS.push_back( pcunit );
    }
  }
  ASSERT( m_CRunTimeUnitList.size() == ( m_vCUnit1Minute.size() + 
                                         m_vCUnit1Second.size() +
                                         m_vCUnit100MS.size() +
                                         m_vCUnit10MS.size() +
										                     m_vCUnit1MS.size() ) );
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 重置部件与编译相关的信息。
// 如果部件已经被封装了，则内部单元序列不再需要重置。
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::ResetCompileFlag( void ) {
  if (!m_fEncapsulated) { // 如果没有封装，则重置内部单元序列
    for (const auto pcunit : m_CUnitList) {
      pcunit->ResetCompileFlag();
    }
  }
  // 重置本部件自身
  m_CRunTimeUnitList.clear();
  CUnitBase::ResetCompileFlag();
}

///////////////////////////////////////////////////////////////////////
//
// 这个函数目前没有错误处理过程，直接断言为假退出了。
//
//
//
////////////////////////////////////////////////////////////////////////
int CUnitComponent::FindNextAvailableParaPosition(void) {
	for (int i = 0; i < 16; i++) {
		if (!m_pInterfacePara[i]->IsLinked()) return(i);
	}
  // 执行到此处时就溢出了。
  ASSERT(0);
  return(16); // 参数最多16个，故而最大的有效返回值为15.返回16即出错了。
}

void CUnitComponent::SetUpperScrollPosition(CPoint pt) {
	m_ptScrollPositionUpper = pt;
}

const CPoint& CUnitComponent::GetUpperScrollPosition(void) {
	return (m_ptScrollPositionUpper);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 此函数虚拟继承自UnitBase，故而其表现形式应与简单单元相似。
// 只有封装后的部件才调用此函数，未封装的部件调用的是三参数的PrepareParaDictionary。
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::PrepareParaDictionary(CDicList &CListDic, ULONG ulType) {
	// 准备参数
  shared_ptr<CUnitDictionary> pDic;
  ULONG ulParaType = ulType & (tMODIFIABLE | tINPUT | tOUTPUT);
  ULONG j = 0;

  ASSERT(m_fEncapsulated); // 调用此函数时，本部件必须被封装了
  ASSERT(ulParaType != 0);// 参数是输出型、输入型或者可被修改的，必须三者之一。
  switch ( ulParaType ) {
  case tMODIFIABLE : // 选择所有的参数，用于Object与Unit生成动态链接
    for (int i = 0; i < 16; i++) {		// 共16个参数
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetDynLinkType() & ulType) && (m_vfSelected[i] == false)) { // 被写入的参数不允许与Object发生数据链接
          m_aulSuitable[j++] = i;
          pDic = make_shared<CUnitDictionary>(this, i, ulType);
          CListDic.push_back(pDic);
        }
      }
    }
    break;
  case tMODIFIABLE + tINPUT : // 输入型或者输出型，用于Unit之间生成动态链接
    for (int i = 0; i < 16; i++) {		// 共16个参数
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetParaType() | ulType) == m_pInterfacePara[i]->GetParaType()) {
          if (m_vfSelected[i] == false) { // 被写入参数必须没有链接源单元
            m_aulSuitable[j++] = i;
            pDic = make_shared<CUnitDictionary>(this, i, ulType);
            CListDic.push_back(pDic);
          }
        }
      }
    }
    break;
  case tMODIFIABLE + tOUTPUT: 
    for (int i = 0; i < 16; i++) {		// 共16个参数
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetParaType() | ulType) == m_pInterfacePara[i]->GetParaType()) {
          if (m_pInterfacePara[i]->GetParaType() & tOUTPUT) {
            m_aulSuitable[j++] = i;
            pDic = make_shared<CUnitDictionary>(this, i, ulType);
            CListDic.push_back(pDic);
          }
        }
      }
    }
    break;
  }
}

// 选择合适的参数,准备参数索引.
void CUnitComponent::SelectParameter(ULONG ) {
  for (int i = 0; i < 16; i++) {
    m_aulSuitable[i] = -1;
  }
}

