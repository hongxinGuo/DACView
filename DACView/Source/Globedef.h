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

extern bool				gl_fShowMessage;		// ��Ҫ��״̬������ʾ��Ϣ��־
extern ULONG			gl_ulMessageID;			// ����ʾ�ı�־ID
extern CString		gl_strMessage;			// ����ʾ����Ϣ��

BOOL ShowMessage( UINT uID, ... );  
BOOL ShowMessageIndirect(UINT uID, CString strMessage);


