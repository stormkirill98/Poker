
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "Menu.h"
#include <string>
#include <cmath>

#include <GdiPlusHeaders.h>
#include <GdiPlusColor.h>

#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.14159265


CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	userCards[0] = Card(-1, -1);
	userCards[1] = Card(-1, -1);

	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_COMMAND(ID_MENU_EXIT, &CClientDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_MAIN, &CClientDlg::OnMenuMain)
	ON_BN_CLICKED(IDC_TEST, &CClientDlg::OnClickedTest)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);

	ShowWindow(SW_MAXIMIZE);

	std::string temp = "player";
	strcpy_s(m_UserName, temp.c_str());

	InitPlayersTest();
	InitPositions();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::InitPlayersTest() {
	Player player = Player("player1");
	player.SetBank(900);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player2");
	player.SetBank(2333);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player3");
	player.SetBank(7575);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player4");
	player.SetBank(1111);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player5");
	player.SetBank(764);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player6");
	player.SetBank(12333);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player7");
	player.SetBank(200);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player8");
	player.SetBank(9111);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player9");
	player.SetBank(228);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player10");
	player.SetBank(1000);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player11");
	player.SetBank(1200);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player12");
	player.SetBank(1234);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player13");
	player.SetBank(977);
	player.SetFold(false);
	players.push_back(player);

	player = Player("player14");
	player.SetBank(100);
	player.SetFold(false);
	players.push_back(player);
}

void CClientDlg::InitPositions() {
	CRect rcInfo;
	CRect rcCard;

	rcInfo.left = 500;
	rcInfo.top = 40;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 550;
	rcCard.top = 150;
	posCards.push_back(rcCard);

	rcInfo.left = 300;
	rcInfo.top = 80;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 360;
	rcCard.top = 200;
	posCards.push_back(rcCard);

	rcInfo.left = 100;
	rcInfo.top = 150;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 220;
	rcCard.top = 260;
	posCards.push_back(rcCard);

	rcInfo.left = 10;
	rcInfo.top = 300;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 140;
	rcCard.top = 370;
	posCards.push_back(rcCard);

	rcInfo.left = 40;
	rcInfo.top = 480;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 170;
	rcCard.top = 500;
	posCards.push_back(rcCard);

	rcInfo.left = 200;
	rcInfo.top = 570;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 320;
	rcCard.top = 560;
	posCards.push_back(rcCard);

	rcInfo.left = 420;
	rcInfo.top = 630;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 500;
	rcCard.top = 620;
	posCards.push_back(rcCard);

	rcInfo.left = 1050;
	rcInfo.top = 630;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 1090;
	rcCard.top = 570;
	posCards.push_back(rcCard);

	rcInfo.left = 1300;
	rcInfo.top = 570;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 1310;
	rcCard.top = 510;
	posCards.push_back(rcCard);

	rcInfo.left = 1450;
	rcInfo.top = 480;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 1450;
	rcCard.top = 410;
	posCards.push_back(rcCard);

	rcInfo.left = 1480;
	rcInfo.top = 300;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 1440;
	rcCard.top = 290;
	posCards.push_back(rcCard);

	rcInfo.left = 1390;
	rcInfo.top = 150;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 1340;
	rcCard.top = 180;
	posCards.push_back(rcCard);

	rcInfo.left = 1190;
	rcInfo.top = 80;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcCard.left = 1150;
	rcCard.top = 140;
	posCards.push_back(rcCard);

	rcInfo.left = 990;
	rcInfo.top = 40;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

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
	strcpy_s(menu.m_UserNameStr, m_UserName);

	menu.DoModal();

	m_IsConnected = menu.m_IsConnected;
	m_sClient = menu.m_sClient;
	strcpy_s(m_UserName, menu.m_UserNameStr);

	menu.DestroyWindow();
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

		dc->TextOut(rcInfo.left + 5, rcInfo.top + 5, player.GetName());
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

void CClientDlg::OnClickedTest() {
	DrawTable();

	userCards[0] = Card(Suit::DIAMONDS, Rank::JACK);
	userCards[1] = Card(Suit::CLUBS, Rank::QUEEN);

	InvalidateRect(NULL, TRUE);
}

void CClientDlg::OnPaint() {
	CPaintDC dc(this); // device context for painting
					   // TODO: добавьте свой код обработчика сообщений
					   // Не вызывать CDialogEx::OnPaint() для сообщений рисования

	DrawTable();
	ShowUserCards(userCards[0], userCards[1]);
	ShowPlayers();

	for (int i = 0; i < players.size(); i++) {
		ShowPlayerCards(i);
	}

	//draw center
	CRect window;
	GetWindowRect(&window);
	int xCenter = window.Width() / 2,
		yCenter = window.Height() / 2 - 70;
	dc.Ellipse(xCenter, yCenter, xCenter + 5, yCenter + 5);
}