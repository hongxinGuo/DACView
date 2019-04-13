#include"stdafx.h"
#include"pch.h"

#include"CObjButton.h"

#include"CUnitAnd.h"

namespace DACViewTest {
  TEST(TestCObejctButton, TestInitialize) {
    CRect rect(100, 100, 200, 200);

    CObjectButton c("abcd", rect);
    EXPECT_STREQ("abcd", c.GetName());
    rect = c.GetSize();
    EXPECT_EQ(100, rect.left);
    EXPECT_EQ(100, rect.top);
    EXPECT_EQ(200, rect.bottom);
    EXPECT_EQ(200, rect.right);

    EXPECT_FALSE(c.CanInSymbol());
    EXPECT_STREQ("Button", c.GetClassNameStr());
  }

  TEST(TestCObjectButton, TestGetDynLinkType) {
    CObjectButton c;

    EXPECT_EQ(tBOOL, c.GetDynLinkType(0));
    EXPECT_EQ(0, c.GetDynLinkType(1));
  }

  TEST(TestCObjectButton, TestExectiveDynLink) {
    CObjectButton cButton;
    CObjectDynLink *pcODL = new CObjectDynLink;
    CUnitAnd cAnd;
    CODLList * podlList;
    CRect rect(100, 100, 300, 300);
    CDC DC;
    CPoint pt(150, 150);

    cButton.SetAllSize(rect);
    pcODL->SetObject(&cButton);
    pcODL->SetUnit(&cAnd);
    pcODL->SetObjectIndex(0);
    pcODL->SetUnitIndex(1); // input2
    pcODL->SetLinkMethod(IDD_INPUT_BOOL);
    podlList = cButton.GetDynLinkList();
    podlList->AddTail(pcODL);
    cAnd.SetBool(1, true);
    cButton.ExectiveDynLink();
    EXPECT_FALSE(cAnd.GetBool(1));
    cButton.InIt(&DC, pt, 1); // begin track
    cButton.InIt(&DC, pt, 2); // set
    cButton.ExectiveDynLink();
    EXPECT_TRUE(cAnd.GetBool(1));
    // 再次按下，即状态改变
    cButton.InIt(&DC, pt, 1); // begin track
    cButton.InIt(&DC, pt, 2); // set
    cButton.ExectiveDynLink();
    EXPECT_FALSE(cAnd.GetBool(1));
  }

  TEST(TestCObjectButton, TestExective) {
    CUnitAnd c;
 
  }

  TEST(TestCObjectButton, TestSelectParameter) {
    CObjectButton c;

    c.SelectParameter(tOUTPUT | tBOOL);
    EXPECT_EQ(0, c.GetIndex(0));
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tINPUT | tBOOL);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tWORD);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tWORD);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(1));
    c.SelectParameter(tOUTPUT | tDOUBLE);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tINPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));
    c.SelectParameter(tOUTPUT | tSTRING);
    EXPECT_EQ(-1, c.GetIndex(0));

  }

}
