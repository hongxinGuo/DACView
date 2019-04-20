///////////////////////////////////////////////////////////////////////////
//	CUnitBase class declaration.
//	Company											: FireBird software Inc.
//	Author 											: guo
//	Create Time									: 1999, 2, 27
//
// ����Unit�࣬���˲����͸��ϵ�Ԫ���������е�Unit�඼�������򵥵�Ԫ����
//
//
///////////////////////////////////////////////////////////////////////////
#pragma once

#include<list>

#include "CObjectPrimitive.h"

#include "CUnitDynLink.h"
#include "cUnitDictionary.h"

class CUnitBase;
class CUnitComponent;

typedef CList<CUnitDictionary *, CUnitDictionary *> CDicList;
typedef CList<CUnitBase *, CUnitBase *> CUnitList; 

class CUnitBase : public CObjectPrimitive {
public :
  CUnitBase(const CString& Name, CPoint pt); 
  CUnitBase( void );
  
  DECLARE_SERIAL(CUnitBase);
  
  ~CUnitBase();

// Attributes
public:
  virtual void Serialize( CArchive& ar ) override;
  
// Operations
public:
  // �õ�����Ԫ������
  virtual CString		GetName(void) override;

	void            SetUpperUnitList(CUnitList * pUnitList);
	CUnitList *     GetUpperUnitList(void);

	// �õ�����Ԫ��λ�ô�С.
	const CRect&			GetSize( void ) const;
	// ���ñ���Ԫ��λ�ô�С.
	void 							SetSize( const CRect& RectSize );

	// ����ע��
	void							SetComment(const CString& strComment // ע��
																);
	//�õ�ע���ַ���
	CString						GetComment(void);


	// �õ�����Ԫ�Ķ�̬�������е�ָ��.
	CUDLList * 				GetDynLinkList( void );

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
	ULONG							HowManyLinkToComponent( void );

  void              LinkFromComponent(bool fFlag); 
 // �Ƿ񱾵�Ԫ�Ĳ�������������Ԫ�Ĳ���ʹ��.
  bool							IsLinkFromComponent(void) { if (m_lLinkFromComponent > 0) return(true); else return(false); }
 // �ж��ٲ������벿��.
  INT32							HowManyLinkFromComponent(void) { return(m_lLinkFromComponent); }

	void 							SetComponentUpper(CUnitComponent * pUnitComponent // ָ���������Ԫ�ĸ��ϲ���.
																					);
	CUnitComponent *	  GetComponentUpper( void ) const;

	// ���ñ���Ԫ�Ķ�̬����ɾ����־.�籾��Ԫ�Ķ�̬��������listUnit, �����ñ�־.
	virtual void			SetDeleteDynLinkFlag( CUnitList& listUnit // ��ɾ���ĵ�Ԫ����,
																				);
	// �������Ԫ�Ķ�̬����ɾ����־.
	virtual void			ClearDeleteDynLinkFlag( void );
	
	// ���ýضϱ�־.
	void 							SetCutOff( bool fFlag // �Ƿ�ضϱ�־.
															);
  bool							IsSetCutOff( void ) const;

  //���õ�Ԫ�ѱ������־
  virtual void      SetCompiledFlag(bool fFlag) { m_fCompiled = fFlag; }
  virtual bool      IsCompiled(void) { return(m_fCompiled); }
	
	// �õ�����Ԫ��ִ������ֵ.
	INT32 						GetExectivePriority( void ) const;
	// ֱ�����ñ���Ԫ��ִ������ֵΪulPriority
	void							SetExectivePriorityDirect( ULONG ulPriority );
	// ���ñ���Ԫ��ִ������ֵ.
	virtual void 			SetExectivePriority( ULONG ulPriority // Ҫ���õ�����ֵ.
																				);
	
  // ���ñ���Ԫ��Ŀ�굥Ԫ����������ֵ.
	virtual void      SetDestUnitPriority( void ); 
	
  // ���ñ����־.
	virtual void			ResetCompileFlag( void );

	// �����Լ�, ������ʹ��,������ԪΪ��
	virtual bool			Encapsulation( CUnitList & listTotalUnit );
	// ��װ���
	virtual bool			IsEncapsulated(void);

  // �ɷ񱻷�װ�� �򵥵�Ԫ���ɱ���װ������������װ�����ϵ�Ԫ������Ĳ��������ɱ���װ��
  virtual bool      IsEncapsulable(void) { return false;  }

	// ׼������ʱ̬����.
	virtual void			PrepareRunTimeList( void );

	// ��һ����̬���Ӽ��뱾��Ԫ�Ķ�̬��������.
	void 							AddDynLink( CUnitDynLink * punitDynLink // ָ��̬���ӵ�ָ��.
															 ) ;
	
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
	ULONG							GetInputParameterNumber( void );
	// ���ñ���Ԫ����Ĳ�������
	void							SetInputParameterNumber(LONG lNumber // ��������ĸ���
																						);

	// ����Ԫ�Ƿ���ڴ��ϲ�����Ĳ���
	virtual bool			IsDynLinkFromUpper(void);

	virtual bool			IsOverFlow(void);
	
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
	virtual CString						GetParaName( ULONG index );			// �õ�����Ĳ�������
	virtual ULONG							GetParaType( ULONG ulIndex );		// �õ�����Ĳ������͡��˺����ǵõ��������е����ͣ���GetDynLinkTypeֻ�õ��������������е�һ��

	// �Ƿ�ɱ�����.
	virtual bool							CanLinkIn( void );

	// �Ƿ�����.
	virtual bool 							IsMe( const CString& strName // �����ĵ�Ԫ����.
																);

	// �õ�����Ԫ�������������,����ͻ����������.��Щ��Ԫֻ������Ͳ��������������ͼ�η������ȡ�
	virtual ULONG			GetUnitType( void );
	
	// ִ�б���Ԫ.
	virtual void  		Exective( void );
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
	virtual INT32			GetIndex( ULONG ulIndex				// ���ʲ���������
															);

	// �õ�����Ԫ��Ĳ������е�ַ.
	virtual ParaName* GetParaNameAddress( void );
	// ���ɲ����ʵ�,�ڲ�ʹ��.
	virtual void			CreateParaDictionary( CDicList &m_CDicList, // �����Ĵǵ�.
																					ULONG ulType // Ҫƥ��Ĳ�������.
																					);
	
	// �Ƿ��ڱ���Ԫ��ʾ��Χ��.
	virtual bool 			InIt( POINT const pt // ��ǰ��. 
												);
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
	virtual INT32*		GetArrayIndex( void );

  virtual bool      IsParameterLocked(ULONG ulIndex) override;

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
  CString  		m_cstrComment;				// ����Ԫ������.
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

	INT32			m_lCountDLToNumber;		// �������Ӽ�����.

	INT32			m_lLinkToComponent;		// ����Ԫ����������Ķ�̬��������.����ʾʱʹ�á�
  INT32     m_lLinkFromComponent;  // ����Ԫ�Ӳ�������Ķ�̬��������������ʾʱʹ�á�
	INT32     m_lLinkFromObject;			// ����Ԫ��Object����Ķ�̬��������������ʾʱʹ�á�

  // �����ñ���
public:

};
