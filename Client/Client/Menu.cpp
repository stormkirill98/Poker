// Menu.cpp: файл реализации
//

#include "stdafx.h"
#include "Client.h"
#include "Menu.h"
#include "string"
#include "afxdialogex.h"

#define DEFAULT_PORT	5150

// Диалоговое окно Menu

IMPLEMENT_DYNAMIC(Menu, CDialogEx)

Menu::Menu(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MENU, pParent)
{

}

Menu::~Menu()
{
}

void Menu::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PORT, m_Port);
	DDX_Control(pDX, IDC_SERVER, m_Server);
	DDX_Control(pDX, IDC_USER_NAME, m_UserName);
}


BEGIN_MESSAGE_MAP(Menu, CDialogEx)
	ON_BN_CLICKED(IDC_CONNECT, &Menu::OnClickedConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, &Menu::OnClickedDisconnect)
	ON_BN_CLICKED(IDCANCEL, &Menu::OnBnClickedCancel)
END_MESSAGE_MAP()


// Обработчики сообщений Menu

BOOL Menu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	char port[128];
	sprintf_s(port, sizeof(port), "%d", DEFAULT_PORT);

	m_Port.SetWindowTextA(LPTSTR(port));
	m_Server.SetWindowTextA("localhost");
	m_UserName.SetWindowTextA(LPTSTR(m_UserNameStr));

	SetConnected(m_IsConnected);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

void Menu::SetConnected(bool IsConnected)
{
	m_IsConnected = IsConnected;

	m_Server.EnableWindow(!IsConnected);
	m_Port.EnableWindow(!IsConnected);
	m_UserName.EnableWindow(!IsConnected);
	GetDlgItem(IDC_CONNECT)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_DISCONNECT)->EnableWindow(IsConnected);

}

void Menu::OnClickedConnect()
{
	char szServer[128];	// Имя или IP-адрес сервера
	int	iPort;			// Порт

	WSADATA	wsd;

	struct sockaddr_in 	server;
	struct hostent		*host = NULL;

	char Str[256];

	m_Server.GetWindowText(szServer, sizeof(szServer));
	m_Port.GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox((LPTSTR)"Port number incorrect", MB_ICONSTOP);
		return;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		AfxMessageBox((LPTSTR)"Failed to load Winsock library!", MB_ICONSTOP);
		return;
	}

	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed: %d\n", WSAGetLastError());
		AfxMessageBox((LPTSTR)Str, MB_ICONSTOP);
		return;
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = inet_addr(szServer);

	if (server.sin_addr.s_addr == INADDR_NONE)
	{
		host = gethostbyname(szServer);
		if (host == NULL)
		{
			sprintf_s(Str, sizeof(Str), "Unable to resolve server: %s", szServer);
			AfxMessageBox((LPTSTR)Str, MB_ICONSTOP);
			return;
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
	}
	if (connect(m_sClient, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "connect() failed: %d", WSAGetLastError());
		AfxMessageBox((LPTSTR)Str, MB_ICONSTOP);
		return;
	}

	SetConnected(true);

	m_UserName.GetWindowText(m_UserNameStr, sizeof(m_UserNameStr));

	std::string successMsg = "Success connect to ";
	successMsg.append(host->h_name);
	AfxMessageBox((LPSTR)successMsg.c_str(), MB_ICONINFORMATION);

	this->OnCancel();
}


void Menu::OnClickedDisconnect()
{
	closesocket(m_sClient);
	WSACleanup();
	SetConnected(false);
}


void Menu::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
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