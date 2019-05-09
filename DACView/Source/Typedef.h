#ifndef __TYPE_DEFINE_H__
#define __TYPE_DEFINE_H__

using namespace std;
#include<vector>
#include<list>
#include<memory>

typedef struct {
                  CString Name;				//����������
                  ULONG   ulType;			//���������ͣ� 
                  ULONG   ulIndex;		//�����ڲ��������е�λ��
									bool		fSelected;	//�˲����ѱ�ѡ��ֻ����CUnitComponent�Ľӿڲ������ȴ�ɾ����
               } ParaName;

// ������������������
enum {
							tBOOL					= 0X0001,			// ������
              tWORD         = 0X0002,			// ������
              tDOUBLE       = 0X0004,			// ��������
              tSTRING       = 0X0008,			// �ַ�����
              tIDD_END_DATA = 0X0080,			// ��ʱ��ʹ��
              tINPUT        = 0X0100,			// ����������
              tOUTPUT       = 0X0200,			// ���������
              tMODIFIABLE   = 0X0400			// �Ƿ�ɱ�����
              };              

// Object�����ӷ����ͷ��򡣷����Ǵ�Unit�Ƕ��Ͽ��ģ��ʶ�input�ķ����Ǵ�Object��Unit����output�Ǵ�Unit��Object��
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

// ������ʾ��̬�����߾���λ�õ�list����
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
