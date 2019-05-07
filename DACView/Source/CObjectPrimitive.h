///////////////////////////////////////////////////////////////////////////
//	CObjectPrimitive class declaration.
//	Company											: FireBird software Inc.
//	Author 											: Guo
//	Create Time									: 1996, 6, 12
//
//	最基本类
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
	// 得到本单元的名称
  virtual CString		GetName(void);
	// 设置本单元的名称
	virtual void 			SetName(const CString& strName ) { m_strName = strName; }
	
	// 得到本类的类名
	virtual const CString&	GetClassNameStr( void );
	
	virtual void 			FormatSave( CArchive& ar );
	virtual void 			FormatRead( CArchive& ar );

	// 设置扫描速率
	void							SetScanRate( ULONG ulScanRate		// 新的扫描速率
																);
	// 得到扫描速率
	ULONG							GetScanRate( void ) const;
	// 倒数计数
	bool							CountDown( ULONG ulTick			// 每次倒数的步数
										);

	// 设置选定标志，（用于显示）
  void 							SetSelect(bool flags) { m_fSelected = flags; }
	// 是否被选择
  bool  						IsSelect(void) const { return m_fSelected; }

  // 清除参数被选择索引
  virtual void      ClearParaSelectedFlag(  void );

  // 设置或解除参数锁.
  virtual bool 			SetParameterLock(ULONG ulIndex, // 参数索引.
                                     bool fSelected // 设置或解除标志.
                                     );
  virtual bool      IsParameterLocked(ULONG ulIndex);
  virtual bool      SetParameterSelected(ULONG ulIndex, bool fSelected);


	// 设置更新标志
  virtual void 			SetUpdateFlag(bool fUpdate) { m_fNeedUpdate = fUpdate; }
	// 是否需要更新
	virtual	bool 			IsNeedUpdate( void ) { return(m_fNeedUpdate); }

	// 自我完整性检查
	virtual bool			CheckSelf( void );

	// 是否可被看进去.
  virtual bool      CanViewIn(void) const { return false; }

	// 转储当前状态
  virtual bool      DumpCurrentStatus( char * pch		// 前缀字符串
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
	INT32						m_lScanRate;	// 扫描速率, 1毫秒/次.
  INT64           m_lReserved1; // 保留，以备后用
  INT64           m_lReserved2; // 保留，以备后用

// no serialized data
	INT32						m_lScanRateDynamicCount;	// 倒数计时器，用于执行单元序列时。

	vector<bool>    m_vfSelected;		// 参数是否被选择标志,用于输入参数。
	bool						m_fSelected;			// 是否被选中标志.
	bool						m_fNeedUpdate;	// 是否需要更新显示标志.
};


