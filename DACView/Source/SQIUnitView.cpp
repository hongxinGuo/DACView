////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  处理SQI文件时调用的显示Unit的View
//
//
// stratvw.cpp : implementation of the CSQIUnitView class
//
// User's Unit interface handle functions. Provide following function : 
//
// CSQIUnitView()      : initialize all variables, create cursor and so on
// ~StrategyView ()     : end up StrategyView
// PreCreateWindow()    : register StrategyView own window class
// OnCreate()
// OnInitialUpdate()    : Setup scroll size
// OnUpdate()           : Recalcute scroll size and resize parent to fit me          
// OnDraw()             : Get clip box and show all units and their dynamic links
// OnPreparePrinting()  : prepare printing
// OnBeginPrinting()    : begin printing
// OnEndPrintint()      : end printing
// AssertValid()        : varify data valid
// Dump()               : dump data
// GetDucument()        : Get view's document
//
// 
// FindUnit()           : Find unit according unit's name  
// AddUnit()            : Add unit to current unit list
// DeleteUnit()         : Delete unit and its dynamic links from current unit list
//                        Also delete some unit's dynamic link that link to this unit                          
// GetUnitDocSize()     : Get document's size 
// IsInRect()           : Check weithe point is in the rect of current unit list's unit
// UnittoBack()         : Set unit's position to head(bottom most)
// UnitToFront()        : Set unit's position to tail(top most)
// SetFocus()           : Set all selected units's focus
// ClearFocus()         : Clear all selected unit's focus
// CreateUniName()      : Create a unique name for unit
//
// DrawInvertDynLinkLine()      : Draw invert dynamic link's line
// AdjustDynLinkLinePosition()  : When unit is moved, recalculate its dynamic link's positon
// DeleteDynLinkPointList()     : Delete dynamic link list's contain and remove all list.
//
// ViewIn()             : Trace in compound unit
// ViewOut()            : return from compound
// 
// CenterAlign()        : Align selected units by center
// LeftAlign()          : Align selected units by left
// RightAlign()         : Align selected units by right
//
// OnLButtonDblClk()    : Process mouse left button double click
// OnLButtonDown()      : Process mouse left button down
// OnLButtonMove()      : Process mouse left button move
// OnLButtonUp()        : Process mouse ldft button up
//
// OnEditCopy()         : Process Edit copy
// OnEditCut()          : Process Edit cut
// OnEditPaste()        : Process Edit Paste
// OnEditDelete()       : Process Edit delete
// OnEditUndo()         : Process Edit Undo
// 
// OnStyleCentered()    : Align selected units to center
// OnStyleLeft()        : Align selected units to left
// OnStyleRight()       : Align selected units to right
// OnStyleJustified()   : Align selected units to justified
//
// OnArrangetoBack()    : Change unit's position to back
// OnArrangeToFront()   : Change unit's position to front
// 
// OnViewViewIn()       : process view into compound
// OnViewviewOut()      : process view out compound
//
// OnArrangeTogglecutoff()  : Toggle CutOff
// OnViewProperty()     : Set unit's property
//
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dacview.h"

#include "mainfrm.h"
#include "globedef.h"

#include"DynLinkPoint.h"

#include "SQIUnitView.h"
#include "SQIFileDoc.h"

#include "CUnitBase.h"
#include"DeleteDynLink.h"

// include Mathmatic unit header
#include "cUnitAdd.h"
#include "cUnitMultiple.h"
#include "cUnitDivide.h"

// include simulate unit header
#include "cUnitQuad.h"
#include "cUnitSine.h"

// include logic unit header
#include "cUnitAnd.h"
#include "cUnitOr.h"
#include "cUnitXor.h"
#include "cUnitPulse.h"
#include "cUnitInvert.h"
#include "cUnitDelay.h"
#include "cUnitFFLP.h"
#include "cUnitTTB.h"

// include select unit header
#include "cUnitSwitch.h"
#include "cUnitHSel.h"
#include "cUnitMSel.h"
#include "cUnitLSel.h"

// include Basic control unit header
#include "cUnitPID.h"

// include Arithmatic unit header
#include "CUnitTotal.h"
#include "CUnitQFLT.h"
#include "cUnitHPBG.h"
#include "CUnitDelayTime.h"

// include Input Output unit Header
#include "CUnitIPOP.H"
#include "CUnitTPO.h"

// include Component unit Header
#include "DlgDeleteComponent.h"

// include Component unit Header
#include "cUnitComponent.h"

// include dynamic link header
#include "cUnitDynLink.h"

#include "DlgChoicePara.h"
#include"DlgChoiceCUCPara.h"

#include"CompileUnitList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSQIUnitView

IMPLEMENT_DYNCREATE(CSQIUnitView, CScrollView)

BEGIN_MESSAGE_MAP(CSQIUnitView, CScrollView)
  //{{AFX_MSG_MAP(CSQIUnitView)
  ON_WM_LBUTTONDBLCLK()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
  ON_COMMAND(ID_EDIT_CUT, OnEditCut)
  ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
  ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
  ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
  ON_COMMAND(ID_STYLE_CENTERED, OnStyleCentered)
  ON_COMMAND(ID_STYLE_LEFT, OnStyleLeft)
  ON_COMMAND(ID_STYLE_RIGHT, OnStyleRight)
  ON_COMMAND(ID_ARRANGE_TOBACK, OnArrangeToback)
  ON_COMMAND(ID_ARRANGE_TOFRONT, OnArrangeTofront)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_TOBACK, OnUpdateArrangeToback)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_TOFRONT, OnUpdateArrangeTofront)
  ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
  ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
  ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
  ON_UPDATE_COMMAND_UI(ID_STYLE_CENTERED, OnUpdateStyleCentered)
  ON_UPDATE_COMMAND_UI(ID_STYLE_RIGHT, OnUpdateStyleRight)
  ON_WM_LBUTTONDOWN()
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_AIN, OnBlockInputoutputAin)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_MATHMATIC_ADD, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_AIN, OnUpdateBlockI)
  ON_COMMAND(ID_BLOCK_MATHMATIC_ADD, OnBlockMathmaticAdd)
  ON_COMMAND(ID_BLOCK_SIMULATE_QUADRATIC, OnBlockSimulateQuadratic)
  ON_COMMAND(ID_BLOCK_SIMULATE_SINE, OnBlockSimulateSine)
  ON_COMMAND(ID_VIEW_VIEWIN, OnViewViewin)
  ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWIN, OnUpdateViewViewin)
  ON_COMMAND(ID_VIEW_VIEWOUT, OnViewViewout)
  ON_UPDATE_COMMAND_UI(ID_VIEW_VIEWOUT, OnUpdateViewViewout)
  ON_COMMAND(ID_BLOCK_COMPOUND, OnBlockCompound)
  ON_WM_CREATE()
  ON_UPDATE_COMMAND_UI(ID_OBJECT_TEXT, OnUpdateObjectBar)
  ON_COMMAND(ID_BLOCK_MATHMATIC_MULTIPLE, OnBlockMathmaticMultiple)
  ON_COMMAND(ID_BLOCK_BASICLOGIC_AND, OnBlockBasiclogicAnd)
  ON_COMMAND(ID_BLOCK_BASICLOGIC_DLAY, OnBlockBasiclogicDlay)
  ON_COMMAND(ID_BLOCK_BASICLOGIC_DROT, OnBlockBasiclogicDrot)
  ON_COMMAND(ID_BLOCK_BASICLOGIC_INV, OnBlockBasiclogicInv)
  ON_COMMAND(ID_BLOCK_BASICLOGIC_OR, OnBlockBasiclogicOr)
  ON_COMMAND(ID_BLOCK_BASICLOGIC_PULS, OnBlockBasiclogicPuls)
  ON_COMMAND(ID_BLOCK_BASICLOGIC_XOR, OnBlockBasiclogicXor)
  ON_COMMAND(ID_BLOCK_ADVANCELOGIC_FFLP, OnBlockAdvancelogicFflp)
  ON_COMMAND(ID_BLOCK_ADVANCELOGIC_ICNT, OnBlockAdvancelogicIcnt)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_QFLT, OnBlockArithmaticQflt)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_HFLT, OnBlockArithmaticHflt)
  ON_COMMAND(ID_BLOCK_ADVANCECONTROL_SCHD, OnBlockAdvancecontrolSchd)
  ON_COMMAND(ID_BLOCK_ADVANCECONTROL_SEQ, OnBlockAdvancecontrolSeq)
  ON_COMMAND(ID_BLOCK_ADVANCECONTROL_SEQE, OnBlockAdvancecontrolSeqe)
  ON_COMMAND(ID_BLOCK_ADVANCELOGIC_TTB, OnBlockAdvancelogicTtb)
  ON_COMMAND(ID_BLOCK_SELECT_HSEL, OnBlockSelectHsel)
  ON_COMMAND(ID_BLOCK_SELECT_LSEL, OnBlockSelectLsel)
  ON_COMMAND(ID_BLOCK_SELECT_MSEL, OnBlockSelectMsel)
  ON_COMMAND(ID_BLOCK_SELECT_SWCH, OnBlockSelectSwch)
  ON_COMMAND(ID_BLOCK_MATHMATIC_EXPR, OnBlockMathmaticExpr)
  ON_COMMAND(ID_BLOCK_MATHMATIC_DIV, OnBlockMathmaticDiv)
  ON_COMMAND(ID_BLOCK_MATHMATIC_AVGT, OnBlockMathmaticAvgt)
  ON_COMMAND(ID_BLOCK_ADVANCELOGIC_TTBE, OnBlockAdvancelogicTtbe)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_TOT, OnBlockArithmaticTot)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_ALRM, OnBlockArithmaticAlrm)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_CHAR, OnBlockArithmaticChar)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_COND, OnBlockArithmaticCond)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_DTIM, OnBlockArithmaticDtim)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_LLAG, OnBlockArithmaticLlag)
  ON_COMMAND(ID_BLOCK_BASICCONTROL_AMB, OnBlockBasiccontrolAmb)
  ON_COMMAND(ID_BLOCK_BASICCONTROL_INTD, OnBlockBasiccontrolIntd)
  ON_COMMAND(ID_BLOCK_BASICCONTROL_PD, OnBlockBasiccontrolPd)
  ON_COMMAND(ID_BLOCK_BASICCONTROL_PID, OnBlockBasiccontrolPid)
  ON_COMMAND(ID_BLOCK_BASICCONTROL_TIME, OnBlockBasiccontrolTime)
  ON_COMMAND(ID_BLOCK_HISTORYTREND_AHST, OnBlockHistorytrendAhst)
  ON_COMMAND(ID_BLOCK_HISTORYTREND_DHST, OnBlockHistorytrendDhst)
  ON_COMMAND(ID_BLOCK_HISTORYTREND_RPRT, OnBlockHistorytrendRprt)
  ON_COMMAND(ID_BLOCK_HISTORYTREND_SPC, OnBlockHistorytrendSpc)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_AINS, OnBlockInputoutputAins)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_AKBD, OnBlockInputoutputAkbd)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_AOTS, OnBlockInputoutputAots)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_AOUT, OnBlockInputoutputAout)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_DIN, OnBlockInputoutputDin)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_DINS, OnBlockInputoutputDins)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_DKBD, OnBlockInputoutputDkbd)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_DOTS, OnBlockInputoutputDots)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_DOUT, OnBlockInputoutputDout)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_HORN, OnBlockInputoutputHorn)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_TPO, OnBlockInputoutputTpo)
  ON_COMMAND(ID_BLOCK_MATHMATIC_AVG, OnBlockMathmaticAvg)
  ON_COMMAND(ID_BLOCK_SIMULATE_LOGARITHM, OnBlockSimulateLogarithm)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_BREAKDYNLINK, OnUpdateArrangeBreakdynlink)
  ON_COMMAND(ID_ARRANGE_BREAKDYNLINK, OnArrangeBreakdynlink)
  ON_COMMAND(ID_BLOCK_ARITHMATIC_HPBG, OnBlockArithmaticHpbg)
  ON_COMMAND(ID_BLOCK_INPUTOUTPUT_SIM, OnBlockInputoutputSim)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_TOGGLECUTOFF, OnUpdateArrangeTogglecutoff)
  ON_COMMAND(ID_ARRANGE_TOGGLECUTOFF, OnArrangeTogglecutoff)
  ON_COMMAND(ID_VIEW_PROPERTY, OnViewProperty)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PROPERTY, OnUpdateViewProperty)
  ON_WM_RBUTTONUP()
  ON_WM_SETCURSOR()
  ON_COMMAND(ID_ARRANGE_MAKEDYNLINK, OnArrangeMakedynlink)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_MAKEDYNLINK, OnUpdateArrangeMakedynlink)
  ON_UPDATE_COMMAND_UI(ID_STYLE_LEFT, OnUpdateStyleLeft)
  ON_COMMAND(ID_BLOCK_COMPONENT, OnBlockComponent)
  ON_UPDATE_COMMAND_UI(ID_ARRANGE_LINK_INTO_COMPONENT, OnUpdateArrangeLinkIntoComponent)
  ON_COMMAND(ID_ARRANGE_LINK_INTO_COMPONENT, OnArrangeLinkIntoComponent)
  ON_UPDATE_COMMAND_UI(ID_BREAK_LINK_OF_COMPONENT, OnUpdateBreakLinkOfComponent)
  ON_COMMAND(ID_BREAK_LINK_OF_COMPONENT, OnBreakLinkOfComponent)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_ALRM, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_SIMULATE_QUADRATIC, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_ROUNDRECT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_RECT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_LINE, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_OVAL, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_GRAPH, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICLOGIC_AND, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICLOGIC_DLAY, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICLOGIC_DROT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICLOGIC_INV, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICLOGIC_OR, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICLOGIC_PULS, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICLOGIC_XOR, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ADVANCELOGIC_FFLP, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ADVANCELOGIC_ICNT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_HFLT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_QFLT, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ADVANCECONTROL_SCHD, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ADVANCELOGIC_TTB, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_SELECT_SWCH, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_SELECT_MSEL, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_SELECT_LSEL, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_SELECT_HSEL, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_MATHMATIC_MULTIPLE, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_MATHMATIC_EXPR, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_MATHMATIC_DIV, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_MATHMATIC_AVGT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ADVANCECONTROL_SEQ, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ADVANCECONTROL_SEQE, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ADVANCELOGIC_TTBE, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_TOT, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_SIMULATE_SINE, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_SIMULATE_LOGARITHM, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_MATHMATIC_AVG, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_TPO, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_HORN, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_DOUT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_DOTS, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_DKBD, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_DINS, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_DIN, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_AOUT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_AOTS, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_AKBD, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_AINS, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_HISTORYTREND_SPC, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_HISTORYTREND_RPRT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_HISTORYTREND_DHST, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_HISTORYTREND_AHST, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_COMPOUND, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICCONTROL_TIME, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICCONTROL_PID, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICCONTROL_PD, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICCONTROL_INTD, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_BASICCONTROL_AMB, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_LLAG, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_DTIM, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_COND, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_CHAR, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_ARITHMATIC_HPBG, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_SLIDER, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_PUSHBUTTON, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_COMPOUND, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_INPUTOUTPUT_SIM, OnUpdateBlock)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_FILLBAR, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_GAUGE, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_KNOB, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_METER, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_BRUSH, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_ERASE, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_PAINT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_PEN, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateBlockI)
  ON_UPDATE_COMMAND_UI(ID_OBJECT_NUMBERINPUT, OnUpdateObjectBar)
  ON_UPDATE_COMMAND_UI(ID_BLOCK_COMPONENT, OnUpdateBlock)
  //}}AFX_MSG_MAP
  // Standard printing commands
//		ON_COMMAND(ID_ARRANGE_ENCAPSULATION, &CSQIUnitView::OnArrangeEncapsulation)
//		ON_UPDATE_COMMAND_UI(ID_ARRANGE_ENCAPSULATION, &CSQIUnitView::OnUpdateArrangeEncapsulation)
//		ON_COMMAND(ID_ARRANGE_DECAPSULATION, &CSQIUnitView::OnArrangeDecapsulation)
//		ON_UPDATE_COMMAND_UI(ID_ARRANGE_DECAPSULATION, &CSQIUnitView::OnUpdateArrangeDecapsulation)
ON_UPDATE_COMMAND_UI(ID_ARRANGE_LinkFromComponent, &CSQIUnitView::OnUpdateArrangeLinkfromcomponent)
ON_COMMAND(ID_ARRANGE_LinkFromComponent, &CSQIUnitView::OnArrangeLinkfromcomponent)
ON_WM_KEYUP()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////// 
// CSQIUnitView construction/destruction
extern CDacviewApp theApp;
UINT CSQIUnitView::m_uUnitFormat = 0;

CSQIUnitView::CSQIUnitView(void)
{
  // TODO: add construction code here
  static int fRegistered = 0;

  if (!fRegistered) {  // register my clipboard format
    m_uUnitFormat = RegisterClipboardFormat((LPCTSTR)"CF_DACVIEW_STRATEGY_FORMAT");
    ASSERT(m_uUnitFormat != 0);
    fRegistered = 1;
  }

  m_fontStrategyView.CreateFont(-4, 0, 0, 0, FW_NORMAL, 0, 0, 0,
    OEM_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
    DEFAULT_QUALITY, (FIXED_PITCH | FF_MODERN),
    (LPCTSTR)"system");

  m_pDoc = nullptr;
  m_nCurrentFunction = UNIT_PRE_SELECT;
  m_nSavedFunction = 0;
  m_pCUnitCurrent = m_pCUnitSecond = m_pCUnitFirst = nullptr;
  m_pCUnitDynLinkCurrent = nullptr;
  m_pCUnitMouseMove = nullptr;

  m_pCUnitListTop = m_pCUnitListCurrent = nullptr;
	m_pObjectList = nullptr;

  // 下面这几段代码，在测试时会出错。将其执行挪至OnCreate。
  //if ( fLoadCursor ) theApp.LoadCursors(&m_hCursorDynamicLink, &m_hCursorNoDrag, &m_hCursorComponent);
  //VERIFY(m_hCursorDynamicLink = theApp.LoadCursor("CURSOR_DYNAMIC_LINK"));
  //VERIFY(m_hCursorNoDrag = theApp.LoadCursor("NO_DRAG"));
  //VERIFY(m_hCursorComponent = theApp.LoadCursor("COMPOUND"));
  //m_hCursorArrow = SetCursor(m_hCursorComponent);
  //SetCursor(m_hCursorArrow);

  m_lComponentLayer = 0;	// 当前部件层为0层（最上层）
  m_fLinkIntoSourceComponent = false;
  m_fLinkIntoDestComponent = false;

  m_pCUnitComponentCurrent = nullptr;       // 不存在包含最上层单元序列的部件。

  m_plistLinkPoint = new CPointList;

  m_lSrcIndex = m_lDestIndex = -1;
	m_ulDynLinkType = 0;
	m_ulDynLinkClass = 0;

}

CSQIUnitView::~CSQIUnitView()
{
  m_plistLinkPoint->clear();
  delete m_plistLinkPoint;
}

BOOL CSQIUnitView::PreCreateWindow(CREATESTRUCT& cs)
{
  ASSERT(cs.lpszClass == NULL);       // must not be specified
  CView::PreCreateWindow(cs);
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
    0, // no class cursor
    (HBRUSH)(COLOR_WINDOW + 1),
    AfxGetApp()->LoadIcon(IDR_STRATEGYTYPE));

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// CSQIUnitView drawing

void CSQIUnitView::OnInitialUpdate() {
  SetScrollSizes(MM_TEXT, GetUnitDocSize());
  OnUpdate();
}

void CSQIUnitView::OnUpdate() {
  // ...
  // Implement a GetDocSize( ) member function in 
  // your document class; it returns a CSize.
  SetScrollSizes(MM_TEXT, GetUnitDocSize());
  ResizeParentToFit();   // Default bShrinkOnly argument
  // ...
}

//////////////////////////////////////////////////////////////////////////////
//
//
// 似乎应该将正在生成的动态链接线的显示函数加入这里，否则当不慎使用Invalidate时，动态连接线就没了。
//
//
//////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnDraw(CDC* pDC)
{
  // TODO: add draw code here
  CRect crectClip;
  CPen p(PS_DOT, 1, RGB(64, 64, 64)), *pp;

  pDC->SetBkColor(RGB(192, 192, 192));
  pDC->GetClipBox(&crectClip);
  pp = pDC->SelectObject(&p);
  // draw background grid
  for (int i = 10; i <= crectClip.right; i += 10) {
    if ((i <= crectClip.right) && (i >= crectClip.left)) {
      for (int j = 10; j <= crectClip.bottom; j += 10) {
        if ((j <= crectClip.bottom) && (j >= crectClip.top)) {
          pDC->SetPixel(i, j, RGB(0, 0, 0));
        }
      }
    }
  }
  pDC->SelectObject(pp);
  // pfontTemp = pDC->SelectObject(&m_fontStrategyView);
  pDC->GetClipBox(&m_crectClip);

  CUnitBase * pcunit;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();
  CRect rectTemp;

  for (int i = 0; i < iTemp; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    pcunit->ToShow(pDC);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CSQIUnitView printing

BOOL CSQIUnitView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  return DoPreparePrinting(pInfo);
}

void CSQIUnitView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add extra initialization before printing
}

void CSQIUnitView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
  // TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSQIUnitView diagnostics

#ifdef _DEBUG
void CSQIUnitView::AssertValid() const
{
  CView::AssertValid();
}

void CSQIUnitView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CSQIFileDoc* CSQIUnitView::GetDocument() // non-debug version is inline
{
  ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSQIFileDoc)));
  return (CSQIFileDoc*)m_pDocument;
}

#endif //_DEBUG

int CSQIUnitView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO: Add your specialized creation code here

  // 设置最高单元序列和当前单元序列为本文件的单元序列
  m_pDoc = GetDocument();
  m_pCUnitListTop = m_pCUnitListCurrent = m_pDoc->GetUnitList();
	m_pObjectList = m_pDoc->GetObjectList();

	// 将读入鼠标类型的过程移至此处。此函数无法测试。
  theApp.LoadCursors(&m_hCursorDynamicLink, &m_hCursorNoDrag, &m_hCursorComponent);
  m_hCursorArrow = SetCursor(m_hCursorComponent);
  SetCursor(m_hCursorArrow);

  return 0;
}

////////////////////////////////////////////////////////////////////////////
//  CSQIUnitView operation

///////////////////////////////////////////////////////////////////////////////////////
//
// DrawInvertLine()
//
///////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::DrawInvertLine(CDC * pdc, ULONG ulWidth, CPoint ptStart, CPoint ptEnd) {
  CPen * oldPen, newPen;

  newPen.CreatePen(PS_SOLID, ulWidth, RGB(0, 0, 0));
  oldPen = pdc->SelectObject(&newPen);
  ASSERT(oldPen != NULL);
  pdc->MoveTo(ptStart);
  int nTemp = pdc->SetROP2(R2_NOT);
  pdc->LineTo(ptEnd);
  pdc->SetROP2(nTemp);
  pdc->SelectObject(oldPen);
}

////////////////////////////////////////////////////////////////////////////
//
// DeleteUnit()
//
// Parameter :
//    CUnitBase * pCUnit : point unit that need deleted from current unit list
//
// Return :
//    BOOL        : true if deleted, false if not find unit from list
// 
// Description :
//    Delete unit from current unit list, remove its dynamic link and clear 
//    the selected flag from its destination unit.
//    If some unit's dynamic link is link to me, also delete them
//
/////////////////////////////////////////////////////////////////////////////
bool CSQIUnitView::DeleteUnit(CUnitBase * pCUnit) {
  POSITION po;

  ASSERT(pCUnit != nullptr);
  if ((po = m_pCUnitListCurrent->Find(pCUnit)) != nullptr) { // find unit ?
    // 清除所有指向本单元的动态链接
    DeleteDynLinkToMe(m_pCUnitListTop, m_pObjectList, pCUnit);
    // 清除所有本单元指向的动态链接
    DeleteDynLinkFromMe(pCUnit);
    // 删除本单元
    m_pCUnitListCurrent->RemoveAt(po);
    delete pCUnit;

    return (true);
  }
  return(false);
}

/////////////////////////////////////////////////////////////////////////////
//
// GetUnitDocSize()
//
// Parameter :
//    no parameter
//
// Return :
//    CSize : return current document's size
//
// Description :
//    Combine all units's size and rect(0, 0, 2000, 1500) into a rect, 
//    and return its size.
//
////////////////////////////////////////////////////////////////////////
CSize CSQIUnitView::GetUnitDocSize(void) {
  CRect rectTemp(0, 0, 2000, 1500);
  CUnitBase * pcunitTemp;
  POSITION pos = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();

  for (int i = 0; i < iTemp; i++) {
    pcunitTemp = m_pCUnitListCurrent->GetNext(pos);
    rectTemp |= pcunitTemp->GetSize();
  }
  return(CSize(rectTemp.Width(), rectTemp.Height()));
}

//////////////////////////////////////////////////////////////////////////
//
// IsInRect()
//
// Parameter :
//    CPoint const pt     : current check point
//    CUnitBase *& pcunit : current set align pointer
//
// Return :
//    BOOL         : true if find pt in one of unit's rect, 
//                     pcunit if set to point to this unit
//                   false if not find, pcunit is set to NULL.
//
// Description :
//    check whether 'pt' is in a 'rect' of current unit list's unit
//    look for from list's tail(upper most) to head(lower most)
//
////////////////////////////////////////////////////////////////////////////
bool CSQIUnitView::IsInRect(CPoint const pt, CUnitBase*& pcunit) {
  CUnitBase* pc;
  POSITION poUnit = m_pCUnitListCurrent->GetTailPosition(); // Tail position is the top most
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();

  for (int i = 0; i < iTemp; i++) {
    pc = m_pCUnitListCurrent->GetPrev(poUnit);
    if (pc->InIt(pt)) {
      pcunit = pc;
      return (true);
    }
  }
  pcunit = nullptr;
  return (false);
}

//////////////////////////////////////////////////////////////////////////
// 
// UnitToBack()
//
// Parameter :
//    CUnitBase * const pCUnit :  current process unit
//
// Return :
//    bool      : alway true
//
// Description :
//   Change an object's position to m_CUnitList's head, and redraw all objects.
//
/////////////////////////////////////////////////////////////////////////  
bool CSQIUnitView::UnitToBack(CUnitList * pUnitList, CUnitBase * const pCUnit) {
  ASSERT(pCUnit != nullptr);
  ASSERT(pUnitList->GetCount() > 0);
  CUnitBase * pc = pCUnit;
  POSITION poUnit;

  poUnit = pUnitList->Find(pCUnit);
  pUnitList->RemoveAt(poUnit);
  pUnitList->AddHead(pc);
  return (true);
}

//////////////////////////////////////////////////////////////////////////
// 
// UnitToFront()
//
// Parameter :
//    CUnitBase * const pCUnit :  current process unit
//
// Return :
//    bool      : alway true
//
// Description :
//   Change an object's position to m_CUnitList's tail(top most), and redraw all objects.
//
////////////////////////////////////////////////////////////////////////////               
bool CSQIUnitView::UnitToFront(CUnitList * pUnitList, CUnitBase * const pCUnit) {
  ASSERT(pCUnit != NULL);
  CUnitBase * pc = pCUnit;
  POSITION poUnit;

  poUnit = pUnitList->Find(pCUnit);
  pUnitList->RemoveAt(poUnit);
  pUnitList->AddTail(pc);
  return (true);
}

///////////////////////////////////////////////////////////////////////////
//
// SetFocus()
//
// Parameter :
//    CDC * pdc   : current DC
//
// Return :
//    no return
//
// Description :
//    Set all object's focus according its select flag.
//
/////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::SetFocus(CDC *pdc) {
  CUnitBase * pcunit;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();

  for (int i = 0; i < iTemp; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {
      pcunit->SetFocus(pdc);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
//
// ClearFocus()
//
// Parameter :
//    CDC * pdc   : current DC
//
// Return :
//    no return
//
// Description :
//    Clear all selected focus and clear select flag
//
/////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::ClearFocus(CDC * pdc) {
  CUnitBase * pcunit;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();

  for (int i = 0; i < iTemp; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {
      pcunit->SetSelect(false);
      pcunit->ClearFocus(pdc);
    }
  }
}

///////////////////////////////////////////////////////////////////////////
//
// ClearAllSelect()
//
// Return :
//    no return
//
// Description :
//    Clear all object's select flag
//
/////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::ClearAllSelect(void) {
  CUnitBase * pcunit;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();

  for (int i = 0; i < iTemp; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {
      pcunit->SetSelect(false);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
//
// CreateUniName()
//
// Parameter :
//    CUnitBase * pCUnit : point to current process unit
//
// Return :
//    no return.
//
// Description :
//    Create a unique name for current process unit
//
/////////////////////////////////////////////////////////////////////////
void CSQIUnitView::CreateUniName(CUnitBase * pCUnit) {
  CUnitList listUnit;
  INT64 i, iCount = m_pCUnitListCurrent->GetCount();
  POSITION po = m_pCUnitListCurrent->GetHeadPosition();
  CUnitBase * pcunit;

  // Send all units(include compound) to listUnit
  for (i = 0; i < iCount; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    pcunit->AddToList(listUnit); // 包括部件本身
  }
  // create unique name 
  pCUnit->CreateUniName(listUnit);
}

void CSQIUnitView::DrawInvertDynLinkLine(CDC * pdc, CPointList * plistLinkPoint, CPoint ptFirst, CPoint ptSecond, CPoint ptCurrent) {
  auto it = plistLinkPoint->begin();
  INT_PTR iCount = plistLinkPoint->size();
  CPoint *ppt1 = nullptr, *ppt2 = nullptr;

  ppt1 = *it++;
  for (int i = 1; i < (iCount - 1); i++) {
    ppt2 = *it++;
    DrawInvertLine(pdc, 1, *ppt1, *ppt2);
    ppt1 = ppt2;
  }
  if (iCount > 2) {
    ASSERT(ppt2 != nullptr);
    ASSERT(ppt2->x == ptFirst.x);
    ASSERT(ppt2->y == ptFirst.y);
  }
  if ( (ptFirst.x != ptSecond.x) || (ptFirst.y != ptSecond.y) ) DrawInvertLine(pdc, 1, ptFirst, ptSecond);
  if ((ptCurrent.x != ptSecond.x) || (ptCurrent.y != ptSecond.y)) DrawInvertLine(pdc, 1, ptSecond, ptCurrent);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 调整当前单元序列的动态链接线
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::AdjustDynLinkLinePosition(CUnitBase * punitCurrent, CPoint ptStart, CPoint ptEnd) {
  POSITION po = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iCount = m_pCUnitListCurrent->GetCount();
  CUnitBase * pcunit;

  for (int i = 0; i < iCount; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    pcunit->AdjustDynLinkLinePosition(punitCurrent, ptStart, ptEnd);
  }
}

void CSQIUnitView::DeleteDynLinkPointList(CPointList * plistLinkPoint) {
  CPoint * ppt;

  for (auto it = plistLinkPoint->begin(); it != plistLinkPoint->end(); it++) {
    ppt = *it;
    delete ppt;
  }
  plistLinkPoint->clear();
}

///////////////////////////////////////////////////////////////////////////////////
//
//  进入部件内操作，存储之前的状态
//
//
////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::ViewIn(CUnitComponent * pCUnit) {
  CPoint pt(0, 0);

  ASSERT(pCUnit->CanViewIn());
  pCUnit->SetComponentUpper(m_pCUnitComponentCurrent);   // 保存当前的部件为本部件的上层部件
  m_pCUnitComponentCurrent = pCUnit;         // 将当前的部件换成本部件
  m_pCUnitComponentCurrent->SetUpperUnitList(m_pCUnitListCurrent);    // 保存上层单元序列
  m_pCUnitComponentCurrent->SetUpperScrollPosition(__GetScrollPosition());// 保存滚动条位置
  m_pCUnitListCurrent = m_pCUnitComponentCurrent->GetUnitList();         // 设置当前的单元序列
  __ScrollToPosition(pt);     // set to origin point
  m_lComponentLayer++;   // goto inner

	__Invalidate();
}

//////////////////////////////////////////////////////////////////
//
//  从部件中返回，恢复以前的状态
//
//////////////////////////////////////////////////////////////////
void CSQIUnitView::ViewOut(void) {
  CRect rect;
  CPoint ptOffset = m_pCUnitComponentCurrent->GetUpperScrollPosition();
  m_pCUnitListCurrent = m_pCUnitComponentCurrent->GetUpperUnitList();     // restore upper list
  __ScrollToPosition(ptOffset); // restore upper scroll position
  ASSERT(m_pCUnitComponentCurrent != nullptr);
  m_pCUnitCurrent = m_pCUnitComponentCurrent;
  m_pCUnitCurrent->SetSelect(true);
  m_pCUnitComponentCurrent = m_pCUnitComponentCurrent->GetComponentUpper();    // restore upper compound
  rect = m_pCUnitCurrent->GetSize() - ptOffset;
  m_pDoc->m_trackerUnit.m_rect = rect;

  m_lComponentLayer--;
  ASSERT(m_lComponentLayer >= 0);

  __Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// CSQIUnitView commands

///////////////////////////////////////////////////////////////////////////////
//
// 以中间为标准对齐单元
//
//
///////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::CenterAlign()
{
  CUnitBase * pcunit;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();
  CRect rectTemp;
  int w, i1;
  CPoint ptEnd, ptStart;

  ptStart.y = 0;
  ptEnd.y = 0;
  do {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {   // if selected
      rectTemp = pcunit->GetSize();
    }
  } while (!pcunit->IsSelect());
  w = rectTemp.left + (rectTemp.right - rectTemp.left) / 2;
  poUnit = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < iTemp; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {   // if selected
      rectTemp = pcunit->GetSize();
      ptStart.x = rectTemp.left;
      i1 = w - ((rectTemp.right - rectTemp.left) / 2);
      rectTemp.right = w + (rectTemp.right - rectTemp.left) - ((rectTemp.right - rectTemp.left) / 2);
      rectTemp.left = i1;
      ptEnd.x = i1;
      pcunit->SetSize(rectTemp);
      AdjustDynLinkLinePosition(pcunit, ptStart, ptEnd);
    }
  }
}

////////////////////////////////////////////////////////////////////////////
//
// 左对齐所选的单元
//
////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::LeftAlign()
{
  CUnitBase * pcunit;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();
  CRect rectTemp;
  int w, h;
  CPoint ptEnd, ptStart;

  ptStart.y = 0;
  ptEnd.y = 0;

  do {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {   // if selected
      rectTemp = pcunit->GetSize();
    }
  } while (!pcunit->IsSelect());
  w = rectTemp.left;
  poUnit = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < iTemp; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {   // if selected
      rectTemp = pcunit->GetSize();
      ptStart.x = rectTemp.left;
      h = rectTemp.right - rectTemp.left;
      rectTemp.left = w;
      rectTemp.right = h + w;
      ptEnd.x = w;
      pcunit->SetSize(rectTemp);
      AdjustDynLinkLinePosition(pcunit, ptStart, ptEnd);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// 右对齐所选的单元
//
/////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::RightAlign()
{
  CUnitBase * pcunit;
  POSITION poUnit = m_pCUnitListCurrent->GetHeadPosition();
  INT_PTR iTemp = m_pCUnitListCurrent->GetCount();
  CRect rectTemp;
  int w, h;
  CPoint ptEnd, ptStart;

  ptEnd.y = ptStart.y = 0;
  do {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {   // if selected
      rectTemp = pcunit->GetSize();
    }
  } while (!pcunit->IsSelect());
  w = rectTemp.right;
  poUnit = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < iTemp; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(poUnit);
    if (pcunit->IsSelect()) {   // if selected
      rectTemp = pcunit->GetSize();
      ptStart.x = rectTemp.left;
      h = rectTemp.right - rectTemp.left;
      rectTemp.right = w;
      rectTemp.left = w - h;
      ptEnd.x = rectTemp.left;
      pcunit->SetSize(rectTemp);
      AdjustDynLinkLinePosition(pcunit, ptStart, ptEnd);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
//
// 双击鼠标左键： 设置单元的状态
//
//////////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CPoint ptDevice, ptOffset = __GetScrollPosition();

  // set property
  ptDevice = ptOffset + point;
  if (IsInRect(ptDevice, m_pCUnitCurrent)) {  // select object to process  
    // change object's property 
    if (m_pCUnitCurrent->SetProperty()) {
			__SetDocModifiedFlag();
    }
    __Invalidate();
  }
  else {
    // do nothing
  }

	__DefaultOnLButtonDblClk(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 处理当鼠标左键按下时的任务。
//
// 共存在九种状态，鼠标左键的三个动作（按下、移动、抬起）要处理所有这九种状态，不存在的状态就报告错误
//
// 状态：
// 1.没有选择任何任务，准备选择单元进行处理（移动、改变大小）。
// 2.选择了动态链接，已经从第一个单元中选择了参数，或者第一个单元为未封装的部件，进入了部件内部，准备选择第一个单元（此时不允许再次选择部件）
// 3.从第一个部件内的单元中选择了参数，返回部件外部准备选择第二个单元。
// 4.第二个单元选择了部件，进入了第二个部件，准备选择第二个单元（此时不允许再次选择部件）。
//
//
//
//  所有双下划线的函数，都是为了测试而采用的解耦合方法。将直接调用改为间接调用虚函数，然后派生类将其解耦合。
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnLButtonDown(UINT nFlags, CPoint point)
{
  CRect WinRect;
  CPoint ptOffset = __GetScrollPosition();
  CDC * pdc = __GetDC();
  CRect rectScreen;
  CDlgChoiceParameter CCPDlg;

  CPoint ptDevice;

  __OnPrepareDC(pdc);

  // set current mouse address  
  if ((m_nCurrentFunction != DYNAMIC_LINK_TO_FIRST_COMPONENT)
    || (m_nCurrentFunction != DYNAMIC_LINK_TO_SECOND_COMPONENT)) { // 当处于链接第一个或第二个部件状态时，不修改鼠标位置
    m_ptMousePosition = point + ptOffset;
  }

  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:				// 选择了处理unit
  case UNIT_GROUP_SELECTED:
  case UNIT_PRE_SELECT:
    if (IsInRect(m_ptMousePosition, m_pCUnitCurrent)) {   // select unit to process
      rectScreen = m_pCUnitCurrent->GetSize() - ptOffset;
      __SetTrackerRect(rectScreen);
    }
    else {
      ResetAll(UNIT_PRE_SELECT);
    }
    if (__MouseHitTest(point) < 0) { // 如果没有选中单元
      __ClearFocus(pdc);
      if (__MouseTrackRubberBand(this, point, false)) { // 拖曳选择区域内的单元
        // 设置拖曳区域内的单元的被选中标志为真
        rectScreen = m_rectTracker = __GetTrackerRect();
        rectScreen += ptOffset;
        CUnitBase * pcUnit = NULL;
        CRect rect;
        bool fFind = false;
        POSITION pos = m_pCUnitListCurrent->GetHeadPosition();
        for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
          pcUnit = m_pCUnitListCurrent->GetNext(pos);
          rect = pcUnit->GetSize();
          if ((rectScreen & rect) == rect) {	// 位于拖曳区域内？
            pcUnit->SetSelect(true);					// 设置被选中标志为真
            m_pCUnitCurrent = pcUnit;
            m_nCurrentFunction = UNIT_GROUP_SELECTED; // 找到单元, 转至UNIT_GROUP_SELECTED
            fFind = true;
          }
        }
        if (!fFind) {	// 没找到单元,转至UNIT_PRE_SELECT.
          ResetAll(UNIT_PRE_SELECT);
        }
      }
      else { // 从新开始.
        ResetAll(UNIT_PRE_SELECT);
        TRACE("Current function is UNIT_PRE_SELECT\n");
      }
			m_rectTracker.SetRectEmpty();
      __SetTrackerRect(m_rectTracker);
    }
    else if (__MouseTrack(this, point, true)) {  // 改变选中单元的大小或者位置？
      ASSERT(m_pCUnitCurrent != NULL);
      // 单元最小的大小为 30X30.
			m_rectTracker = __GetTrackerRect();
      if (m_rectTracker.Height() < 30) {
        m_rectTracker.bottom = m_rectTracker.top + 30;
      }
      if (m_rectTracker.Width() < 30) {
        m_rectTracker.right = m_rectTracker.left + 30;
      }
			__SetTrackerRect(m_rectTracker);
			m_rectCurrent = m_rectTracker;;
      rectScreen = m_pCUnitCurrent->GetSize();
      rectScreen -= ptOffset;
      ptDevice.x = m_ptMousePosition.x + m_rectCurrent.left - rectScreen.left;
      ptDevice.y = m_ptMousePosition.y + m_rectCurrent.top - rectScreen.top;
      m_pCUnitCurrent->SetSize(m_rectCurrent + ptOffset);
      AdjustDynLinkLinePosition(m_pCUnitCurrent, m_ptMousePosition, ptDevice); //调整动态链接线的位置
      __SetDocModifiedFlag(); // document's content is changed
      m_pCUnitCurrent->SetSelect(true);
      m_nCurrentFunction = UNIT_SELECTED;
    }
    else { // 选中了一个单元.
      ASSERT(m_pCUnitCurrent != NULL);
      m_pCUnitCurrent->SetSelect(true);
      TRACE("Current function is UNIT_SELECTED\n");
      m_nCurrentFunction = UNIT_SELECTED;
    }
    __Invalidate();
    break;
  case DYNAMIC_LINK_FIRST_UNIT:   // 选择目的单元
    if (IsInRect(m_ptMousePosition, m_pCUnitCurrent)) {  // 选择第二个单元来处理
      if (m_pCUnitCurrent->CanLinkIn()) { // 处理部件
        m_nCurrentFunction = DYNAMIC_LINK_TO_SECOND_COMPONENT;
        // determine the dynamic link class
        if (m_ulDynLinkClass == COMPONENT_TO_UNIT) {
          m_ulDynLinkClass = COMPONENT_TO_COMPONENT;
        }
        else if (m_ulDynLinkClass == UNIT_TO_UNIT) {
          m_ulDynLinkClass = UNIT_TO_COMPONENT;
        }
        else { // 类型错误
          ASSERT(0);
        }
        ViewIn((CUnitComponent *)m_pCUnitCurrent); // trace into inner unit list
        m_fLinkIntoDestComponent = true;
      }
      else { // 处理简单单元
        ASSERT(m_pCUnitSecond == nullptr);
        m_pCUnitSecond = m_pCUnitCurrent;
        m_nCurrentFunction = DYNAMIC_LINK_SECOND_UNIT;
      }
      ASSERT(m_rectSecondUnit.IsRectEmpty());
      m_rectSecondUnit = m_rectCurrent = m_pCUnitCurrent->GetSize();
      // 此时已决定了动态链接线的所有位置。下面计算其具体位置
      CreateDynLinkPoint(m_plistLinkPoint, m_rectSecondUnit, m_ptFirst, m_ptSecond, m_ptCurrent); // 有终点。
    } // 如果选择了第二个单元
    else { // 如果鼠标没有在第二个单元范围内，则设置动态链接线。此时尚未完成建立动态链接线的动作
      CreateDynLinkPoint(m_plistLinkPoint, &m_ptFirst, &m_ptSecond, m_ptCurrent); // 无终点
    }
    break;
  case DYNAMIC_LINK_TO_FIRST_COMPONENT: // 在第一个部件内选择单元
    break;
  case DYNAMIC_LINK_TO_SECOND_COMPONENT:  // process second compound unit
    ASSERT((m_ulDynLinkClass == UNIT_TO_COMPONENT) || (m_ulDynLinkClass == COMPONENT_TO_COMPONENT));
    ASSERT(m_pCUnitSecond == nullptr);
    if (IsInRect(m_ptMousePosition, m_pCUnitSecond)) {  // 选择需处理的单元
      ASSERT(m_pCUnitSecond->IsEncapsulated());     // 不允许再次链接未封装的或不允许封装的部件，动态链接只允许陷入一层
      m_nCurrentFunction = DYNAMIC_LINK_SECOND_UNIT;
    }
    else {  // cancelled
      ResetAll(UNIT_PRE_SELECT);
    }
    break;
  case NO_DRAG:
    break;
  case CREATE_NEW_UNIT:
    // 生成新单元在OnLButtonUp中处理
    break;
  case DYNAMIC_LINK_SECOND_UNIT:
    break;
  default: // select first unit
    ASSERT(0);
    break;
  } // switch ( m_nCurrentFunction )

#ifdef _DEBUG
  switch (m_nCurrentFunction) {
  case UNIT_PRE_SELECT:
    TRACE("Current function is UNIT_PRE_SELECT\n");
    break;
  case CREATE_NEW_UNIT:
    TRACE("Current function is CREATE_NEW_UNIT\n");
    break;
  case DYNAMIC_LINK_FIRST_UNIT:
    TRACE("Current function is DYNAMIC_LINK_FIRST_UNIT\n");
    break;
  case DYNAMIC_LINK_SECOND_UNIT:
    TRACE("Current function is DYNAMIC_LINK_SECOND_UNIT\n");
    break;
  case DYNAMIC_LINK_TO_FIRST_COMPONENT:
    TRACE("Current function is DYNAMIC_LINK_TO_FIRST_COMPONENT\n");
    break;
  case DYNAMIC_LINK_TO_SECOND_COMPONENT:
    TRACE("Current function is DYNAMIC_LINK_TO_SECOND_COMPONENT\n");
    break;
  case UNIT_GROUP_SELECTED:
    TRACE("Current function is UNIT_GROUP_SELECTED\n");
    break;
  case NO_DRAG:
    TRACE("Current function is NO_DRAG\n");
    break;
  case UNIT_SELECTED:
    TRACE("Current function is UNIT_SELECTED\n");
    break;
  default:   // must an error!
    TRACE("m_nCurrentFunction is out of range in SQIUnitView's OnLButtonDown\n");
    ASSERT(0);
    break;
  } // switch  
#endif 

  //Invalidate(); 
  //由于有连续动作存在（生成动态链接线时），故而不能统一使用更新窗口这个函数，只能时在不同的状态下才可能用，否则生成的动态链接线会被更新掉
  // 即ToShow这个函数不显示正在生成的动态链接线
  __ReleaseDC(pdc);
	
	__DefaultOnLButtonDown(nFlags, point);	

}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当鼠标移动时，处理相关的任务。
//
// 共存在九种状态，鼠标左键的三个动作（按下、移动、抬起）要处理所有这九种状态，不存在的状态就报告错误
//
// 状态：
// 1.正在第一个部件内选择第一个单元。
// 2.正在第二个部件内选择第二个单元。
// 3.已经选择了一个单元，准备处理。
// 4.没有任何任务。
// 5.正在准备选择第二个单元。
// 6.在第一个或者第二个部件内，鼠标所在的单元是部件。
//
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnMouseMove(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CDC *pdc = __GetDC();
  __OnPrepareDC(pdc);
  CPoint ptDevice, ptOffset = __GetScrollPosition();
  CRect rectTemp;
  CString str = "  ";
  CUnitBase * pcUnit;

  ptDevice = point + ptOffset;
  switch (m_nCurrentFunction) {
  case DYNAMIC_LINK_FIRST_UNIT:
    ASSERT(m_fLinkIntoSourceComponent == false);
    if (!IsInRect(ptDevice, pcUnit)) { // 如果当前鼠标不在单元内的话， 则画动态链接线（当鼠标位于单元范围内时，动态链接线就不动了，不太好）
      DrawInvertDynLinkLine(pdc, m_plistLinkPoint, m_ptFirst, m_ptSecond, m_ptCurrent);
      m_ptCurrent = ptDevice;
      if (m_plistLinkPoint->size() == 1) { // 创建第一个动态链接点
        AdjustDynLinkPoint(m_rectFirstUnit, m_ptFirst, m_ptSecond, m_ptCurrent);
      }
      else {
        AdjustDynLinkPoint(m_ptFirst, m_ptSecond, m_ptCurrent);
      }
      DrawInvertDynLinkLine(pdc, m_plistLinkPoint, m_ptFirst, m_ptSecond, m_ptCurrent);
    }
    break;
  case DYNAMIC_LINK_TO_FIRST_COMPONENT:	// 第一个单元为部件
    ASSERT(m_fLinkIntoSourceComponent);
    if (m_fLinkIntoSourceComponent) {
      if (IsInRect(ptDevice, m_pCUnitNoDrag)) {
        if (m_pCUnitNoDrag->CanLinkIn()) { // 如果在陷入部件后还遇到部件，则不允许再次陷入。
          m_nSavedFunction = m_nCurrentFunction;  // 保存当前状态
          m_nCurrentFunction = NO_DRAG;           // 设置状态为禁止陷入
        }
      }
    }
    break;
  case DYNAMIC_LINK_TO_SECOND_COMPONENT: // 第二个单元为部件
    ASSERT(m_fLinkIntoDestComponent);
    if (m_fLinkIntoDestComponent) {
      if (IsInRect(ptDevice, m_pCUnitNoDrag)) {
        if (!m_pCUnitNoDrag->IsEncapsulated()) {
          m_nSavedFunction = m_nCurrentFunction;
          m_nCurrentFunction = NO_DRAG;
        }
      }
    }
    break;
  case NO_DRAG:
    BOOL f;
    f = IsInRect(ptDevice, pcUnit);
    if (!f) {
      m_nCurrentFunction = m_nSavedFunction;
    }
    else {
      if (pcUnit != m_pCUnitNoDrag) {
        m_nCurrentFunction = m_nSavedFunction;
      }
    }
    break;
  case UNIT_PRE_SELECT:
    ASSERT(m_pCUnitCurrent == NULL);
  case UNIT_SELECTED:
    if (IsInRect(ptDevice, m_pCUnitMouseMove)) {  // current mouse position is in object ?
      // show current unit's name on status bar
      str = m_pCUnitMouseMove->GetClassNameStr();
      str += "->";
      str += m_pCUnitMouseMove->GetName();
    }
    __SetStatusBarMessage(str);
    break;
  case UNIT_GROUP_SELECTED:
    break;
  case CREATE_NEW_UNIT:
    break;
  case DYNAMIC_LINK_SECOND_UNIT:

    break;
  default:
    break;
  } // switch ( m_nCurrentFunction )
  __ReleaseDC(pdc);

  __SetMousePosition(ptDevice);

	__DefaultOnMouseMove(nFlags, point);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// 当鼠标左键抬起时，处理相关的任务
//
// 共存在九种状态，鼠标左键的三个动作（按下、移动、抬起）要处理所有这九种状态，不存在的状态就报告错误
//
// 状态：
// 1.创建新的单元。
// 2.没有任何任务。
// 3.正在准找第二个单元。
// 
// 鼠标左键连续动作的退出，有四个状态：UNIT_SELECTED、UNIT_GROUP_SELECTED、 CREATE_NEW_UNIT和UNIT_PRE_SELECTED
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnLButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  CDlgChoiceParameter CCPDlg;
  CRect  rectTemp;

  CDC *pdc = __GetDC();
  __OnPrepareDC(pdc);
  CString strTemp;
  char s[20];
  CPoint ptDevice, ptOffset = __GetScrollPosition();
  CRect rectScreen;

  ptDevice = point + ptOffset;
  switch (m_nCurrentFunction) {
  case  CREATE_NEW_UNIT:  // create new object, add new unit into UnitList
    __ClipCursor(NULL);
    __ReleaseCapture();
    // strTemp = "_"; // 名字中不再带下划线了
    _itoa(m_pDoc->GetUnitNumber() + 1, s, 10);
    m_pDoc->SetUnitNumber(m_pDoc->GetUnitNumber() + 1);
    strTemp += s;
    switch (gl_ulUnitLibrary) {
      ////////////////////////////////////////////////////////////////////////////////////
        // Input output function
    case ID_BLOCK_INPUTOUTPUT_SIM:
      m_pCUnitCurrent = new CUnitInputOutput(strTemp, ptDevice);
      break;
    case ID_BLOCK_INPUTOUTPUT_TPO:     // creat a time proportione unit
      m_pCUnitCurrent = new CUnitTPO(strTemp, ptDevice);
      break;
      ////////////////////////////////////////////////////////////////////////////////////
        // Arithmatic function
    case ID_BLOCK_ARITHMATIC_DTIM:     // delay time unit
      m_pCUnitCurrent = new CUnitDelayTime(strTemp, ptDevice);
      break;
    case ID_BLOCK_ARITHMATIC_HPBG:     // create a High Pass Filter unit
      m_pCUnitCurrent = new CUnitHighPassBargin(strTemp, ptDevice);
      break;
    case ID_BLOCK_ARITHMATIC_QFLT:     // create a second order filter unit
      m_pCUnitCurrent = new CUnitQuadFilt(strTemp, ptDevice);
      break;
    case ID_BLOCK_ARITHMATIC_TOT:      // create a total unit
      m_pCUnitCurrent = new CUnitTOT(strTemp, ptDevice);
      break;
      ////////////////////////////////////////////////////////////////////////////////////////
        // Mathmaitc function
    case ID_BLOCK_MATHMATIC_ADD:    // create an ADD unit
      m_pCUnitCurrent = new CUnitAdd(strTemp, ptDevice);
      break;
    case ID_BLOCK_MATHMATIC_MULTIPLE:
      m_pCUnitCurrent = new CUnitMultiple(strTemp, ptDevice);
      break;
    case ID_BLOCK_MATHMATIC_DIV:
      m_pCUnitCurrent = new CUnitDivide(strTemp, ptDevice);
      break;
      //////////////////////////////////////////////////////////////////////////////////////
        // Select function
    case ID_BLOCK_SELECT_SWCH:         // create a switch unit
      m_pCUnitCurrent = new CUnitSwitch(strTemp, ptDevice);
      break;
    case ID_BLOCK_SELECT_HSEL:         // create a High Select unit
      m_pCUnitCurrent = new CUnitHighSelect(strTemp, ptDevice);
      break;
    case ID_BLOCK_SELECT_MSEL:         // create a Middle Select unit
      m_pCUnitCurrent = new CUnitMiddleSelect(strTemp, ptDevice);
      break;
    case ID_BLOCK_SELECT_LSEL:         // create a Low Select unit
      m_pCUnitCurrent = new CUnitLowSelect(strTemp, ptDevice);
      break;
      ////////////////////////////////////////////////////////////////////////////////////
        // Basic Logic function
    case ID_BLOCK_BASICLOGIC_AND:      // create a logic AND unit
      m_pCUnitCurrent = new CUnitAnd(strTemp, ptDevice);
      break;
    case ID_BLOCK_BASICLOGIC_OR:       // create a logic OR unit
      m_pCUnitCurrent = new CUnitOr(strTemp, ptDevice);
      break;
    case ID_BLOCK_BASICLOGIC_XOR:      // create a logic XOR unit
      m_pCUnitCurrent = new CUnitXor(strTemp, ptDevice);
      break;
    case ID_BLOCK_BASICLOGIC_PULS:     // create a logic PULSE unit
      m_pCUnitCurrent = new CUnitPulse(strTemp, ptDevice);
      break;
    case ID_BLOCK_BASICLOGIC_INV:      // create a logic invert unit
      m_pCUnitCurrent = new CUnitInvert(strTemp, ptDevice);
      break;
    case ID_BLOCK_BASICLOGIC_DLAY:     // create a delay unit
      m_pCUnitCurrent = new CUnitDelay(strTemp, ptDevice);
      break;
      //////////////////////////////////////////////////////////////////////////////////////
        // Advance logic function
    case ID_BLOCK_ADVANCELOGIC_FFLP:
      m_pCUnitCurrent = new CUnitFFLP(strTemp, ptDevice);
      break;
    case ID_BLOCK_ADVANCELOGIC_TTB:    // create a logic true TABLE unit
      m_pCUnitCurrent = new CUnitTTB(strTemp, ptDevice);
      break;
    case ID_BLOCK_ADVANCELOGIC_ICNT:
      break;
      ///////////////////////////////////////////////////////////////////////////////////////
        // Basic Control function
    case ID_BLOCK_BASICCONTROL_PID:
      m_pCUnitCurrent = new CUnitPID(strTemp, ptDevice);
      break;
      /////////////////////////////////////////////////////////////////////////////////////
        // Simulate function
    case ID_BLOCK_SIMULATE_QUADRATIC:  // create a quadratic function maker
      m_pCUnitCurrent = new CUnitQuad(strTemp, ptDevice);
      break;
    case ID_BLOCK_SIMULATE_SINE:       // create a sine function maker
      m_pCUnitCurrent = new CUnitSine(strTemp, ptDevice);
      break;
      /////////////////////////////////////////////////////////////////////////////////////
        // Component unit
    case ID_BLOCK_COMPOUND:  // 
      m_pCUnitCurrent = new CUnitComponent(strTemp, ptDevice, false); // 复合单元就是不允许封装的部件
      m_pCUnitCurrent->SetUpperUnitList(m_pCUnitListCurrent);
      break;
      //////////////////////////////////////////////////////////////////////////////////////
        // Component unit
    case ID_BLOCK_COMPONENT: // component 
      m_pCUnitCurrent = new CUnitComponent(strTemp, ptDevice, true);	// 默认的部件允许封装
      m_pCUnitCurrent->SetUpperUnitList(m_pCUnitListCurrent);

      break;
      ///////////////////////////////////////////////////////////////////////////////////////
    default:
      TRACE("Error!, can't create unit!, gl_ulUnitLibrary not in range\n");
      break;
    } // switch 

    ASSERT(m_pCUnitCurrent != NULL);
    strTemp = m_pCUnitCurrent->GetClassNameStr() + strTemp;
    m_pCUnitCurrent->SetName(strTemp);
    m_pCUnitCurrent->SetComponentUpper(m_pCUnitComponentCurrent);
		__SetDocModifiedFlag();  // document's content is changed
    __ClearFocus(pdc);
    m_pCUnitCurrent->SetSelect(true);
    m_pCUnitListCurrent->AddTail(m_pCUnitCurrent);
    m_rectCurrent.SetRectEmpty();
    m_nCurrentFunction = UNIT_SELECTED;
    __Invalidate(); // draw this unit

    break;
  case DYNAMIC_LINK_SECOND_UNIT: //寻找目的单元
    ASSERT((m_ulDynLinkType & (tINPUT | tOUTPUT | tMODIFIABLE)) == 0);
    m_ulDynLinkType &= (tDOUBLE | tWORD | tBOOL | tSTRING); // 只保留数据类型
    m_pCUnitSecond->PrepareParaDictionary(CCPDlg.GetDicList(), tMODIFIABLE | tINPUT | m_ulDynLinkType);
    __GetCursorPos(&ptDevice);    // save cursor position
    switch (CCPDlg.DoModal()) {
    case IDOK:
      __SetCursorPos(ptDevice.x, ptDevice.y);   // restore cursor position
      m_lDestIndex = m_pCUnitSecond->GetIndex(CCPDlg.GetIndex());
      m_pCUnitSecond->SetParameterLock(m_lDestIndex, true); // 设置参数锁（输入型参数只允许一个源单元与其相链接
      // 生成新的动态链接
      m_pCUnitDynLinkCurrent = new CUnitDynLink();  // create Dynamic Link
      m_pCUnitDynLinkCurrent->SetDynLinkType(m_ulDynLinkType);
      m_pCUnitDynLinkCurrent->SetDynLinkClass(m_ulDynLinkClass);
      m_pCUnitDynLinkCurrent->SetSrcUnit(m_pCUnitFirst);
      m_pCUnitDynLinkCurrent->SetDestUnit(m_pCUnitSecond);
      m_pCUnitDynLinkCurrent->SetLinkPointList(m_plistLinkPoint);
      m_plistLinkPoint->clear();
      m_pCUnitDynLinkCurrent->SetSrcIndex(m_lSrcIndex);
      m_pCUnitDynLinkCurrent->SetDestIndex(m_lDestIndex);
      m_pCUnitFirst->AddDynLink(m_pCUnitDynLinkCurrent);
			__SetDocModifiedFlag();  // document's content is changed
      UnitListLoopDetect(m_pCUnitListTop); // 检查所有的单元序列是否存在动态链接的循环
      break;
    case IDCANCEL:
      ResetAll(UNIT_SELECTED);
      break;
    default:
      ResetAll(UNIT_PRE_SELECT);
      break;
    } // switch idTemp
    ResetAll(UNIT_PRE_SELECT);
    if (m_fLinkIntoDestComponent) {
      ViewOut();    // return to upper unit list
      m_nCurrentFunction = UNIT_SELECTED;
      m_fLinkIntoDestComponent = false;
    }
    __Invalidate();   // update screen
    break;
  case DYNAMIC_LINK_TO_SECOND_COMPONENT: // 链接至目的部件
    ASSERT(m_pCUnitCurrent->IsKindOf(RUNTIME_CLASS(CUnitComponent)));
    break;
  case UNIT_PRE_SELECT: // 恢复最初状态
    if (m_fLinkIntoSourceComponent) {
      ViewOut();  // return to upper unit list
      m_fLinkIntoSourceComponent = false;
    }
    if (m_fLinkIntoDestComponent) {
      ViewOut(); // return to upper unit list
      m_fLinkIntoDestComponent = false;
    }
    DeleteDynLinkPointList(m_plistLinkPoint);
    break;
  case NO_DRAG:
    TRACE("m_nCurrentFunction is NO_GRAG in OnLButtonUp\n");
    break;
  case UNIT_SELECTED:
    TRACE("m_nCurrentFunction is UNIT_SELECTED in OnLButtonUp\n");
    break;
  case DYNAMIC_LINK_TO_FIRST_COMPONENT:
    ASSERT(m_ulDynLinkClass == COMPONENT_TO_UNIT);
    if (IsInRect(m_ptMousePosition, m_pCUnitCurrent)) {  // 选择部件的内部单元处理
      ASSERT(!m_pCUnitCurrent->CanLinkIn()); // 不允许再次链接未封装的或不允许封装的部件，动态链接只允许陷入一层
      ASSERT(m_pCUnitFirst == nullptr);
      m_pCUnitFirst = m_pCUnitCurrent;
      ASSERT(m_plistLinkPoint->size() == 1);
      m_pCUnitFirst->PrepareParaDictionary(CCPDlg.GetDicList(), tMODIFIABLE | tOUTPUT | tDOUBLE | tBOOL | tWORD | tSTRING);
      switch (CCPDlg.DoModal()) {
      case IDOK:
        m_lSrcIndex = m_pCUnitFirst->GetIndex(CCPDlg.GetIndex());
        m_ulDynLinkType = m_pCUnitFirst->GetDynLinkType(m_lSrcIndex);      
        m_nCurrentFunction = DYNAMIC_LINK_FIRST_UNIT;
        break;
      case IDCANCEL:
        ResetAll(UNIT_PRE_SELECT);
        break;
      default:
        ResetAll(UNIT_PRE_SELECT);
        break;
      } // switch idTemp
      if (m_fLinkIntoSourceComponent) {
        ViewOut();        // 返回上层
        ASSERT(m_pCUnitCurrent != nullptr);
        if (m_plistLinkPoint->size() == 1) { // 创建第一个动态链接点
          AdjustDynLinkPoint(m_rectFirstUnit, m_ptFirst, m_ptSecond, m_ptCurrent); // 从下层上来后要调整动态链接线
        }
        else {
          AdjustDynLinkPoint(m_ptFirst, m_ptSecond, m_ptCurrent);
        }
        m_fLinkIntoSourceComponent = false;
        __UpdateWindow(); // 必须马上更新窗口，不能用Invalidate(),因为Invalidate()会延后更新，导致随后画的第一条动态链接线是画在下层了。 
      }
    } // 选择了一个unit
    else { // cancelled
      ResetAll(UNIT_PRE_SELECT);
    }
    break;
  case DYNAMIC_LINK_FIRST_UNIT:
    break;
  case UNIT_GROUP_SELECTED:
    break;
  default:
    TRACE("m_nCurrentFunction is default in OnLButtonUp\n");
    ASSERT(0);
  } // switch m_nCurrentFunction

  __ReleaseDC(pdc);
	__DefaultOnLButtonUp(nFlags, point);
}

void CSQIUnitView::OnRButtonUp(UINT nFlags, CPoint point)
{
  // TODO: Add your message handler code here and/or call default
  switch (m_nCurrentFunction) {
  case DYNAMIC_LINK_FIRST_UNIT:
    ResetAll(UNIT_PRE_SELECT);
    __Invalidate();
    break;
  default:
    ResetAll(UNIT_PRE_SELECT);
    break;
  }

	__DefaultOnRButtonUp(nFlags, point);
}

void CSQIUnitView::OnEditCopy()
{
  // TODO: Add your command handler code here
  char *strFileName = "STRATEGY.CLP";
  char * lpData;

  if ((hData = GlobalAlloc(GMEM_MOVEABLE, 128)) == NULL) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  if ((lpData = (char *)GlobalLock(hData)) == NULL) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  lstrcpy(lpData, strFileName);			// 只在剪辑板中传递文件名，具体数据存储于此文件中

  CFile cFile;
  char buffer[512];

  if (!cFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite)) {
    ShowMessage(ID_ERROR_OPEN_FILE, strFileName);
  }
  CArchive ar(&cFile, CArchive::store, 512, buffer);

  CUnitList listUnit;
  CUnitBase * pcunit;
  INT64 iCount = 0;
  POSITION po = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      pcunit->AddToList(listUnit); // 包括部件本身
      iCount++;
    }
  }
  po = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      pcunit->SetDeleteDynLinkFlag(listUnit);
    }
  }
  po = m_pCUnitListCurrent->GetHeadPosition();
  ar << iCount;
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      ar << pcunit;
    }
  }
  po = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      pcunit->ClearDeleteDynLinkFlag();
    }
  }

  if (OpenClipboard()) {
    EmptyClipboard();
    SetClipboardData(m_uUnitFormat, hData);
    GlobalUnlock(hData);
    CloseClipboard();
  }
  hData = NULL;
}

void CSQIUnitView::OnEditCut()
{
  // TODO: Add your command handler code here
  char *strFileName = "STRATEGY.CLP";
  char * lpData;

  if ((hData = GlobalAlloc(GMEM_MOVEABLE, 128)) == NULL) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  if ((lpData = (char *)GlobalLock(hData)) == NULL) {
    ShowMessage(ID_ERROR_OUT_OF_MEMORY);
    return;
  }
  lstrcpy(lpData, strFileName);		// 只在剪辑板中传递文件名，具体数据存储于此文件中

  CFile cFile;
  char buffer[512];

  if (!cFile.Open(strFileName, CFile::modeCreate | CFile::modeWrite)) {
    ShowMessage(ID_ERROR_OPEN_FILE, strFileName);
  }
  CArchive ar(&cFile, CArchive::store, 512, buffer);

  CUnitList listUnit;
  CUnitBase * pcunit;
  INT64 iDelete = 0, iCount;
  POSITION po = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      pcunit->AddToList(listUnit); // 包括部件本身
      iDelete++;
    }
  }
  // 将动态连接进行分类,决定是否跟随单元一起存储.如果不能一起存储的则设置删除标志。
  po = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      pcunit->SetDeleteDynLinkFlag(listUnit);
    }
  }
  po = m_pCUnitListCurrent->GetHeadPosition();
  ar << iDelete;
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      ar << pcunit;
    }
  }
  po = m_pCUnitListCurrent->GetHeadPosition();
  iCount = m_pCUnitListCurrent->GetCount();
  for (int i = 0; i < iCount; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      pcunit->ClearDeleteDynLinkFlag();
    }
  }
  po = m_pCUnitListCurrent->GetHeadPosition();
  iCount = m_pCUnitListCurrent->GetCount();
  for (int i = 0; i < iCount; i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      VERIFY(DeleteUnit(pcunit));      // delete cut units
    }
  }

  ResetAll(UNIT_PRE_SELECT);

  if (OpenClipboard()) {
    EmptyClipboard();
    SetClipboardData(m_uUnitFormat, hData);
    GlobalUnlock(hData);
    CloseClipboard();
  }
  hData = NULL;

	__SetDocModifiedFlag();  // document's content is changed
  UnitListLoopDetect(m_pCUnitListTop); // 检查所有的单元序列是否存在动态链接的循环

  __Invalidate();
}

void CSQIUnitView::OnEditDelete()
{
  // TODO: Add your command handler code here
  CUnitBase * pcunit;
  POSITION po = m_pCUnitListCurrent->GetHeadPosition();
  po = m_pCUnitListCurrent->GetHeadPosition();
  po = m_pCUnitListCurrent->GetHeadPosition();
  for (int i = 0; i < m_pCUnitListCurrent->GetCount(); i++) {
    pcunit = m_pCUnitListCurrent->GetNext(po);
    if (pcunit->IsSelect()) {
      VERIFY(DeleteUnit(pcunit));      // delete units
    }
  }

  ResetAll(UNIT_PRE_SELECT);

	__SetDocModifiedFlag(); // document's content is changed
  UnitListLoopDetect(m_pCUnitListTop); // 检查所有的单元序列是否存在动态链接的循环

  __Invalidate();
}

void CSQIUnitView::OnEditPaste()
{
  // TODO: Add your command handler code here
  CFile cFile;
  char buffer[512];
  char * strFileName;
  CUnitList unitlistTemp;

  if (OpenClipboard()) {
    if ((hData = GetClipboardData(m_uUnitFormat)) == NULL) {
      CloseClipboard();
      return;
    }
    if ((strFileName = (char *)GlobalLock(hData)) == NULL) {
      ShowMessage(ID_ERROR_OUT_OF_MEMORY);
      CloseClipboard();
      return;
    }
  }

  cFile.Open(strFileName, CFile::modeRead);
  CArchive ar(&cFile, CArchive::load, 512, buffer);
  CUnitList listUnit;
  INT64 iCount = 0;
  POSITION po;
  CUnitBase * pcunit;

  ClearAllSelect();

  ar >> iCount;
  for (int i = 0; i < iCount; i++) {
    ar >> m_pCUnitCurrent;
    CreateUniName(m_pCUnitCurrent);
    m_pCUnitCurrent->ResetCompileFlag();
    unitlistTemp.AddTail(m_pCUnitCurrent); // 先暂存单元于暂时的单元序列中，以备单独处理之

  }
  m_pCUnitCurrent->SetSelect(true);

  // 重置编译标志
  ReSetCompileFlag(&unitlistTemp);
  // 设置内部状态
  SetParaLockFlag(&unitlistTemp, m_pObjectList);

  // 将设置好的单元加入当前层的单元序列中
  po = unitlistTemp.GetHeadPosition();
  iCount = unitlistTemp.GetCount();
  for (int i = 0; i < iCount; i++) {
    pcunit = unitlistTemp.GetNext(po);
    m_pCUnitListCurrent->AddTail(pcunit); 
  }

  GetDocument()->m_trackerUnit.m_rect = m_pCUnitCurrent->GetSize() - GetScrollPosition();
  m_nCurrentFunction = UNIT_SELECTED;
  GlobalUnlock(hData);
  CloseClipboard();

	__SetDocModifiedFlag(); // document's content is changed
  UnitListLoopDetect(m_pCUnitListTop); // 检查所有的单元序列是否存在动态链接的循环

  __Invalidate();
}

void CSQIUnitView::OnEditUndo()
{
  // TODO: Add your command handler code here

}

void CSQIUnitView::OnStyleCentered()
{
  // TODO: Add your command handler code here
	__SetDocModifiedFlag(); // document's content is changed
  CenterAlign();
  __Invalidate();
}

void CSQIUnitView::OnStyleLeft()
{
  // TODO: Add your command handler code here
  GetDocument()->SetModifiedFlag(true); // document's content is changed
  LeftAlign();
  __Invalidate();
}

void CSQIUnitView::OnStyleRight()
{
  // TODO: Add your command handler code here
	__SetDocModifiedFlag(); // document's content is changed
  RightAlign();
  __Invalidate();
}

void CSQIUnitView::OnArrangeToback()
{
  // TODO: Add your command handler code here

  GetDocument()->SetModifiedFlag(true); // document's content is changed
  ASSERT(m_pCUnitCurrent != NULL);
  UnitToBack(m_pCUnitListCurrent, m_pCUnitCurrent);
  __Invalidate();
}

void CSQIUnitView::OnArrangeTofront()
{
  // TODO: Add your command handler code here

	__SetDocModifiedFlag(); // document's content is changed
  ASSERT(m_pCUnitCurrent != NULL);
  UnitToFront(m_pCUnitListCurrent, m_pCUnitCurrent);
  __Invalidate();
}

void CSQIUnitView::OnUpdateArrangeToback(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
  } // switch
}

void CSQIUnitView::OnUpdateArrangeTofront(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
  } // switch
}

void CSQIUnitView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
  case UNIT_GROUP_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

void CSQIUnitView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
  case UNIT_GROUP_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

void CSQIUnitView::OnUpdateEditDelete(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
  case UNIT_GROUP_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

void CSQIUnitView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (OpenClipboard()) {
    if (IsClipboardFormatAvailable(m_uUnitFormat)) {
      pCmdUI->Enable(true);
    }
    else pCmdUI->Enable(false);
  }
  else pCmdUI->Enable(false);
  CloseClipboard();
}

void CSQIUnitView::OnUpdateEditUndo(CCmdUI*)
{
  // TODO: Add your command update UI handler code here

}

void CSQIUnitView::OnUpdateStyleCentered(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_GROUP_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

void CSQIUnitView::OnUpdateStyleLeft(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_GROUP_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

void CSQIUnitView::OnUpdateStyleRight(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_GROUP_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

void CSQIUnitView::OnUpdateBlock(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->SetCheck((UINT)(gl_ulUnitLibrary) == pCmdUI->m_nID);
}

void CSQIUnitView::OnUpdateBlockI(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->Enable(false);
}

void CSQIUnitView::OnBlockMathmaticAdd()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_MATHMATIC_ADD;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputAin()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_AIN;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockSimulateQuadratic()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_SIMULATE_QUADRATIC;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockSimulateSine()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_SIMULATE_SINE;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnViewViewin()
{
  // TODO: Add your command handler code here

  ASSERT(m_pCUnitCurrent != nullptr);
  ViewIn((CUnitComponent *)m_pCUnitCurrent);
  m_pCUnitCurrent = nullptr;     // set current process unit to NULL
  m_nCurrentFunction = UNIT_PRE_SELECT;
  __SetCurrentUnitList(m_pCUnitListCurrent);
  m_pDoc->m_trackerUnit.m_rect.SetRectEmpty();
}

void CSQIUnitView::OnUpdateViewViewin(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here

  if (m_pCUnitCurrent != nullptr) {
    if (m_pCUnitCurrent->CanViewIn()) {
      pCmdUI->Enable(true);
    }
    else {
      pCmdUI->Enable(false);
    }
  }
  else {
    pCmdUI->Enable(false);
  }
}

void CSQIUnitView::OnViewViewout()
{
  // TODO: Add your command handler code here
  if (m_pCUnitCurrent != nullptr) m_pCUnitCurrent->SetSelect(false);
  ViewOut();
  AdjustDynLinkPoint(m_ptFirst, m_ptSecond, m_ptCurrent);
  m_nCurrentFunction = UNIT_SELECTED;
  __SetCurrentUnitList(m_pCUnitListCurrent);
}

void CSQIUnitView::OnUpdateViewViewout(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here

  if (m_lComponentLayer > 0) {
    pCmdUI->Enable(true);
  }
  else {
    pCmdUI->Enable(false);
  }
}

void CSQIUnitView::OnBlockCompound()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_COMPOUND;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockComponent()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_COMPONENT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnUpdateObjectBar(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  pCmdUI->Enable(false);
}

void CSQIUnitView::OnBlockMathmaticMultiple()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_MATHMATIC_MULTIPLE;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiclogicAnd()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICLOGIC_AND;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiclogicDlay()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICLOGIC_DLAY;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiclogicDrot()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICLOGIC_DROT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiclogicInv()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICLOGIC_INV;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiclogicOr()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICLOGIC_OR;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiclogicPuls()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICLOGIC_PULS;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiclogicXor()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICLOGIC_XOR;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockAdvancelogicFflp()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ADVANCELOGIC_FFLP;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockAdvancelogicIcnt()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ADVANCELOGIC_ICNT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticQflt()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_QFLT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticHflt()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_HFLT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockAdvancecontrolSchd()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ADVANCECONTROL_SCHD;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockAdvancecontrolSeq()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ADVANCECONTROL_SEQ;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockAdvancecontrolSeqe()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ADVANCECONTROL_SEQE;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockAdvancelogicTtb()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ADVANCELOGIC_TTB;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockSelectHsel()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_SELECT_HSEL;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockSelectLsel()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_SELECT_LSEL;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockSelectMsel()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_SELECT_MSEL;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockSelectSwch()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_SELECT_SWCH;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockMathmaticExpr()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_MATHMATIC_EXPR;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockMathmaticDiv()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_MATHMATIC_DIV;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockMathmaticAvgt()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_MATHMATIC_AVGT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockAdvancelogicTtbe()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ADVANCELOGIC_TTBE;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticTot()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_TOT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticAlrm()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_ALRM;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticChar()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_CHAR;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticCond()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_COND;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticDtim()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_DTIM;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticLlag()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_LLAG;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockArithmaticHpbg()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_ARITHMATIC_HPBG;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiccontrolAmb()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICCONTROL_AMB;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiccontrolIntd()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICCONTROL_INTD;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiccontrolPd()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICCONTROL_PD;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiccontrolPid()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICCONTROL_PID;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockBasiccontrolTime()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_BASICCONTROL_TIME;
}

void CSQIUnitView::OnBlockHistorytrendAhst()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_HISTORYTREND_AHST;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockHistorytrendDhst()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_HISTORYTREND_DHST;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockHistorytrendRprt()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_HISTORYTREND_RPRT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockHistorytrendSpc()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_HISTORYTREND_SPC;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputAins()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_AINS;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputAkbd()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_AINS;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputAots()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_AOTS;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputAout()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_AOUT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputDin()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_DIN;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputDins()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_DINS;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputDkbd()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_DKBD;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputDots()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_DOTS;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputDout()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_DOUT;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputHorn()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_HORN;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputTpo()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_TPO;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockInputoutputSim()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_INPUTOUTPUT_SIM;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockMathmaticAvg()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_MATHMATIC_AVG;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnBlockSimulateLogarithm()
{
  // TODO: Add your command handler code here
  gl_ulUnitLibrary = ID_BLOCK_SIMULATE_LOGARITHM;
  m_nCurrentFunction = CREATE_NEW_UNIT;
}

void CSQIUnitView::OnUpdateArrangeBreakdynlink(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
    ASSERT(m_pCUnitCurrent != nullptr);
    if (m_pCUnitCurrent->GetDynLinkList()->GetCount() > 0) {  // have dynamic link ?
      pCmdUI->Enable(true);
    }
    else {
      pCmdUI->Enable(false);
    }
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

/////////////////////////////////////////////////////////////////
//
// 删除动态链接
//
/////////////////////////////////////////////////////////////////
void CSQIUnitView::OnArrangeBreakdynlink()
{
  // TODO: Add your command handler code here
  if (m_pCUnitCurrent->ArrangeDynLink()) {
		__SetDocModifiedFlag();
  }
  __Invalidate();
}


void CSQIUnitView::OnUpdateArrangeTogglecutoff(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

///////////////////////////////////////////////////////////////////////////////////
//
// 切换截断标志
//
//////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnArrangeTogglecutoff()
{
  // TODO: Add your command handler code here
  if (m_pCUnitCurrent->IsSetCutOff()) {	// 如已经设置了截断,则清除之.
    m_pCUnitCurrent->SetCutOff(false);
    UnitListLoopDetect(m_pCUnitListCurrent);  // 回路检查
    __Invalidate();
    return;
  }
  else {			// 设置截断.
    CUnitComponent * pcpd;
    pcpd = m_pCUnitCurrent->GetComponentUpper();
    if (pcpd != nullptr) {	// 有部件包含我？
      if (!AlreadyHaveCutOff(m_pCUnitCurrent, m_pCUnitListCurrent)) { // 我不在部件的循环中.
        m_pCUnitCurrent->SetCutOff(true);	// 设置截断标志.
        UnitListLoopDetect(m_pCUnitListCurrent);  // 
        __Invalidate();
      }
      return; // 返回,不再检查整个序列的情况.
    }
    else { // 最上层序列
      if (!AlreadyHaveCutOff(m_pCUnitCurrent, m_pCUnitListCurrent)) {	// 我不在整个序列的循环中?
        m_pCUnitCurrent->SetCutOff(true);	// 设置截断标志.
        UnitListLoopDetect(m_pCUnitListCurrent);  // 
        __Invalidate();
        return;
      }
    }
  }
}

void CSQIUnitView::OnViewProperty()
{
  // TODO: Add your command handler code here
  if (m_pCUnitCurrent->SetProperty()) {
		__SetDocModifiedFlag();
  }
  __Invalidate();
}

void CSQIUnitView::OnUpdateViewProperty(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (m_pCUnitCurrent != NULL) {
    pCmdUI->Enable(true);
  }
  else {
    pCmdUI->Enable(false);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 根据当前的状态设置鼠标样式
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSQIUnitView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
  // TODO: Add your message handler code here and/or call default

  if (pWnd == this && m_pDoc->m_trackerUnit.SetCursor(this, nHitTest))
    return true;
  switch (m_nCurrentFunction) {
  case DYNAMIC_LINK_FIRST_UNIT:
  case DYNAMIC_LINK_TO_FIRST_COMPONENT:
  case DYNAMIC_LINK_TO_SECOND_COMPONENT:
    SetCursor(m_hCursorDynamicLink); // 设置动态链接光标
    break;
  case NO_DRAG:
    SetCursor(m_hCursorNoDrag);			//设置禁止光标
    break;
  case UNIT_PRE_SELECT:
    if (m_pCUnitMouseMove != nullptr) {
      if (m_pCUnitMouseMove->CanViewIn()) { // 部件？
        SetCursor(m_hCursorComponent);	// 设置联入光标
        break;
      }
      else {
        SetCursor(m_hCursorArrow); // 正常光标
      }
    }
    else {
      SetCursor(m_hCursorArrow); // 正常光标
    }
    break;
  default:
    SetCursor(m_hCursorArrow); // 正常光标
    break;
  }
  return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////////////
//
// 生成动态链接，设置当前工作状态（m_nCurrentFunction)为DYNAMIC_LINK_FIRST_UNIT或者
// DYNAMIC_LINK_TO_FIRST_COMPONENT(如果第一个单元类型为部件）。
//
// 如果函数执行完成，则设置以下变量：
//	m_nCurrentFunction = DYNAMIC_LINK_FIRST_UNIT 或者 DYNAMIC_LINK_TO_FIRST_COMPONENT
//  m_ulDynLinkClass = UNIT_TO_UNIT 或者 COMPONENT_TO_UNIT
//  m_pCUnitFirst = m_pCUnitCurrent;
//	m_rectFirstUnit = m_pCUnitCurrent->GetSize(); // 设置第一个unit的位置
//	m_ptFirst.x = m_ptSecond.x = m_ptCurrent.x = m_rectFirstUnit.right;
//	m_ptFirst.y = m_ptSecond.y = m_ptCurrent.y = m_rectFirstUnit.top;
//
//
//
//
////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnArrangeMakedynlink()
{
  // TODO: Add your command handler code here
  CPoint * ppt;
  CDlgChoiceParameter CCPDlg;

  ASSERT(m_pCUnitCurrent != NULL);
  ASSERT(m_plistLinkPoint->size() == 0);
  m_pCUnitFirst = m_pCUnitSecond = nullptr;
  m_plistLinkPoint->clear();
  if (m_pCUnitCurrent->IsKindOf(RUNTIME_CLASS(CUnitComponent))) { // 部件？
    if (m_pCUnitCurrent->IsEncapsulated()) { // 已经封装了的部件？则视其为简单单元
      m_pCUnitFirst = m_pCUnitCurrent;
      m_nCurrentFunction = DYNAMIC_LINK_FIRST_UNIT;
      m_ulDynLinkClass = UNIT_TO_UNIT;
    }
    else { // 不是封装了的部件
      m_nCurrentFunction = DYNAMIC_LINK_TO_FIRST_COMPONENT;
      m_ulDynLinkClass = COMPONENT_TO_UNIT;
    }
  }
  else { // 简单单元
    ASSERT(m_pCUnitFirst == nullptr);
    m_pCUnitFirst = m_pCUnitCurrent;
    m_nCurrentFunction = DYNAMIC_LINK_FIRST_UNIT;
    m_ulDynLinkClass = UNIT_TO_UNIT;
  }
  ASSERT(m_rectFirstUnit.IsRectEmpty());
  m_rectFirstUnit = m_pCUnitCurrent->GetSize(); // 设置第一个单元的位置
  m_ptFirst.x = m_ptSecond.x = m_ptCurrent.x = m_rectFirstUnit.right;
  m_ptFirst.y = m_ptSecond.y = m_ptCurrent.y = m_rectFirstUnit.top;
  ASSERT(m_plistLinkPoint->size() == 0); // 确保没有动态链接点
  ppt = new CPoint;
  TRACE("Start position：%d %d\n", m_ptFirst.x, m_ptFirst.y);
  *ppt = m_ptFirst;
  m_plistLinkPoint->push_back(ppt);

  if (m_ulDynLinkClass == COMPONENT_TO_UNIT) { // 部件
    ASSERT(m_pCUnitCurrent->IsKindOf(RUNTIME_CLASS(CUnitComponent)));
    ViewIn((CUnitComponent *)m_pCUnitCurrent);  // trace into inner unit list
    m_fLinkIntoSourceComponent = true;

  }
  else { // 简单单元或者封装后的部件
    m_pCUnitFirst->PrepareParaDictionary(CCPDlg.GetDicList(), tMODIFIABLE | tOUTPUT | tDOUBLE | tBOOL | tWORD | tSTRING);
    switch (__DlgChoicePara(&CCPDlg)) { // 选择参数
    case IDOK: // 成功
      m_lSrcIndex = m_pCUnitFirst->GetIndex(CCPDlg.GetIndex()); // 当前参数的位置
      m_ulDynLinkType = m_pCUnitFirst->GetDynLinkType(m_lSrcIndex); // 当前参数的类型
      break;
    default:	// 放弃
      DeleteDynLinkPointList(m_plistLinkPoint);
      m_pCUnitFirst = m_pCUnitCurrent = nullptr;
      m_nCurrentFunction = UNIT_PRE_SELECT; 
      break;
    } // switch
  }
}

void CSQIUnitView::OnUpdateArrangeMakedynlink(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  switch (m_nCurrentFunction) {
  case UNIT_SELECTED:
    pCmdUI->Enable(true);
    break;
  default:
    pCmdUI->Enable(false);
    break;
  } // switch
}

void CSQIUnitView::OnUpdateArrangeLinkIntoComponent(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  CUnitComponent * pc;

  if ((m_nCurrentFunction == UNIT_SELECTED) && ((pc = m_pCUnitCurrent->GetComponentUpper()) != NULL)) {
    if (pc->IsEncapsulable()) { // 复合单元不允许设置动态链接
      if (!m_pCUnitCurrent->CanViewIn()) { // 本单元必须为简单单元，
        pCmdUI->Enable(true);
        return;
      }
    }
  }
  pCmdUI->Enable(false);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//
// 从部件内部的单元中选择一个输出型参数，将其链接至部件的参数中。
// 在封装部件时，在此源单元中生成一个新的动态链接
//
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnArrangeLinkIntoComponent()
{
  // TODO: Add your command handler code here
  ULONG ulDestIndex, ulSrcIndex;
  CDlgChoiceParameter CCPDlgSrc;
  CDlgChoiceCUCPara CCPDlgDest;
  CString strName;

  ASSERT(m_pCUnitCurrent != NULL);
  m_pCUnitCurrent->PrepareParaDictionary(CCPDlgSrc.GetDicList(), tMODIFIABLE | tOUTPUT | tDOUBLE | tBOOL | tWORD | tSTRING);// 任何输出型参数皆可做为联出参数
  if (CCPDlgSrc.DoModal() == IDOK) {
    ulSrcIndex = m_pCUnitCurrent->GetIndex(CCPDlgSrc.GetIndex());
  }
  else return;

  m_pCUnitComponentCurrent->PrepareParameterDictionary(CCPDlgDest.GetDicList(), tMODIFIABLE | tOUTPUT | tDOUBLE | tBOOL | tWORD | tSTRING, 0);	// 任何可被修改的且没有动态链接的参数皆可
  if (CCPDlgDest.DoModal() == IDOK) {
    ulDestIndex = m_pCUnitComponentCurrent->GetIndex(CCPDlgDest.GetIndex());
    // 设置部件参数的具体数据
    strName = m_pCUnitComponentCurrent->GetParaName(ulDestIndex);
    strName = strName + "(" + m_pCUnitCurrent->GetName() + "." + m_pCUnitCurrent->GetParaName(ulSrcIndex) + ")";
    m_pCUnitComponentCurrent->SetParaName(ulDestIndex, strName);
    m_pCUnitComponentCurrent->SetParaLinkedFlag(ulDestIndex, true);					// 设置选中标志
    m_pCUnitComponentCurrent->SetParaSrcUnit(ulDestIndex, m_pCUnitCurrent);	// 
    m_pCUnitComponentCurrent->SetParaSrcIndex(ulDestIndex, ulSrcIndex);
    m_pCUnitComponentCurrent->SetParaType(ulDestIndex, m_pCUnitCurrent->GetParaType(ulSrcIndex) | tMODIFIABLE | tOUTPUT);
    m_pCUnitComponentCurrent->SetParaLinkedFlag(ulDestIndex, true);

    m_pCUnitCurrent->LinkToComponent(true);
		__SetDocModifiedFlag(); // document's content is changed
  }
}

void CSQIUnitView::OnUpdateBreakLinkOfComponent(CCmdUI* pCmdUI)
{
  // TODO: Add your command update UI handler code here
  if (m_pCUnitCurrent != NULL) {
    if (m_pCUnitCurrent->IsKindOf(RUNTIME_CLASS(CUnitComponent))) {
      if (((CUnitComponent *)m_pCUnitCurrent)->HaveParameter()) {
        pCmdUI->Enable(true);
        return;
      }
    }
  }
  pCmdUI->Enable(false);
}

void CSQIUnitView::OnBreakLinkOfComponent()
{
  // TODO: Add your command handler code here
  CDlgDeleteComponentPara CDlg;

  CDlg.SetLink((CUnitComponent *)m_pCUnitCurrent, ((CUnitComponent *)m_pCUnitCurrent)->GetParaInterface());
  CDlg.DoModal();

}

void CSQIUnitView::ResetAll(ULONG ulType) {

  m_pCUnitFirst = m_pCUnitSecond = nullptr;
  m_rectFirstUnit.SetRectEmpty();
  m_rectSecondUnit.SetRectEmpty();
  m_rectCurrent.SetRectEmpty();
  m_ptFirst.x = m_ptFirst.y = m_ptSecond.x = m_ptSecond.y = 0;
	m_ulDynLinkType = 0;
	m_lSrcIndex = m_lDestIndex = -1; // 故意设成无效值。
	
	CRect rect;

  switch (ulType) {
  case UNIT_PRE_SELECT:
    DeleteDynLinkPointList(m_plistLinkPoint);
    m_pCUnitCurrent = nullptr;
		rect.SetRectEmpty();
    __SetTrackerRect(rect);
    m_nCurrentFunction = UNIT_PRE_SELECT;
    break;
  case UNIT_SELECTED:
    DeleteDynLinkPointList(m_plistLinkPoint);
    m_nCurrentFunction = UNIT_SELECTED;
    break;
  default:
    ASSERT(0);
    break;
  }
}

void CSQIUnitView::OnUpdateArrangeLinkfromcomponent(CCmdUI *pCmdUI)
{
  // TODO: 在此添加命令更新用户界面处理程序代码
  CUnitComponent * pc;

  if ((m_nCurrentFunction == UNIT_SELECTED)
    && ((pc = m_pCUnitCurrent->GetComponentUpper()) != NULL)) {
    if (!m_pCUnitCurrent->CanViewIn()) { // 不允许再次陷入部件
      pCmdUI->Enable(true);
      return;
    }
  }
  pCmdUI->Enable(false);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// 从包含自己的部件参数中联入一个动态链接至本单元的输入型参数
//
////////////////////////////////////////////////////////////////////////////////////////////////
void CSQIUnitView::OnArrangeLinkfromcomponent()
{
  // TODO: 在此添加命令处理程序代码

  ULONG ulSrcIndex, ulDestIndex;
  CDlgChoiceParameter CCPDlgDest;
  CDlgChoiceCUCPara CCPDlgSrc;
  CString strName;

  m_pCUnitComponentCurrent->PrepareParameterDictionary(CCPDlgSrc.GetDicList(), tINPUT, -1);	// 任何可被修改的输入型参数或者没有链接皆可
  if (CCPDlgSrc.DoModal() == IDOK) {
    ulSrcIndex = m_pCUnitComponentCurrent->GetIndex(CCPDlgSrc.GetIndex());
  }
  else { // 未选择则退出
    return;
  }

  ASSERT(m_pCUnitCurrent != NULL);
  m_pCUnitCurrent->PrepareParaDictionary(CCPDlgDest.GetDicList(), tMODIFIABLE | tINPUT | tDOUBLE | tBOOL | tWORD | tSTRING);// 任何可被修改的且尚未动态链接的输入型参数方可做为参数
  if (CCPDlgDest.DoModal() == IDOK) {
    ulDestIndex = m_pCUnitCurrent->GetIndex(CCPDlgDest.GetIndex());
    strName = m_pCUnitComponentCurrent->GetParaName(ulSrcIndex);
    strName = strName + "(" + m_pCUnitCurrent->GetName() + "." + m_pCUnitCurrent->GetParaName(ulDestIndex) + ")";
    m_pCUnitComponentCurrent->SetParaName(ulSrcIndex, strName);
    m_pCUnitComponentCurrent->SetParaLinkedFlag(ulSrcIndex, true);					// 设置选中标志
    m_pCUnitComponentCurrent->SetParaDestUnit(ulSrcIndex, m_pCUnitCurrent);	// 
    m_pCUnitComponentCurrent->SetParaDestIndex(ulSrcIndex, ulDestIndex);
    m_pCUnitComponentCurrent->SetParaType(ulSrcIndex, m_pCUnitCurrent->GetParaType(ulDestIndex) | tMODIFIABLE | tINPUT);
    m_pCUnitCurrent->SetParameterLock(ulDestIndex, true); // 输入型参数只允许链接一次
    m_pCUnitCurrent->LinkFromComponent(true);
		__SetDocModifiedFlag(); // document's content is changed	
  }
}

void CSQIUnitView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: 在此添加消息处理程序代码和/或调用默认值

  CScrollView::OnKeyUp(nChar, nRepCnt, nFlags);

}
