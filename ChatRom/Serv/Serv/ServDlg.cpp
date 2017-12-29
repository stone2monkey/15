
// ServDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Serv.h"
#include "ServDlg.h"
#include "afxdialogex.h"
#include "StartupServ.h"
#include "_Global.h"
#include "Signup.h"
#include "Signin.h"
#include "Offline.h"
#include "AddFriend.h"
#include "MultiChat.h"
#include "SingleChat.h"
#include "Telecontrol.h"
#include "DataBaseCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServDlg 对话框



CServDlg::CServDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_SERV_DIALOG, pParent)
    , m_wstrIP(_T(""))
    , m_uPort(0)
    , m_wstrShowMsg(_T(""))
    , m_uTelecontrolQQ(0)
    , m_wstrTelecontrol(_T(""))
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    //  DDX_Text(pDX, IDBTN_IP, m_wstrIp);
    DDX_Text(pDX, IDEDT_IP, m_wstrIP);
    DDX_Text(pDX, IDEDT_PORT, m_uPort);
    DDX_Text(pDX, IDEDT_SHOWMSG, m_wstrShowMsg);
    DDX_Control(pDX, IDLC_ONLINEQQ, m_lcOnlineQQ);
    DDX_Text(pDX, IDBTN_TELECONTROLQQ, m_uTelecontrolQQ);
    DDX_Text(pDX, IDEDT_WSTRTELECONTROL, m_wstrTelecontrol);
}

BEGIN_MESSAGE_MAP(CServDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDBTN_STARTUP, &CServDlg::OnBnClickedStartup)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDBTN_SNEDTELECONTROL, &CServDlg::OnBnClickedSnedtelecontrol)
END_MESSAGE_MAP()


// CServDlg 消息处理程序

BOOL CServDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    // TODO: 在此添加额外的初始化代码

    {	// 初始化对话框 
        // 0. 启动服务器相关
        SetDlgItemTextW(IDEDT_IP, L"192.168.1.54");
        SetDlgItemTextW(IDEDT_PORT, L"5555");

        // 1. 列表控件
        CRect rect;
        m_lcOnlineQQ.GetClientRect(&rect);
        m_lcOnlineQQ.SetExtendedStyle(m_lcOnlineQQ.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        m_lcOnlineQQ.InsertColumn(0, L"QQ", LVCFMT_CENTER, 120);
        m_lcOnlineQQ.InsertColumn(1, L"Addr", LVCFMT_CENTER, rect.Width() - 120);

        // 2. 初始化全局QQ
        {
            // 0. 得到数据库数据
            _RESULT re;
            CDataBaseCtrl oDataBaseCtrl;
            oDataBaseCtrl.LinkDataBase("0", "0", "0", "0");
            char str[0x400] = "select * from t_qqsignupped;";
            oDataBaseCtrl.ExecSqlByRecordSet(re, str);

            // 1. 把数据写入全局已注册QQ
            int iLoop = (int)re.vec_Data.size();
            for (int i = 0; i < iLoop; i++)
            {
                // 用临时变量得到QQ PW FRIENDS 
                SQQSinguppedInfo sQQSinguppedInfo;
                sQQSinguppedInfo.uQQ = ((UINT*)(&re.vec_Data.at(i).at(0)))[0];
                sQQSinguppedInfo.uPW = ((UINT*)(&re.vec_Data.at(i).at(0)))[1];
                for (int j = 0; j < 50; j++)
                {
                    UINT uQQFriend = ((UINT*)(&re.vec_Data.at(i).at(0)))[2 + j];
                    if (0 != uQQFriend)
                    {
                        sQQSinguppedInfo.vectFriends.push_back(uQQFriend);
                    }
                }
                // 压入全局已注册QQ vector 中
                g_vectQQSinguppedInfo.push_back(sQQSinguppedInfo);
            }
        }

        // 3. 更新QQ列表
        UpdateOnlineQQListCtrl();
    }

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServDlg::OnPaint()
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
HCURSOR CServDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CServDlg::OnBnClickedStartup()
{
    if (0 != m_oMySocket.m_sockLsn) { return; }

    UpdateData();
    CStartupServ oStartupServ(m_wstrIP, m_uPort, m_oMySocket);
    int iRes = oStartupServ.Startup();
    if (0 == iRes)		// 启动服务器成功
    {
        {	// 0. 界面的响应
            // 消息记录
            CString wstrTime;
            FillCurrentTime(wstrTime);
            CString wstrAddr;
            FillAddrClnt(m_oMySocket.m_addrServ, wstrAddr);
            CString wstr = wstrTime + wstrAddr;
            wstr += L"启动服务器成功!\r\n";
            m_wstrShowMsg += wstr;
            SetDlgItemTextW(IDEDT_SHOWMSG, m_wstrShowMsg);
            // 界面的响应
            SetDlgItemTextW(IDBTN_STARTUP, L"启动服务器成功");
            GetDlgItem(IDBTN_STARTUP)->EnableWindow(0);
            GetDlgItem(IDEDT_IP)->EnableWindow(0);
            GetDlgItem(IDEDT_PORT)->EnableWindow(0);
        }

        {	// 1. 线程LoopAccept();
            SParamToThread * pParamToThread = new SParamToThread();
            pParamToThread->pServDlg = this;
            pParamToThread->pMySocket = &m_oMySocket;
            ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CServDlg::LoopAccept, pParamToThread, 0, 0);
        }
    }
    else           // 启动服务器失败
    {
        CString wstr;
        FillCurrentTime(wstr);
        CString temp;
        temp.Format(L"启动服务器失败!失败码%d!", iRes);
        wstr += temp;

        m_oMySocket.m_sockLsn = 0;

        MessageBoxW(wstr);
    }
}

void CServDlg::LoopAccept(SParamToThread * pAcceptParamToThread)
{
    // 得到要用到的数据
    CServDlg* pServDlg = pAcceptParamToThread->pServDlg;
    CMySocket* pMySocket = pAcceptParamToThread->pMySocket;

    // 循环
    while (true)
    {
        // ::accept()
        pMySocket->m_sockConn = pMySocket->Accept();
        // 如果pMySocket->m_sockConn正确,开启线程;这样就有一个线程与客户端对应;
        if (INVALID_SOCKET != pMySocket->m_sockConn)
        {
            // 消息记录
            CString wstrTime;
            FillCurrentTime(wstrTime);
            CString wstrAddr;
            FillAddrClnt(pMySocket->m_addrClnt, wstrAddr);
            CString wstr = wstrTime + wstrAddr + L"有客户端连接!\r\n";
            pServDlg->m_wstrShowMsg += wstr;
            pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pServDlg->m_wstrShowMsg);

            // 线程LoopRecv();
            SParamToThread * pParamToThread = new SParamToThread();
            pParamToThread->pServDlg = pAcceptParamToThread->pServDlg;
            pParamToThread->pMySocket = pAcceptParamToThread->pMySocket;
            pParamToThread->sockConn = pMySocket->m_sockConn;
            pParamToThread->addrClnt = pMySocket->m_addrClnt;
            ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CServDlg::LoopRecv, pParamToThread, 0, 0);
        }
    }
}

void CServDlg::LoopRecv(SParamToThread * pParamToThread)
{
    // 得到要用到的数据
    CServDlg* pServDlg = pParamToThread->pServDlg;
    CMySocket* pMySocket = pParamToThread->pMySocket;
    SOCKET sockConn = pParamToThread->sockConn;
    SOCKADDR_IN addrClnt = pParamToThread->addrClnt;

    // 循环
    while (true)
    {
        // 0. 接收消息
        // 1. 解析消息
        // 2. 处理消息

        // 0. 接收消息
        char strMsgRecv[10240] = {};
        int iRes = pMySocket->Recv(sockConn, strMsgRecv, sizeof(strMsgRecv));
        if (SOCKET_ERROR == iRes || 0 == iRes)	// 客户端断开连接
        {
            {	// 0. QQ没有登陆时
                if (0 == pParamToThread->uQQ)
                {
                    // 消息记录
                    CString wstrTime;
                    FillCurrentTime(wstrTime);
                    CString wstrAddr;
                    FillAddrClnt(pMySocket->m_addrClnt, wstrAddr);
                    CString wstr = wstrTime + wstrAddr + L"断开连接了!\r\n";
                    pServDlg->m_wstrShowMsg += wstr;
                    pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pServDlg->m_wstrShowMsg);

                    //
                    delete pParamToThread;

                    return;
                }
            }

            {	// 0. QQ有登陆时
                if (0 != pParamToThread->uQQ)
                {
                    // 消息记录
                    CString wstrTime;
                    FillCurrentTime(wstrTime);
                    CString wstrAddr;
                    FillAddrClnt(pMySocket->m_addrClnt, wstrAddr);
                    CString wstrQQ;
                    FillQQ(pParamToThread->uQQ, wstrQQ);
                    CString wstr = wstrTime + wstrAddr + wstrQQ + L"下线了!\r\n";
                    pServDlg->m_wstrShowMsg += wstr;
                    pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pServDlg->m_wstrShowMsg);

                    // 更新在线QQ列表 
                    int iLoop = (int)g_vectOnlineQQInfo.size();
                    for (int i = 0; i < iLoop; i++)
                    {
                        if (pParamToThread->uQQ == g_vectOnlineQQInfo.at(i).uQQ)
                        {
                            g_vectOnlineQQInfo.erase(g_vectOnlineQQInfo.begin() + i);
                            break;
                        }
                    }

                    // 发送下线消息
                    COffline oOffline;
                    oOffline.SendOfflineMsg(pParamToThread);

                    // 7. 更新在线QQ列表
                    pParamToThread->pServDlg->UpdateOnlineQQListCtrl();

                    //
                    delete pParamToThread;

                    return;
                }
            }
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
            default:
                break;
        }
    }
}


void CServDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值


}

void CServDlg::UpdateOnlineQQListCtrl()
{
    m_lcOnlineQQ.DeleteAllItems();

    int iLoop = (int)g_vectQQSinguppedInfo.size();
    int jLoop = (int)g_vectOnlineQQInfo.size();
    for (int i = 0; i < iLoop; i++)
    {
        CString wstrQQ;
        FillQQ(g_vectQQSinguppedInfo.at(i).uQQ, wstrQQ);
        m_lcOnlineQQ.InsertItem(i, wstrQQ);

        for (int j = 0; j < jLoop; j++)
        {
            if (g_vectOnlineQQInfo.at(j).uQQ == g_vectQQSinguppedInfo.at(i).uQQ)
            {
                CString wstrAddr;
                FillAddrClnt(g_vectOnlineQQInfo.at(j).addrClnt, wstrAddr);
                m_lcOnlineQQ.SetItemText(i, 1, wstrAddr);
                break;
            }
        }
    }
}


void CServDlg::OnOK()
{
}


void CServDlg::OnBnClickedSnedtelecontrol()
{
    CTelecontrol oTelecontrol;
    _SParamToThread oParamToThread;
    oParamToThread.pServDlg = this;
    oParamToThread.pMySocket = &m_oMySocket;
    oTelecontrol.SendTelecontrolMsg(&oParamToThread);
}
