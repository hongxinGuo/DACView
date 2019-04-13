@echo off
REM -- First make map file from App Studio generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by DACVIEW.HPJ. >hlp\dacview.hm
echo. >>hlp\dacview.hm
echo // Commands (ID_* and IDM_*) >>hlp\dacview.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\dacview.hm
echo. >>hlp\dacview.hm
echo // Prompts (IDP_*) >>hlp\dacview.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\dacview.hm
echo. >>hlp\dacview.hm
echo // Resources (IDR_*) >>hlp\dacview.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\dacview.hm
echo. >>hlp\dacview.hm
echo // Dialogs (IDD_*) >>hlp\dacview.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\dacview.hm
echo. >>hlp\dacview.hm
echo // Frame Controls (IDW_*) >>hlp\dacview.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\dacview.hm
REM -- Make help for Project DACVIEW
echo.
