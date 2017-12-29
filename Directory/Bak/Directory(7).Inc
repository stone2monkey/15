; 联系人信息结构体
SContInfo struct
	strTelphone byte 12 dup(0)	;手机号
	strName byte 20 dup(0)	;姓名
SContInfo ends

.data
g_arrContInfo SContInfo 100 dup(<'0'>)	; 联系人数组
g_tempContInfo SContInfo <'0'>		; 临时联系人
g_iMaxCount dword 100		; 最大联系人数量 
g_iCurCount dword 0			; 当前联系人数量






.data?
g_bCharInputed byte 100 dup (?)	; 主菜单时被输入的字符
g_hWnd HWND ?			; 窗口句柄

; --------------------


.const
;主菜单字符串
g_strMenu byte "	1. Add		5. ShowAll		",0dh,0ah,
"	2. Delete	6. Exit		",0dh,0ah,
"	3. Seek				",0dh,0ah,
"	4. Alter				",0dh,0ah,0
g_strFmt0 byte "%c",0 
g_strFmt1 byte "%s",0 
g_strFmt2 byte "%d. 	%s - %s",0dh,0ah,0	;输出所有联系人时的FMT
g_strTips0 byte "请输入对应的数字进入相应的功能 : ",0
g_strClassName byte "ConsoleWindowClass",0
g_strWndName byte "C:\MyProgramFiles\RadASM\Masm\Projects\Directory\Directory.exe",0
g_strClearScreen byte "cls",0
g_strPause byte "pause",0
g_strNullCaption byte " ",0
g_strText0 byte "通讯录里没有联系人,不能查找!即将返回主菜单!",0
g_strText1 byte "请输入对应数字选择查找方式 (0.手机号  1.姓名 other: 输入有问题,返回菜单!) : ",0
g_strText2 byte "请输入要查找的手机号 : ",0
g_strText3 byte "通讯录里联系人已满,不能添加!即将返回主菜单!",0
g_strText4 byte "请输入手机号 (11位) : ",0
g_strText5 byte "请输入姓名 (19位内) : ",0
g_strText6 byte "添加成功!即将返回主菜单! ",0
g_strText7 byte "通讯录里找不到该手机号!即将返回主菜单!",0
g_strText8 byte "通讯录里找到该手机号!即将返回主菜单!",0
g_strText9 byte "请输入要查找的姓名 : ",0
g_strText10 byte "通讯录里找不到该姓名!即将返回主菜单!",0
g_strText11 byte "通讯录里找到该姓名!即将返回主菜单!",0
g_strText12 byte "输入有问题!即将返回主菜单!",0
g_strText13 byte "没有找到要删除的联系人!即将返回主菜单!",0

; function prototype declaration
_Init		proto
_Main		proto
_Add		proto
_Seek		proto
_Alter		proto
_Delete		proto
_ShowAll		proto
_Exit		proto
; ---------------------------------
_ClearScreen	proto
_ShowMenu	proto
_GetCharInputed	proto
_CmpStr		proto str0:dword, str1:dword 
_CmpMultiStr	proto str0:dword, MultiStr1:dword, iCmpFrequency:dword, iOffset:dword