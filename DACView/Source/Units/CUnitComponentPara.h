///////////////////////////////////////////////////////////////////////////////////////////////
//
//  �����Ľӿڲ���ʵ���ࡣ
//
//	��������һ��ʵ���࣬���Ҳ�����Ϊ�ռ�ʵ���಻�ٳ�Ϊ������Ļ��࣬�ʶ��ڲ�������������ʽ���Լ��ⲿ���õĸ��Ӷȡ�
//
//	����һ��16���������������飬�������ô˶�̬�����Ƿ�������ϲ�ı�־��
//	����˲���Ϊ���������ݣ����Ҵ����ݾ����ڲ���Ԫ�Ķ�̬���������ӵ��ӿڵ���������Ͳ����ϣ���������Ӧ�ı�־�������ϲ㵥Ԫ����ѭ����
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

	bool						IsLinked(void);					// �����Ƿ������ⲿ��Ԫ������
	bool						SetLinkedFlag(bool fFlag); // ���ò������ӱ�־

	bool						IsInnerDataLinked(long lIndex); // �ڲ�������֮���Ƿ������������ӣ���ͨ���ڲ������ⲿ��Ԫ֮��Ķ�̬���Ӷ������������ӣ�
	bool						SetInnerDataLinked(long lIndex, bool fFlag); // ���ò����ڲ�֮�����ӱ�־

	void						SetExectivePriority(LONG lPriority) { m_lExectivePriority = lPriority; }
	INT32						GetExectivePriority(void) { return m_lExectivePriority; }

protected:
	// Serialized Data
	CUnitBasePtr		m_pSrcUnit;				// ָ��ѡ�е����뵥Ԫ
	CUnitBasePtr		m_pDestUnit;			// ָ��ѡ�е�������Ԫ
	CString				m_strName;				//���ӿڲ���������
	INT64					m_ulParaType;			//������̬���ӵ�����: tBOOL��tDOUBLE��tWORD��tSTRING��tINPUT��tOUTPUT��tMODIFIABLE

	INT64					m_lSrcIndex;			//������ѡ�е����뵥Ԫ��m_pSrcUnit�����������е�λ��
	INT64					m_lDestIndex;		//������ѡ�е�������Ԫ��m_pDestUnit�����������е�λ��
	bool          m_fLinked;				//�˲����Ƿ��Ѿ����ⲿ�����ڲ���������Ԫ����
	bool					m_afInnerDataLinked[16]; // ����˲���Ϊ���������ݣ����Ҵ����ݾ����ڲ���Ԫ�Ķ�̬���������ӵ��ӿڵ���������Ͳ����ϣ���������Ӧ�ı�־�������ϲ㵥Ԫ����ѭ����

	// No Serialized Data
	INT32 				m_lExectivePriority;// ������ִ������ֵ,����ȷ��ִ��˳��,1������.�����Ͳ��������ȼ������ϲ㵥Ԫ���о���������͵����ȼ������ڲ���Ԫ����
	bool					m_fValue;
	LONGLONG			m_llValue;
	double			  m_dValue;
	CString				m_strValue;
};

#endif
