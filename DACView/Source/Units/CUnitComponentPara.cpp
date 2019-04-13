///////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 部件参数接口的实现类。
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include"stdafx.h"

#include "CUnitComponentPara.h"

IMPLEMENT_SERIAL(CUCPara, CObjectPrimitive, 1 | VERSIONABLE_SCHEMA);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif  

CUCPara::CUCPara() : CObjectPrimitive() {
	CString str = "Para";

	m_pSrcUnit = nullptr;
	m_pDestUnit = nullptr;
	m_strName = str;
	m_ulParaType = 0;
	m_lSrcIndex = -1;
	m_lDestIndex = -1;
	m_fLinked = false;

  m_fValue = false;
  m_llValue = 0;
  m_dValue = 0;
  m_strValue = "";

	for (int i = 0; i < 16; i++) {
		m_afInnerDataLinked[i] = false;
	}
}

CUCPara::~CUCPara()
{
	for (int i = 0; i < 16; i++) {

	}
}

void	CUCPara::Serialize(CArchive& ar) {
	CObjectPrimitive::Serialize(ar);
	INT64 itemp;
  INT32 iInnerDataLinked;

	if (ar.IsStoring()) {
		ar << m_pSrcUnit << m_pDestUnit;
		ar << m_strName;
		ar << m_lSrcIndex << m_lDestIndex;
		ar << m_ulParaType << (INT64)m_fLinked;
    for (int i = 0; i < 16; i++) {
      // 参数内部数据链接标志也需要存储起来。封装后的部件再次编译需要时间，且容易出错，故而无法再次设置此标志。
      ar << (INT32)m_afInnerDataLinked[i]; 
    }
	}
	else {
		ar >> m_pSrcUnit >> m_pDestUnit;
		ar >> m_strName;
		ar >> m_lSrcIndex >> m_lDestIndex;
		ar >> m_ulParaType >> itemp;
		m_fLinked = (bool)itemp;
    for (int i = 0; i < 16; i++) {
      ar >> iInnerDataLinked;
      m_afInnerDataLinked[i] = (bool)iInnerDataLinked;
    }
	}
}

CUnitBase *	CUCPara::GetSrcUnit(void) {
	return(m_pSrcUnit);
}

CUnitBase *	CUCPara::GetDestUnit(void) {
	return(m_pDestUnit);
}

bool	CUCPara::SetSrcUnit(CUnitBase * pc) {
	m_pSrcUnit = pc;
	return(true);
}

bool	CUCPara::SetDestUnit(CUnitBase * pc) {
	m_pDestUnit = pc;
	return(true);
}

CString	CUCPara::GetName() {
	return(m_strName);
}

bool CUCPara::SetName(CString Name) {
	m_strName = Name;
	return(true);
}

ULONG CUCPara::GetDynLinkType(void) {
	return(m_ulParaType & (tBOOL | tWORD | tDOUBLE | tSTRING));
}

bool CUCPara::SetDynLinkType(ULONG ulType) {
  ulType &= tBOOL | tWORD | tDOUBLE | tSTRING;
  m_ulParaType &= tMODIFIABLE | tINPUT | tOUTPUT;
  m_ulParaType |= ulType;
	return(true);
}

ULONG CUCPara::GetParaType(void)
{
  return(m_ulParaType);
}

bool CUCPara::SetParaType(ULONG ulType)
{
  m_ulParaType = ulType;
  return false;
}

INT64	CUCPara::GetSrcIndex(void) {
	return(m_lSrcIndex);
}

INT64	CUCPara::GetDestIndex(void) {
	return(m_lDestIndex);
}
bool CUCPara::SetSrcIndex(LONG lValue) {
	m_lSrcIndex = lValue;
	return(true);
}

bool CUCPara::SetDestIndex(LONG lValue) {
	m_lDestIndex = lValue;
	return(true);
}

bool CUCPara::SetDataLinked(ULONG ulIndex, bool fFlag) {
	ASSERT((ulIndex >= 0) && (ulIndex < 16));
	m_afInnerDataLinked[ulIndex] = fFlag;
	return(true);
}

bool CUCPara::HaveDataLink(void) {
	for (int i = 0; i < 16; i++) {
		if (m_afInnerDataLinked[i]) return(true);
	}
	return(false);
}

bool CUCPara::IsDataLinked(ULONG ulIndex) {
	ASSERT((ulIndex >= 0) && (ulIndex < 16));
	return(m_afInnerDataLinked[ulIndex]);
}

bool CUCPara::SetBoolValue(bool Value) {
	ASSERT(m_ulParaType & tBOOL);
	m_fValue = Value;

	return(true);
}

bool CUCPara::SetIntegerValue(LONGLONG Value) {
	ASSERT(m_ulParaType & tWORD);
	m_llValue = Value;
	return(true);
}

bool CUCPara::SetDoubleValue(double Value) {
	ASSERT(m_ulParaType & tDOUBLE);
	m_dValue = Value;
	return(true);
}

bool CUCPara::SetStringValue(CString Value) {
	ASSERT(m_ulParaType & tSTRING);
	m_strValue = Value;
	return(true);
}

bool CUCPara::GetBoolValue(void) {
	return(m_fValue);
}

LONGLONG CUCPara::GetIntegerValue(void) {
	return(m_llValue);
}

double CUCPara::GetDoubleValue(void) {
	return(m_dValue);
}

CString CUCPara::GetStringValue(void) {
	return(m_strValue);
}

bool CUCPara::IsLinked(void) {
	return(m_fLinked);
}

bool CUCPara::SetLinkedFlag(bool fFlag) {
	m_fLinked = fFlag;
	return(true);
}

bool CUCPara::IsInnerDataLinked(long lIndex) {
	return(m_afInnerDataLinked[lIndex]);
}

bool CUCPara::SetInnerDataLinked(long lIndex, bool fFlag) {
	m_afInnerDataLinked[lIndex] = fFlag;
	return true;
}


