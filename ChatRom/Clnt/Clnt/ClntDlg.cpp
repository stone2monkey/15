
// ClntDlg.cpp : 实现文件
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


// CClntDlg 对话框



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


// CClntDlg 消息处理程序

BOOL CClntDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	{	// 初始化对话框 
		// 0. 连接服务器相关
		SetDlgItemTextW(IDEDT_IP, L"192.168.1.54");
		SetDlgItemTextW(IDEDT_PORT, L"5555");

		// 1. 列表控件
		CRect rect;
		m_lcOnlineQQ.GetClientRect(&rect);
		m_lcOnlineQQ.SetExtendedStyle(m_lcOnlineQQ.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		m_lcOnlineQQ.InsertColumn(0, L"QQ", LVCFMT_CENTER, 120);
		m_lcOnlineQQ.InsertColumn(1, L"在线?", LVCFMT_CENTER, rect.Width() - 120);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClntDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	if (0 == iRes)		// 连接服务器成功
	{
		// 0. 界面的响应
		{	
			// 消息记录
			CString wstrTime;
			FillCurrentTime(wstrTime);
			CString wstrAddr;
			FillAddrClnt(m_oMySocket.m_addrServ, wstrAddr);
			CString wstr = wstrTime + wstrAddr;
			wstr += L"连接服务器成功!\r\n";
			m_wstrShowMsg += wstr;
			SetDlgItemTextW(IDEDT_SHOWMSG, m_wstrShowMsg);
			// 界面的响应
			SetDlgItemTextW(IDBTN_CONNSERV, L"连接服务器成功!");
			GetDlgItem(IDBTN_CONNSERV)->EnableWindow(0);
			GetDlgItem(IDEDT_IP)->EnableWindow(0);
			GetDlgItem(IDEDT_PORT)->EnableWindow(0);
            GetDlgItem(IDEDT_QQ)->EnableWindow(1);
            GetDlgItem(IDEDT_PW)->EnableWindow(1);
            GetDlgItem(IDBTN_LOGUP)->EnableWindow(1);
            GetDlgItem(IDBTN_LOGIN)->EnableWindow(1);
		}

		// 1. 线程LoopAccept();
		{	
			SParamToThread * pParamToThread = new SParamToThread();
			pParamToThread->pClntDlg = this;
			pParamToThread->pMySocket = &m_oMySocket;
			::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CClntDlg::LoopRecv, pParamToThread, 0, 0);
		}
	}
	else                // 连接服务器失败
	{
		CString wstr;
		FillCurrentTime(wstr);
		CString temp;
		temp.Format(L"连接服务器失败!失败码%d!", iRes);
		wstr += temp;

		m_oMySocket.m_sockConn = 0;

		MessageBoxW(wstr);
	}
}


void CClntDlg::OnBnClickedLogup()
{
	UpdateData();

	// 不能注册为0的QQ
	if (0 == m_uQQ) { MessageBoxW(L"不能注册为0的QQ!"); return; }

	CSignup oSignup;
	SParamToThread sParamToThread;
	sParamToThread.pClntDlg = this;
	sParamToThread.pMySocket = &m_oMySocket;
	oSignup.SendSignupMsg(m_uQQ, m_uPW, &sParamToThread);
}


void CClntDlg::LoopRecv(SParamToThread * pParamToThread)
{
	// 得到要用到的数据
	CClntDlg* pClntDlg = pParamToThread->pClntDlg;
	CMySocket* pMySocket = pParamToThread->pMySocket;

	// 循环
	while (true)
	{
		// 0. 接收消息
		// 1. 解析消息
		// 2. 处理消息

		// 0. 接收消息
		char strMsgRecv[10240] = {};
		int iRes = pMySocket->Recv(pMySocket->m_sockConn, strMsgRecv, sizeof(strMsgRecv));
		if (SOCKET_ERROR == iRes || 0 == iRes)	// 断开连接
		{
			pClntDlg->MessageBoxW(L"与服务器断开连接!");

			return;
		}

		// 1. 解析消息
		SMsg* pMsg = (SMsg*)strMsgRecv;

        // 2. 处理消息
        switch (pMsg->uMsgCode)
        {
                // 客户端注册消息码
            case C_SIGNUP_MSGCODE:
                {
                    CSignup oSignup;
                    oSignup.RespondSignupMsg(strMsgRecv, pParamToThread);
                }break;
                // 客户端登陆消息码
            case C_SIGNIN_MSGCODE:
                {
                    CSignin oSignin;
                    oSignin.RespondSigninMsg(strMsgRecv, pParamToThread);
                }break;
                // 客户端上线消息码
            case S_ONLINE_MSGCODE:
                {
                    COnline oOnline;
                    oOnline.RespondOnlineMsg(strMsgRecv, pParamToThread);
                }break;
                // 客户端下线消息码
            case S_OFFLINE_MSGCODE:
                {
                    COffline oOffline;
                    oOffline.RespondOfflineMsg(strMsgRecv, pParamToThread);
                }break;
                // 客户端添加好友消息码
            case C_ADDFRIEND_MSGCODE:
                {
                    CAddFriend oAddFriend;
                    oAddFriend.RespondAddFriendMsg(strMsgRecv, pParamToThread);
                }break;
                // 客户端群聊消息码
            case C_MUILTCHAT_MSGCODE:
                {
                    CMultiChat oMultiChat;
                    oMultiChat.RespondMultiChatMsg(strMsgRecv, pParamToThread);
                }break;
                // 客户端单聊消息码
            case C_SINGLECHAT_MSGCODE:
                {
                    CSingleChat oSingleChat;
                    oSingleChat.RespondSingleChatMsg(strMsgRecv, pParamToThread);
                }break;
                // 服务端远程消息码
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
	if (0 == m_uAddSeek) { MessageBoxW(L"错误QQ!"); return; }
	if (g_uQQ == m_uAddSeek) { MessageBoxW(L"不能添加自己!"); return; }
	int iLoop = (int)g_vectFriends.size();
	for (int i = 0; i < iLoop; i++)
	{
		if (m_uAddSeek == g_vectFriends.at(i)) { MessageBoxW(L"已经是你的好友,不要重复添加!"); return; }
	}

	// 发送添加好友消息
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
		wstrQQ += L"是你的好友!";
		if (m_uAddSeek == g_vectFriends.at(i)) { MessageBoxW(wstrQQ); }
	}
}


void CClntDlg::OnOK()
{
}

void CClntDlg::UpdateOnlineQQListCtrl()
{
	// 0. 更新QQ列表 
	{
		m_lcOnlineQQ.DeleteAllItems();

		int iLoop = (int)g_vectFriends.size();
		int jLoop = (int)g_vectOnlineFriends.size();
		for (int i = 0; i < iLoop; i++)
		{
			// 所有好友QQ上列表
			CString wstrQQ;
			FillQQ(g_vectFriends.at(i), wstrQQ);
			m_lcOnlineQQ.InsertItem(i, wstrQQ);

			// 如果QQ在线, 后面加"Online"
			for (int j = 0; j < jLoop; j++)
			{
				if (g_vectFriends.at(i) == g_vectOnlineFriends.at(j))		// 好友QQ在线,后面加"Online"
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
	// TODO: 在此添加控件通知处理程序代码

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
