// Menu.cpp: файл реализации
//

#include "stdafx.h"
#include "Client.h"
#include "Menu.h"
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
	m_UserName.SetWindowTextA(m_UserNameStr.c_str());

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
	struct hostent *host = NULL;

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

	m_UserName.GetWindowTextA((LPTSTR)m_UserNameStr.c_str(), sizeof(m_UserNameStr.c_str()));

	std::string successMsg = "Success connect to ";
	if (host != NULL) {
		successMsg.append(host->h_name);
	}
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