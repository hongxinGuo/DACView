//////////////////////////////////////////////////////////////////////////////////////
//  
//	全局变量定义，声明在globedef.h中
//    
//
//
//
///////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxpriv.h>    // for idle-update windows message

#include "globedef.h"

// globe variables define 
volatile ULONGLONG  gl_ulRealTimeProcessTime;
volatile ULONGLONG  gl_ulSystemProcessTime;
volatile ULONGLONG	gl_ulRealTimeProcessTime10MS;
volatile ULONGLONG	gl_ulRealTimeProcessTime100MS;
volatile ULONGLONG	gl_ulRealTimeProcessTime1Second;
volatile ULONGLONG	gl_ulRealTimeProcessTime1Minute;
volatile ULONGLONG	gl_ulRealTimeProcessTime1Hour;
volatile ULONGLONG  gl_ulInternalSystemTimeShowPeriod = 60;

bool			gl_fShowMessage;		// 需要在状态栏中显示消息标志
ULONG			gl_ulMessageID;			// 需显示的标志ID
CString		gl_strMessage;			// 需显示的信息。

ULONG           gl_ulCountShowStatus = 1;		// show object or unit's status 1 second per time

CFont  					gl_fontSmall;

CSize						gl_sizeDoc(1280, 1024); // default view and show document's size
COLORREF				gl_clrBkGrd = RGB(255, 255, 255);	// default back ground color is white

