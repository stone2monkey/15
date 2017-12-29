
// ServDlg.cpp : ʵ���ļ�
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


// CServDlg �Ի���



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


// CServDlg ��Ϣ�������

BOOL CServDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������

    {	// ��ʼ���Ի��� 
        // 0. �������������
        SetDlgItemTextW(IDEDT_IP, L"192.168.1.54");
        SetDlgItemTextW(IDEDT_PORT, L"5555");

        // 1. �б�ؼ�
        CRect rect;
        m_lcOnlineQQ.GetClientRect(&rect);
        m_lcOnlineQQ.SetExtendedStyle(m_lcOnlineQQ.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
        m_lcOnlineQQ.InsertColumn(0, L"QQ", LVCFMT_CENTER, 120);
        m_lcOnlineQQ.InsertColumn(1, L"Addr", LVCFMT_CENTER, rect.Width() - 120);

        // 2. ��ʼ��ȫ��QQ
        {
            // 0. �õ����ݿ�����
            _RESULT re;
            CDataBaseCtrl oDataBaseCtrl;
            oDataBaseCtrl.LinkDataBase("0", "0", "0", "0");
            char str[0x400] = "select * from t_qqsignupped;";
            oDataBaseCtrl.ExecSqlByRecordSet(re, str);

            // 1. ������д��ȫ����ע��QQ
            int iLoop = (int)re.vec_Data.size();
            for (int i = 0; i < iLoop; i++)
            {
                // ����ʱ�����õ�QQ PW FRIENDS 
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
                // ѹ��ȫ����ע��QQ vector ��
                g_vectQQSinguppedInfo.push_back(sQQSinguppedInfo);
            }
        }

        // 3. ����QQ�б�
        UpdateOnlineQQListCtrl();
    }

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServDlg::OnPaint()
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
    if (0 == iRes)		// �����������ɹ�
    {
        {	// 0. �������Ӧ
            // ��Ϣ��¼
            CString wstrTime;
            FillCurrentTime(wstrTime);
            CString wstrAddr;
            FillAddrClnt(m_oMySocket.m_addrServ, wstrAddr);
            CString wstr = wstrTime + wstrAddr;
            wstr += L"�����������ɹ�!\r\n";
            m_wstrShowMsg += wstr;
            SetDlgItemTextW(IDEDT_SHOWMSG, m_wstrShowMsg);
            // �������Ӧ
            SetDlgItemTextW(IDBTN_STARTUP, L"�����������ɹ�");
            GetDlgItem(IDBTN_STARTUP)->EnableWindow(0);
            GetDlgItem(IDEDT_IP)->EnableWindow(0);
            GetDlgItem(IDEDT_PORT)->EnableWindow(0);
        }

        {	// 1. �߳�LoopAccept();
            SParamToThread * pParamToThread = new SParamToThread();
            pParamToThread->pServDlg = this;
            pParamToThread->pMySocket = &m_oMySocket;
            ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CServDlg::LoopAccept, pParamToThread, 0, 0);
        }
    }
    else           // ����������ʧ��
    {
        CString wstr;
        FillCurrentTime(wstr);
        CString temp;
        temp.Format(L"����������ʧ��!ʧ����%d!", iRes);
        wstr += temp;

        m_oMySocket.m_sockLsn = 0;

        MessageBoxW(wstr);
    }
}

void CServDlg::LoopAccept(SParamToThread * pAcceptParamToThread)
{
    // �õ�Ҫ�õ�������
    CServDlg* pServDlg = pAcceptParamToThread->pServDlg;
    CMySocket* pMySocket = pAcceptParamToThread->pMySocket;

    // ѭ��
    while (true)
    {
        // ::accept()
        pMySocket->m_sockConn = pMySocket->Accept();
        // ���pMySocket->m_sockConn��ȷ,�����߳�;��������һ���߳���ͻ��˶�Ӧ;
        if (INVALID_SOCKET != pMySocket->m_sockConn)
        {
            // ��Ϣ��¼
            CString wstrTime;
            FillCurrentTime(wstrTime);
            CString wstrAddr;
            FillAddrClnt(pMySocket->m_addrClnt, wstrAddr);
            CString wstr = wstrTime + wstrAddr + L"�пͻ�������!\r\n";
            pServDlg->m_wstrShowMsg += wstr;
            pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pServDlg->m_wstrShowMsg);

            // �߳�LoopRecv();
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
    // �õ�Ҫ�õ�������
    CServDlg* pServDlg = pParamToThread->pServDlg;
    CMySocket* pMySocket = pParamToThread->pMySocket;
    SOCKET sockConn = pParamToThread->sockConn;
    SOCKADDR_IN addrClnt = pParamToThread->addrClnt;

    // ѭ��
    while (true)
    {
        // 0. ������Ϣ
        // 1. ������Ϣ
        // 2. ������Ϣ

        // 0. ������Ϣ
        char strMsgRecv[10240] = {};
        int iRes = pMySocket->Recv(sockConn, strMsgRecv, sizeof(strMsgRecv));
        if (SOCKET_ERROR == iRes || 0 == iRes)	// �ͻ��˶Ͽ�����
        {
            {	// 0. QQû�е�½ʱ
                if (0 == pParamToThread->uQQ)
                {
                    // ��Ϣ��¼
                    CString wstrTime;
                    FillCurrentTime(wstrTime);
                    CString wstrAddr;
                    FillAddrClnt(pMySocket->m_addrClnt, wstrAddr);
                    CString wstr = wstrTime + wstrAddr + L"�Ͽ�������!\r\n";
                    pServDlg->m_wstrShowMsg += wstr;
                    pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pServDlg->m_wstrShowMsg);

                    //
                    delete pParamToThread;

                    return;
                }
            }

            {	// 0. QQ�е�½ʱ
                if (0 != pParamToThread->uQQ)
                {
                    // ��Ϣ��¼
                    CString wstrTime;
                    FillCurrentTime(wstrTime);
                    CString wstrAddr;
                    FillAddrClnt(pMySocket->m_addrClnt, wstrAddr);
                    CString wstrQQ;
                    FillQQ(pParamToThread->uQQ, wstrQQ);
                    CString wstr = wstrTime + wstrAddr + wstrQQ + L"������!\r\n";
                    pServDlg->m_wstrShowMsg += wstr;
                    pServDlg->SetDlgItemTextW(IDEDT_SHOWMSG, pServDlg->m_wstrShowMsg);

                    // ��������QQ�б� 
                    int iLoop = (int)g_vectOnlineQQInfo.size();
                    for (int i = 0; i < iLoop; i++)
                    {
                        if (pParamToThread->uQQ == g_vectOnlineQQInfo.at(i).uQQ)
                        {
                            g_vectOnlineQQInfo.erase(g_vectOnlineQQInfo.begin() + i);
                            break;
                        }
                    }

                    // ����������Ϣ
                    COffline oOffline;
                    oOffline.SendOfflineMsg(pParamToThread);

                    // 7. ��������QQ�б�
                    pParamToThread->pServDlg->UpdateOnlineQQListCtrl();

                    //
                    delete pParamToThread;

                    return;
                }
            }
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
            default:
                break;
        }
    }
}


void CServDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


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
