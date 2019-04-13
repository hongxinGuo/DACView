////////////////////////////////////////////////////////////////////////////////////////////////
//
// 部件类，是单元类的一个封装体，正式名称为“部件”，被当成一个单独的单元来处理，外部无法看进去。
// 有外特性，无内部特性，与外界的信息交流只存在于部件的动态连接内。
//
// CUnitComponent有16个参数池，用于封装部件保存联入和联出的参数，内部单元需要联出或者联入的参数，封装后与参数池的某一个参数链接，
// 此后外部与此部件的交流，必须通过参数池中的参数。部件参数的链接方向，以部件为主体，从部件联出到上层单元的，其动态链接类型为tOUTPUT，
// 从上层单元联入本部件内部单元的，其动态链接类型为tINPUT。
//
// 部件有两种封装模式：
// 1.已经有与外部链接的参数。此种封装，就是把现有的联入联出参数，都用参数池来接力，切断部件内部与外部的直接联系，此后与部件的联系只能通过参数接口来进行。
// 2.参数尚未与外部链接。此种封装，就是创建参数池中的参数与需要联入或者联出的参数的动态链接。此后外部把此部件视同为一个简单部件。
//
//
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
//动作：
// 1.检验本单元序列包含的动态链接中是否存在循环，存在的话退出并报告。
// 2.将所有的单元组成一个单独的单元序列.部件视同简单单元，复合单元把自己的子单元加入单元序列中，但本身排除在外。
// 3.每个单元封装前，都预先把自己下层的部件封装起来。
// 4.设置目标单元的有源标志.
// 5.前四个步骤是准备工作，在CompileUnitList函数中完成。
// 6.找到联入和联出本部件的动态链接，都链接到部件的参数数组上去
// 7.部件的联出联入方向，从内部看是输入性的，从外部看就是输出型，故而定义参数的输出方向，与从外部看的方向相同，即从外部看是输出型，则此参数为输出型参数。
// 8.封装后的部件天然具有数据截断效应，故而其m_fCutOff为真。
//   如果其内部单元序列的动态链接导致其接口的两个参数之间产生了数据链接，则部件就丧失了其固有的截断效应。
// 9.固件本身的运行优先级，封装后由上层单元序列决定。固件执行时，首先执行输入参数的读取（已经完成，可不做）；
//   然后将输入型数据存储到内部相关单元的参数中；
//   接着执行内部单元序列。固件内部单元序列的执行优先级，独立确定，不受上层单元序列的影响。
//   最后执行输出型参数的数据输出以及部件本身的动态链接（只有输出类型，是封装后的部件再生成的动态链接）。
//10.封装后的部件在进行动态链接时，输入型的修改相关的参数内容（因为输入型只允许一个动态链接）；
//   输出型则建立新的动态链接，存储与部件本省的动态链接序列中。
//
//实现:
//  1. 每个部件最多允许有16个输入或输出参数.
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
  CUnitComponent(void);// 生成可封装的部件

  DECLARE_SERIAL(CUnitComponent);

  ~CUnitComponent();

  // Attributes
public:
  virtual void Serialize(CArchive& ar);

public:
  // 用于存储ViewIn时上层的滚动条位置
  void            SetUpperScrollPosition(CPoint pt);
  const CPoint&   GetUpperScrollPosition(void);

  // 设置或解除参数锁.
  virtual bool 			SetParameterLock(ULONG ulIndex, bool fSelected) override;
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected) override;
  virtual void      SetParaLockFlag(void) override;

  // 检查本单元的动态链接是否形成了循环
  virtual bool			LoopDetect(CUnitList * pCUnitList) override;
  // 检查本单元的循环中是否存在截断
  virtual bool      CheckCutOff(CUnitList * pCUnitList) override;
  // 设置本单元是否有动态链接至包含本单元的部件参数处.
  virtual void			CheckInnerDataLink(INT64 lSrcIndex,         // 部件数据源参数的位置
                                           INT64 lDestParaPos,      // 联入本部件动态链接的目的参数位置 
                                           CUnitList * pCUnitList) override; // 存储当前寻找到的单元序列.

  virtual const CString&    GetClassNameStr(void) override;
  virtual bool              IsMe(const CString& strName) override;
  virtual void              ToShow(CDC * const pdc) override;    // draw myself on DC

  virtual void              Exective(void) override;
  virtual bool							ExectiveDynLink(void) override;

  virtual void              SetDestUnitPriority(void) override;

  virtual void              SetCompiledFlag(bool fFlag) override;
  virtual bool              IsCompiled(void) override;

  virtual bool              SetProperty(void) override;          // 设置部件的参数等

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
  virtual ULONG							GetParaType(ULONG lIndex) override;		// 得到本类的参数类型,其类型就是源单元参数的类型

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
  // 选择合适的参数,准备参数索引.
  virtual void     	SelectParameter(ULONG ulType	// 被选择参数的类型
                                    ) override;

  // 这一个参数的是独立的准备参数字典，用于未封装时选择未链接的参数位置，与内部单元的某个参数相链接。与简单单元那个没关系。
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

  // 检查内部单元序列中是否存在于部件输入输出参数皆相链接的单元，如有则设置参数内部相链接标志（用于封装后外部单元是否存在动态链接循环的检查）
  bool              SetInnerDataLinkFlag(void);
  bool              SetInnerDataLinked(INT64 lSrcIndex, INT64 lDestIndex, bool fFlag);
  bool              IsInnerDataLinked(ULONG ulIndex, ULONG ulDestIndex);

	bool							HaveParameter( void );
	void							SetViewInFlag(bool fFlag);
	CUCPara **        GetParaInterface(void);

	virtual INT32*		GetArrayIndex(void) override;

  virtual bool      IsParameterLocked(ULONG ulIndex) override;
  // 清除参数被选择索引
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
  // 以下几个函数，是为了测试Encapsulation函数方便而设立的，当在调试状态下编译时外部可见。不允许单独调用，必须按顺序连续。
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
	bool						m_fEncapsulationPermitted; // 是否允许封装此部件。不允许封装的部件其表现形式与复合部件完全一样。此变量在初始化时决定。
  INT64           m_lReserved11; // 保留，以备后用
  INT64           m_lReserved12; // 保留，以备后用

// No Serialized Data
  INT32		       	m_aulSuitable[16];	// 参数池的选择容器，用于建立合适的词典

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
  // 以下两个变量用于测试接口输入输出的数量
  INT32           m_lInputParaNumber;
  INT32           m_lOutputParaNumber;
};                        


  
