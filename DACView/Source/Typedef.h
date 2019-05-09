#ifndef __TYPE_DEFINE_H__
#define __TYPE_DEFINE_H__

using namespace std;
#include<vector>
#include<list>
#include<memory>

typedef struct {
                  CString Name;				//参数的名称
                  ULONG   ulType;			//参数的类型： 
                  ULONG   ulIndex;		//参数在参数数组中的位置
									bool		fSelected;	//此参数已被选择，只用于CUnitComponent的接口参数，等待删除掉
               } ParaName;

// 用于描述参数的类型
enum {
							tBOOL					= 0X0001,			// 布尔型
              tWORD         = 0X0002,			// 整数型
              tDOUBLE       = 0X0004,			// 浮点数型
              tSTRING       = 0X0008,			// 字符串型
              tIDD_END_DATA = 0X0080,			// 暂时不使用
              tINPUT        = 0X0100,			// 数据输入型
              tOUTPUT       = 0X0200,			// 数据输出型
              tMODIFIABLE   = 0X0400			// 是否可被更改
              };              

// Object的连接方法和方向。方向是从Unit角度上看的，故而input的方向是从Object到Unit，而output是从Unit到Object。
enum {
  IDD_INPUT_BOOL		      			  = 1,
  IDD_INPUT_DWORD   			      	= 2,
  IDD_INPUT_DOUBLE					      = 3,
	IDD_INPUT_STRING								= 4,

	IDD_OUTPUT_GRAPH								= 100,
	IDD_OUTPUT_METER								= 101,
	IDD_OUTPUT_FILLBAR							= 102, 
	IDD_OUTPUT_GAUGE								= 103,
	IDD_OUTPUT_STRING								= 104,
  IDD_OUTPUT_LOCATION							= 105,
  IDD_OUTPUT_OBJECT_SIZE	        = 106,
  IDD_OUTPUT_ROTATE               = 107,
  IDD_OUTPUT_FILL_COLOR						= 108, 
  IDD_OUTPUT_VISIBILITY           = 110,
  IDD_END_LINK_METHOD             = 1001,
  };

// 用于显示动态连接线具体位置的list类型
typedef list<shared_ptr<CPoint>> CPointList;

class CUnitBase;
typedef list<CUnitBase *> CUnitList;
typedef vector<CUnitBase *> CUnitVector;

class CObjectBase;
typedef vector<CObjectBase *> CObjectVector;
typedef list<CObjectBase *> CObjectList;

class CUnitDictionary;
typedef list<shared_ptr<CUnitDictionary>> CDicList;

#endif
