; ��ϵ����Ϣ�ṹ��
SContInfo struct
	strTelphone byte 12 dup(0)	;�ֻ���
	strName byte 20 dup(0)	;����
SContInfo ends

.data
g_arrContInfo SContInfo 100 dup(<'0'>)	; ��ϵ������
g_tempContInfo SContInfo <'0'>		; ��ʱ��ϵ��
g_iMaxCount dword 100		; �����ϵ������ 
g_iCurCount dword 0			; ��ǰ��ϵ������






.data?
g_bCharInputed byte 100 dup (?)	; ���˵�ʱ��������ַ�
g_hWnd HWND ?			; ���ھ��

; --------------------


.const
;���˵��ַ���
g_strMenu byte "	1. Add		5. ShowAll		",0dh,0ah,
"	2. Delete	6. Exit		",0dh,0ah,
"	3. Seek				",0dh,0ah,
"	4. Alter				",0dh,0ah,0
g_strFmt0 byte "%c",0 
g_strFmt1 byte "%s",0 
g_strFmt2 byte "%d. 	%s - %s",0dh,0ah,0	;���������ϵ��ʱ��FMT
g_strTips0 byte "�������Ӧ�����ֽ�����Ӧ�Ĺ��� : ",0
g_strClassName byte "ConsoleWindowClass",0
g_strWndName byte "C:\MyProgramFiles\RadASM\Masm\Projects\Directory\Directory.exe",0
g_strClearScreen byte "cls",0
g_strPause byte "pause",0
g_strNullCaption byte " ",0
g_strText0 byte "ͨѶ¼��û����ϵ��,���ܲ���!�����������˵�!",0
g_strText1 byte "�������Ӧ����ѡ����ҷ�ʽ (0.�ֻ���  1.���� other: ����������,���ز˵�!) : ",0
g_strText2 byte "������Ҫ���ҵ��ֻ��� : ",0
g_strText3 byte "ͨѶ¼����ϵ������,��������!�����������˵�!",0
g_strText4 byte "�������ֻ��� (11λ) : ",0
g_strText5 byte "���������� (19λ��) : ",0
g_strText6 byte "���ӳɹ�!�����������˵�! ",0
g_strText7 byte "ͨѶ¼���Ҳ������ֻ���!�����������˵�!",0
g_strText8 byte "ͨѶ¼���ҵ����ֻ���!�����������˵�!",0
g_strText9 byte "������Ҫ���ҵ����� : ",0
g_strText10 byte "ͨѶ¼���Ҳ���������!�����������˵�!",0
g_strText11 byte "ͨѶ¼���ҵ�������!�����������˵�!",0
g_strText12 byte "����������!�����������˵�!",0
g_strText13 byte "û���ҵ�Ҫɾ������ϵ��!�����������˵�!",0

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