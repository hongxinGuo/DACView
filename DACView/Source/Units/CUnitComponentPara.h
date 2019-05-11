///////////////////////////////////////////////////////////////////////////////////////////////
//
//  部件的接口参数实现类。
//
//	由于这是一个实现类，而且部件作为终极实现类不再成为其他类的基类，故而内部变量采用了形式，以简化外部调用的复杂度。
//
//	增加一个16个布尔变量的数组，用于设置此动态链接是否输出到上层的标志。
//	如果此参数为输入型数据，而且此数据经过内部单元的动态链接又链接到接口的其他输出型参数上，则设置相应的标志（用于上层单元查找循环）
//
//////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __UNIT_COMPONENT_PARAMETER_H__
#define __UNIT_COMPONENT_PARAMETER_H__

#include"stdafx.h"

#include"CObjectPrimitive.h"
#include"CUnitBase.h"

class CUCPara : public CObjectPrimitive {
public:
	CUCPara();

	DECLARE_SERIAL(CUCPara);

	~CUCPara();

public:
	virtual void		Serialize(CArchive& ar) override;

public:
	CUnitBasePtr			GetSrcUnit(void);
	CUnitBasePtr			GetDestUnit(void);
	bool						SetSrcUnit(CUnitBasePtr pcSrc);
	bool						SetDestUnit(CUnitBasePtr pcDest);


	virtual CString	GetName(void) override;
	bool						SetName(CString Name);

	ULONG						GetDynLinkType(void);
	bool						SetDynLinkType(ULONG ulType);

  ULONG						GetParaType(void);
  bool						SetParaType(ULONG ulType);

	bool						SetDataLinked(ULONG ulIndex, bool fFlag);
	bool						HaveDataLink(void);
	bool						IsDataLinked(ULONG ulIndex);

	INT64						GetSrcIndex(void);
	INT64						GetDestIndex(void);
	bool						SetSrcIndex(LONG ulValue);
	bool						SetDestIndex(LONG ulValue);

	bool						SetBoolValue(bool Value);
	bool						SetIntegerValue(LONGLONG Value);
	bool						SetDoubleValue(double Value);
	bool						SetStringValue(CString Value);

	bool						GetBoolValue(void);
	LONGLONG				GetIntegerValue(void);
	double					GetDoubleValue(void);
	CString					GetStringValue(void);

	bool						IsLinked(void);					// 参数是否与内外部单元相链接
	bool						SetLinkedFlag(bool fFlag); // 设置参数链接标志

	bool						IsInnerDataLinked(long lIndex); // 内部两参数之间是否间接有数据链接（即通过内部或者外部单元之间的动态链接而产生数据链接）
	bool						SetInnerDataLinked(long lIndex, bool fFlag); // 设置参数内部之间链接标志

	void						SetExectivePriority(LONG lPriority) { m_lExectivePriority = lPriority; }
	INT32						GetExectivePriority(void) { return m_lExectivePriority; }

protected:
	// Serialized Data
	CUnitBasePtr		m_pSrcUnit;				// 指向选中的联入单元
	CUnitBasePtr		m_pDestUnit;			// 指向选中的联出单元
	CString				m_strName;				//本接口参数的名称
	INT64					m_ulParaType;			//参数动态链接的类型: tBOOL、tDOUBLE、tWORD、tSTRING、tINPUT、tOUTPUT、tMODIFIABLE

	INT64					m_lSrcIndex;			//参数在选中的联入单元（m_pSrcUnit）参数数组中的位置
	INT64					m_lDestIndex;		//参数在选中的联出单元（m_pDestUnit）参数数组中的位置
	bool          m_fLinked;				//此参数是否已经与外部或者内部的其他单元链接
	bool					m_afInnerDataLinked[16]; // 如果此参数为输入型数据，而且此数据经过内部单元的动态链接又链接到接口的其他输出型参数上，则设置相应的标志（用于上层单元查找循环）

	// No Serialized Data
	INT32 				m_lExectivePriority;// 参数的执行优先值,用于确定执行顺序,1最优先.输入型参数的优先级，由上层单元序列决定；输出型的优先级，由内部单元决定
	bool					m_fValue;
	LONGLONG			m_llValue;
	double			  m_dValue;
	CString				m_strValue;
};

#endif
