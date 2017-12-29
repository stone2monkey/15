.386
.model flat, stdcall
option casemap:none

include windows.inc
include user32.inc
include kernel32.inc
include msvcrt.inc
include Directory.inc

includelib user32.lib
includelib kernel32.lib
includelib msvcrt.lib






.code
_Init		proc
	; 得到控制台句柄
	;invoke FindWindow,offset g_strWndClassName,NULL
	push	offset g_strWndName
	push	NULL
	call	FindWindow	
	mov	g_hWnd, eax
	;invoke MessageBox, g_hWnd, offset g_strFmt0, offset g_strFmt0, MB_OK
	
	ret
_Init		endp
_Main		proc
	call	_Init
	
	@Loop:
	
	call	_ClearScreen
	call	_ShowMenu
	call	_GetCharInputed
	
	cmp	g_bCharInputed, '1'
	je	@_Add
	
	cmp	g_bCharInputed, '2'
	je	@_Delete
	
	cmp	g_bCharInputed, '3'
	je	@_Seek
	
	cmp	g_bCharInputed, '4'
	je	@_Alter
	
	cmp	g_bCharInputed, '5'
	je	@_ShowAll
	
	cmp	g_bCharInputed, '6'
	je	@_Exit
	
	@_Add:
	call	_Add
	jmp	@Loop
	
	@_Delete:
	call	_Delete
	jmp	@Loop
	
	@_Seek:
	call	_Seek
	jmp	@Loop
	
	@_Alter:
	call	_Alter
	jmp	@Loop
	
	@_ShowAll:
	call	_ShowAll
	jmp	@Loop
	
	@_Exit:
	call	_Exit
	jmp	@Loop
	
	ret
_Main		endp
_Add		proc
	; 0. 联系人是否已满
	; 1. 添加 
		; 1.0 请输入手机号 (11位) :
		; 1.1 请输入姓名 (19位内) : 
		; 1.2 输出添加成功	
	; 2. g_iCurCount++
	; 3. 返回主菜单
	
	; 0. 联系人是否已满
	cmp	g_iCurCount, 100
	jz	@FullContPerson
	
	; 1. 添加 
	; 清屏
	call	_ClearScreen
	;定位
	imul	eax, g_iCurCount, sizeof SContInfo
	lea	ebp, g_arrContInfo
	add	ebp, eax
	; 1.0 请输入手机号 (11位) :
	push	offset g_strText4
	call	crt_printf		
	add	esp, 4
	; 得到输入
	push	ebp
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	; 1.1 请输入姓名 (19位内) : 
	push	offset g_strText5
	call	crt_printf		
	add	esp, 4
	; 得到输入
	add	ebp, 12
	push	ebp
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	; 1.2 输出添加成功	
	push	offset g_strText6
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	
	; 2. g_iCurCount++
	inc 	g_iCurCount
	
	; 3. 返回主菜单
	ret
	
	@FullContPerson:
	push 	MB_ICONERROR
	push	offset g_strNullCaption
	push	offset g_strText3
	push	g_hWnd
	call	MessageBox
	ret
_Add		endp
_Delete		proc
	LOCAL	@iIndex:dword
	mov	@iIndex, -1

	; 0. 没有联系人
	; 1. 得到联系人index
	; 2. 删除最后一个
	; 3. 删除其它
	
	
	
	; 0. 没有联系人
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. 得到联系人index
	call	_Seek
	mov	@iIndex, eax
	cmp	@iIndex, -1
	jz	@NotFoundContPerson	; 没有找到联系人
	mov	eax, g_iCurCount
	dec	eax
	cmp	eax, @iIndex		
	jz	@LastContPerson		; 删除最后一个联系人
	; 删除其它位置联系人
	; memcpy
	imul	eax, @iIndex, sizeof SContInfo
	lea	edi, g_arrContInfo
	add	edi, eax
	mov	esi, edi
	add	esi, sizeof SContInfo
	mov	eax, g_iCurCount
	sub	eax, @iIndex
	dec	eax
	imul	eax, sizeof SContInfo
	push	eax
	push	esi
	push	edi
	call	crt_memcpy
	add	esp, 0ch
	;
	dec	g_iCurCount
	push	offset g_strText14
	call	crt_printf			; "删除联系人成功!即将返回主菜单!"	
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	
	
	
	
	@NoContPerson:
	;invoke MessageBox, g_hWnd, offset g_strText0, offset g_strNullCaption,MB_ICONERROR
	push 	MB_ICONERROR
	push	offset g_strNullCaption
	push	offset g_strText0
	push	g_hWnd
	call	MessageBox
	ret
	
	@NotFoundContPerson:		; "没有找到要删除的联系人!即将返回主菜单!",
	push	offset g_strText13
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	@LastContPerson:			; 删除最后一个联系人
	dec	g_iCurCount
	push	offset g_strText14
	call	crt_printf			; "删除联系人成功!即将返回主菜单!"	
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
_Delete		endp
_Seek		proc
	; 0. 是否有联系人
	; 1. 查找
		; 1.0 查找方式
			; 1.0.0 手机号查找 
			; 1.0.1 姓名查找
	; 2. 输入有问题,返回菜单
	
	; 0. 是否有联系人
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. 查找
	; 清屏
	call	_ClearScreen	
	; 请输入对应数字选择查找方式 (0.手机号  1.姓名) : 
	push	offset g_strText1
	call	crt_printf		
	add	esp, 4
	; 得到输入
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	; 1.0 查找方式
	cmp	g_bCharInputed, '0'
	jz	@SeekViaTelphone		; 1.0.0 手机号查找 
	cmp	g_bCharInputed, '1'
	jz	@SeekViaName		; 1.0.1 姓名查找
	
	; 2. 输入有问题,返回菜单
	push	offset g_strText12
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	@NoContPerson:
	;invoke MessageBox, g_hWnd, offset g_strText0, offset g_strNullCaption,MB_ICONERROR
	push 	MB_ICONERROR
	push	offset g_strNullCaption
	push	offset g_strText0
	push	g_hWnd
	call	MessageBox
	ret
	
	; 1.0.0 手机号查找 
	@SeekViaTelphone:
	; 清屏
	call	_ClearScreen
	; 请输入要查找的手机号 : 
	push	offset g_strText2
	call	crt_printf		
	add	esp, 4
	; 得到输入
	push	offset g_tempContInfo.strTelphone
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	;与数组比较
	push 	sizeof SContInfo
	push	g_iCurCount
	push	offset g_arrContInfo
	push	offset g_tempContInfo.strTelphone
	call	_CmpMultiStr
	cmp	eax, -1	; jz 没有找到
	jnz	@FoundViaTelphone	; jnz 找到
	; 没有找到
	; 输出	"通讯录里找不到该手机号!即将返回主菜单!",0
	push	offset g_strText7
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	
	; 1.0.1 姓名查找
	@SeekViaName:
	; 清屏
	call	_ClearScreen
	; 请输入要查找的姓名 : 
	push	offset g_strText9
	call	crt_printf		
	add	esp, 4
	; 得到输入
	push	offset g_tempContInfo.strName
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	;与数组比较
	push 	sizeof SContInfo
	push	g_iCurCount
	lea 	eax, offset g_arrContInfo
	add	eax, 12
	push	eax
	push	offset g_tempContInfo.strName
	call	_CmpMultiStr
	cmp	eax, -1	; jz 没有找到
	jnz	@FoundViaName	; jnz 找到
	; 没有找到
	; 输出	"通讯录里找不到该姓名!即将返回主菜单!",0
	push	offset g_strText10
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	@FoundViaTelphone:
	; 输出	"通讯录里找到该手机号!即将返回主菜单!",0
	push	eax
	push	offset g_strText8
	call	crt_printf		
	add	esp, 4
	pop	eax
	; 暂停
	push	eax
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	pop	eax
	ret
	
	@FoundViaName:
	; 输出	"通讯录里找到该姓名!即将返回主菜单!",0
	push	eax
	push	offset g_strText11
	call	crt_printf		
	add	esp, 4
	pop	eax
	; 暂停
	push	eax
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	pop	eax
	ret
	
	
_Seek		endp
_Alter		proc

	LOCAL	@iIndex:dword
	mov	@iIndex, -1
	
	; 0. 没有联系人
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. 得到联系人index
	call	_Seek
	mov	@iIndex, eax
	cmp	@iIndex, -1
	jz	@NotFoundContPerson	; 没有找到联系人
	
	; 2. 
	; 请输入对应数字选择修改方式 (0.手机号  1.姓名) : 
	push	offset g_strText15
	call	crt_printf		
	add	esp, 4
	; 得到输入
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	cmp	g_bCharInputed, '0'
	jz	@AlterViaTelphone
	cmp	g_bCharInputed, '1'
	jz	@AlterViaName
	
	; 3. 输入有问题,返回菜单
	push	offset g_strText12
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	
	
	@AlterViaTelphone:
	; 请输入要替换成的手机号 : 
	push	offset g_strText16
	call	crt_printf		
	add	esp, 4
	; 得到输入
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	;替换
	lea	eax, g_bCharInputed
	push	eax
	call	crt_strlen		; eax保存长度
	add	esp, 4
	inc	eax		; eax++,加上最后00
	lea	esi, g_bCharInputed
	lea	ebx, g_arrContInfo
	imul	edi, @iIndex, sizeof SContInfo
	add	edi, ebx
	push	eax
	push	esi
	push	edi
	call	crt_memcpy
	add	esp, 0ch
	; 替换手机号成功! 
	push	offset g_strText17
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	@AlterViaName:
	; 请输入要替换成的姓名 : 
	push	offset g_strText18
	call	crt_printf		
	add	esp, 4
	; 得到输入
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	;替换
	lea	eax, g_bCharInputed
	push	eax
	call	crt_strlen		; eax保存长度
	add	esp, 4
	inc	eax		; eax++,加上最后00
	lea	esi, g_bCharInputed
	lea	ebx, g_arrContInfo
	imul	edi, @iIndex, sizeof SContInfo
	add	edi, 0ch 
	add	edi, ebx
	push	eax
	push	esi
	push	edi
	call	crt_memcpy
	add	esp, 0ch
	; 替换姓名成功! 
	push	offset g_strText19
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	@NoContPerson:
	;invoke MessageBox, g_hWnd, offset g_strText0, offset g_strNullCaption,MB_ICONERROR
	push 	MB_ICONERROR
	push	offset g_strNullCaption
	push	offset g_strText0
	push	g_hWnd
	call	MessageBox
	ret
	
	
	@NotFoundContPerson:		; "没有找到要删除的联系人!即将返回主菜单!",
	push	offset g_strText13
	call	crt_printf		
	add	esp, 4
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
_Alter		endp
_ShowAll		proc

	; 0. 是否有联系人
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. 输出所有联系人		printf("%d. 	%s-%s", ecx, 1, 2)
	; 清屏
	call	_ClearScreen
	; loop
	mov	ecx, g_iCurCount
	@@:
	mov	eax, g_iCurCount
	sub	eax, ecx
	mov	ebx, eax
	imul	eax, sizeof	 SContInfo
	mov	edi, offset g_arrContInfo
	add	edi, eax
	mov	esi, edi
	add	esi, 12
	push	ecx	; 保存ecx,printf会改变它
	push	esi
	push	edi
	push	ebx
	push	offset g_strFmt2
	call	crt_printf
	add	esp, 10h
	pop	ecx	;恢复ecx
	
	loop 	@B
	; 暂停
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	
	@NoContPerson:
	;invoke MessageBox, g_hWnd, offset g_strText0, offset g_strNullCaption,MB_ICONERROR
	push 	MB_ICONERROR
	push	offset g_strNullCaption
	push	offset g_strText0
	push	g_hWnd
	call	MessageBox
	ret
	
_ShowAll		endp
_Exit		proc
	push	NULL
	call	ExitProcess
	ret
_Exit		endp
_ClearScreen	proc
	push	offset g_strClearScreen
	call	crt_system
	add	esp, 4
	ret
_ClearScreen	endp
_ShowMenu	proc
	push	offset g_strMenu
	call	crt_printf
	add	esp, 4
	ret
_ShowMenu	endp
_GetCharInputed	proc
	; tips
	push	offset g_strTips0
	call	crt_printf
	add	esp, 4
	; 得到主菜单中输入的功能字符
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	ret
_GetCharInputed 	endp
_CmpStr		proc str0:dword, str1:dword

	; 得到字符串长度,
	push	str0
	call	crt_strlen
	mov	ecx, eax
	inc	ecx
	add	esp, 4
	; 比较
	mov	esi, str0
	mov	edi, str1
	repe cmpsb
	;结果
	jnz	@NotEqual
	jmp	@Equal
	
	
	@NotEqual:
	mov	eax, 0
	ret
	
	@Equal:
	mov	eax, 1
	ret
	
_CmpStr		endp
_CmpMultiStr	proc str0:dword, MultiStr1:dword, iCmpFrequency:dword, iOffset:dword

	mov	ecx, iCmpFrequency
	@@:
	push	ecx
	;
	push	str0
	mov	eax, iCmpFrequency
	sub	eax, ecx
	imul	eax, sizeof	 SContInfo
	mov	edi, MultiStr1
	add	edi, eax
	push	edi
	call	_CmpStr
	pop	ecx
	cmp	eax, 1	; 
	jz	@F
	loop @B
	
	;没有找到
	mov	eax, -1
	ret
	
	; 找到
	@@:
	mov	eax, iCmpFrequency
	sub	eax, ecx
	ret
	
_CmpMultiStr	endp

start:	; ***************************************************************************************************************************************************************************************

	call	_Main
end start ; *************************************************************************************************************************************************************************************









