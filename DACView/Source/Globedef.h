extern volatile 	ULONGLONG	gl_ulRealTimeProcessTime;
extern volatile 	ULONGLONG gl_ulSystemProcessTime;
extern volatile 	ULONGLONG	gl_ulRealTimeProcessTime10MS;
extern volatile 	ULONGLONG	gl_ulRealTimeProcessTime100MS;
extern volatile 	ULONGLONG	gl_ulRealTimeProcessTime1Second;
extern volatile 	ULONGLONG	gl_ulRealTimeProcessTime1Minute;	
extern volatile 	ULONGLONG	gl_ulRealTimeProcessTime1Hour;
extern volatile 	ULONGLONG gl_ulInternalSystemTimeShowPeriod;

extern ULONG			gl_ulCountShowStatus;

extern CFont  		gl_fontSmall;

extern CSize			gl_sizeDoc;					// default view and show document's size
extern COLORREF		gl_clrBkGrd;				// default back ground color

extern bool				gl_fShowMessage;		// 需要在状态栏中显示消息标志
extern ULONG			gl_ulMessageID;			// 需显示的标志ID
extern CString		gl_strMessage;			// 需显示的信息。

BOOL ShowMessage( UINT uID, ... );  
BOOL ShowMessageIndirect(UINT uID, CString strMessage);


