// Menu.cpp: файл реализации
//


#include "stdafx.h"
#include "Client.h"
#include "Menu.h"
#include "afxdialogex.h"
#include "winsock2.h"
#include <conio.h>

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
	DDX_Control(pDX, IDC_USER_NAME, m_UserName);
	DDX_Control(pDX, IDC_SERVER_COMBO, m_ServerCombo);
}


BEGIN_MESSAGE_MAP(Menu, CDialogEx)
	ON_BN_CLICKED(IDC_CONNECT, &Menu::OnClickedConnect)
	ON_BN_CLICKED(IDC_DISCONNECT, &Menu::OnClickedDisconnect)
	ON_BN_CLICKED(IDCANCEL, &Menu::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Обработчики сообщений Menu

BOOL Menu::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	GetListPCIp();

	// TODO:  Добавить дополнительную инициализацию
	char port[128];
	sprintf_s(port, sizeof(port), "%d", DEFAULT_PORT);

	m_Port.SetWindowTextA(LPTSTR(port));
	m_ServerCombo.SetWindowTextA("localhost");
	m_UserName.SetWindowTextA(m_UserNameStr.c_str());

	SetConnected(m_IsConnected);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

void Menu::GetListPCIp()
{
	std::string strTemp;
	struct hostent *host;

	struct in_addr *ptr; // To retrieve the IP Address 

	DWORD dwScope = RESOURCE_CONTEXT;
	NETRESOURCE *NetResource = NULL;
	HANDLE hEnum;
	WNetOpenEnum(dwScope, NULL, NULL,
		NULL, &hEnum);

	if (hEnum)
	{
		DWORD Count = 0xFFFFFFFF;
		DWORD BufferSize = 2048;
		LPVOID Buffer = new char[2048];
		WNetEnumResource(hEnum, &Count, Buffer, &BufferSize);
		NetResource = (NETRESOURCE*)Buffer;

		char szHostName[200];
		unsigned int i;

		for (i = 0;
			i < BufferSize / sizeof(NETRESOURCE);
			i++, NetResource++)
		{
			if (NetResource->dwUsage ==
				RESOURCEUSAGE_CONTAINER &&
				NetResource->dwType ==
				RESOURCETYPE_ANY)
			{
				if (NetResource->lpRemoteName)
				{
					std::string strFullName = NetResource->lpRemoteName;
					if (0 == strFullName.substr(0, 2).compare("\\\\"))
						strFullName = strFullName.substr(2);

					gethostname(szHostName, strlen(szHostName));

					int error = WSAGetLastError();

					host = gethostbyname(strFullName.c_str());

					if (host == NULL) continue;
					ptr = (struct in_addr *)
						host->h_addr_list[0];

					// Eg. 211.40.35.76 split up like this.             
					int a = ptr->S_un.S_un_b.s_b1;  // 211           
					int b = ptr->S_un.S_un_b.s_b2;  // 40
					int c = ptr->S_un.S_un_b.s_b3;  // 35
					int d = ptr->S_un.S_un_b.s_b4;  // 76

					char ip[128];
					sprintf_s(ip, "%d.%d.%d.%d", a, b, c, d);
					_cprintf(ip);
					m_ServerCombo.AddString(ip);
				}
			}
		}

		delete Buffer;
		WNetCloseEnum(hEnum);
	}
}

void Menu::SetConnected(bool IsConnected)
{
	m_IsConnected = IsConnected;

	m_ServerCombo.EnableWindow(!IsConnected);
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

	m_ServerCombo.GetWindowText(szServer, sizeof(szServer));
	m_Port.GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox((LPTSTR)"Port number incorrect", MB_ICONSTOP);
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

	std::string successMsg = "Success connect";
	AfxMessageBox((LPSTR)successMsg.c_str(), MB_ICONINFORMATION);

	this->OnCancel();
}


void Menu::OnClickedDisconnect()
{
	closesocket(m_sClient);
	SetConnected(false);
}


void Menu::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void Menu::OnClose()
{
	WSACleanup();

	CDialogEx::OnClose();
}
