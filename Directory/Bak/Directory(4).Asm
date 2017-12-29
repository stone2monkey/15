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
	; �õ�����̨���
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
	; 0. ��ϵ���Ƿ�����
	; 1. ���� 
		; 1.0 �������ֻ��� (11λ) :
		; 1.1 ���������� (19λ��) : 
		; 1.2 ������ӳɹ�	
	; 2. g_iCurCount++
	; 3. �������˵�
	
	; 0. ��ϵ���Ƿ�����
	cmp	g_iCurCount, 100
	jz	@FullContPerson
	
	; 1. ���� 
	; ����
	call	_ClearScreen
	;��λ
	imul	eax, g_iCurCount, sizeof SContInfo
	lea	ebp, g_arrContInfo
	add	ebp, eax
	; 1.0 �������ֻ��� (11λ) :
	push	offset g_strText4
	call	crt_printf		
	add	esp, 4
	; �õ�����
	push	ebp
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	; 1.1 ���������� (19λ��) : 
	push	offset g_strText5
	call	crt_printf		
	add	esp, 4
	; �õ�����
	add	ebp, 12
	push	ebp
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	; 1.2 ������ӳɹ�	
	push	offset g_strText6
	call	crt_printf		
	add	esp, 4
	; ��ͣ
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	
	; 2. g_iCurCount++
	inc 	g_iCurCount
	
	; 3. �������˵�
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

	; 0. û����ϵ��
	; 1. �õ���ϵ��index
	; 2. ɾ�����һ��
	; 3. ɾ������
	
	
	
	; 0. û����ϵ��
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. �õ���ϵ��index
	call	_Seek
	mov	@iIndex, eax
	cmp	@iIndex, -1
	jz	@NotFoundContPerson	; û���ҵ���ϵ��
	mov	eax, g_iCurCount
	dec	eax
	cmp	eax, @iIndex		
	jz	@LastContPerson		; ɾ�����һ����ϵ��
	; ɾ������λ����ϵ��
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
	call	crt_printf			; "ɾ����ϵ�˳ɹ�!�����������˵�!"	
	add	esp, 4
	; ��ͣ
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
	
	@NotFoundContPerson:		; "û���ҵ�Ҫɾ������ϵ��!�����������˵�!",
	push	offset g_strText13
	call	crt_printf		
	add	esp, 4
	; ��ͣ
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	@LastContPerson:			; ɾ�����һ����ϵ��
	dec	g_iCurCount
	push	offset g_strText14
	call	crt_printf			; "ɾ����ϵ�˳ɹ�!�����������˵�!"	
	add	esp, 4
	; ��ͣ
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
_Delete		endp
_Seek		proc
	; 0. �Ƿ�����ϵ��
	; 1. ����
		; 1.0 ���ҷ�ʽ
			; 1.0.0 �ֻ��Ų��� 
			; 1.0.1 ��������
	; 2. ����������,���ز˵�
	
	; 0. �Ƿ�����ϵ��
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. ����
	; ����
	call	_ClearScreen	
	; �������Ӧ����ѡ����ҷ�ʽ (0.�ֻ���  1.����) : 
	push	offset g_strText1
	call	crt_printf		
	add	esp, 4
	; �õ�����
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	; 1.0 ���ҷ�ʽ
	cmp	g_bCharInputed, '0'
	jz	@SeekViaTelphone		; 1.0.0 �ֻ��Ų��� 
	cmp	g_bCharInputed, '1'
	jz	@SeekViaName		; 1.0.1 ��������
	
	; 2. ����������,���ز˵�
	push	offset g_strText12
	call	crt_printf		
	add	esp, 4
	; ��ͣ
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
	
	; 1.0.0 �ֻ��Ų��� 
	@SeekViaTelphone:
	; ����
	call	_ClearScreen
	; ������Ҫ���ҵ��ֻ��� : 
	push	offset g_strText2
	call	crt_printf		
	add	esp, 4
	; �õ�����
	push	offset g_tempContInfo.strTelphone
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	;������Ƚ�
	push 	sizeof SContInfo
	push	g_iCurCount
	push	offset g_arrContInfo
	push	offset g_tempContInfo.strTelphone
	call	_CmpMultiStr
	cmp	eax, -1	; jz û���ҵ�
	jnz	@FoundViaTelphone	; jnz �ҵ�
	; û���ҵ�
	; ���	"ͨѶ¼���Ҳ������ֻ���!�����������˵�!",0
	push	offset g_strText7
	call	crt_printf		
	add	esp, 4
	; ��ͣ
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	
	; 1.0.1 ��������
	@SeekViaName:
	; ����
	call	_ClearScreen
	; ������Ҫ���ҵ����� : 
	push	offset g_strText9
	call	crt_printf		
	add	esp, 4
	; �õ�����
	push	offset g_tempContInfo.strName
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	;������Ƚ�
	push 	sizeof SContInfo
	push	g_iCurCount
	lea 	eax, offset g_arrContInfo
	add	eax, 12
	push	eax
	push	offset g_tempContInfo.strName
	call	_CmpMultiStr
	cmp	eax, -1	; jz û���ҵ�
	jnz	@FoundViaName	; jnz �ҵ�
	; û���ҵ�
	; ���	"ͨѶ¼���Ҳ���������!�����������˵�!",0
	push	offset g_strText10
	call	crt_printf		
	add	esp, 4
	; ��ͣ
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	@FoundViaTelphone:
	; ���	"ͨѶ¼���ҵ����ֻ���!�����������˵�!",0
	push	eax
	push	offset g_strText8
	call	crt_printf		
	add	esp, 4
	pop	eax
	; ��ͣ
	push	eax
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	pop	eax
	ret
	
	@FoundViaName:
	; ���	"ͨѶ¼���ҵ�������!�����������˵�!",0
	push	eax
	push	offset g_strText11
	call	crt_printf		
	add	esp, 4
	pop	eax
	; ��ͣ
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
	
	; 0. û����ϵ��
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. �õ���ϵ��index
	call	_Seek
	mov	@iIndex, eax
	cmp	@iIndex, -1
	jz	@NotFoundContPerson	; û���ҵ���ϵ��
	
	; 2. 
	; �������Ӧ����ѡ���޸ķ�ʽ (0.�ֻ���  1.����) : 
	push	offset g_strText15
	call	crt_printf		
	add	esp, 4
	; �õ�����
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	cmp	g_bCharInputed, '0'
	jz	@AlterViaTelphone
	cmp	g_bCharInputed, '1'
	jz	@AlterViaName
	
	; 3. ����������,���ز˵�
	push	offset g_strText12
	call	crt_printf		
	add	esp, 4
	; ��ͣ
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
	
	
	@AlterViaTelphone:
	; ������Ҫ�滻�ɵ��ֻ��� : 
	push	offset g_strText16
	call	crt_printf		
	add	esp, 4
	; �õ�����
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	;�滻
	lea	eax, g_bCharInputed
	push	eax
	call	crt_strlen		; eax���泤��
	inc	eax		; eax++,�������00
	lea	esi, g_bCharInputed
	lea	ebx, g_arrContInfo
	imul	edi, @iIndex, sizeof SContInfo
	mov	edi, ebx
	
	@AlterViaName:
	; �������Ӧ����ѡ���޸ķ�ʽ (0.�ֻ���  1.����) : 
	push	offset g_strText15
	call	crt_printf		
	add	esp, 4
	; �õ�����
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	
	@NoContPerson:
	;invoke MessageBox, g_hWnd, offset g_strText0, offset g_strNullCaption,MB_ICONERROR
	push 	MB_ICONERROR
	push	offset g_strNullCaption
	push	offset g_strText0
	push	g_hWnd
	call	MessageBox
	ret
	
	
	@NotFoundContPerson:		; "û���ҵ�Ҫɾ������ϵ��!�����������˵�!",
	push	offset g_strText13
	call	crt_printf		
	add	esp, 4
	; ��ͣ
	push 	offset g_strPause
	call	crt_system
	add	esp, 4
	ret
	
	
_Alter		endp
_ShowAll		proc

	; 0. �Ƿ�����ϵ��
	cmp	g_iCurCount, 0
	jz	@NoContPerson
	
	; 1. ���������ϵ��		printf("%d. 	%s-%s", ecx, 1, 2)
	; ����
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
	push	ecx	; ����ecx,printf��ı���
	push	esi
	push	edi
	push	ebx
	push	offset g_strFmt2
	call	crt_printf
	add	esp, 10h
	pop	ecx	;�ָ�ecx
	
	loop 	@B
	; ��ͣ
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
	; �õ����˵�������Ĺ����ַ�
	push	offset g_bCharInputed
	push	offset g_strFmt1
	call	crt_scanf
	add	esp, 8
	ret
_GetCharInputed 	endp
_CmpStr		proc str0:dword, str1:dword

	; �õ��ַ�������,
	push	str0
	call	crt_strlen
	mov	ecx, eax
	inc	ecx
	add	esp, 4
	; �Ƚ�
	mov	esi, str0
	mov	edi, str1
	repe cmpsb
	;���
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
	
	;û���ҵ�
	mov	eax, -1
	ret
	
	; �ҵ�
	@@:
	mov	eax, iCmpFrequency
	sub	eax, ecx
	ret
	
_CmpMultiStr	endp

start:	; ***************************************************************************************************************************************************************************************

	call	_Main
end start ; *************************************************************************************************************************************************************************************








