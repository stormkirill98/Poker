#pragma managed(push, off)

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "Menu.h"
#include <string>
#include <cmath>
#include <conio.h>
#include <iostream>

#include <GdiPlusHeaders.h>
#include <GdiPlusColor.h>

#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_UPDATE_TIMER (WM_USER+1)

#define PI 3.14159265
#define SIZE_MSG 4096

#define TIME_POLL 15
#define TIME_START 15

typedef struct THREADSTRUCT
{
	CClientDlg* _this;
} THREADSTRUCT;

CClientDlg* _this;

CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BET, m_SliderBet);
	DDX_Control(pDX, IDC_BET, m_BetText);
	DDX_Control(pDX, IDC_CASHGAIN, m_CashGainText);
	DDX_Control(pDX, IDC_CHECK, btn_Check);
	DDX_Control(pDX, IDC_CALL, btn_Call);
	DDX_Control(pDX, IDC_FOLD, btn_Fold);
	DDX_Control(pDX, IDC_BANK, m_BankOutput);
	DDX_Control(pDX, IDC_START, m_CountBackStart);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_COMMAND(ID_MENU_EXIT, &CClientDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_MAIN, &CClientDlg::OnMenuMain)
	ON_MESSAGE(WM_UPDATE_TIMER, &CClientDlg::OnUpdateTimer)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK, &CClientDlg::OnClickedCheck)
	ON_BN_CLICKED(IDC_CALL, &CClientDlg::OnClickedCall)
	ON_BN_CLICKED(IDC_FOLD, &CClientDlg::OnClickedFold)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

#ifdef _DEBUG
	if (!AllocConsole())
		AfxMessageBox("Failed to create the console!", MB_ICONEXCLAMATION);
#endif

	_this = this;

	ShowWindow(SW_MAXIMIZE);

	m_UserCards[0] = Card(-1, -1);
	m_UserCards[1] = Card(-1, -1);

	m_SliderBet.SetRange(m_MinBet, m_MaxBet);

	Init();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::Init() {
	CRect rcInfo;
	CRect rcCard;
	CRect rcTimer;
	CEdit* ceTimer;
	int x, y, height;
	
	x = 650;
	y = 710;
	height = 50;

	CFont *myFont1 = new (CFont);
	myFont1->CreateFontA(100, 0, 0, 0, FW_HEAVY, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Courier New"));
	m_CountBackStart.SetFont(myFont1);


	CFont *myFont = new (CFont);
	myFont->CreateFontA(height, 0, 0, 0, FW_HEAVY, false, false,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		FIXED_PITCH | FF_MODERN, _T("Courier New"));

	timer = new(CEdit);
	CRect size = CRect(x, y - height, x + (height * 1.3), y);
	timer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		size, this, 105);
	timer->ShowWindow(SW_HIDE);
	timer->SetFont(myFont);

	rcInfo.left = 500;
	rcInfo.top = 40;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 550;
	rcCard.top = 150;
	posCards.push_back(rcCard);


	rcInfo.left = 300;
	rcInfo.top = 80;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 360;
	rcCard.top = 200;
	posCards.push_back(rcCard);



	rcInfo.left = 100;
	rcInfo.top = 150;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 220;
	rcCard.top = 260;
	posCards.push_back(rcCard);


	rcInfo.left = 10;
	rcInfo.top = 300;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 140;
	rcCard.top = 370;
	posCards.push_back(rcCard);


	rcInfo.left = 40;
	rcInfo.top = 480;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 170;
	rcCard.top = 500;
	posCards.push_back(rcCard);


	rcInfo.left = 200;
	rcInfo.top = 570;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 320;
	rcCard.top = 560;
	posCards.push_back(rcCard);


	rcInfo.left = 420;
	rcInfo.top = 630;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 500;
	rcCard.top = 620;
	posCards.push_back(rcCard);


	rcInfo.left = 1050;
	rcInfo.top = 630;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 1090;
	rcCard.top = 570;
	posCards.push_back(rcCard);


	rcInfo.left = 1300;
	rcInfo.top = 570;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 1310;
	rcCard.top = 510;
	posCards.push_back(rcCard);


	rcInfo.left = 1450;
	rcInfo.top = 480;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 1450;
	rcCard.top = 410;
	posCards.push_back(rcCard);


	rcInfo.left = 1480;
	rcInfo.top = 300;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 1440;
	rcCard.top = 290;
	posCards.push_back(rcCard);


	rcInfo.left = 1390;
	rcInfo.top = 150;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 1340;
	rcCard.top = 180;
	posCards.push_back(rcCard);


	rcInfo.left = 1190;
	rcInfo.top = 80;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 1150;
	rcCard.top = 140;
	posCards.push_back(rcCard);


	rcInfo.left = 990;
	rcInfo.top = 40;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	x = rcInfo.left;
	y = rcInfo.top;
	height = 30;
	rcTimer = CRect(x, y - height, x + (height * 1.3), y);
	ceTimer = new CEdit;
	ceTimer->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_NOHIDESEL | ES_READONLY,
		rcTimer, this, 105);
	ceTimer->ShowWindow(SW_HIDE);
	timers.push_back(ceTimer);

	rcCard.left = 980;
	rcCard.top = 110;
	posCards.push_back(rcCard);
}

void CClientDlg::OnMenuExit()
{
	this->DestroyWindow();
}

void CClientDlg::OnMenuMain()
{
	Menu menu;

	menu.m_IsConnected = m_IsConnected;
	menu.m_sClient = m_sClient;

	menu.DoModal();

	if (menu.m_sClient == NULL) {
		menu.DestroyWindow();
		return;
	}

	ClearFields();

	m_IsConnected = menu.m_IsConnected;
	m_sClient = menu.m_sClient;

	sockaddr_in client_info;
	int addrsize = sizeof(client_info);
	getsockname(m_sClient, (struct sockaddr *)&client_info, &addrsize);
	char *ip = inet_ntoa(client_info.sin_addr);
	m_Ip = ip;

	THREADSTRUCT *_param = new THREADSTRUCT;
	_param->_this = this;
	recvThread = AfxBeginThread(RecvThread, _param);

	menu.DestroyWindow();
}

void CClientDlg::OnPaint() {
	CPaintDC dc(this);

	DrawTable();
	ShowUserCards(m_UserCards[0], m_UserCards[1]);
	ShowPlayers();
	ShowCards();

	for (int i = 0; i < players.size(); i++) {
		ShowPlayerCards(i);
	}
}


void CClientDlg::LoadCard(CImage* card, int suit, int rank)
{
	CString path = "..\\images\\";

	switch (suit) {
	case Suit::CLUBS: path.Append("clubs\\"); break;
	case Suit::DIAMONDS: path.Append("diamonds\\"); break;
	case Suit::HEARTS: path.Append("hearts\\"); break;
	case Suit::SPADES: path.Append("spades\\"); break;
	}

	switch (rank) {
	case Rank::TWO: path.Append("2.gif"); break;
	case Rank::THREE: path.Append("3.gif"); break;
	case Rank::FOUR: path.Append("4.gif"); break;
	case Rank::FIVE: path.Append("5.gif"); break;
	case Rank::SIX: path.Append("6.gif"); break;
	case Rank::SEVEN: path.Append("7.gif"); break;
	case Rank::EIGHT: path.Append("8.gif"); break;
	case Rank::NINE: path.Append("9.gif"); break;
	case Rank::TEN: path.Append("10.gif"); break;
	case Rank::JACK: path.Append("j.gif"); break;
	case Rank::QUEEN: path.Append("q.gif"); break;
	case Rank::KING: path.Append("k.gif"); break;
	case Rank::ACE: path.Append("a.gif"); break;
	}

	card->Load(path);
}

void CClientDlg::LoadShirt(CImage* shirt) {
	shirt->Load((CString)"..\\images\\shirt.gif");
}


void CClientDlg::DrawTable() {
	CDC* dc = AfxGetMainWnd()->GetDC();

	CBrush MyBrush(RGB(20, 140, 10));
	dc->SelectObject(MyBrush);

	CRect window;
	GetWindowRect(&window);

	CRect ellipse;
	ellipse.left = window.right * 0.06;
	ellipse.top = window.bottom * 0.1;
	ellipse.right = window.right * 0.94;
	ellipse.bottom = window.bottom * 0.75;

	dc->Ellipse(ellipse);
}

void CClientDlg::ShowUserCards(Card card1, Card card2)
{
	int suit1 = card1.m_Suit,
		rank1 = card1.m_Rank,
		suit2 = card2.m_Suit,
		rank2 = card2.m_Rank;

	if (suit1 == -1 || rank1 == -1 ||
		suit2 == -1 || rank2 == -1) {
		return;
	}

	CImage ImgCard1;
	LoadCard(&ImgCard1, suit1, rank1);
	CImage ImgCard2;
	LoadCard(&ImgCard2, suit2, rank2);
	if (ImgCard1 == NULL || ImgCard2 == NULL) {
		return;
	}

	CRect window;
	GetWindowRect(&window);

	int SpaceCards = 10;

	int y1 = window.bottom * 0.75 - ImgCard1.GetHeight();
	int x1 = (window.right / 2) - (ImgCard1.GetWidth() + SpaceCards) / 2;
	
	int y2 = window.bottom * 0.75 - ImgCard1.GetHeight();
	int x2 = (window.right / 2) + (ImgCard1.GetWidth() + SpaceCards) / 2;

	CDC *dc = AfxGetMainWnd()->GetDC();
	HDC hdc = *dc;
	
	ImgCard1.Draw(hdc, x1, y1);
	ImgCard2.Draw(hdc, x2, y2);
}

void CClientDlg::ShowPlayers() {
	CDC* dc = AfxGetMainWnd()->GetDC();

	for (int i = 0; i < players.size(); i++) {
		CRect rcInfo = posInfo.at(i);
		Player player = players.at(i);
		dc->Rectangle(rcInfo);

		char sct[16];
		_itoa_s(player.GetSct(), sct, 10);

		dc->TextOut(rcInfo.left + 5, rcInfo.top + 5, sct);
		string bankStr = "Bank: " + to_string(player.GetBank());
		dc->TextOut(rcInfo.left + 5, rcInfo.top + 22, bankStr.c_str());
		string betStr = "Bet: " + to_string(player.GetBet());
		dc->TextOut(rcInfo.left + 5, rcInfo.top + 40, betStr.c_str());
	}
}

void CClientDlg::ShowPlayerCards(int index) {
	if (index >= posCards.size() ||	index >= posInfo.size()) {
		return;
	}

	if (players.at(index).IsFold()) {
		return;
	}

	CRect pos = posCards.at(index);
	int x = pos.left,
		y = pos.top;

	double angle = CountAngle(x, y);
	if (index < 7) {
		angle *= -1;
	}

	for (int i = 0; i < 2; i++, x += 30) {
		CImage shirt;
		LoadShirt(&shirt);
		if (shirt == NULL) {
			return;
		}

		CDC *dc = AfxGetMainWnd()->GetDC();
		HDC hdc = *dc;

		IStream* ps;
		ImgToStream(&shirt, &ps);

		Gdiplus::Image* img = Gdiplus::Image::FromStream(ps);
		Gdiplus::Graphics graphics(hdc);

		RotateImg(&graphics, x, y, angle);
	
		graphics.DrawImage(img, Gdiplus::PointF(0, 0));
	}
}

void CClientDlg::ShowCards() {
	int x = 620,
		y = 320;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		CImage imgCard;
		LoadCard(&imgCard, card.m_Suit, card.m_Rank);

		if (imgCard == NULL) {
			return;
		}

		CDC *dc = AfxGetMainWnd()->GetDC();
		HDC hdc = *dc;

		imgCard.Draw(hdc, x, y);

		x += 70;
	}
}


double CClientDlg::CountAngle(int x, int y) {
	CRect window;
	GetWindowRect(&window);

	int xCenter = window.Width() / 2,
		yCenter = window.Height() / 2 - 70;

	double cathetus = yCenter - y,
		   hipotenuse = sqrt(pow(xCenter - x, 2) + pow(yCenter - y, 2));

	double cosAngle = cathetus / hipotenuse;

	return acos(cosAngle) * 180.0 / PI;
}

void CClientDlg::RotateImg(Gdiplus::Graphics* graphics, 
							int x, int y, int angle) {
	graphics->TranslateTransform(x, y, Gdiplus::MatrixOrderAppend);
	graphics->RotateTransform(angle);
}

void CClientDlg::ImgToStream(CImage* img, IStream **ppStream) {
	HBITMAP  hsrc = img->Detach();
	img->Attach(hsrc);

	CImage   cimg;
	IStream* ps = NULL;
	HRESULT  hr = CreateStreamOnHGlobal(NULL, TRUE, &ps);

	if (FAILED(hr))
		return;

	cimg.Attach(hsrc);
	hr = cimg.Save(ps, Gdiplus::ImageFormatBMP);
	if (FAILED(hr))
	{
		ps->Release();
		return;
	}

	*ppStream = ps;
}


void CClientDlg::SetFoldPlayers(bool isFold) {
	for (int i = 0; i < players.size(); i++) {
		Player *player = &players.at(i);
		player->SetFold(isFold);
	}
}

void CClientDlg::SetBet(int bet)
{
	if (bet < 0) {
		bet = 0;
	}

	m_Bet = bet;
	m_SliderBet.SetPos(bet);

	CString text = to_string(bet).c_str();
	m_BetText.SetWindowTextA(text);
}

void CClientDlg::SetRangeBet(int min, int max)
{
	if (min > max) {
		return;
	}
	if (min < 0) {
		return;
	}

	m_MinBet = min;
	m_MaxBet = max;

	m_SliderBet.SetRange(m_MinBet, m_MaxBet);
}

void CClientDlg::SetCashGain(int cashGain)
{
	if (cashGain < 0) {
		cashGain = 0;
	}

	m_CashGain = cashGain;

	CString text = to_string(cashGain).c_str();
	m_CashGainText.SetWindowTextA(text);
}


void CClientDlg::SendMsg(char* msg)
{
	int	ret;

	ret = send(m_sClient, msg, strlen(msg), 0);

	if (ret == SOCKET_ERROR)
	{
		ShowError(GetLastError());
		return;
	}
}

void CClientDlg::ShowError(int error)
{
	string msg = "Error: " + to_string(error);

	AfxMessageBox(msg.c_str());
}


void CClientDlg::OnClickedCheck()
{
	btn_Call.EnableWindow(false);
	btn_Check.EnableWindow(false);
	btn_Fold.EnableWindow(false);

	timer->ShowWindow(SW_HIDE);

	char szMessage[SIZE_MSG] = "check";

	SendMsg(szMessage);
}

void CClientDlg::OnClickedCall()
{
	btn_Call.EnableWindow(false);
	btn_Check.EnableWindow(false);
	btn_Fold.EnableWindow(false);

	timer->ShowWindow(SW_HIDE);

	char szMessage[SIZE_MSG];
	string msg = "call ";
	msg.append(to_string(m_Bet));

	sprintf_s(szMessage, sizeof(szMessage), msg.c_str());

	SendMsg(szMessage);
}

void CClientDlg::OnClickedFold()
{
	btn_Call.EnableWindow(false);
	btn_Check.EnableWindow(false);
	btn_Fold.EnableWindow(false);

	timer->ShowWindow(SW_HIDE);

	char szMessage[SIZE_MSG] = "fold";

	SendMsg(szMessage);
}


void CClientDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* sliderBet = reinterpret_cast<CSliderCtrl*>(pScrollBar);
	
	int value = sliderBet->GetPos();
	SetBet(value);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CClientDlg::OnUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	int index = (int)lParam;
	int time = (int)wParam;

	char strTime[128];
	_itoa_s(time, strTime, 10);

	CEdit* bTimer;
	if (index == -1) {
		bTimer = timer;
	}
	else {
		bTimer = timers.at(index);
	}

	if (time == TIME_POLL) {																					
		bTimer->ShowWindow(SW_SHOW);

		btn_Call.EnableWindow(true);
		btn_Check.EnableWindow(true);
		btn_Fold.EnableWindow(true);
	}

	if (time == 0) {
		btn_Call.EnableWindow(false);
		btn_Check.EnableWindow(false);
		btn_Fold.EnableWindow(false);
	}
	
	bTimer->SetWindowTextA(strTime);

	return LRESULT();
}


UINT CClientDlg::RecvThread(PVOID lpParam) {
	THREADSTRUCT*    ts = (THREADSTRUCT*)lpParam;
	CClientDlg* _this = ts->_this;
	SOCKET* sClient = &_this->m_sClient;

	char szBuffer[SIZE_MSG];
	int	ret;

	char Str[256];

	do {
		ret = recv(*sClient, szBuffer, SIZE_MSG, 0);

		if (ret == 0) {	// Корректное завершение
			return 0;
		} else if (ret == SOCKET_ERROR)
		{
			int error = WSAGetLastError();

			if (error == WSAECONNRESET) {
				AfxMessageBox("Connection with server is lost");
				continue;
			}

			if (error == WSAECONNABORTED || error == WSAENOTSOCK) {
				if (_this->parseThread != NULL) {
					DWORD exit_code = NULL;
					GetExitCodeThread(_this->parseThread->m_hThread, &exit_code);

					if (exit_code == STILL_ACTIVE)
					{
						TerminateThread(_this->parseThread->m_hThread, 0);
						CloseHandle(_this->parseThread->m_hThread);
					}
				}

				closesocket(*sClient);
				_this->ClearFields();

				ExitThread(NULL);
			}

			sprintf_s(Str, sizeof(Str), "recv() failed: %d", error);
			AfxMessageBox(Str);
			continue;
		}
		szBuffer[ret] = '\0';

		if (_this->parseThread != NULL)
		{
			DWORD exit_code = NULL;
			GetExitCodeThread(_this->parseThread->m_hThread, &exit_code);

			if (exit_code == STILL_ACTIVE)
			{
				::TerminateThread(_this->parseThread->m_hThread, 0);
				CloseHandle(_this->parseThread->m_hThread);
			}
			_this->parseThread->m_hThread = NULL;
			_this->parseThread = NULL;
		}

		_this->parseThread = AfxBeginThread(ParseMsg, (LPVOID)szBuffer);
		WaitForSingleObject(_this->parseThread->m_hThread, 35000);
	} while (ret > 0);

	return 0;
}


UINT CClientDlg::ParseMsg(LPVOID lParam) {
	char* Msg = (char*)lParam;

	vector<string> msgs;
	_this->SplitStr(Msg, &msgs, ';');

	string buf = Msg;
	buf.append("\n");
	_cprintf(buf.c_str());

	for (string msg : msgs) {
		string command = msg.substr(0, 4);

		if (command == "card") {
			_this->CommandCards(msg.substr(5).c_str());
			_this->ShowUserCards(_this->m_UserCards[0], _this->m_UserCards[1]);
			_this->SetFoldPlayers(false);
			_this->cards.clear();
			_this->InvalidateRect(NULL);
			continue;
		}

		if (command == "play") {
			msg = msg.substr(6);
			_this->CommandPlayer(msg.c_str());
			_this->InvalidateRect(NULL);
			continue;
		}

		if (command == "wait") {
			CommandWait(msg.substr(5).c_str());
			continue;
		}

		if (command == "minb") {
			_this->CommandMinBet(msg.substr(6).c_str());
			continue;
		}

		if (command == "newc") {
			_this->CommandNewCard(msg.substr(5).c_str());
			_this->InvalidateRect(NULL);
			continue;
		}

		if (command == "bank") {
			_this->CommandBank(msg.substr(5).c_str());
			continue;
		}

		if (command == "star") {
			_this->CommandStart(msg.substr(6).c_str());
			continue;
		}

		if (command == "disc") {
			_this->CommandDisconnect(msg.substr(11).c_str());
			continue;
		}
	}

	return 0;
}

void CClientDlg::SplitStr(const char* str, vector<string>* res, char separator) {
	string line = str;
	string buffer = "";      //буфферная строка

	for (int i = 0; i < line.size(); i++) {
		if (line[i] != separator) {
			buffer += line[i];
		}
		else {
			if (buffer == "") {
				continue;
			}

			res->push_back(buffer);
			buffer = "";
		}
	}
	if (buffer != "") {
		res->push_back(buffer);
	}
}


void CClientDlg::CommandBank(const char *str) {
	SetCashGain(atoi(str));
}

void CClientDlg::CommandMinBet(const char *str) {
	int minBet = atoi(str);
	m_MinBet = minBet;
	m_SliderBet.SetRangeMin(minBet);
	if (m_Bank < m_MinBet) {
		m_Bet = m_Bank;
	}
	else {
		m_Bet = m_MinBet;

	}
	m_SliderBet.SetPos(m_Bet);
	m_BetText.SetWindowTextA(str);
}

void CClientDlg::CommandNewCard(const char *msg) {
	vector<string> nums;

	SplitStr(msg, &nums);

	int suit = atoi(nums.at(0).c_str());
	int rank = atoi(nums.at(1).c_str());

	Card card = Card(suit, rank);
	cards.push_back(card);
}

void CClientDlg::CommandPlayer(const char* command) {
	vector<string> args;
	SplitStr(command, &args);

	string sctStr = args.at(0);
	int bank = atoi(args.at(1).c_str());
	int bet = atoi(args.at(2).c_str());

	if (sctStr == "you") {
		m_Bank = bank;
		m_MaxBet = bank;
		m_SliderBet.SetRangeMax(m_MaxBet);
		m_BankOutput.SetWindowTextA(to_string(bank).c_str());
		return;
	}

	int sct = atoi(sctStr.c_str());

	if (!PlayerIsCreated(sct)) {
		CreatePlayer(sct, bank, bet);
		return;
	}

	Player* player = GetPlayer(sct);

	int indexPlayer = GetPlayerIndex(sct);
	timers.at(indexPlayer)->ShowWindow(SW_HIDE);

	if (player == nullptr) {
		string msg = "No find player with ";
		msg.append(sctStr);
		AfxMessageBox(msg.c_str());
		return;
	}

	player->SetBank(bank);
	if (bet < 0) {
		player->SetFold(true);
	}
	else {
		player->SetBet(bet);
	}
}

void CClientDlg::CommandCards(const char* cards) {
	vector<string> nums;

	SplitStr(cards, &nums);

	int suit1 = atoi(nums.at(0).c_str());
	int rank1 = atoi(nums.at(1).c_str());

	int suit2 = atoi(nums.at(2).c_str());
	int rank2 = atoi(nums.at(3).c_str());

	m_UserCards[0] = Card(suit1, rank1);
	m_UserCards[1] = Card(suit2, rank2);
}

void CClientDlg::CommandWait(const char* command) {
	vector<string> args;
	_this->SplitStr(command, &args);

	int sct = atoi(args[0].c_str());
	int time = atoi(args[1].c_str());

	int index = _this->GetPlayerIndex(sct);

	::SendMessage(_this->m_hWnd, WM_UPDATE_TIMER, time, index);
}

void CClientDlg::CommandStart(const char* command) {
	int time = atoi(command);

	if (time == TIME_START) {
		m_CountBackStart.ShowWindow(SW_SHOW);
	}

	if (time == 0) {
		m_CountBackStart.ShowWindow(SW_HIDE);
	}

	char str[32];
	sprintf_s(str, "start %d", time);

	m_CountBackStart.SetWindowTextA(str);
}

void CClientDlg::CommandDisconnect(const char* command) {
	int playerIndex = GetPlayerIndex(atoi(command));
	if (playerIndex == -1) {
		return;
	}

	players.erase(players.begin() + playerIndex);
	timers.at(playerIndex)->ShowWindow(SW_HIDE);

	InvalidateRect(NULL);
}

bool CClientDlg::PlayerIsCreated(int sct) {
	for (Player player : players) {
		if (player.GetSct() == sct) {
			return true;
		}
	}

	return false;
}

void CClientDlg::CreatePlayer(int sct, int bank, int bet) {
	Player player = Player(sct);

	player.SetBank(bank);
	player.SetBet(bet);
	player.SetFold(true);

	players.push_back(player);
}


Player* CClientDlg::GetPlayer(int sct) {
	for (int i = 0; i < players.size(); i++) {
		Player* player = &players.at(i);
		if (player->GetSct() == sct) {
			return player;
		}
	}

	return nullptr;
}

int CClientDlg::GetPlayerIndex(int sct) {
	for (int i = 0; i < players.size(); i++) {
		Player* buf = &players.at(i);

		if (buf->GetSct() == sct) {
			return i;
		}
	}

	return -1;
}


void CClientDlg::ClearFields() {
	m_CountBackStart.ShowWindow(SW_HIDE);
	timer->ShowWindow(SW_HIDE);

	players.clear();
	cards.clear();

	btn_Call.EnableWindow(false);
	btn_Check.EnableWindow(false);
	btn_Fold.EnableWindow(false);

	m_BetText.SetWindowTextA("");
	m_CashGainText.SetWindowTextA("");
	m_BankOutput.SetWindowTextA("");

	m_SliderBet.SetRange(0, 100);
	m_SliderBet.SetPos(0);

	for (CEdit* timer : timers) {
		timer->ShowWindow(SW_HIDE);
	}

	m_UserCards[0] = Card(-1, -1);
	m_UserCards[1] = Card(-1, -1);

	m_IsConnected = false;
	m_sClient = NULL;
	m_Ip = "";

	m_MinBet = 0;
	m_MaxBet = 100;
	m_Bet = 0;
	m_Bank = 0;
	m_CashGain = 0;

	InvalidateRect(NULL);
}