///////////////////////////////////////////////////////////////////////////
//	CObjectPrimitive class declaration.
//	Company											: FireBird software Inc.
//	Author 											: Guo
//	Create Time									: 1996, 6, 12
//
//	�������
//
///////////////////////////////////////////////////////////////////////////
#pragma once

#include"afx.h"

using namespace std;

#include<vector>

class CObjectPrimitive : public CObject {
public :
  CObjectPrimitive(const CString& Name);
  CObjectPrimitive( void );
  
  DECLARE_SERIAL(CObjectPrimitive);
  
  ~CObjectPrimitive();

// Attributes
public:
  virtual void Serialize( CArchive& ar );
  
// Operations
public:
	// �õ�����Ԫ������
  virtual CString		GetName(void);
	// ���ñ���Ԫ������
	virtual void 			SetName(const CString& strName ) { m_strName = strName; }
	
	// �õ����������
	virtual const CString&	GetClassNameStr( void );
	
	virtual void 			FormatSave( CArchive& ar );
	virtual void 			FormatRead( CArchive& ar );

	// ����ɨ������
	void							SetScanRate( ULONG ulScanRate		// �µ�ɨ������
																);
	// �õ�ɨ������
	ULONG							GetScanRate( void ) const;
	// ��������
	bool							CountDown( ULONG ulTick			// ÿ�ε����Ĳ���
										);

	// ����ѡ����־����������ʾ��
  void 							SetSelect(bool flags) { m_fSelected = flags; }
	// �Ƿ�ѡ��
  bool  						IsSelect(void) const { return m_fSelected; }

  // ���������ѡ������
  virtual void      ClearParaSelectedFlag(  void );

  // ���û���������.
  virtual bool 			SetParameterLock(ULONG ulIndex, // ��������.
                                     bool fSelected // ���û�����־.
                                     );
  virtual bool      IsParameterLocked(ULONG ulIndex);
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected);


	// ���ø��±�־
  virtual void 			SetUpdateFlag(bool fUpdate) { m_fNeedUpdate = fUpdate; }
	// �Ƿ���Ҫ����
	virtual	bool 			IsNeedUpdate( void ) { return(m_fNeedUpdate); }

	// ���������Լ��
	virtual bool			CheckSelf( void );

	// �Ƿ�ɱ�����ȥ.
  virtual bool      CanViewIn(void) const { return false; }

	// ת����ǰ״̬
  virtual bool      DumpCurrentStatus( char * pch		// ǰ׺�ַ���
										) const;

protected: 
	// assist function

#ifdef _DEBUG
	virtual	void AssertValid() const;
	virtual	void Dump(CDumpContext& dc) const;
#endif  

// Class data declaration
protected :

protected :
// Serialized Data
  CString  				m_strName; 		// this object's name, used for ID
	INT32						m_lScanRate;	// ɨ������, 1����/��.
  INT64           m_lReserved1; // �������Ա�����
  INT64           m_lReserved2; // �������Ա�����

// no serialized data
	INT32						m_lScanRateDynamicCount;	// ������ʱ��������ִ�е�Ԫ����ʱ��

	vector<bool>    m_vfSelected;		// �����Ƿ�ѡ���־,�������������
	bool						m_fSelected;			// �Ƿ�ѡ�б�־.
	bool						m_fNeedUpdate;	// �Ƿ���Ҫ������ʾ��־.
};


