////////////////////////////////////////////////////////////////////////////////////////////////
//
// �����࣬�ǵ�Ԫ���һ����װ�壬��ʽ����Ϊ����������������һ�������ĵ�Ԫ�������ⲿ�޷�����ȥ��
// �������ԣ����ڲ����ԣ���������Ϣ����ֻ�����ڲ����Ķ�̬�����ڡ�
//
// CUnitComponent��16�������أ����ڷ�װ������������������Ĳ������ڲ���Ԫ��Ҫ������������Ĳ�������װ��������ص�ĳһ���������ӣ�
// �˺��ⲿ��˲����Ľ���������ͨ���������еĲ������������������ӷ����Բ���Ϊ���壬�Ӳ����������ϲ㵥Ԫ�ģ��䶯̬��������ΪtOUTPUT��
// ���ϲ㵥Ԫ���뱾�����ڲ���Ԫ�ģ��䶯̬��������ΪtINPUT��
//
// ���������ַ�װģʽ��
// 1.�Ѿ������ⲿ���ӵĲ��������ַ�װ�����ǰ����е������������������ò��������������жϲ����ڲ����ⲿ��ֱ����ϵ���˺��벿������ϵֻ��ͨ�������ӿ������С�
// 2.������δ���ⲿ���ӡ����ַ�װ�����Ǵ����������еĲ�������Ҫ������������Ĳ����Ķ�̬���ӡ��˺��ⲿ�Ѵ˲�����ͬΪһ���򵥲�����
//
//
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
//������
// 1.���鱾��Ԫ���а����Ķ�̬�������Ƿ����ѭ�������ڵĻ��˳������档
// 2.�����еĵ�Ԫ���һ�������ĵ�Ԫ����.������ͬ�򵥵�Ԫ�����ϵ�Ԫ���Լ����ӵ�Ԫ���뵥Ԫ�����У��������ų����⡣
// 3.ÿ����Ԫ��װǰ����Ԥ�Ȱ��Լ��²�Ĳ�����װ������
// 4.����Ŀ�굥Ԫ����Դ��־.
// 5.ǰ�ĸ�������׼����������CompileUnitList��������ɡ�
// 6.�ҵ�����������������Ķ�̬���ӣ������ӵ������Ĳ���������ȥ
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
//ʵ��:
//  1. ÿ���������������16��������������.
//
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "cUnitBase.h"
#include"CUnitComponentPara.h"
	
class CUnitComponent : public CUnitBase {
public:
  CUnitComponent(const CString& Name, CPoint pt, bool fEncapsulationPermitted);
  CUnitComponent(bool fEncapsulationPermitted);
  CUnitComponent(void);// ���ɿɷ�װ�Ĳ���

  DECLARE_SERIAL(CUnitComponent);

  ~CUnitComponent();

  // Attributes
public:
  virtual void Serialize(CArchive& ar);

public:
  // ���ڴ洢ViewInʱ�ϲ�Ĺ�����λ��
  void            SetUpperScrollPosition(CPoint pt);
  const CPoint&   GetUpperScrollPosition(void);

  // ���û���������.
  virtual bool 			SetParameterLock(ULONG ulIndex, bool fSelected) override;
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected) override;
  virtual void      SetParaLockFlag(void) override;

  // ��鱾��Ԫ�Ķ�̬�����Ƿ��γ���ѭ��
  virtual bool			LoopDetect(CUnitList * pCUnitList) override;
  // ��鱾��Ԫ��ѭ�����Ƿ���ڽض�
  virtual bool      CheckCutOff(CUnitList * pCUnitList) override;
  // ���ñ���Ԫ�Ƿ��ж�̬��������������Ԫ�Ĳ���������.
  virtual void			CheckInnerDataLink(INT64 lSrcIndex,         // ��������Դ������λ��
                                           INT64 lDestParaPos,      // ���뱾������̬���ӵ�Ŀ�Ĳ���λ�� 
                                           CUnitList * pCUnitList) override; // �洢��ǰѰ�ҵ��ĵ�Ԫ����.

  virtual const CString&    GetClassNameStr(void) override;
  virtual bool              IsMe(const CString& strName) override;
  virtual void              ToShow(CDC * const pdc) override;    // draw myself on DC

  virtual void              Exective(void) override;
  virtual bool							ExectiveDynLink(void) override;

  virtual void              SetDestUnitPriority(void) override;

  virtual void              SetCompiledFlag(bool fFlag) override;
  virtual bool              IsCompiled(void) override;

  virtual bool              SetProperty(void) override;          // ���ò����Ĳ�����

  virtual bool 							GetBool(ULONG index) override;
  virtual bool 							SetBool(ULONG index, bool fVaule) override;
  virtual LONG 							GetInteger(ULONG index) override;
  virtual bool 							SetInteger(ULONG index, LONG iValue) override;
  virtual double 						GetDouble(ULONG index) override;
  virtual bool  						SetDouble(ULONG index, double dValue) override;
  virtual const CString		  GetString(ULONG index) override;
  virtual bool              SetString(ULONG index, const CString& strValue) override;

  virtual INT32			        GetIndex(ULONG ulIndex) override;
  virtual ULONG			        GetDynLinkType(ULONG ulIndex) override;
  virtual ULONG							GetParaType(ULONG lIndex) override;		// �õ�����Ĳ�������,�����;���Դ��Ԫ����������

  virtual void      AddToList(CUnitList& UnitList) override;

  virtual bool      DeleteDynLink(CUnitBase * pUnit) override;

  virtual bool      CreateUniName(CUnitList& listUnit) override;

  virtual void      SetDeleteDynLinkFlag(CUnitList& listUnit) override;
  virtual void      ClearDeleteDynLinkFlag(void) override;

	virtual void      ClearLoopDetectFlag(void) override;
  virtual void      AdjustDynLinkLinePosition(CUnitBase * pc, CPoint ptStart, CPoint ptEnd) override;

  virtual bool      CanViewIn(void) const override;
  virtual bool			CanLinkIn(void) override;


  CUnitList *       GetUnitList(void);
  CUnitList *       GetRunTimeUnitList(void);

  virtual void			ResetCompileFlag(void) override;


  virtual void			PrepareParaDictionary(CDicList &CListDic, ULONG ulType) override;
  // ѡ����ʵĲ���,׼����������.
  virtual void     	SelectParameter(ULONG ulType	// ��ѡ�����������
                                    ) override;

  // ��һ���������Ƕ�����׼�������ֵ䣬����δ��װʱѡ��δ���ӵĲ���λ�ã����ڲ���Ԫ��ĳ�����������ӡ���򵥵�Ԫ�Ǹ�û��ϵ��
  void							PrepareParameterDictionary(CDicList &CListDic, ULONG ulType, int iLinked);

  bool							SetParaSrcUnit(LONG lSrcIndex, CUnitBase * pUnit);
  bool							SetParaDestUnit(LONG lDestIndex, CUnitBase * pUnit);
  CUnitBase *				GetParaSrcUnit(LONG lSrcIndex);
  CUnitBase *				GetParaDestUnit(LONG lDestIndex);
  bool							SetParaExectivePriority(LONG lIndex, LONG lPriority);


  virtual CString		GetParaName(ULONG ulindex) override;
  bool							SetParaName(LONG index, CString strName);

  bool							SetParaType(LONG index, LONG ulType);
  bool							SetParaLinkedFlag(LONG lIndex, bool fFlag);
  bool              IsParaLinked(LONG lIndex);
  INT64							GetParaSrcIndex(LONG ulSrcIndex);
  INT64							GetParaDestIndex(LONG ulDestIndex);
  bool							SetParaSrcIndex(LONG ulSrcIndex, LONG ulValue);
  bool							SetParaDestIndex(LONG ulDestIndex, LONG ulValue);

  virtual bool			Encapsulation(CUnitList & listTotalUnit) override;
  virtual bool			IsEncapsulated(void) override;
  virtual bool			IsEncapsulable(void)  override { return m_fEncapsulationPermitted; }
  
  void              SetEncapsulateFlag(bool fFlag) { m_fEncapsulated = fFlag; }
  bool              IsPermitEncapsulation(void) { return m_fEncapsulationPermitted; }

	virtual void			PrepareRunTimeList( void ) override;

// implememtation

  // ����ڲ���Ԫ�������Ƿ�����ڲ���������������������ӵĵ�Ԫ�����������ò����ڲ������ӱ�־�����ڷ�װ���ⲿ��Ԫ�Ƿ���ڶ�̬����ѭ���ļ�飩
  bool              SetInnerDataLinkFlag(void);
  bool              SetInnerDataLinked(INT64 lSrcIndex, INT64 lDestIndex, bool fFlag);
  bool              IsInnerDataLinked(ULONG ulIndex, ULONG ulDestIndex);

	bool							HaveParameter( void );
	void							SetViewInFlag(bool fFlag);
	CUCPara **        GetParaInterface(void);

	virtual INT32*		GetArrayIndex(void) override;

  virtual bool      IsParameterLocked(ULONG ulIndex) override;
  // ���������ѡ������
  virtual void      ClearParaSelectedFlag(  void ) override;


#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

#ifdef _DEBUG
public:
#else 
private:
#endif 
  // ���¼�����������Ϊ�˲���Encapsulation��������������ģ����ڵ���״̬�±���ʱ�ⲿ�ɼ��������������ã����밴˳��������
  bool             EncapsulateBelowComponent(CUnitList & listTotalUnit);
  bool             CheckComponentSelf(void);
  bool             CreateRunTimeUnitList(void);
  bool             CreateNewDynLinkFromInterfaceOutputTypePara(void);
  bool             HandleTheDynLinkedInComponent(CUnitList & listTotalUnit);
  bool             SetMyselfExectivePriority(void);
  bool             HandleTheDynLinkedfromComponent(void);

private:
	int							FindNextAvailableParaPosition(void);

protected :
// Serialized Data
	CUnitList				m_CRunTimeUnitList;
	CUnitList       m_CUnitList;

	CUCPara  *      m_pInterfacePara[16];

	bool						m_fCanViewIn;
	bool						m_fEncapsulated;
	bool						m_fEncapsulationPermitted; // �Ƿ������װ�˲������������װ�Ĳ����������ʽ�븴�ϲ�����ȫһ�����˱����ڳ�ʼ��ʱ������
  INT64           m_lReserved11; // �������Ա�����
  INT64           m_lReserved12; // �������Ա�����

// No Serialized Data
  INT32		       	m_aulSuitable[16];	// �����ص�ѡ�����������ڽ������ʵĴʵ�

	CPoint          m_ptScrollPositionUpper;

	CUnitList				m_CUnitList1MS;
	CUnitList				m_CUnitList10MS;
	CUnitList 			m_CUnitList100MS;
	CUnitList 			m_CUnitList1Second;
	CUnitList 			m_CUnitList1Minute;

	volatile LONG   m_lCount10MS;
	volatile LONG		m_lCount100MS;
	volatile LONG		m_lCountSecond;
	volatile LONG		m_lCountMinute;
	volatile LONG		m_lCountHour;

public:
  // ���������������ڲ��Խӿ��������������
  INT32           m_lInputParaNumber;
  INT32           m_lOutputParaNumber;
};                        


  
