
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "Menu.h"
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winsock2.h>

#define DEFAULT_COUNT	10
#define DEFAULT_PORT	5150
#define DEFAULT_BUFFER	2048
#define DEFAULT_MESSAGE	"This is a test message"

// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_EXIT, &CClientDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_MAIN, &CClientDlg::OnMenuMain)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	char Str[128];

	sprintf_s(Str, sizeof(Str), "%d", DEFAULT_COUNT);
	sprintf_s(Str, sizeof(Str), "%d", DEFAULT_PORT);

	ShowWindow(SW_MAXIMIZE);

	std::string temp = "player";
	strcpy_s(m_UserName, temp.c_str());

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//void CClientDlg::OnBnClickedSend()
//{
//	// TODO: Add your control notification handler code here
//	char	szMessage[1024];		// Сообщение для отправки
//	BOOL	bSendOnly = FALSE;	// Только отправка данных
//
//	char	szBuffer[DEFAULT_BUFFER];
//	int		ret,
//			i;
//
//	char	Str[256];
//
//	UpdateData();
//	if (m_Number < 1 || m_Number>20)
//	{
//		m_ListBox.AddString("Number of messages must be between 1 and 20");
//		return;
//	}
//
//	GetDlgItem(IDC_MESSAGE)->GetWindowText(szMessage, sizeof(szMessage));
//	if (m_NoEcho.GetCheck() == 1)
//		bSendOnly = TRUE;
//
//	// Отправка и прием данных 
//	//
//	for (i = 0; i < m_Number; i++)
//	{
//		ret = send(m_sClient, szMessage, strlen(szMessage), 0);
//
//		if (ret == 0)
//			break;
//		else if (ret == SOCKET_ERROR)
//		{
//			sprintf_s(Str, sizeof(Str), "send() failed: %d", WSAGetLastError());
//			m_ListBox.AddString((LPTSTR)Str);
//			break;
//		}
//
//		sprintf_s(Str, sizeof(Str), "Send %d bytes\n", ret);
//		m_ListBox.AddString((LPTSTR)Str);
//
//		if (!bSendOnly)
//		{
//			ret = recv(m_sClient, szBuffer, DEFAULT_BUFFER, 0);
//			if (ret == 0)	// Корректное завершение
//				break;
//			else if (ret == SOCKET_ERROR)
//			{
//				sprintf_s(Str, sizeof(Str), "recv() failed: %d", WSAGetLastError());
//				m_ListBox.AddString((LPTSTR)Str);
//				break;
//			}
//			szBuffer[ret] = '\0';
//			sprintf_s(Str, sizeof(Str), "RECV [%d bytes]: '%s'", ret, szBuffer);
//			m_ListBox.AddString((LPTSTR)Str);
//		}
//	}
//}



void CClientDlg::OnMenuExit()
{
	this->DestroyWindow();
}


void CClientDlg::OnMenuMain()
{
	Menu menu;

	menu.m_IsConnected = m_IsConnected;
	menu.m_sClient = m_sClient;
	strcpy_s(menu.m_UserNameStr, m_UserName);

	menu.DoModal();

	m_IsConnected = menu.m_IsConnected;
	m_sClient = menu.m_sClient;
	strcpy_s(m_UserName, menu.m_UserNameStr);

	menu.DestroyWindow();
}
