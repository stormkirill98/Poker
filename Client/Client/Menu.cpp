// Menu.cpp: файл реализации
//

#include "stdafx.h"
#include "Client.h"
#include "Menu.h"
#include "afxdialogex.h"


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
	m_Port.SetWindowTextA("5150");
	m_Server.SetWindowTextA("localhost");
	m_UserName.SetWindowTextA("player_name");

	SetConnected(m_IsConnected);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

void Menu::SetConnected(bool IsConnected)
{
	m_IsConnected = IsConnected;

	GetDlgItem(IDC_SERVER)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_PORT)->EnableWindow(!IsConnected);
	GetDlgItem(IDC_CONNECT)->EnableWindow(!IsConnected);
}

void Menu::OnClickedConnect()
{
	char szServer[128];	// Имя или IP-адрес сервера
	int	iPort;			// Порт

	WSADATA	wsd;

	struct sockaddr_in 	server;
	struct hostent		*host = NULL;

	char Str[256];

	GetDlgItem(IDC_SERVER)->GetWindowText(szServer, sizeof(szServer));
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox((LPTSTR)"Port number incorrect");
		return;
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		AfxMessageBox((LPTSTR)"Failed to load Winsock library!");
		return;
	}

	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str), "socket() failed: %d\n", WSAGetLastError());
		AfxMessageBox((LPTSTR)Str);
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
			AfxMessageBox((LPTSTR)Str);
			return;
		}
		CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
	}
	if (connect(m_sClient, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str), "connect() failed: %d", WSAGetLastError());
		AfxMessageBox((LPTSTR)Str);
		return;
	}

	SetConnected(true);
}


void Menu::OnClickedDisconnect()
{
	closesocket(m_sClient);
	WSACleanup();
	SetConnected(false);
}


void Menu::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}



