#ifndef __DELETE_DYNAMIC_LINK_H__
#define __DELETE_DYNAMIC_LINK_H__

// ɾ������Ԫ���еĶ�̬����
void DeleteDynLinkFromMe(CUnitBasePtr pUnit);
// ɾ��pUnitList��pObjectList������ָ�򱾵�Ԫ�Ķ�̬����
void DeleteDynLinkToMe(CUnitList * pUnitList, CObjectList * pObjectList, CUnitBasePtr pUnit);

#endif
