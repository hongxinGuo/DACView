#include"stdafx.h"
#include"pch.h"

#include"CObjComponentBase.h"
#include"CObjComponent.h"

namespace DACViewTest {

  TEST(TestCObjCompoundBase, TestInitialize) {
    CString str = "abcde";
    CRect rect(100, 200, 300, 400), rect2;
    CObjectComponentBase c(str, rect), c2;

    rect2 = c.GetSize();
    EXPECT_EQ(100, rect2.left);
    EXPECT_EQ(200, rect2.top);
    EXPECT_EQ(300, rect2.right);
    EXPECT_EQ(400, rect2.bottom);
    EXPECT_STREQ("abcde", c.GetName());
    EXPECT_STREQ("", c2.GetName());
    EXPECT_TRUE(c.CanViewIn());
    EXPECT_TRUE(c2.CanViewIn());
  }

  TEST(TestCObjCompoundBase, TestSetUpperObjectList) {
    CObjectComponentBase c;
    CObjectList objlist;

    c.SetUpperObjectList(&objlist);
    EXPECT_EQ(&objlist, c.GetUpperObjectList());
  }
  
  TEST(TestCObjCompoundBase, TestSetUpperObjectComponent) {
    CObjectComponentBase c;
    CObjectComponent objCpt;

    c.SetUpperObjectComponent(&objCpt);
    EXPECT_EQ(&objCpt, c.GetUpperObjectComponent());

  }

  TEST(TestCObjCompoundBase, TestSetUpperScrollPosition) {
    CObjectComponentBase c;
    CPoint pt(100, 200), pt2;

    c.SetUpperScrollPosition(pt);
    pt2 = c.GetUpperScrollPosition();
    EXPECT_EQ(pt2.x, 100);
    EXPECT_EQ(pt2.y, 200);
  }

  TEST(TestCObjCompoundBase, TestSetInteger) {

  }

  TEST(TestCObjCompoundBase, TestSerialize) {

  }

  TEST(TestCObjCompoundBase, TestAddToList) {

  }

  TEST(TestCObjCompoundBase, TestCreateUniName) {

  }
  
  TEST(TestCObjCompoundBase, TestSetUpdateFlag) {

  }

}