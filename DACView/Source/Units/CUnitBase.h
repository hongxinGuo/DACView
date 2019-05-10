///////////////////////////////////////////////////////////////////////////
//	CUnitBase class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo
//	Create Time									: 1999, 2, 27
//
// ����Unit�࣬���˲������������е�Unit�඼�������򵥵�Ԫ����
//
//
///////////////////////////////////////////////////////////////////////////
#ifndef __UNIT_UNITBASE_H__
#define __UNIT_UNITBASE_H__

#include"typedef.h"
#include "CObjectPrimitive.h"

#include "CUnitDynLink.h"
#include "cUnitDictionary.h"

class CUnitComponent;

class CUnitBase : public CObjectPrimitive {
public :
  CUnitBase(const CString& Name, CPoint pt); 
  CUnitBase();
  
  DECLARE_SERIAL(CUnitBase);
  
  ~CUnitBase();

// Attributes
public:
  virtual void Serialize( CArchive& ar ) override;
  
// Operations
public:
  // �õ�����Ԫ������
  virtual CString		GetName(void) override;

  void            SetUpperUnitList(CUnitList * pUnitList) { m_pUnitListUpper = pUnitList; }
  CUnitList *     GetUpperUnitList(void) { return m_pUnitListUpper; }

	// �õ�����Ԫ��λ�ô�С.
  const CRect&			GetSize(void) const { return m_rectArea; }
	// ���ñ���Ԫ��λ�ô�С.
  void 							SetSize(const CRect& RectSize) { m_rectArea = RectSize; }

	// ����ע��
  void							SetComment(const CString& strComment) { m_strComment = strComment; }
	//�õ�ע���ַ���
  CString						GetComment(void) { return m_strComment; }


	// �õ�����Ԫ�Ķ�̬�������е�ָ��.
  CUDLList * 				GetDynLinkList(void) { return &m_listDynLink; }

	// �Ƿ񱾵�Ԫ�Ĳ�������������Ԫ�Ĳ���ʹ��.
  bool							IsLinkToComponent( void );
	// ���ò��������������Ԫ�Ĳ�����־.
	void							LinkToComponent( bool fFlag // ����Ϊ��,ȡ������Ϊ��.
																		);

	// �Ƿ񱾵�Ԫ��Object����������
	bool							IsLinkFromObject(void) { return m_lLinkFromObject; }
	// ���ô�Object��������������
	void							SetLinkFromObjectFlag(bool fFlag  // ����Ϊ�棬ȡ������Ϊ��
																					);
	// �ж��ٲ������벿��.
  INT32							HowManyLinkToComponent(void) { return m_lLinkToComponent; }

  void              LinkFromComponent(bool fFlag); 
 // �Ƿ񱾵�Ԫ�Ĳ�������������Ԫ�Ĳ���ʹ��.
  bool							IsLinkFromComponent(void) { if (m_lLinkFromComponent > 0) return(true); else return(false); }
 // �ж��ٲ������벿��.
  INT32							HowManyLinkFromComponent(void) { return(m_lLinkFromComponent); }

  void 							SetComponentUpper(CUnitComponent * pUnitComponent) { m_pUnitComponentUpper = pUnitComponent; } 
  CUnitComponent *	GetComponentUpper(void) const { return m_pUnitComponentUpper; }

	// ���ñ���Ԫ�Ķ�̬����ɾ����־.�籾��Ԫ�Ķ�̬��������listUnit, �����ñ�־.
	virtual void			SetDeleteDynLinkFlag( CUnitList& listUnit // ��ɾ���ĵ�Ԫ����,
																				);
	// �������Ԫ�Ķ�̬����ɾ����־.
	virtual void			ClearDeleteDynLinkFlag( void );
	
	// ���ýضϱ�־.
  void 							SetCutOff(bool fFlag) { m_fCutOff = fFlag; }
  bool							IsCutoff(void) const { return m_fCutOff; }

  //���õ�Ԫ�ѱ������־
  void              SetCompiledFlag(bool fFlag) { m_fCompiled = fFlag; }
  bool              IsCompiled(void) { return(m_fCompiled); }
	
	// �õ�����Ԫ��ִ������ֵ.
  INT32 						GetExectivePriority(void) const { return m_lExectivePriority; }
	// ֱ�����ñ���Ԫ��ִ������ֵΪulPriority
  void							SetExectivePriorityDirect(INT32 lPriority) { m_lExectivePriority = lPriority; }
	// ���ñ���Ԫ��ִ������ֵ.
	virtual bool 			SetExectivePriority( ULONG ulPriority // Ҫ���õ�����ֵ.
																				);
	
  // ���ñ���Ԫ��Ŀ�굥Ԫ����������ֵ.
	virtual void      SetDestUnitPriority( void ); 
	
  // ���ñ����־.
	virtual void			ResetCompileFlag( void );

  // �����Լ�, ������ʹ��,������ԪΪ��
  virtual bool			Encapsulation(CUnitList & ) { return true; } // ��װ�Լ��� �˺������ڷ�װ������,������Ԫ�޶���
  virtual bool      Compilation(void); // �����Լ����˺������ڱ��벿������������Ԫֻ�����ñ����־��

  // ��װ���
	virtual bool			IsEncapsulated(void);
  // �ɷ񱻷�װ�� �򵥵�Ԫ���ɱ���װ������������װ�����ϵ�Ԫ�����ɷ�װ�����ⲿ�������ɱ���װ��
  virtual bool      IsEncapsulable(void) { return false;  }

  bool              IsEncapsulating(void) { return m_fEncapsulating; }  // ���ڷ�װ�����б�־
  void              SetEncapsulatingFlag(bool fFlag) { m_fEncapsulating = fFlag; } // ���ý����װ���̱�־

	// ׼������ʱ̬����.
	virtual void			PrepareRunTimeList( void );

	// ��һ����̬���Ӽ��뱾��Ԫ�Ķ�̬��������.
  void 							AddDynLink(shared_ptr<CUnitDynLink> punitDynLink) { m_listDynLink.push_back(punitDynLink); }

	// �۲첢������Ԫ�Ķ�̬����.
	virtual bool 			ArrangeDynLink( void );
	// ɾ������Ԫ��ָ��pUnit�Ķ�̬����.
	virtual bool			DeleteDynLink( CUnitBase * pUnit	// ָ��ɾ����unit
																	);

	// ���ɶ�һ�޶��ĵ�Ԫ����.
	virtual bool			CreateUniName( CUnitList& listUnit // ָ����Ҫ�����unit list
																	);
	// ���ڱ���Ԫ��̬���ӵ�����λ��.
  virtual void      AdjustDynLinkLinePosition( CUnitBase * pc,	// ָ�򱻵���λ�õĵ�Ԫ
																							 CPoint ptStart,	//��̬�����ߵ���ʼ��
																							 CPoint ptEnd);		// ��̬�����ߵ���ֹ��
	
	// ���ñ���Ԫ��̬������ָ��ĵ�Ԫ�Ĳ�����.
	virtual void 			SetParaLockFlag( void );

	// �õ�����Ԫ�������������.
  ULONG							GetInputParameterNumber(void) { return m_lDynLinkToNumber; }
	// ���ñ���Ԫ����Ĳ�������
  void							SetInputParameterNumber(INT32 lNumber) { ASSERT(lNumber >= 0); m_lDynLinkToNumber = lNumber; }

	// ����Ԫ�Ƿ���ڴ��ϲ�����Ĳ���
	virtual bool			IsDynLinkFromUpper(void);

  virtual bool			IsOverFlow(void) { return m_fOverFlow; }
	
	bool							IsHaveSourceUnit( void ) const;
	void							SetDynLinkToNumber(long lValue) { m_lDynLinkToNumber = lValue; }
	
	// ���ñ���Ԫ�ķ���ѭ����־.
  bool              SetLoopDetectFlag( CUnitBase * pcunit // ָ�򱻼��ĵ�Ԫ
																			);

	// �������Ԫ�ķ���ѭ����־.
	virtual void      ClearLoopDetectFlag( void );

	// ��鱾��Ԫ�Ƿ���ѭ����pCUnitList��.
	virtual bool			LoopDetect( CUnitList * pCUnitList	// �洢��ǰѰ�ҵ��ĵ�Ԫ����.
																);

  // ��鱾��Ԫ��ѭ�����Ƿ���ڽض�
  virtual bool      CheckCutOff(CUnitList * pCUnitList);

  // ���ñ���Ԫ�Ƿ��ж�̬��������������Ԫ�Ĳ���������.
  virtual void			CheckInnerDataLink(INT64 lSrcIndex, INT64 lDestParaPos, CUnitList * pCUnitList); // �洢��ǰѰ�ҵ��ĵ�Ԫ����.
  
  // ��鱾��Ԫ���ڲ��Ƿ��д���.
	virtual bool			CheckSelf( void );
	
	virtual const CString&		GetClassNameStr( void ) override;				// �õ����������
  virtual CString						GetParaName(ULONG ) { ASSERT(false); return(""); }		// �õ�����Ĳ�������
  virtual ULONG							GetParaType(ULONG ) { ASSERT(false); return 0; }		// �õ�����Ĳ������͡��˺����ǵõ��������е����ͣ���GetDynLinkTypeֻ�õ��������������е�һ��

	// �Ƿ�ɱ�����. UnitBase�಻����
  virtual bool							CanLinkIn(void) { return false; }

	// �Ƿ�����.
	virtual bool 							IsMe( const CString& strName // �����ĵ�Ԫ����.
																);

	// �õ�����Ԫ�������������,����ͻ����������.��Щ��Ԫֻ������Ͳ��������������ͼ�η������ȡ�
  // CUnitBase�༰��󲿷���������������������̬���ӣ������������������Դ��ڡ�
  // �����������ࣨĿǰֻ��CUnitSine��CUnitInputOutput��CUnitQuad�������ֻࣩ��������������������������������������������������������
	virtual ULONG			GetUnitType( void ) { return(tINPUT | tOUTPUT); }
	
	// ִ�б���Ԫ.
  virtual void  		Exective(void) { ASSERT(false); }
	// ִ�б���Ԫ�Ķ�̬����.
	virtual bool 			ExectiveDynLink( void );

	// �õ�����ֵ��ֵ
	virtual bool 							GetBool( ULONG index	// ����λ��
																		);
	// ���ò���ֵ����
	virtual bool 							SetBool( ULONG index,	// ����λ��
																		 bool fValue	// ��־
																		);
	// �õ�������ֵ
	virtual LONG 							GetInteger(ULONG index	// ����λ��
																			);
	// ������������
	virtual bool 							SetInteger(ULONG index, // ����λ��
																			LONG iValue		// ������ֵ
																			);
	// �õ���������ֵ
	virtual double 						GetDouble(ULONG index		// ����λ��
																			);
	// ���ø���������
	virtual bool  						SetDouble(ULONG index,	// ����λ��
																			double eValue	// ������ֵ
																			);
	// �õ��ַ�����ֵ
	virtual const CString 		GetString(ULONG index		// ����λ��
																			);     
	// �����ַ�������
	virtual bool		 					SetString(ULONG index,	// ����λ��
																			const CString& strValue	// ������ֵ
																			); 

	// �õ���̬���ӵ����ͣ�������,������,ʵ���͵�
	virtual ULONG 		GetDynLinkType( ULONG ulIndex		// ����λ��
																		);
	// ���û���������.
	virtual bool 			SetParameterLock( ULONG ulIndex, // ��������.
																			bool fSelected // ���û�����־.
																			) override;

  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected) override;

	// ׼�������ǵ�.
	virtual void			PrepareParaDictionary( CDicList &CListDic,	// �����Ĵǵ�.
																					 ULONG ulType					// Ҫƥ��Ĳ�������.
																					 );
	// ѡ����ʵĲ���,׼����������.
	virtual void     	SelectParameter( ULONG ulType  // ��ѡ��Ĳ�������
                                    );  
	// �õ��������Ͳ���������.
  virtual INT32			GetIndex(ULONG ) { ASSERT(false); return 0; }

	// �õ�����Ԫ��Ĳ������е�ַ.
	virtual ParaName* GetParaNameAddress( void ) { ASSERT(false); return nullptr; }
	// ���ɲ����ʵ�,�ڲ�ʹ��.
	virtual void			CreateParaDictionary( CDicList &m_CDicList, // �����Ĵǵ�.
																					ULONG ulType // Ҫƥ��Ĳ�������.
																					);
	
	// �Ƿ��ڱ���Ԫ��ʾ��Χ��.
	virtual bool 			InIt(POINT const pt) { return (m_rectArea.PtInRect(pt)); }
	// ��ʾ����Ԫ.
  virtual void 			ToShow(CDC * const pdc);
	// ���ý���.
  virtual void 			SetFocus( CDC *pdc);
	// ɾ������.
  virtual void 			ClearFocus( CDC *pdc);
	// ���ñ���Ԫ���ڲ�ѡ��.
  virtual bool 			SetProperty( void );

	// ������Ԫ���뵥Ԫ������.
	virtual void 			AddToList(CUnitList& UnitList);

	// �õ�����Ԫ��Ĳ���ѡ����еĵ�ַ, �ڲ�ʹ��.
  virtual INT32*		GetArrayIndex(void) { ASSERT(false); return nullptr; }

  virtual bool      IsParameterLocked(ULONG ) override { ASSERT(false); return false; }

  // �����Ƿ�pUnit��Ԫ������pCpt�����У�����Ƕ�װ�����
  bool              IsInThisComponent(CUnitComponent * pCpt, CUnitBase * pUnit);


public : 
	// assist function
	void DrawBitmap( CDC * pdc, CBitmap& bitmap, CRect& rect );

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif

public:			
    
protected :
// Serialized Data
  CString  		m_strComment;				// ����Ԫ������.
  CRect    		m_rectArea;  					// ��Ԫ������λ��.
  CUDLList		m_listDynLink;				// ��Ԫ�Ķ�̬��������.

	bool 				m_fCutOff;						// �Ƿ����ýض�.
	bool	 			m_fAutoExective;			// �Ƿ��Զ�ִ��.
  INT64       m_lReserved3;         // �������Ա�����
  // �������Ĳ����洢ʱ����ô洢�����������������Ͳ���Ҫ�ٴ�ȷ��������������ֵ�ˡ�
	INT32			  m_lDynLinkToNumber;	// ����Ԫ�������ӵĸ���. 
	INT32 		  m_lExectivePriority;	// ��Ԫ��ִ������ֵ,����ȷ��ִ��˳��,1������.

// No Serialized Data
protected:
	CUnitComponent * m_pUnitComponentUpper;	// ָ���������Ԫ�ĸ��ϵ�Ԫ,nullptr���û��.
	CUnitList *     m_pUnitListUpper;	// ָ���ϲ㲿��(���߾��Ǳ�SQI�ļ�)�ĵ�Ԫ����

	bool			m_fOverFlow;						// �Ƿ����, ����ʱʹ��.

  bool      m_fCompiled;            // ��Ԫ�����ѱ����롣����ǲ������򲿼��ڲ���Ԫ�����ѱ����롣
  bool      m_fEncapsulating;       // ��װ�б�־��������ȷ��ִ�����ȼ�ʱ�ж��Ƿ���뱾�������ڲ���Ԫ���У������е���Ҫ���룩

	INT32			m_lCountDLToNumber;		  // �������Ӽ�����.

	INT32			m_lLinkToComponent;		  // ����Ԫ����������Ķ�̬��������.����ʾʱʹ�á�
  INT32     m_lLinkFromComponent;   // ����Ԫ�Ӳ�������Ķ�̬��������������ʾʱʹ�á�
	INT32     m_lLinkFromObject;			// ����Ԫ��Object����Ķ�̬��������������ʾʱʹ�á�

  // �����ñ���
public:
  bool      mtest_fEncapsulating;   // ��¼�Ƿ��ڱ����С����ڱ�����ɺ󣬼�¼�˴α�����ĵ�Ԫ
};

typedef shared_ptr<CUnitBase> CUnitBasePtr;

#endif

