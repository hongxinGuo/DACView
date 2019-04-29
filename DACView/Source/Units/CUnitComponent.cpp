//////////////////////////////////////////////////////////////////////////
//
// CUnitComponent implement file.
//
// �������Ƿ�װ�࣬���һ���Ĺ��ܡ��޷�������ȥ��ֻ�������ԡ��������ݵķ������ڲ���Ԫ������ݵ�Ϊ�����ͣ����ⲿ��Ԫ������ݵ�Ϊ����͡�
//
// �����ǹ������ԵĻ���,������װ���ڲ�ϸ��,�����ܺ�ʵ�ֳַ�������, ʹ������νṹ��Ϊ����.
//
//	������������ϵ��ͨ������ֻͨ����ӿ��ṩ�Ĳ���������ʵ�֣��ڲ����������ӵ��ӿڲ��������У��ṩ�����ʹ�ã��ڲ���Ҫ����Ϣ��Ҳ���ⲿͨ���ӿڲ�������
//  ���ݵ������ڲ���
//
//
// �����ķ�װ����������Ҫ��һ��ܡ��ṩ�˲�����ʵ�ֺ͵��Ե�������
//
// ����:
//    ���������Լ��ĵ�Ԫ����.������������ӳ��Ķ�̬�����ᵽ�˲����Ĳ���������Ȼ���ֹ��翴��ȥ��
//
// ������
// 1.���鱾Unitlist�����Ķ�̬�������Ƿ����ѭ�������ڵĻ��˳������档
// 2.�����еĵ�Ԫ���һ�������ĵ�Ԫ����.�����Ĳ�����ͬ�򵥵�Ԫ����δ����Ļ��������Ĳ������Լ����ӵ�Ԫ�Ͳ���������뵥Ԫ�����С�
// 3.ÿ����Ԫ��װǰ����Ԥ�Ȱ��Լ��²�Ĳ�����װ������
// 4.���Ŀ�굥Ԫ���е��ڲ������־�󣨴��ⲿPaste�����Ĳ����ڲ�������ǰ�ı����־����Ҫ������������ٴ�����Ŀ�굥Ԫ����Դ��־.
// 5.ǰ�ĸ�������׼����������CompileUnitList��������ɡ�
// 6.�ҵ�����������������Ķ�̬���ӣ������ӵ������Ĳ���������ȥ����������͵Ķ�̬���ӣ����ڲ����Ķ�̬����������������һ����̬���ӡ�
// 7.�������������뷽�򣬴��ڲ����������Եģ����ⲿ����������ͣ��ʶ���������������������ⲿ���ķ�����ͬ�������ⲿ��������ͣ���˲���Ϊ����Ͳ�����
// 8.��װ��Ĳ�����Ȼ�������ݽض�ЧӦ���ʶ���m_fCutOffΪ�档
//   ������ڲ���Ԫ���еĶ�̬���ӵ�����ӿڵ���������֮��������������ӣ��򲿼���ɥʧ������еĽض�ЧӦ��
// 9.�̼�������������ȼ�����װ�����ϲ㵥Ԫ���о������̼�ִ��ʱ������ִ����������Ķ�ȡ���Ѿ���ɣ��ɲ�������
//   Ȼ�����������ݴ洢���ڲ���ص�Ԫ�Ĳ����У�
//   ����ִ���ڲ���Ԫ���С��̼��ڲ���Ԫ���е�ִ�����ȼ�������ȷ���������ϲ㵥Ԫ���е�Ӱ�졣
//   ���ִ������Ͳ�������������Լ���������Ķ�̬���ӣ�ֻ��������ͣ��Ƿ�װ��Ĳ��������ɵĶ�̬���ӣ���
//10.��װ��Ĳ����ڽ��ж�̬����ʱ�������͵��޸���صĲ������ݣ���Ϊ������ֻ����һ����̬���ӣ���
//   ����������µĶ�̬���ӣ��洢�벿����ʡ�Ķ�̬���������С�
//
//  ʵ��:
//  1. ÿ���������������16��������������.
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
	m_lScanRate = 100;		// ��ʼɨ������Ϊÿ��100ms��
	m_fCanViewIn = true;	// ��ʼʱ���Ա�����ȥ����������޷�����ȥ�ˡ�
	m_fEncapsulated = false;		// �Ƿ񱻱������

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
	m_lScanRate = 100; // 100����ÿ��
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
	m_lScanRate = 100; // 100����ÿ��
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
// �洢��ʽ��
//   ��INT64)m_fEncapsulated  << (INT64)iTotal << m_CRunTimeUnitList << (INT64)m_fCanViewIn
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
      pcunit->SetUpperUnitList(&m_CUnitList); // ���ñ���Ԫ���ϲ㲿��
    }

    INT64 a, b;
    // ������������,
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
//    CUnitList * plistUnit : �����Եĵ�Ԫ���У�������ֶ�̬���ӵ�ѭ������˵�Ԫ���а����γɵĵ�Ԫѭ������
//
// Return :
//    bool      : �ҵ�ѭ��ʱΪ�棬����Ϊ��.
//
// Decription :
//		���������̬���ӵ�Ŀ�ĵ�Ԫ�Ƿ��Ա�������ΪĿ�ĵ�Ԫ������У��򷵻ط���ѭ����־�����û�У�����Ŀ�ĵ�Ԫ��Ŀ�ĵ�Ԫ�Ƿ��Ա���ԪΪĿ�ĵ�Ԫ��
//		����һ���ݹ���̡�
//		
//    ���û�з�װ�������ڲ���Ԫ���У������װ�ˣ����鱾���������ͱ������Ķ�̬��������
//    
//
////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::LoopDetect(CUnitList * pCUnitList) {
  CUnitBase *pcunit;
  CUDLList * pDLList;   

  if (m_fEncapsulated) { // ������װ�ˣ�
    //����װ��Ĳ���
  pCUnitList->push_back(this); 
  for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked() && (m_pInterfacePara[i]->GetParaType() & tINPUT)) { // ����������Ͳ���
        for (int j = 0; j < 16; j++) {
          if (m_pInterfacePara[i]->IsInnerDataLinked(j)) { // �����������֮�����ڲ��������ӣ�ͨ���ڲ���Ԫ���еĶ�̬���Ӷ�������
            // �ڱ������Ķ�̬����������Ѱ����Ӧ�Ķ�̬����
            pDLList = this->GetDynLinkList();
            for (const auto pDL : *pDLList) {
              if ( pDL->GetSrcIndex() == j) { // �����̬���ӵ�Դ����λ������Ĳ���λ����ͬ
                pcunit = pDL->GetDestUnit(); // 
                if (!pcunit->IsSetCutOff()) {   // �����Ѱ��ѭ������û�����ýضϱ�־
                  if (find(pCUnitList->begin(), pCUnitList->end(), pcunit) != pCUnitList->end()) {	// �ҵ���ѭ����
                    // ������Ԫ���뵥Ԫ���У����ұ��淢����ѭ��
                    pCUnitList->push_back(pcunit);
                    return(true);
                  }
                  else {
                    if (pcunit->LoopDetect(pCUnitList)) { // �������������̬���ӵ�Ŀ�ĵ�Ԫ�Ƿ���ѭ������
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
  return(false); // û���ҵ� 
  }
  else { // ������δ��װ������ڲ���Ԫ�����Ƿ����ѭ��
    for (const auto pcunit1 : m_CUnitList) {
      if (pcunit1->LoopDetect(pCUnitList)) {
        return(true);
      }
    }
    return(false); // �����ڲ���Ԫ���ж�û���ҵ�
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// ��鶯̬����ѭ�����Ƿ���ڽض�.������ڵĻ�������pCUnitListΪ����ѭ���ضϵĵ�Ԫ����
//
//
// �������ʮ������������󣬲�����ȥ��鲿������Ķ�̬���ӣ���Ϊ������ʱ����������������Ͳ���������֮�����ӵ�����Ͳ�����Ӧ�����ж�̬���Ӷ��Ѿ������ˡ�
// ����д���δ�����Ķ�̬���ӣ���ô�����̬����û����֮��Ӧ�����룬�ʶ��������ѭ�����ӡ�
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::CheckCutOff(CUnitList * pCUnitList) {
  CUnitBase *pcunit;
  CUDLList * pDLList;

  if (m_fEncapsulated) { // ������װ�ˣ�
    //����װ��Ĳ���
    pCUnitList->push_back(this);
    for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked() && (m_pInterfacePara[i]->GetParaType() & tINPUT)) { // ����������Ͳ���
        for (int j = 0; j < 16; j++) {
          if (m_pInterfacePara[i]->IsInnerDataLinked(j)) { // �����������֮�����ڲ��������ӣ�ͨ���ڲ���Ԫ���еĶ�̬���ӣ�
            pDLList = GetDynLinkList();
            for (const auto pDL : *pDLList) {
              if (pDL->GetSrcIndex() == j) { // �����̬���ӵ�Դ����λ������Ĳ���λ����ͬ
                pcunit = pDL->GetDestUnit(); // 
                if (!pcunit->IsSetCutOff()) {   // �����Ѱ��ѭ������û�����ýضϱ�־
                  if (find(pCUnitList->begin(), pCUnitList->end(), pcunit) != pCUnitList->end()) {	// �ҵ���ѭ����
                    // ������Ԫ���뵥Ԫ���У����ұ��淢���˽ض�
                    pCUnitList->push_back(pcunit);
                    return(true);
                  }
                  else {
                    if (pcunit->CheckCutOff(pCUnitList)) { // �������������̬���ӵ�Ŀ�ĵ�Ԫ�Ƿ��нضϱ�־
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
    // �������ʮ������������󣬲�����ȥ��鲿������Ķ�̬���ӣ���Ϊ������ʱ����������������Ͳ���������֮�����ӵ�����Ͳ�����Ӧ�����ж�̬���Ӷ��Ѿ������ˡ�
    // ����д���δ�����Ķ�̬���ӣ���ô�����̬����û����֮��Ӧ�����룬�ʶ��������ѭ�����ӡ�

    // ���е���������û�з��ֽضϱ�־��
    pcunit = pCUnitList->back();
    pCUnitList->pop_back(); // remove me from tail
    ASSERT(pcunit == this);
    return(false);     // ���ؼٵ�ֻ�����λ�ã��������صĵ����Ϊ�档
  }
  else { // ������δ��װ���������ڲ���Ԫ����
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
//    INT64 lSrcIndex       : ����鲿���Ĳ���λ�ã��ڲ��������ӵ�Դλ�ã�
//    INT64 lDestParaPos    : �����Ŀ�ĵ�Ԫ�Ĳ���λ�ã�������Ǳ����Ĳ�����������ڲ��������ӵ�Ŀ��λ�ã�
//    CUnitList * plistUnit : �����Եĵ�Ԫ���У�������ֶ�̬���ӵ�ѭ������˵�Ԫ���а����γɵĵ�Ԫѭ������
//
// Return :
//    bool      : �ҵ�ѭ��ʱΪ�棬����Ϊ��.
//
// Decription :
//    Check weither my dynamic link's link to unit is in a loop list
//		���������̬���ӵ�Ŀ�ĵ�Ԫ�Ƿ��Ա�������ΪĿ�ĵ�Ԫ������У��򷵻ط���ѭ����־�����û�У�����Ŀ�ĵ�Ԫ��Ŀ�ĵ�Ԫ�Ƿ��Ա���ԪΪĿ�ĵ�Ԫ��
//		����һ���ݹ���̡�
//		
//    ���û�з�װ�������ڲ���Ԫ���У������װ�ˣ����鱾���������ͱ������Ķ�̬��������
//    
//
////////////////////////////////////////////////////////////////////////////
void CUnitComponent::CheckInnerDataLink(INT64 lSrcIndex, INT64 lDestParaPos, CUnitList * pCUnitList) {
  CUnitBase *pcunit, *pcunit2;
  CUDLList * pDLList, pDLList2;

  ASSERT(m_fEncapsulated); // ���������뱻��װ��
  ASSERT(m_fCutOff == false); // �������Ľضϱ�־�Ѿ��ڵ��ñ�����֮ǰ���ж�Ϊ����
  for (int i = 0; i < 16; i++) {
    if (IsInnerDataLinked(lDestParaPos, i)) { // �������lDestParaPos���ڲ���������������i
      ASSERT((m_pInterfacePara[i]->GetParaType() & (tINPUT | tOUTPUT)) == tOUTPUT);//
      ASSERT((m_pInterfacePara[lDestParaPos]->GetParaType() & (tINPUT | tOUTPUT)) == tINPUT);
      pCUnitList->push_back(this);
      pDLList = this->GetDynLinkList();
      for (const auto pDL2 : *pDLList) {
        if ((pDL2->GetSrcIndex() == i) && (pDL2->GetSrcUnit() == this)) { // �ҵ��ӱ�������Ӧ����λ�ô������Ķ�̬����
          pcunit2 = pDL2->GetDestUnit();
          if (((CUnitComponent *)pcunit2) == (this->GetComponentUpper())) { // �ҵ���
            ((CUnitComponent *)pcunit2)->SetInnerDataLinked(lSrcIndex, pDL2->GetDestIndex(), true);
          }
          else { // ����������
            pCUnitList->push_back(pcunit2);
            pcunit2->CheckInnerDataLink(lSrcIndex, pDL2->GetDestIndex(), pCUnitList);
          }
        }
      }
    }
  }
  // ���е���������û�з���ѭ�����߽ضϱ�־
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
	if (m_fEncapsulated) { //�����װ�ˣ�
		return(CUnitBase::IsMe(strName));
	}
	else {	// û�з�װ��
		fFound = CUnitBase::IsMe(strName);
		if (fFound) return(true);

    // ���ڲ���Ԫ��������
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
// ��ʾ���������ڲ���Ԫ�����������Ķ�̬���ӣ�����COMPONENT_TO_UNIT����COMPONENT_TO_COMPONENT���͵ģ���
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
        cb.CreateSolidBrush(RGB(128, 128, 128)); // ��װ��Ĳ�����ɫ����
      }
      else {
        cb.CreateSolidBrush(RGB(192, 192, 192));
      }
      pcb = (CBrush *)pdc->SelectObject(&cb);
			ASSERT(pcb != nullptr);
			pdc->Rectangle(m_rectArea);           // draw myself
			if (m_fEncapsulated) { // ����Ѿ���װ�ˣ��򻭽�ֹ���ڱ�־����������
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
		else {  // �������װ����ʾ����Ƕ�׳�����,��ɫΪ��ɫ��
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

  // ���û��ຯ������ʾ��������Ķ�̬������
  CUnitBase::ToShow(pdc);   // show status

  // ��ʾ�����ڲ���Ԫ���еĶ�̬�����ߣ��������Ϊ COMPONENT_TO_UNIT��COMPONENT_TO_COMPONENT����������������
  for (const auto pUnit : m_CUnitList) {
    pDynLinkList = pUnit->GetDynLinkList();
    // �������д��룬����ʹ�õ���m_listDynLink������pDynLinkList����ʹ��PositionѰַʱ����Ȼ������󣬵�ִ��ȷ����ȷ�ģ������Ǳ�����������
    // ʹ�÷�Χѭ����䣬�ͼ������������󣬹ʶ���m_listDynLink�滻������ȷ��pDynLinkList
    // ��ȥ����һ�£������������һֱ���ڡ�
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
// ���Ƚ������Ͳ���������ע���ڲ���Ӧ�ĵ�Ԫ��Ȼ�������ڲ��ĵ�Ԫ���У��������Ͳ��������ݷ����ϲ���Ӧ�ĵ�Ԫ�����С�
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::Exective( void ) {
	CUnitBase * punit;
	INT_PTR iCount;

  ASSERT(m_fEncapsulated); // ��װ��Ĳ���������ִ�д˺���

	// �������е�����������д�뱾�����еĵ�Ԫ�ڣ�����ִ�����������ݣ�
	for (int i = 0; i < 16; i++) {
		if (m_pInterfacePara[i]->GetParaType() & tINPUT) { // �Ƚ��ⲿ��������ݶ����ڲ���Ӧ�ĵ�Ԫ������
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
				TRACE("Error in ������������\n");
				break;
			}
		}
	}

  // ִ�и�����ʱ��Ԫ����
  // exective per 1ms Task��Ŀǰ�������ÿ10����ִ�������С�
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

	// �������е����������д���ϲ㵥Ԫ�����ִ����������ݣ�
	for (int i = 0; i < 16; i++) {
		if (m_pInterfacePara[i]->GetParaType() & tOUTPUT) { // ����ڲ����������д���ϲ���Ӧ�ĵ�Ԫ������
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
				TRACE("Error in ������������\n");
				break;
			}
		}
	}
	//������ʾ��־
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
// �����Ķ�̬���ӣ����ڲ�����װ��ſ���ִ�еġ���װǰ����̬����ͨ�������������������벿���޹أ�
// ��װ��֮ǰͨ�������Ķ�̬�����ڲ���������ϣ���Ϣ�ڲ�������ʼ�м̡��˺󲿼��ͱ���Ϊһ���򵥵�Ԫ��������벿��������̬���ӣ�
// ������16����������������������������ѡ����ʵĲ�����û��ʹ�õĲ������򲻿ɼ��ˡ�
// �����ʱ��Ҫ�����µĲ�������ֻ���Ƚ������ڵ�Ԫ�Ĳ���������������Ȼ��ſ��Դ��ⲿ��֮�����ӡ�
//
// ������װ�󣬲�������Ͳ������ⲿ��Ԫ�����Ķ�̬���ӣ��洢�ڲ�������Ķ�̬���������У��ɴ˺���ִ�С�
// 
// Ŀǰ�˺�����ִ�������ĺ�����ȫһ�����ʶ����û��ຯ�����ɡ�
//
///////////////////////////////////////////////////////////////////////////
bool CUnitComponent::ExectiveDynLink( void ) {
  return(CUnitBase::ExectiveDynLink()); //���û���ĺ���
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// δ��װʱ�Ĳ����������ӵ�������Ͳ�������˲������������ڲ���Ԫ�������ȼ����Ϊ2������Ҫ�����������������á�
//
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::SetDestUnitPriority(void) {
  if (!m_fEncapsulated) { // �����δ������
    ASSERT(m_lExectivePriority == 1);
    for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetParaType() & (tINPUT | tOUTPUT)) == tINPUT) {
          m_pInterfacePara[i]->GetDestUnit()->SetExectivePriority(2); // ���ϲ㲿���������ݵĵ�Ԫ�������ȼ����Ϊ2.
        }
      }
    }
  }
  else { // ������ͬΪ�򵥵�Ԫ
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
// ��ⲿ���Ƿ񱻱�����
//
// ��ⲿ�����ڲ���Ԫ�����Ƿ񶼱������ˣ�Ȼ���ⲿ�������Ƿ񱻱����ˣ����ɱ���Ĳ�����ԶΪ�棬
// �ɱ���Ĳ���������m_fCompiled��־��
//
//
/////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::IsCompiled(void) {
  
  if ( m_fEncapsulated ) return(true); // ��װ��Ĳ�������Ϊ�������

  for (const auto punit : m_CUnitList) {
    if (!punit->IsCompiled()) return(false);
    if (punit->IsEncapsulable()) { // �ɱ���Ĳ���������Ҫ���Ա����Ƿ������
      if (!m_fCompiled) return false;
    }
  }
  return m_fCompiled;
}

bool CUnitComponent::DeleteDynLink( CUnitBase * pUnit ) {
  if (m_fEncapsulated) { // �����װ�ˣ���ֻɾ��������������pUnit֮��Ķ�̬���ӣ�����еĻ���
    return(CUnitBase::DeleteDynLink(pUnit));// ������Ҳ�������Լ��Ķ�̬���ӣ��ʶ�Ҳ��Ҫɾ��֮�����pUnit�����еĻ�����
  }
  else { // δ��װ�Ĳ������䵥Ԫ�����м��
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
//    ULONG ulIndex     : ����λ��.
//    bool fSelected    : ��ǰ�����õ�״̬.
//
// Description :
//   �ı�ulIndexָ��Ĳ�������״̬. �����û����������.
//  �����ɻ�ɾ����̬����ʱ,����Ҫ���ô˺���. 
//
//////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetParameterLock(ULONG ulIndex, bool fSelected) {
  if ((ulIndex < 0) || (ulIndex > 15)) {
    throw "index out of range";
  }
  // �˺���ֻ�����������.
  ASSERT(m_pInterfacePara[ulIndex]->IsLinked());
  if ((m_pInterfacePara[ulIndex]->GetParaType() & tINPUT)) { // ����˲���Ϊ�����������ͣ������ò�����
    if (fSelected == true) {		// ������ѡ���־
      ASSERT(m_vfSelected[ulIndex] == false);
      m_vfSelected[ulIndex] = true;
      m_lDynLinkToNumber++; // ����Ԫ��ΪĿ�ĵ�Ԫ��������һ
    }
    else {		// �����ѡ���־
      ASSERT(m_vfSelected[ulIndex] == true);
      m_vfSelected[ulIndex] = false;
      m_lDynLinkToNumber--; // ����Ԫ��ΪĿ�ĵ�Ԫ��������һ
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
//    ULONG ulIndex     : ����λ��.
//    bool fSelected    : ��ǰ�����õ�״̬.
//
// Description :
//   �ı�ulIndexָ��Ĳ�������״̬. �����û����������.
//  �˺���ֻ����Object->Unit֮��Ķ�̬���ӣ������ݵķ���Ϊ��������Ԫ��
//
// Object������Unit�Ķ�̬���ӣ��޷�ȷ��ִ�����ȼ����ʶ����ܸ���m_ulDynLinkToNumber��
// ������Ԫ��Ϊ������ʱ���ֱ������ò�����ѡ���˵ı�־���Է�ֹ�ٴ�ѡ��������Ͳ�����
//////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetParameterSelected(ULONG ulIndex, bool fSelected) {
  if ((ulIndex < 0) || (ulIndex > 15)) {
    throw "index out of range";
  }
  // �˺���ֻ�����������.
  ASSERT(m_pInterfacePara[ulIndex]->IsLinked());
  if ((m_pInterfacePara[ulIndex]->GetParaType() & tINPUT)) { // ����˲���Ϊ�����������ͣ������ò�����
    if (fSelected == true) {		// ������ѡ���־
      ASSERT(m_vfSelected[ulIndex] == false);
      m_vfSelected[ulIndex] = true;
    }
    else {		// �����ѡ���־
      ASSERT(m_vfSelected[ulIndex] == true);
      m_vfSelected[ulIndex] = false;
    }
    return(true);
  }
  else return(false);
}

/////////////////////////////////////////////////////////////////////////////////////
//
//  ������װǰ����Ҫ�������ڲ���Ԫ���еı�־���ã��Ͳ���������������ı�־����
//  ������װ�����ڲ���Ԫ������Ҫ����
//
//
//////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::SetParaLockFlag( void ) {

	// ������벿���Ĳ��������������,������������˲���.
  if (!m_fEncapsulated) { // ���û�з�װ����ִ���ڲ���Ԫ���е����ù���
    for (int i = 0; i < 16; i++) {
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetParaType() & (tINPUT | tOUTPUT)) == tINPUT) { // �����Ͳ�������Ҫ���ò�����
          ASSERT(m_pInterfacePara[i]->GetDestUnit() != nullptr);
          ASSERT(m_pInterfacePara[i]->GetDestIndex() != -1);
          ASSERT(m_pInterfacePara[i]->GetSrcUnit() == nullptr);
          // ASSERT(m_pInterfacePara->GetSrcIndex(i) == -1); // ��ʱ���ã���Ϊ��Щ�����ݻ���0
          m_pInterfacePara[i]->GetDestUnit()->SetParameterLock(m_pInterfacePara[i]->GetDestIndex(), true); // ����Ŀ�ĵ�Ԫ�Ĳ�����
          m_pInterfacePara[i]->GetDestUnit()->LinkFromComponent(true);  // �������벿����־
        }
        else { // ����Ͳ���������Դ��Ԫ���벿����־
          ASSERT(m_pInterfacePara[i]->GetSrcUnit() != nullptr);
          ASSERT(m_pInterfacePara[i]->GetSrcIndex() != -1);
          ASSERT(m_pInterfacePara[i]->GetDestUnit() == nullptr);
          // ASSERT(m_pInterfacePara->GetDestIndex(i) == -1); // ��ʱ���ã���Ϊ��Щ�����ݻ���0
          m_pInterfacePara[i]->GetSrcUnit()->LinkToComponent(true);  // �������벿����־
        }
      }
    }
    // �����ڲ���Ԫ���еı�־
    for (const auto pUnit : m_CUnitList) {
      pUnit->SetParaLockFlag();
    }
  }

  // ���÷�װ������Ķ�̬���Ӳ���������װ�������ɶ�̬����ֻ��������ͣ�����Ͷ�̬����λ�ڲ�������Ķ�̬���������У��ʶ����û��ຯ�����ɡ�
	CUnitBase::SetParaLockFlag();
}

/////////////////////////////////////////////////////////////////////
//
//	AddToList
//
//  return : no
//
//  ����:
//			CUnitList& UnitList : ������ĵ�Ԫ����. 
//
//	����:
//		��������Ѿ���װ���򽫱�����������뵥Ԫ�����У����򽫱������ڵĵ�Ԫ�ͱ������������뵥Ԫ�����С�
// ʹ�ô˺����Ĺ���������һ����createUniName����һ����CompileUnitListʱ��������ʱ��Ԫ���С�
// �������װ�Ĳ����޶������ڲ�������������ν�������װ�Ĳ�������ӵ�������Ͳ����������ڴ˵�Ԫ�����С�
//
/////////////////////////////////////////////////////////////////////////////   
void CUnitComponent::AddToList( CUnitList& UnitList) {
	if (!m_fEncapsulated) { // ������û�з�װ���򽫱��������ڲ���Ԫ����list�����򽫱����������������
    for (const auto pUnit : m_CUnitList) {
			pUnit->AddToList(UnitList);
		}
    // �Ѳ�������Ҳ��ӽ���Ԫ����
    UnitList.push_back(this);
	}
  else {
    UnitList.push_back(this);
  }
}

bool CUnitComponent::SetProperty( void ) {
  // �ȴ�ʵ��.
	if (m_fEncapsulated) { // �����װ�ˣ�����ʾĿǰ16��������״̬
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
    // ��װ��ֻ���ò�������̬���ӵı�־
    CUnitBase::SetDeleteDynLinkFlag(listUnit);
  }
  else { // δ��װ�Ĳ��������䵥Ԫ���еĶ�̬���ӵı�־
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
  if (m_fEncapsulated) { // �����װ�ˣ�������Ϊ�򵥵�Ԫ
    CUnitBase::AdjustDynLinkLinePosition(pcSrc, ptStart, ptEnd);
  }
  else { // δ��װ�Ĳ���ͬʱ����������ڲ���Ԫ����
    shared_ptr<CPoint> ppt1, ppt2;
    CPointList * plist;

    CUDLList * plistDynLink;

    // �����������λ�÷������ƶ�
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
            // �����Ԫ�ı��˴�С,��Ҫ����ppt1.
            if (ppt1->x == ppt2->x) {	// ��ֱ��.
              if (ppt2->y > m_rectArea.bottom) {
                ppt1->y = m_rectArea.bottom;
              }
              else if (ppt2->y < m_rectArea.top) {
                ppt1->y = m_rectArea.top;
              }
            }
            else if (ppt1->y == ppt2->y) {	// ˮƽ��.
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
        if ((pcunit = pDL->GetDestUnit()->GetComponentUpper()) == pcSrc) { // �ҵ��ӵ�Ԫָ��ĵ�Ԫ��ĸ��Ԫ�ı���λ�á�
          fDo = true;
        }
        else if ((pcunit = pDL->GetDestUnit()) == pcSrc) { // �ҵ��ӵ�Ԫָ��ĵ�Ԫ�ı���λ�á�
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
          ppt1 = *--itLine;   // ������λ�����һ��Ԫ��֮�󣬹ʶ���Ҫ�ȵݼ��ٸ�ֵ��
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

          // �����Ԫ�ı��˴�С,��Ҫ����ppt1.
          if (ppt1->x == ppt2->x) {	// ��ֱ��.
            if (ppt2->y > rectSrc.bottom) {
              ppt1->y = rectSrc.bottom;
            }
            else if (ppt2->y < rectSrc.top) {
              ppt1->y = rectSrc.top;
            }
          }
          else if (ppt1->y == ppt2->y) {	// ˮƽ��.
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
// �����Ƿ�������ȥ��־.
// �˺���ֻ�����ڲ������С����ϵ�Ԫ����Զ������ȥ���򵥵�Ԫ����Զ��������ȥ��
//
///////////////////////////////////////////////////////////////////
void CUnitComponent::SetViewInFlag(bool fFlag) {
	m_fCanViewIn = fFlag;
}


////////////////////////////////////////////////////////////////////////////
//
// CUnitComponent�������װ����������̬���ӽ��ڲ���Units, δ��װ�Ĳ�������̬����
// Ŀǰ����״̬���������ӽ��ڲ�
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
// ���ǲ���δ��װʱʹ�õ�׼����������
//
//
// ������
//		CDicList &CListDic  // �洢���ʵĲ����ֵ�
//    ULONG ulType				// ��Ҫѡ�����������
//    int iLinked					// ��Ҫѡ��Ĳ������ͣ�0Ϊѡ��û�ж�̬���ӹ��Ĳ�����1Ϊѡ���ж�̬���ӹ��Ĳ�����-1Ϊѡ�������ʺϵĲ����������Ƿ��ж�̬���ӣ�
//
// ���ɿ��ò����ֵ䡣
//  ��������ʽ��һ����û�б�ʹ�õĲ����ֵ䣬�������������������ݵ�λ�ã�
//             һ���Ǳ�ʹ�ù��Ĳ����ֵ䣬������������͵Ķ�̬����
// �������ֵ䣬�ֱ����ڣ�
//  1.�������ڲ���Ԫ��һ������Ͳ������ӵ����������С�������Ҫѡ��û�����ӵĲ������飬��������Դ��ԪΪ���ڲ���Ԫ��
//  2.�������������ӵ������ڲ���Ԫ��һ�������Ͳ����С�������Ҫѡ�������ӵĲ������飬��������Ŀ�ĵ�ԪΪ���ڲ���Ԫ��
//  3.��װ����ʱ���ҵ�����������������ͬʱ����Դ��Ԫ��Ŀ�ĵ�Ԫ��
//
//  ��������δ��װʱ���ⲿ��Ԫ�벿���Ĳ��������ӣ��벿�������ӵĶ����ڲ���Ԫ��
//
//	Ŀǰ�������ط���Ҫ�õ�����������ʶ��ƺ�����Ҫ������������ˣ����о���
//      1.�������벿�����������벿����������ѡ��û�����ӵĲ��������������Ͳ������ӵ�λ�ã������Ͳ����ǲ����ڵģ���Ϊ��װǰ�������ⲿ��Ԫ�벿�����������ӣ�
//      2.������������������������������ֻ��ѡ��û�����ӵĲ���λ�á�����������Ͳ��������������Ͳ�����������������������ӡ�
// ���е����Ӷ���һ�����������Ҫ�ҵ�δ���ӵĲ���λ�á�
// 
// 
//
//////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::PrepareParameterDictionary(CDicList &CListDic, ULONG ulType, int iLinked) {
  shared_ptr<CUnitDictionary> pDic;

	ULONG j = 0;

	switch (iLinked) {
	case 0: // ѡ��û�ж�̬���ӵĲ���
  case 1:
  case -1:
		for (int i = 0; i < 16; i++) {
			if (!m_pInterfacePara[i]->IsLinked()) { // û�ж�̬���ӣ�
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
// �������ֻ�ڲ�����װʱ���á��˺����Ĳ���ֻ����ȫ�������뵥Ԫ���С�
//
//������
// 1.���ò�������֮����ڲ����ӱ�־
// 2.���ñ������Ƿ���ж�̬���ӵĽضϹ��ܡ�
//
/////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetInnerDataLinkFlag(void)
{
  CUnitList listUnit;
  bool fFindInnerDataLink = false;

  if (m_fCutOff) { // ����Ѿ������˽ضϱ�־�����˳���
    return true;
  }

  // �ݹ����Ƿ��ڲ�����֮��������
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (m_pInterfacePara[i]->GetParaType() & tINPUT) { // ���������Ͳ���
        listUnit.clear();
        listUnit.push_front(this);
        (m_pInterfacePara[i]->GetDestUnit())->CheckInnerDataLink(i, m_pInterfacePara[i]->GetDestIndex(), &listUnit); /// �������ڲ���������
      }
    }
  }
  
  // �����Ƿ����ڲ��������ӱ�־�����������ýضϱ�־
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (m_pInterfacePara[i]->GetParaType() & tINPUT) { // ���������Ͳ���
        for (int j = 0; j < 16; j++) {
          if (m_pInterfacePara[i]->IsInnerDataLinked(j)) {
            fFindInnerDataLink = true;
          }
        }
      }
    }
  }

  if (fFindInnerDataLink) { // ��������ڲ�����֮���ж�̬���ӣ������ýض�Ϊ��
    m_fCutOff = false;
  }
  else { // ���û�з����ڲ�����֮���ж�̬���ӣ������ýض�Ϊ�档
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
//  �ڲ�������ֻ����Encapsulation����
//
//  ��װ�������������²㲿��
//
//////////////////////////////////////////////////////////////////////
bool CUnitComponent::EncapsulateBelowComponent(CUnitList & listTotalUnit) {
  // ��װ�²㲿��������еĻ���	
  for (const auto punit : m_CUnitList) {
    ASSERT(punit->IsEncapsulating());     // 
    punit->Encapsulation(listTotalUnit);
    punit->SetCompiledFlag(true);
  }
  return(true);
}

bool CUnitComponent::CheckComponentSelf() {
  // ��鲿��״̬�Ƿ���ȷ
  bool fGood = true;
  for (const auto punit : m_CUnitList) {
    if (!punit->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      ASSERT(((CUnitComponent *)punit)->GetExectivePriority() == 0); // ȷ�����в�������δ������
    }
    else { // ���²㲿��
      if (punit->IsEncapsulable()) {
        ASSERT(punit->IsEncapsulated());
      }
      fGood = ((CUnitComponent *)punit)->CheckComponentSelf(); // �������²�
    }
  }
  return(fGood);
}

/////////////////////////////////////////////////////////////////////////
//
// ���ɱ�����������ʱ��Ԫ����
//
//
//
/////////////////////////////////////////////////////////////////////////
bool CUnitComponent::CreateRunTimeUnitList() {
  // �����ڲ���Ԫ���е�����ʱ��Ԫ����
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
// �����������д������������ʱ�����ڲ���Ӧ��Ԫ������һ���µĶ�̬���ӡ�
// ���Ƿ�װ�����е�һ�����̣����ܵ������á�
//
//
///////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::CreateNewDynLinkFromInterfaceOutputTypePara() {
  CUnitBase * pcUnit;
  CUDLList *pDLList;
  shared_ptr<CUnitDynLink> pDL;
  // ����������������͵����ڲ�Դ��Ԫ������һ���µĶ�̬���ӣ�Ŀ�ĵ�ԪΪ��������
  // �˲���Ҫ���ڴ������������Ĺ���ִ�С�
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (m_pInterfacePara[i]->GetParaType() & tOUTPUT) { // ��������Ͳ���
        //��Դ��Ԫ�м���һ�µĶ�̬����
        pDL = make_shared<CUnitDynLink>();
        pDL->SetDestUnit(this);
        pDL->SetDestIndex(i);
        pDL->SetSrcUnit(pcUnit = m_pInterfacePara[i]->GetSrcUnit());
        pDL->SetSrcIndex(m_pInterfacePara[i]->GetSrcIndex());
        pDL->SetDynLinkClass(UNIT_TO_UNIT); // ������COMPONENT_TO_UNIT��COMPONENT_TO_COMPONENT�����֣�����ᱻ����Ĵ���
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
// ��������Ķ�̬����
// 
// �����̬����������UNIT_TO_COMPONENT����Դ��Ԫλ���ϲ㵥Ԫ�����У�
// �����̬��������ΪCOMPONENT_TO_COMPONENT,��Դ��Ԫλ���ϲ㵥Ԫ�����еĲ������ڲ���Ԫ�����С�
// 
// COMPONENT_TO_COMPONENTѰ�ҵķ������ǴӴ���Ĳ���unitTotalUist�У��ҵ��Ա���ԪΪĿ�ĵ�Ԫ��Դ��Ԫ��
// Ȼ��ȷ�ϴ�Դ��Ԫ��λ�ڱ������ĵ�Ԫ�����У��Ϳ��Դ����ˡ��˶�̬���ӱ�Ȼ��COMPONENT_TO_COMPONENT��
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::HandleTheDynLinkedInComponent(CUnitList & listTotalUnit) {
  CUnitComponent * pUpperCpt = nullptr;
  pUpperCpt = (CUnitComponent *)this;
  ASSERT(pUpperCpt != nullptr);

  // Ѱ���Ƿ�������뱾�����Ķ�̬����

  CString strParaName;
  CUDLList *pDLList = nullptr;
  int iPo = 16;

  ASSERT(m_lDynLinkToNumber == 0); // ��װǰ������������붯̬����Ϊ��
  for (const auto pcunit : listTotalUnit) { // �����������ĵ�Ԫ������Ѱ��
    pDLList = pcunit->GetDynLinkList();
    for (const auto pDL : *pDLList) {
      switch (pDL->GetDynLinkClass()) {
      case COMPONENT_TO_COMPONENT: // Դ��Ԫλ���ϲ㵥Ԫ�����в������ڲ���Ԫ����      
      case UNIT_TO_COMPONENT: // Դ��Ԫλ���ϲ㵥Ԫ������
        if (find(m_CUnitList.begin(), m_CUnitList.end(), pDL->GetDestUnit()) != m_CUnitList.end()) { // �ҵ������뵥Ԫ
          if ((pDL->GetSrcUnit()->GetComponentUpper()) != this) { // ���Դ��Ԫ��λ�ڱ������ڣ����ҵ���
            iPo = FindNextAvailableParaPosition();
            ASSERT((iPo >= 0) && (iPo < 16));
            strParaName = this->GetParaName(iPo) + "-(" + (pDL->GetSrcUnit())->GetName() + "." + pDL->GetSrcUnit()->GetParaName(pDL->GetSrcIndex()) + ")";
            this->SetParaName(iPo, strParaName); // ���������ƺ�����������ݵ�Ԫ������
            this->SetParaSrcUnit(iPo, pDL->GetSrcUnit());       // �����͵Ĳ�����Դ��Ԫ���ϲ��������ݵ�Ԫ
            this->SetParaDestUnit(iPo, pDL->GetDestUnit());
            this->SetParaDestIndex(iPo, pDL->GetDestIndex());
            this->SetParaSrcIndex(iPo, pDL->GetSrcIndex());
            this->SetParaType(iPo, pDL->GetDynLinkType() | tINPUT | tMODIFIABLE); // �˲����������Ͳ��������ӷ���Ϊ�ⲿ��Ԫ�����������ڲ��ĵ�Ԫ
            ASSERT((this->GetParaType(iPo) & tOUTPUT) == 0); // ������ͬʱ��������ͱ�־
            this->SetParaLinkedFlag(iPo, true);
            this->SetParameterLock(iPo, true); // �����뱾�����Ķ�̬��������һ
            // ���˶�̬���ӵ�Ŀ�ĵ�Ԫ����Ϊ������
            pDL->SetDestUnit(this); // Ŀ�ĵ�ԪΪ������
            pDL->SetDestIndex(iPo);	// ��������Ϊѡ��Ĳ���λ��
            if (pDL->GetDynLinkClass() == COMPONENT_TO_COMPONENT) {
              pDL->SetDynLinkClass(COMPONENT_TO_UNIT); // ������װ����ͬΪ�򵥵�Ԫ
            }
            else {
              pDL->SetDynLinkClass(UNIT_TO_UNIT);// ������װ����ͬΪ�򵥵�Ԫ
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
// ���ñ�������ִ�����ȼ�
//
//
//
/////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::SetMyselfExectivePriority( void ) {
  ASSERT(m_fCompiled); // ����Ǳ����ϲ㲿�����������ļ�������Ҫ������������ִ�����ȼ�����ԶΪ��)
  for (int i = 0; i < 16; i++) {
    if (m_pInterfacePara[i]->IsLinked()) {
      if (((m_pInterfacePara[i]->GetParaType()) & tINPUT) && (m_vfSelected[i] == true)) { // �����Ͳ����Ҵ���Դ��Ԫ��ֻ���ϲ㵥Ԫ��������
        if ((m_pInterfacePara[i]->GetSrcUnit())->GetExectivePriority() > m_lExectivePriority) {
          m_lExectivePriority = (m_pInterfacePara[i]->GetSrcUnit())->GetExectivePriority();    
          m_lExectivePriority++; //��������ִ�����ȼ�Ҫ���������뵥Ԫ��ִ�����ȼ���һ��(�������Ϊû������Դ��Ԫ����ִ�����ȼ�Ϊ2����

        }
      }
    }
  }
  return(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//
// ���������������Ķ�̬���ӡ�
//
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CUnitComponent::HandleTheDynLinkedfromComponent( void ) {
  // �ڱ�����������һ���µĶ�̬���ӣ��������Ķ�̬���Ӹ�����µĶ�̬���ӣ���Դ��Ԫ��Ϊ��������
  // �����ڲ�Դ��Ԫ��Ŀ�ĵ�Ԫ����Ϊ�����������ò��������е�Ŀ�ĵ�Ԫָ���Ŀ�Ĳ���������
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
        // ���ò������������
        strParaName = this->GetParaName(iPo) + "-(" + (pDL->GetSrcUnit())->GetName()
          + "." + pDL->GetSrcUnit()->GetParaName(pDL->GetSrcIndex()) + ")";
        this->SetParaName(iPo, strParaName); // ���������ƺ�����������ݵ�Ԫ������
        this->SetParaSrcUnit(iPo, pDL->GetSrcUnit());
        this->SetParaDestUnit(iPo, nullptr);  // ���Ŀ�ĵ�Ԫָ��
        this->SetParaDestIndex(iPo, -1);      // ���Ŀ�ĵ�Ԫ��������
        this->SetParaSrcIndex(iPo, pDL->GetSrcIndex());
        this->SetParaType(iPo, pDL->GetDynLinkType() | tOUTPUT | tMODIFIABLE); // �˲���������Ͳ��������ӷ���Ϊ�Ӳ����ڲ������������ⲿ
        ASSERT((this->GetParaType(iPo) & tINPUT) == 0); // ������ͬʱ���������ͱ�־
        this->SetParaLinkedFlag(iPo, true);
        //����һ���µĶ�̬���ӣ����뱾�����Ķ�̬����������
        pDLNew = make_shared<CUnitDynLink>();
        pDLNew->SetDestUnit(pDL->GetDestUnit());  // Ŀ�ĵ�Ԫ�����ڴ˶�̬���Ӵ�
        pDLNew->SetDestIndex(pDL->GetDestIndex());// Ŀ�ĵ�Ԫ��������
        pDLNew->SetSrcUnit(this);
        pDLNew->SetSrcIndex(iPo);
        pDLNew->SetDynLinkType(pDL->GetDynLinkType());
        if (pDL->GetDynLinkClass() == COMPONENT_TO_UNIT) {
          pDLNew->SetDynLinkClass(UNIT_TO_UNIT); // ���Ķ�̬��������
        }
        else pDLNew->SetDynLinkClass(UNIT_TO_COMPONENT); // ���Ķ�̬��������
        pDLNew->SetLinkPointList(pDL->GetLinkPointList());  // ����̬���������зŵ�������
        this->m_listDynLink.push_back(pDLNew);
        pDL->GetLinkPointList()->clear();   // �������ڲ���Ԫ����ʾ������
        // ��ԭ��̬���ӵ�Ŀ�ĵ�Ԫ����Ϊ����������������Ҳ��Ϊ��صĲ���λ��
        pDL->mTest_pDestUnitSaved = pDL->GetDestUnit(); // �����ã����浱ǰĿ�ĵ�Ԫ��
        pDL->SetDestUnit(this); // Ŀ�ĵ�ԪΪ������
        pDL->SetDestIndex(iPo);	// ��������Ϊѡ��Ĳ���λ��
        // Ŀǰ�Ȳ����������
        //pDL->SetDynLinkClass(UNIT_TO_CptPARAMETER); // �˶�̬���ӵ�����ΪUNIT_TO_CptPARAMETER.ֻ�д˴�������������͡�
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
// Encapsulation() ������װ����
//
// ������
//    CUnitList &listTotalUnit  �˴α�������е�Ԫ���ܺ͡�
//
// ����ֵ:
//		bool : ����ɹ�Ϊ��, ����Ϊ��.
//
// ����:
//    ���������Լ��ĵ�Ԫ����.������������ӳ��Ķ�̬�����ᵽ�˲����Ĳ���������Ȼ���ֹ��翴��ȥ��
//    ��װ�����������һ�Ǳ��������ļ�ʱ������װ�����Ķ�������ʱ�����ڲ���Ԫ�Ѿ���������ˣ�
//                  ���ǵ���ִ�з�װ������������ʱ���ڲ���Ԫû�б��루�˹���Ŀǰδʵ�֣���
//
// ��Ҫ��   
//    ��װ�����Ķ��������뷢���ڲ�������֮ǰ��
// ������
// 1.ÿ����Ԫ������Encapsulation���Ȱ��²�Ĳ�����װ������
// 2.�ҵ�����������������Ķ�̬���ӣ������ӵ������Ĳ���������ȥ
// 3.�������������뷽������ⲿ���ķ�����ͬ��
//   �����ⲿ��������ͣ���˲���Ϊ����Ͳ�����������װ��Ĳ�������ͬΪһ�򵥵�Ԫ������Ϊ��򵥵�Ԫ�����ơ�
// 4.����Ƿ���ڲ���֮����ڲ��������ӣ���ͨ���ڲ���Ԫ���еĶ�̬���ӣ����������������������ӣ����粻���������ñ������Ľضϱ�־��
//
////////////////////////////////////////////////////////////////////
bool CUnitComponent::Encapsulation(CUnitList & listTotalUnit) {
  if (m_fEncapsulated) { // �Ѿ���װ���򷵻�
    return(true);
  }
  ASSERT(!m_fCompiled); // �����������û�б��������

  if (m_fEncapsulationPermitted) { // ����ǿɷ�װ����
    // ��װ�²㲿��������еĻ���	
    EncapsulateBelowComponent(listTotalUnit);

    // ��鲿��״̬�Ƿ���ȷ
    CheckComponentSelf();

    // ����������������͵����ڲ�Դ��Ԫ������һ���µĶ�̬���ӣ�Ŀ�ĵ�ԪΪ��������
    // �˲���Ҫ���ڴ������������Ĺ���ִ�С�
    CreateNewDynLinkFromInterfaceOutputTypePara();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // ����ӿڲ��������ڲ������ĺ��ⲿ����Ķ�̬�������ӵ�����������

    // �������뱾�����Ķ�̬����
    HandleTheDynLinkedInComponent(listTotalUnit);

    // ���������������Ķ�̬���ӡ�
    // �ڱ�����������һ���µĶ�̬���ӣ��������Ķ�̬���Ӹ�����µĶ�̬���ӣ���Դ��Ԫ��Ϊ��������
    // �����ڲ�Դ��Ԫ��Ŀ�ĵ�Ԫ����Ϊ�����������ò��������е�Ŀ�ĵ�Ԫָ���Ŀ�Ĳ���������
    HandleTheDynLinkedfromComponent();

    // Ѱ���Ƿ��ڲ�������֮�����������ӣ�����Ӧ���ò�������Ľضϱ�־.��ʱ��δ���÷�װ��־
    // ����������ñ��������һ���������ڲ���̬������δ���úã�������ԡ�
    SetInnerDataLinkFlag();

    m_fEncapsulated = true; // ������÷�װ��־
  }
  else { // ���ɷ�װ��������ֻ��װ���²㲿�������ڲ���Ԫ����Ҫ�������ϲ�Ĳ��������߾��������ļ����ĵ�Ԫ�����С�
    // ��װ�²㲿��������еĻ���	
    EncapsulateBelowComponent(listTotalUnit);

    // ��鲿��״̬�Ƿ���ȷ
    CheckComponentSelf();
    ASSERT(m_fEncapsulated == false);
  }
  return (true);
}

bool CUnitComponent::Compilation(void)
{
  // ���뱾�������ڲ���Ԫ���С�����ǲ�������������.���ɷ�װ�Ĳ���ҲҪ�����ҡ�
  for (const auto punit : m_CUnitList) {
    if (punit->IsEncapsulating()) { // �ų��ѷ�װ�Ĳ�����
      ASSERT(!punit->IsCompiled());
      ASSERT(!punit->IsEncapsulated());
      punit->Compilation();
    }
  }

  // ��ʱ�²㲿�����ѱ�������,ȷ��֮��
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
 
  // ��ʼ����
  CompileUnitList(&m_CUnitList, &m_CRunTimeUnitList); // ����˵�Ԫ����

  return true;
}

///////////////////////////////////////////////////////////////////////
//
// ����ǿɷ�װ�Ĳ������򷵻��Ƿ��װ��־������ǲ������װ�Ĳ���������ԶΪ��
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
// ��������ʱ��Ԫ���У�������Ӧ��ִ������
// create exective list 1ms, 10ms, 100ms, 1Second, 1Minute
// �ṩ���Ƶ��Ϊÿ����ִ��һ�ε����е�Ԫ��Ŀǰ��һ����ÿ�εĹ���10����������ִ�С�
//
////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::PrepareRunTimeList(void) {	

  for (const auto pcunit : m_CRunTimeUnitList) {
		pcunit->PrepareRunTimeList();		// ���ɲ���������ʱ̬����.
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
// ���ò����������ص���Ϣ��
// ��������Ѿ�����װ�ˣ����ڲ���Ԫ���в�����Ҫ���á�
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::ResetCompileFlag( void ) {
  if (!m_fEncapsulated) { // ���û�з�װ���������ڲ���Ԫ����
    for (const auto pcunit : m_CUnitList) {
      pcunit->ResetCompileFlag();
    }
  }
  // ���ñ���������
  m_CRunTimeUnitList.clear();
  CUnitBase::ResetCompileFlag();
}

///////////////////////////////////////////////////////////////////////
//
// �������Ŀǰû�д�������̣�ֱ�Ӷ���Ϊ���˳��ˡ�
//
//
//
////////////////////////////////////////////////////////////////////////
int CUnitComponent::FindNextAvailableParaPosition(void) {
	for (int i = 0; i < 16; i++) {
		if (!m_pInterfacePara[i]->IsLinked()) return(i);
	}
  // ִ�е��˴�ʱ������ˡ�
  ASSERT(0);
  return(16); // �������16�����ʶ�������Ч����ֵΪ15.����16�������ˡ�
}

void CUnitComponent::SetUpperScrollPosition(CPoint pt) {
	m_ptScrollPositionUpper = pt;
}

const CPoint& CUnitComponent::GetUpperScrollPosition(void) {
	return (m_ptScrollPositionUpper);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// �˺�������̳���UnitBase���ʶ��������ʽӦ��򵥵�Ԫ���ơ�
// ֻ�з�װ��Ĳ����ŵ��ô˺�����δ��װ�Ĳ������õ�����������PrepareParaDictionary��
//
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////
void CUnitComponent::PrepareParaDictionary(CDicList &CListDic, ULONG ulType) {
	// ׼������
  shared_ptr<CUnitDictionary> pDic;
  ULONG ulParaType = ulType & (tMODIFIABLE | tINPUT | tOUTPUT);
  ULONG j = 0;

  ASSERT(m_fEncapsulated); // ���ô˺���ʱ�����������뱻��װ��
  ASSERT(ulParaType != 0);// ����������͡������ͻ��߿ɱ��޸ĵģ���������֮һ��
  switch ( ulParaType ) {
  case tMODIFIABLE : // ѡ�����еĲ���������Object��Unit���ɶ�̬����
    for (int i = 0; i < 16; i++) {		// ��16������
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetDynLinkType() & ulType) && (m_vfSelected[i] == false)) { // ��д��Ĳ�����������Object������������
          m_aulSuitable[j++] = i;
          pDic = make_shared<CUnitDictionary>(this, i, ulType);
          CListDic.push_back(pDic);
        }
      }
    }
    break;
  case tMODIFIABLE + tINPUT : // �����ͻ�������ͣ�����Unit֮�����ɶ�̬����
    for (int i = 0; i < 16; i++) {		// ��16������
      if (m_pInterfacePara[i]->IsLinked()) {
        if ((m_pInterfacePara[i]->GetParaType() | ulType) == m_pInterfacePara[i]->GetParaType()) {
          if (m_vfSelected[i] == false) { // ��д���������û������Դ��Ԫ
            m_aulSuitable[j++] = i;
            pDic = make_shared<CUnitDictionary>(this, i, ulType);
            CListDic.push_back(pDic);
          }
        }
      }
    }
    break;
  case tMODIFIABLE + tOUTPUT: 
    for (int i = 0; i < 16; i++) {		// ��16������
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

// ѡ����ʵĲ���,׼����������.
void CUnitComponent::SelectParameter(ULONG ) {
  for (int i = 0; i < 16; i++) {
    m_aulSuitable[i] = -1;
  }
}

