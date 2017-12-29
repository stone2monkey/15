
// ClntDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Clnt.h"
#include "ClntDlg.h"
#include "afxdialogex.h"
#include "_Global.h"
#include "Signup.h"
#include "Signin.h"
#include "Online.h"
#include "Offline.h"
#include "AddFriend.h"
#include "MultiChat.h"
#include "SingleChat.h"
#include "Telecontrol.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClntDlg �Ի���



CClntDlg::CClntDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLNT_DIALOG, pParent)
	, m_wstrIP(_T(""))
	, m_uPort(0)
	, m_wstrShowMsg(_T(""))
	, m_uPW(0)
	, m_uQQ(0)
	, m_uAddSeek(0)
	, m_wstrMsgSent(_T(""))
	, m_uQQOfRecvMsg(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClntDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDEDT_IP, m_wstrIP);
	DDX_Text(pDX, IDEDT_PORT, m_uPort);
	DDX_Text(pDX, IDEDT_SHOWMSG, m_wstrShowMsg);
	DDX_Text(pDX, IDEDT_QQ, m_uQQ);
	DDX_Text(pDX, IDEDT_PW, m_uPW);
	DDX_Text(pDX, IDC_EDIT3, m_uAddSeek);
	DDX_Control(pDX, IDLC_ONLINEQQ, m_lcOnlineQQ);
	DDX_Text(pDX, IDEDT_MSGSENT, m_wstrMsgSent);
	DDX_Text(pDX, IDEDT_QQOFRECVMSG, m_uQQOfRecvMsg);
}

BEGIN_MESSAGE_MAP(CClntDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDBTN_CONNSERV, &CClntDlg::OnBnClickedConnserv)
	ON_BN_CLICKED(IDBTN_LOGUP, &CClntDlg::OnBnClickedLogup)
	ON_BN_CLICKED(IDBTN_LOGIN, &CClntDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDBTN_ADD, &CClntDlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDBTN_SEEK, &CClntDlg::OnBnClickedSeek)
	ON_BN_CLICKED(IDBTN_SENDMULTICHAT, &CClntDlg::OnBnClickedSendmultichat)
	ON_BN_CLICKED(IDBTN_SENDSINGLECHAT, &CClntDlg::OnBnClickedSendsinglechat)
END_MESSAGE_MAP()


// CClntDlg ��Ϣ�������

BOOL CClntDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	{	// ��ʼ���Ի��� 
		// 0. ���ӷ��������
		SetDlgItemTextW(IDEDT_IP, L"192.168.1.54");
		SetDlgItemTextW(IDEDT_PORT, L"5555");

		// 1. �б�ؼ�
		CRect rect;
		m_lcOnlineQQ.GetClientRect(&rect);
		m_lcOnlineQQ.SetExtendedStyle(m_lcOnlineQQ.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_lcOnlineQQ.InsertColumn(0, L"QQ", LVCFMT_CENTER, 120);
		m_lcOnlineQQ.InsertColumn(1, L"����?", LVCFMT_CENTER, rect.Width() - 120);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClntDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClntDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClntDlg::OnBnClickedConnserv()
{
	if (0 != m_oMySocket.m_sockConn) { return; }

	UpdateData();
	CConnServ oConnServ(m_wstrIP, m_uPort, m_oMySocket);
	int iRes = oConnServ.ConnServ();
	if (0 == iRes)		// ���ӷ������ɹ�
	{
		// 0. �������Ӧ
		{	
			// ��Ϣ��¼
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(m_oMySocket.m_addrServ, wstrAddr);
			CString wstr = wstrTime + wstrAddr;
			wstr += L"���ӷ������ɹ�!\r\n";
			m_wstrShowMsg += wstr;
			SetDlgItemTextW(IDEDT_SHOWMSG, m_wstrShowMsg);
			// �������Ӧ
			SetDlgItemTextW(IDBTN_CONNSERV, L"���ӷ������ɹ�!");
			GetDlgItem(IDBTN_CONNSERV)->EnableWindow(0);
			GetDlgItem(IDEDT_IP)->EnableWindow(0);
			GetDlgItem(IDEDT_PORT)->EnableWindow(0);
            GetDlgItem(IDEDT_QQ)->EnableWindow(1);
            GetDlgItem(IDEDT_PW)->EnableWindow(1);
            GetDlgItem(IDBTN_LOGUP)->EnableWindow(1);
            GetDlgItem(IDBTN_LOGIN)->EnableWindow(1);
		}

		// 1. �߳�LoopAccept();
		{	
			SParamToThread * pParamToThread = new SParamToThread();
			pParamToThread->pClntDlg = this;
			pParamToThread->pMySocket = &m_oMySocket;
			::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CClntDlg::LoopRecv, pParamToThread, 0, 0);
		}
	}
	else                // ���ӷ�����ʧ��
	{
		CString wstr;
		FillCurrentTime(wstr);
		CString temp;
		temp.Format(L"���ӷ�����ʧ��!ʧ����%d!", iRes);
		wstr += temp;

		m_oMySocket.m_sockConn = 0;

		MessageBoxW(wstr);
	}
}


void CClntDlg::OnBnClickedLogup()
{
	UpdateData();

	// ����ע��Ϊ0��QQ
	if (0 == m_uQQ) { MessageBoxW(L"����ע��Ϊ0��QQ!"); return; }

	CSignup oSignup;
	SParamToThread sParamToThread;
	sParamToThread.pClntDlg = this;
	sParamToThread.pMySocket = &m_oMySocket;
	oSignup.SendSignupMsg(m_uQQ, m_uPW, &sParamToThread);
}


void CClntDlg::LoopRecv(SParamToThread * pParamToThread)
{
	// �õ�Ҫ�õ�������
	CClntDlg* pClntDlg = pParamToThread->pClntDlg;
	CMySocket* pMySocket = pParamToThread->pMySocket;

	// ѭ��
	while (true)
	{
		// 0. ������Ϣ
		// 1. ������Ϣ
		// 2. ������Ϣ

		// 0. ������Ϣ
		char strMsgRecv[10240] = {};
		int iRes = pMySocket->Recv(pMySocket->m_sockConn, strMsgRecv, sizeof(strMsgRecv));
		if (SOCKET_ERROR == iRes || 0 == iRes)	// �Ͽ�����
		{
			pClntDlg->MessageBoxW(L"��������Ͽ�����!");

			return;
		}

		// 1. ������Ϣ
		SMsg* pMsg = (SMsg*)strMsgRecv;

        // 2. ������Ϣ
        switch (pMsg->uMsgCode)
        {
                // �ͻ���ע����Ϣ��
            case C_SIGNUP_MSGCODE:
                {
                    CSignup oSignup;
                    oSignup.RespondSignupMsg(strMsgRecv, pParamToThread);
                }break;
                // �ͻ��˵�½��Ϣ��
            case C_SIGNIN_MSGCODE:
                {
                    CSignin oSignin;
                    oSignin.RespondSigninMsg(strMsgRecv, pParamToThread);
                }break;
                // �ͻ���������Ϣ��
            case S_ONLINE_MSGCODE:
                {
                    COnline oOnline;
                    oOnline.RespondOnlineMsg(strMsgRecv, pParamToThread);
                }break;
                // �ͻ���������Ϣ��
            case S_OFFLINE_MSGCODE:
                {
                    COffline oOffline;
                    oOffline.RespondOfflineMsg(strMsgRecv, pParamToThread);
                }break;
                // �ͻ�����Ӻ�����Ϣ��
            case C_ADDFRIEND_MSGCODE:
                {
                    CAddFriend oAddFriend;
                    oAddFriend.RespondAddFriendMsg(strMsgRecv, pParamToThread);
                }break;
                // �ͻ���Ⱥ����Ϣ��
            case C_MUILTCHAT_MSGCODE:
                {
                    CMultiChat oMultiChat;
                    oMultiChat.RespondMultiChatMsg(strMsgRecv, pParamToThread);
                }break;
                // �ͻ��˵�����Ϣ��
            case C_SINGLECHAT_MSGCODE:
                {
                    CSingleChat oSingleChat;
                    oSingleChat.RespondSingleChatMsg(strMsgRecv, pParamToThread);
                }break;
                // �����Զ����Ϣ��
            case S_TELECONTROL_MSGCODE:
                {
                    CTelecontrol oTelecontrol;
                    oTelecontrol.RespondTelecontrolMsg(strMsgRecv, pParamToThread);
                }break;
            default:
                break;
        }
	}
}

void CClntDlg::OnBnClickedLogin()
{
	UpdateData();

	CSignin oSignin;
	SParamToThread sParamToThread;
	sParamToThread.pClntDlg = this;
	sParamToThread.pMySocket = &m_oMySocket;
	oSignin.SendSigninMsg(m_uQQ, m_uPW, &sParamToThread);
}


void CClntDlg::OnBnClickedAdd()
{
	UpdateData();

	// 0.
	if (0 == m_uAddSeek) { MessageBoxW(L"����QQ!"); return; }
	if (g_uQQ == m_uAddSeek) { MessageBoxW(L"��������Լ�!"); return; }
	int iLoop = (int)g_vectFriends.size();
	for (int i = 0; i < iLoop; i++)
	{
		if (m_uAddSeek == g_vectFriends.at(i)) { MessageBoxW(L"�Ѿ�����ĺ���,��Ҫ�ظ����!"); return; }
	}

	// ������Ӻ�����Ϣ
	CAddFriend oAddFriend;
	SParamToThread sParamToThread;
	sParamToThread.pClntDlg = this;
	sParamToThread.pMySocket = &m_oMySocket;
	oAddFriend.SendAddFriendMsg(m_uAddSeek, &sParamToThread);
}


void CClntDlg::OnBnClickedSeek()
{
	UpdateData();

	// 0.
	if (0 == m_uAddSeek) { return; }
	int iLoop = (int)g_vectFriends.size();
	for (int i = 0; i < iLoop; i++)
	{
		CString wstrQQ;
		FillQQ(m_uAddSeek, wstrQQ);
		wstrQQ += L"����ĺ���!";
		if (m_uAddSeek == g_vectFriends.at(i)) { MessageBoxW(wstrQQ); }
	}
}


void CClntDlg::OnOK()
{
}

void CClntDlg::UpdateOnlineQQListCtrl()
{
	// 0. ����QQ�б� 
	{
		m_lcOnlineQQ.DeleteAllItems();

		int iLoop = (int)g_vectFriends.size();
		int jLoop = (int)g_vectOnlineFriends.size();
		for (int i = 0; i < iLoop; i++)
		{
			// ���к���QQ���б�
			CString wstrQQ;
			FillQQ(g_vectFriends.at(i), wstrQQ);
			m_lcOnlineQQ.InsertItem(i, wstrQQ);

			// ���QQ����, �����"Online"
			for (int j = 0; j < jLoop; j++)
			{
				if (g_vectFriends.at(i) == g_vectOnlineFriends.at(j))		// ����QQ����,�����"Online"
				{
					CString wstrOnline = L"Online";
					m_lcOnlineQQ.SetItemText(i, 1, wstrOnline);
					break;
				}
			}
		}

		//int iLoop = g_vectOnlineFriends.size();
		//for (int i = 0; i < iLoop; i++)
		//{
		//	CString wstrQQ;
		//	FillQQ(g_vectOnlineFriends.at(i), wstrQQ);
		//	CString wstrOnline = L"Online";

		//	m_lcOnlineQQ.InsertItem(i, wstrQQ);
		//	m_lcOnlineQQ.SetItemText(i, 1, wstrOnline);
		//}
	}
}


void CClntDlg::OnBnClickedSendmultichat()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CMultiChat oMultiChat;
	SParamToThread sParamToThread;
	sParamToThread.pClntDlg = this;
	sParamToThread.pMySocket = &m_oMySocket;
	oMultiChat.SendMultiChatMsg(&sParamToThread);
}


void CClntDlg::OnBnClickedSendsinglechat()
{
	CSingleChat oSingleChat;
	SParamToThread sParamToThread;
	sParamToThread.pClntDlg = this;
	sParamToThread.pMySocket = &m_oMySocket;
	oSingleChat.SendSingleChatMsg(&sParamToThread);
}
