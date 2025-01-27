
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
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "DefineCombination.h"


using namespace std;

#define PORT 5150			// Порт по умолчанию
int  iPort = PORT; 	 // Порт для прослушивания подключений
#define DATA_BUFSIZE 8192 	// Размер буфера по умолчанию
#define WM_SOCKET (WM_USER + 1)  // Сообщение о событии
#define SIZE_MSG 4096

#define TIME_POLL 15
#define TIME_START 15
#define TIME_PAUSE 5

enum Combination {HIGH_CARD, PAIR, TWO_PAIR, THREE1, STRAIGHT, FLUSH, FULL_HOUSE, KARE, STRAIGHT_FLUSH, ROYAL_FLUSH};

typedef struct COMBINATION {
	int combination;
	int maxRank;
	int kiker;
} COMBINATION;

typedef struct _PLAYER_INFORMATION {
	SOCKET Socket;
	int bet;
	int bank;
	bool fold = false;
	Card card1;
	Card card2;
	COMBINATION combination;
	CWinThread* thread;
	_PLAYER_INFORMATION *Next;
} PLAYER_INFORMATION, *LPPLAYER_INFORMATION;

void CreatePlayerInformation(SOCKET s, char *Str, CListBox *pLB);
LPPLAYER_INFORMATION GetPlayerInformation(SOCKET s);
void FreePlayerInformation(SOCKET s);

HWND MakeWorkerWindow(char *Str, CListBox *pLB);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, 
							WPARAM wParam, LPARAM lParam);

LPPLAYER_INFORMATION PlayerInfoList;
vector<LPPLAYER_INFORMATION> TopPlayers;

HWND hWnd_LB;  // Для вывода в других потоках

UINT ListenThread(PVOID lpParam);
UINT GameThread(PVOID lpParam);
UINT ListenPlayer(PVOID lpParam);

void DealCards();
Card RandomCard();
bool IsDeal(Card card);

char *GetIp(SOCKET sct);

int SendMsg(SOCKET sct, const char *msg);
void SendPlayer(PLAYER_INFORMATION* player);
void SendPlayers();
void SendWaiting(PLAYER_INFORMATION* player, int time);
void SendBank();
void SendMinBet();
void SendStart(const char* msg);
void CreateAndSendCard();
void SendPlayerDisconnect(PLAYER_INFORMATION* player);

void ParseCommand(char *str, PLAYER_INFORMATION* player);
void PlayersPoll();
void SetPlayersBet(int bet);
int CountPlayersIsNotFold();

void DefineCombitahions();
void DefineCombination(PLAYER_INFORMATION *PI);
void DefineTopPlayers();
COMBINATION DefineTopCombination();

void DistributeBets();
bool BetsIsEquals();
bool PlayersIsAllIn();
void ProcessBets();

CWinThread* gameThread = NULL;

vector<CWinThread*> threadsForFree;

bool isEnd = false;
int countPlayers = 0;
vector<Card> cards;
int m_Bank = 0;
int m_MinBet = 0;

bool playerSendMsg = false;

CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTBOX, m_ListBox);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
END_MESSAGE_MAP()

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	char Str[SIZE_MSG];

	sprintf_s(Str, sizeof(Str), "%d", iPort);
	GetDlgItem(IDC_PORT)->SetWindowText(Str);

	return TRUE;  // return TRUE  unless you set the focus to a control
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, 
							WPARAM wParam, LPARAM lParam)
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

				if (countPlayers > 1 && gameThread == NULL) {												//TODO: не забыть про кол-во игроков
					gameThread = AfxBeginThread(GameThread, NULL, THREAD_PRIORITY_LOWEST);
				}

				SendPlayers();

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


void CreatePlayerInformation(SOCKET s, char *Str, CListBox *pLB)
{
	LPPLAYER_INFORMATION PI;

	if ((PI = (LPPLAYER_INFORMATION)GlobalAlloc(GPTR,
		sizeof(PLAYER_INFORMATION))) == NULL)
	{
		sprintf_s(Str, sizeof(Str),
			"GlobalAlloc() failed with error %d",
			GetLastError());
		pLB->AddString(Str);
		return;
	}

	// Подготовка структуры для использования.

	PI->Socket = s;
	PI->bank = 1000;
	PI->bet = 0;
	PI->card1 = Card(-1, -1);
	PI->card2 = Card(-1, -1);

	PI->Next = PlayerInfoList;

	PlayerInfoList = PI;

	countPlayers++;
}

LPPLAYER_INFORMATION GetPlayerInformation(SOCKET s)
{
	PLAYER_INFORMATION *PI = PlayerInfoList;
	while (PI)
	{
		if (PI->Socket == s)
			return PI;
		PI = PI->Next;
	}

	return NULL;
}

void FreePlayerInformation(SOCKET s)
{
	PLAYER_INFORMATION *PI = PlayerInfoList;
	PLAYER_INFORMATION *PrevSI = NULL;

	DWORD currentThreadId = GetCurrentThreadId();

	SendPlayerDisconnect(GetPlayerInformation(s));

	while (PI)
	{
		if (PI->Socket == s)
		{
			if (PrevSI)
				PrevSI->Next = PI->Next;
			else
				PlayerInfoList = PI->Next;

			if (PI->thread != NULL) {
				if (PI->thread->m_nThreadID != currentThreadId) {
					DWORD exit_code = NULL;
					GetExitCodeThread(PI->thread->m_hThread, &exit_code);

					if (exit_code == STILL_ACTIVE)
					{
						TerminateThread(PI->thread->m_hThread, 0);
						CloseHandle(PI->thread->m_hThread);
					}
				}
			}

			closesocket(PI->Socket);
			GlobalFree(PI);

			countPlayers--;
			if (countPlayers < 2) {
				if (gameThread != NULL) {
					if (gameThread->m_nThreadID != currentThreadId) {
						DWORD exit_code = NULL;
						GetExitCodeThread(gameThread->m_hThread, &exit_code);

						if (exit_code == STILL_ACTIVE)
						{
							TerminateThread(gameThread->m_hThread, NULL);
							CloseHandle(gameThread->m_hThread);
							gameThread = NULL;
						}
					}
					else {
						ExitThread(NULL);
					}
				}
			}

			return;
		}
		PrevSI = PI;
		PI = PI->Next;
	}
}


UINT GameThread(PVOID lpParam) {
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	for (int i = TIME_START; i > -1; i--) {
		Sleep(1000);

		string buf = "start ";
		char s[16];
		_itoa_s(i, s, 10);
		buf.append(s);

		pLB->AddString(buf.c_str());
		
		SendStart(buf.c_str());
	}

	pLB->AddString("---START GAME---");

	PLAYER_INFORMATION *PI = PlayerInfoList;
	while (PI)
	{
		CWinThread* thread = AfxBeginThread(ListenPlayer, PI, THREAD_PRIORITY_HIGHEST);
		PI->thread = thread;

		PI = PI->Next;
	}

	while (!isEnd && countPlayers > 1) {
		DealCards();
		cards.clear();
		
		for (int step = 0; step < 4; step++) {
			if (PlayersIsAllIn()) {
				CreateAndSendCard();
				continue;
			}

			m_MinBet = 20;
			SetPlayersBet(m_MinBet);
			SendPlayers();

			SendMinBet();

			PlayersPoll();

			int countNoFold = CountPlayersIsNotFold();
			if (countNoFold == 0) {
				break;
			}
			if (countNoFold != 1) {
				while (!BetsIsEquals() || PlayersIsAllIn()) {
					PlayersPoll();
					countNoFold = CountPlayersIsNotFold();
					if (countNoFold == 1) {
						break;
					}
				}
			}
			if (countNoFold == 0) {
				break;
			}

			ProcessBets();
			SendBank();
			SendPlayers();
			if (step == 3) {
				continue;
			}

			if (step == 0) {
				for (int countCard = 0; countCard < 3; countCard++) {
					CreateAndSendCard();
				}
			}
			else {
				CreateAndSendCard();
			}
		}

		DefineCombitahions();
		DefineTopPlayers();
		DistributeBets();
		SendBank();
		SendPlayers();

		for (int i = TIME_PAUSE; i > 0; i--) {
			Sleep(1000);

			string buf = "pause ";
			char s[16];
			_itoa_s(i, s, 10);
			buf.append(s);

			pLB->AddString(buf.c_str());
		}
	}

	return 0;
}

void ProcessBets() {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		m_Bank += PI->bet;

		PI->bank = PI->bank - PI->bet;
		PI->bet = 0;

		PI = PI->Next;
	}
}

void DistributeBets() {
	int countTopPlayers = TopPlayers.size();
	if (countTopPlayers == 0) {
		m_Bank = 0;
		return;
	}

	int winningEveryone = m_Bank / countTopPlayers;

	for (PLAYER_INFORMATION* player : TopPlayers) {
		player->bank += winningEveryone;
	}

	m_Bank = 0;
}

void PlayersPoll() {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		if (PI == NULL) {
			return;
		}

		if (PI->fold) {
			PI = PI->Next;
			continue;
		}

		int timeBack = TIME_POLL;
		playerSendMsg = false;
		while (timeBack > -1) {
			Sleep(1000);

			if (playerSendMsg) {
				break;
			}
			SendWaiting(PI, timeBack);
			timeBack--;
		}

		if (timeBack == -1) {
			PI->fold = true;
			m_Bank += PI->bet;
			PI->bet = -1;
			SendPlayer(PI);
		}

		PI = PI->Next;
	}
}

bool BetsIsEquals() {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	if (PI == NULL) {
		return false;
	}

	int bet = PI->bet;
	while (PI)
	{
		if (PI->bet != bet) {
			return false;
		}

		PI = PI->Next;
	}

	return true;
}

bool PlayersIsAllIn() {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		if (PI->bet != PI->bank) {
			return false;
		}

		PI = PI->Next;
	}

	return true;
}


void DefineCombitahions() {
	PLAYER_INFORMATION *PI = PlayerInfoList;
	while (PI)
	{
		DefineCombination(PI);

		PI = PI->Next;
	}
}

void DefineTopPlayers() {
	TopPlayers.clear();
	COMBINATION topCombination = DefineTopCombination();

	PLAYER_INFORMATION *PI = PlayerInfoList;
	while (PI)
	{
		if (PI->fold) {
			PI = PI->Next;
			continue;
		}

		COMBINATION combination = PI->combination;
		if (combination.combination == topCombination.combination &&
			combination.maxRank == topCombination.maxRank) {
			TopPlayers.push_back(PI);
		}

		PI = PI->Next;
	}
}

void DefineCombination(PLAYER_INFORMATION *PI) {
	vector<Card> allCards = cards;
	allCards.push_back(PI->card1);
	allCards.push_back(PI->card2);

	int maxRank;

	if (DefineCombination::RoyalFlush(allCards)) {
		PI->combination.combination = Combination::ROYAL_FLUSH;
		PI->combination.maxRank = 12;
		return;
	}

	maxRank = DefineCombination::StraightFlush(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::STRAIGHT_FLUSH;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::Kare(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::KARE;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::FullHouse(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::FULL_HOUSE;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::Flush(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::FLUSH;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::Straight(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::STRAIGHT;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::Three(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::THREE1;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::TwoPair(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::TWO_PAIR;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::Pair(allCards);
	if (maxRank > -1) {
		PI->combination.combination = Combination::PAIR;
		PI->combination.maxRank = maxRank;
		return;
	}

	maxRank = DefineCombination::HighCard(PI->card1, PI->card2);
	if (maxRank > -1) {
		PI->combination.combination = Combination::HIGH_CARD;
		PI->combination.maxRank = maxRank;
		return;
	}
}

COMBINATION DefineTopCombination() {
	COMBINATION topCombination;
	topCombination.combination = Combination::HIGH_CARD;
	topCombination.maxRank = 0;

	PLAYER_INFORMATION *PI = PlayerInfoList;
	while (PI)
	{
		COMBINATION combination = PI->combination;
		if (combination.combination > topCombination.combination) {
			topCombination.combination = combination.combination;
			topCombination.maxRank = combination.maxRank;
		}

		if (combination.combination == topCombination.combination) {
			if (combination.maxRank > topCombination.maxRank) {
				topCombination.maxRank = combination.maxRank;
			}
		}

		PI = PI->Next;
	}

	return topCombination;
}

int CountPlayersIsNotFold() {
	int count = 0;

	PLAYER_INFORMATION *PI = PlayerInfoList;
	while (PI)
	{
		if (!PI->fold) {
			count++;
		}

		PI = PI->Next;
	}

	return count;
}


void SetPlayersBet(int bet) {
	PLAYER_INFORMATION *PI = PlayerInfoList;
	while (PI)
	{
		PI->bet = bet;

		PI = PI->Next;
	}
}

UINT ListenPlayer(PVOID lpParam) {
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	PLAYER_INFORMATION *player = (PLAYER_INFORMATION*)lpParam;

	char input[SIZE_MSG];
	char Str[256];
	int ret;

	SOCKET sct = player->Socket;
	char* ip = GetIp(sct); 

	do {
		ret = recv(sct, input, SIZE_MSG, 0);

		if (ret == 0)	// Корректное завершение
			return 0;

		if (ret == SOCKET_ERROR)
		{
			int error = WSAGetLastError();
			//ждем данных
			if (error == WSAEWOULDBLOCK) {
				ret = 1;
				continue;
			}

			sprintf_s(Str, sizeof(Str), "%s recv() failed: %d", ip, error);									
			pLB->AddString(Str);

			FreePlayerInformation(sct);
			return -1;
		}

		input[ret] = '\0';
		sprintf_s(Str, sizeof(Str), "recv %s: '%s'", ip, input);

		pLB->AddString(Str);

		ParseCommand(input, player);
	} while (ret > 0);

	return 0;
}


void DealCards() {
	srand(time(NULL));
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SOCKET sct = PI->Socket;

		PI->fold = false;

		char* ip = GetIp(sct);

		Card card1 = RandomCard();
		PI->card1 = card1;

		Card card2 = RandomCard();
		PI->card2 = card2;

		char Str[SIZE_MSG];
		sprintf_s(Str, sizeof(Str), "cards %d %d %d %d",
			card1.m_Suit, card1.m_Rank, card2.m_Suit, card2.m_Rank);

		SendMsg(sct, Str);

		PI = PI->Next;
	}
}

Card RandomCard() {
	Card card;

	int suit = (rand() % (3));
	int rank = (rand() % (12));

	while (IsDeal(card)) {
		card = RandomCard();
	}

	return Card(suit, rank);
}

bool IsDeal(Card card) {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		Card buf = PI->card1;
		if (card.m_Rank == buf.m_Rank &&
			card.m_Suit == buf.m_Suit) {
			return true;
		}

		buf = PI->card2;
		if (card.m_Rank == buf.m_Rank &&
			card.m_Suit == buf.m_Suit) {
			return true;
		}

		PI = PI->Next;
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

void ParseCommand(char *Str, PLAYER_INFORMATION* player) {
	string str = Str;

	if (str.empty()) {
		return;
	}

	string action = str.substr(0, 4);
	playerSendMsg = true;

	gameThread->SuspendThread();
	if (action == "chec") {
		player->bet = m_MinBet;

		SendPlayer(player);

		gameThread->ResumeThread();
		return;
	}

	if (action == "call") {
		string betStr = str.substr(5);
		int bet = atoi(betStr.c_str());

		m_MinBet = bet;
		SendMinBet();

		player->bet = bet;
		SendPlayer(player);

		gameThread->ResumeThread();
		return;
	}

	if (action == "fold") {
		player->fold = true;
		m_Bank += player->bet;
		player->bet = -1;
		SendPlayer(player);

		gameThread->ResumeThread();
		return;
	}
}


void SendStart(const char* msg) {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SendMsg(PI->Socket, msg);

		PI = PI->Next;
	}
}

void SendPlayers() {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SendPlayer(PI);

		PI = PI->Next;
	}
}

void SendPlayer(PLAYER_INFORMATION* player) {
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SOCKET sctTo = PI->Socket;

		string msg = "player ";

		if (sctTo == player->Socket) {
			//send his own info
			msg.append("you ");
		}
		else {
			char socket[32];
			_itoa_s(player->Socket, socket, 10);

			msg.append(socket);
			msg.append(" ");
		}

		int bank = player->bank;
		char bankStr[16];
		_itoa_s(bank, bankStr, 10);
		msg.append(bankStr);
		msg.append(" ");

		int bet = player->bet;
		char betStr[16];
		_itoa_s(bet, betStr, 10);
		msg.append(betStr);

		SendMsg(sctTo, msg.c_str());

		PI = PI->Next;
	}
}

void SendWaiting(PLAYER_INFORMATION* player, int time) {
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SOCKET sctTo = PI->Socket;

		string msg = "wait ";
		
		if (sctTo == player->Socket) {
			//send his own info
			msg.append("you ");
		}
		else {
			char socket[32];
			_itoa_s(player->Socket, socket, 10);

			msg.append(socket);
			msg.append(" ");
		}

		char timeStr[8];
		_itoa_s(time, timeStr, 10);
		msg.append(timeStr);
		msg.append("\0");

		SendMsg(sctTo, msg.c_str());

		PI = PI->Next;
	}
}

void SendBank() {
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SOCKET sct = PI->Socket;

		string msg = "bank ";
		char bankStr[64];
		_itoa_s(m_Bank, bankStr, 10);
		msg.append(bankStr);

		SendMsg(sct, msg.c_str());

		PI = PI->Next;
	}
}

void SendMinBet() {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SOCKET sctTo = PI->Socket;
		
		char minBetStr[SIZE_MSG];
		sprintf_s(minBetStr, "minbet %d", m_MinBet);

		SendMsg(sctTo, minBetStr);

		PI = PI->Next;
	}
}

void CreateAndSendCard() {
	if (cards.size() > 4) {
		return;
	}

	Card card = RandomCard();
	cards.push_back(card);

	PLAYER_INFORMATION *PI = PlayerInfoList;

	while (PI)
	{
		SOCKET sct = PI->Socket;

		char Str[SIZE_MSG];
		sprintf_s(Str, sizeof(Str), "newc %d %d",
			card.m_Suit, card.m_Rank);

		SendMsg(sct, Str);

		PI = PI->Next;
	}
}

void SendPlayerDisconnect(PLAYER_INFORMATION* player) {
	PLAYER_INFORMATION *PI = PlayerInfoList;

	char msg[SIZE_MSG];
	sprintf_s(msg, "disconnect %d", player->Socket);	


	while (PI)
	{
		if (PI == player) {
			PI = PI->Next;
			continue;
		}

		SOCKET sctTo = PI->Socket;

		SendMsg(sctTo, msg);

		PI = PI->Next;
	}
}

int SendMsg(SOCKET sct, const char *msg) {
	CListBox *pLB = (CListBox *)(CListBox::FromHandle(hWnd_LB));

	char socket[32];
	_itoa_s(sct, socket, 10);

	string toConsole = "send ";
	toConsole.append(socket);
	toConsole.append(" ");
	toConsole.append(msg);

	pLB->AddString(toConsole.c_str());

	string Msg = msg;
	Msg.append(";");

	int	ret;
	ret = send(sct, Msg.c_str(), strlen(Msg.c_str()), 0);

	if (ret == SOCKET_ERROR) {
		char szBuffer[SIZE_MSG];

		sprintf_s(szBuffer, sizeof(szBuffer), "send() failed: %d", WSAGetLastError());
		pLB->AddString(szBuffer);

		FreePlayerInformation(sct);

		return -1;
	}

	return 0;
}