
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winsock2.h>
#include "Card.h"
#include "Enums.h"
#include <vector>

#define PORT 5150			// Порт по умолчанию
int  iPort = PORT; 	 // Порт для прослушивания подключений
#define DATA_BUFSIZE 8192 	// Размер буфера по умолчанию
#define WM_SOCKET (WM_USER + 1)  // Сообщение о событии

typedef struct _PLAYER_INFORMATION {
	SOCKET Socket;
	int bet;
	int bank;
	Card card1;
	Card card2;
	_PLAYER_INFORMATION *Next;
} PLAYER_INFORMATION, *LPPLAYER_INFORMATION;

void CreatePlayerInformation(SOCKET s, char *Str, CListBox *pLB);
LPPLAYER_INFORMATION GetPlayerInformation(SOCKET s);
void FreePlayerInformation(SOCKET s);

HWND MakeWorkerWindow(char *Str, CListBox *pLB);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LPPLAYER_INFORMATION SocketInfoList;

HWND   hWnd_LB;  // Для вывода в других потоках

UINT ListenThread(PVOID lpParam);
UINT GameThread(PVOID lpParam);

void DealCards();
Card RandomCard();
bool IsDeal(Card card);
char *GetIp(SOCKET sct);

CWinThread* timerThread = NULL;
CWinThread* gameThread = NULL;

bool isEnd = false;
int countPlayers = 0;
std::vector<Card> cards;

CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
END_MESSAGE_MAP()

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	char Str[128];

	sprintf_s(Str, sizeof(Str), "%d", iPort);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnPaint()
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

HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::OnBnClickedStart()
{
	char Str[81];

	hWnd_LB = m_ListBox.m_hWnd;  // Для ListenThread
	GetDlgItem(IDC_PORT)->GetWindowText(Str, sizeof(Str));
	iPort = atoi(Str);
	if (iPort <= 0 || iPort >= 0x10000)
	{
		AfxMessageBox("Incorrect Port number");
		return;
	}

	AfxBeginThread(ListenThread, NULL);

	GetDlgItem(IDC_START)->EnableWindow(false);
}

UINT ListenThread(PVOID lpParam)
{
	MSG msg;
	DWORD Ret;
	SOCKET Listen;
	SOCKADDR_IN InternetAddr;
	HWND Window;
	WSADATA wsaData;

	char  Str[200];
	CListBox  *pLB =
		(CListBox *)(CListBox::FromHandle(hWnd_LB));

	if ((Window = MakeWorkerWindow(Str, pLB)) == NULL)
		return 1;

	if ((Ret = WSAStartup(0x0202, &wsaData)) != 0)
	{
		sprintf_s(Str, sizeof(Str),
			"WSAStartup failed with error %d", Ret);
		pLB->AddString(Str);
		return 1;
	}

	if ((Listen = socket(PF_INET, SOCK_STREAM, 0)) ==
		INVALID_SOCKET)
	{
		sprintf_s(Str, sizeof(Str),
			"socket() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}

	WSAAsyncSelect(Listen, Window, WM_SOCKET,
		FD_ACCEPT | FD_CLOSE);

	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(iPort);

	if (bind(Listen, (PSOCKADDR)&InternetAddr,
		sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		sprintf_s(Str, sizeof(Str),
			"bind() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}

	if (listen(Listen, 5))
	{
		sprintf_s(Str, sizeof(Str),
			"listen() failed with error %d",
			WSAGetLastError());
		pLB->AddString(Str);
		return 1;
	}

	while (Ret = GetMessage(&msg, NULL, 0, 0))
	{
		if (Ret == -1)
		{
			sprintf_s(Str, sizeof(Str),
				"GetMessage() failed with error %d",
				GetLastError());
			pLB->AddString(Str);
			return 1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET Accept;
	LPPLAYER_INFORMATION SocketInfo;
	DWORD RecvBytes, SendBytes;
	DWORD Flags;

	char  Str[200];
	CListBox  *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	if (uMsg == WM_SOCKET)
	{
		if (WSAGETSELECTERROR(lParam))
		{
			sprintf_s(Str, sizeof(Str),
				"Socket failed with error %d",
				WSAGETSELECTERROR(lParam));
			pLB->AddString(Str);
			FreePlayerInformation(wParam);
		}
		else
		{
			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
				if ((Accept = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
				{
					sprintf_s(Str, sizeof(Str),
						"accept() failed with error %d",
						WSAGetLastError());
					pLB->AddString(Str);
					break;
				}

				// Создание структуры с информацией о сокете
				// и занесение ее в список
				CreatePlayerInformation(Accept, Str, pLB);
				sprintf_s(Str, sizeof(Str),
					"Socket number %d connected",
					Accept);
				pLB->AddString(Str);
				WSAAsyncSelect(Accept, hwnd, WM_SOCKET,
					FD_READ | FD_WRITE | FD_CLOSE);

				if (countPlayers > 0 && gameThread == NULL) {
					AfxBeginThread(GameThread, NULL);
				}

				break;

			case FD_CLOSE:

				sprintf_s(Str, sizeof(Str),
					"Closing socket %d", wParam);
				pLB->AddString(Str);
				FreePlayerInformation(wParam);

				break;
			}
		}
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreatePlayerInformation(SOCKET s, char *Str, CListBox *pLB)
{
	LPPLAYER_INFORMATION SI;

	if ((SI = (LPPLAYER_INFORMATION)GlobalAlloc(GPTR,
		sizeof(PLAYER_INFORMATION))) == NULL)
	{
		sprintf_s(Str, sizeof(Str),
			"GlobalAlloc() failed with error %d",
			GetLastError());
		pLB->AddString(Str);
		return;
	}

	// Подготовка структуры для использования.

	SI->Socket = s;
	SI->bank = 1000;
	SI->bet = 0;
	SI->card1 = Card(-1, -1);
	SI->card2 = Card(-1, -1);

	SI->Next = SocketInfoList;

	SocketInfoList = SI;

	countPlayers++;
}

LPPLAYER_INFORMATION GetPlayerInformation(SOCKET s)
{
	PLAYER_INFORMATION *SI = SocketInfoList;
	while (SI)
	{
		if (SI->Socket == s)
			return SI;
		SI = SI->Next;
	}

	return NULL;
}

void FreePlayerInformation(SOCKET s)
{
	PLAYER_INFORMATION *SI = SocketInfoList;
	PLAYER_INFORMATION *PrevSI = NULL;

	while (SI)
	{
		if (SI->Socket == s)
		{
			if (PrevSI)
				PrevSI->Next = SI->Next;
			else
				SocketInfoList = SI->Next;

			closesocket(SI->Socket);
			GlobalFree(SI);
			return;
		}
		PrevSI = SI;
		SI = SI->Next;
	}
}

HWND MakeWorkerWindow(char *Str, CListBox *pLB)
{
	WNDCLASS wndclass;
	CHAR *ProviderClass = "AsyncSelect";
	HWND Window;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = ProviderClass;

	if (RegisterClass(&wndclass) == 0)
	{
		sprintf_s(Str, sizeof(Str),
			"RegisterClass() failed with error %d",
			GetLastError());
		pLB->AddString(Str);
		return NULL;
	}

	// Собственно создание окна.
	if ((Window = CreateWindow(
		ProviderClass,
		"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL)) == NULL)
	{
		sprintf_s(Str, sizeof(Str),
			"CreateWindow() failed with error %d",
			GetLastError());
		pLB->AddString(Str);
		return NULL;
	}

	return Window;
}

UINT GameThread(PVOID lpParam) {
	AfxMessageBox("Start game");

	char	szBuffer[128];
	char	Str[256];

	Sleep(3000);
	DealCards();

	//PLAYER_INFORMATION *SI = SocketInfoList;
	//while (SI)
	//{
	//	SOCKET sct = SI->Socket;

	//	int ret = recv(sct, szBuffer, 128, 0);

	//	if (ret == 0)	// Корректное завершение
	//		return 0;

	//	if (ret == SOCKET_ERROR)
	//	{
	//		sprintf_s(Str, sizeof(Str), "recv() failed: %d", WSAGetLastError());
	//		continue;
	//	}

	//	szBuffer[ret] = '\0';
	//	sprintf_s(Str, sizeof(Str), "RECV [%d bytes]: '%s'", ret, szBuffer);

	//	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));
	//	pLB->AddString(Str);

	//	SI = SI->Next;
	//}

	return 0;
}

void DealCards() {
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	PLAYER_INFORMATION *SI = SocketInfoList;

	while (SI)
	{
		SOCKET sct = SI->Socket;

		char* ip = GetIp(sct);

		Card card1 = RandomCard();
		SI->card1 = card1;

		Card card2 = RandomCard();
		SI->card2 = card2;

		char Str[128];
		sprintf_s(Str, sizeof(Str), "cards %d %d %d %d",
			card1.m_Suit, card1.m_Rank, card2.m_Suit, card2.m_Rank);

		int	ret;

		ret = send(sct, Str, strlen(Str), 0);

		if (ret == SOCKET_ERROR)
		{
			char	szBuffer[128];

			sprintf_s(szBuffer, sizeof(szBuffer), "send() failed: %d", WSAGetLastError());

			pLB->AddString(szBuffer);
			continue;
		}

		std::string str = "";
		str.append(ip);
		str.append(" ");
		str.append(Str);

		pLB->AddString(str.c_str());

		SI = SI->Next;
	}
}

Card RandomCard() {
	Card card;

	int suit = (rand() % (4)) + 1;
	int rank = (rand() % (13)) + 1;

	while (IsDeal(card)) {
		card = RandomCard();
	}

	return Card(suit, rank);
}

bool IsDeal(Card card) {
	PLAYER_INFORMATION *SI = SocketInfoList;

	while (SI)
	{
		Card buf = SI->card1;
		if (card.m_Rank == buf.m_Rank &&
			card.m_Suit == buf.m_Suit) {
			return true;
		}

		buf = SI->card2;
		if (card.m_Rank == buf.m_Rank &&
			card.m_Suit == buf.m_Suit) {
			return true;
		}

		SI = SI->Next;
	}

	for (Card buf : cards) {
		if (card.m_Rank == buf.m_Rank &&
			card.m_Suit == buf.m_Suit) {
			return true;
		}
	}

	return false;
}

char *GetIp(SOCKET sct) {
	sockaddr_in client_info;
	int addrsize = sizeof(client_info);
	getpeername(sct, (struct sockaddr *)&client_info, &addrsize);

	char *ip = inet_ntoa(client_info.sin_addr);

	return ip;
}