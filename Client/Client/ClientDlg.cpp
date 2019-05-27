
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

	Player player1 = Player("player1");
	player1.SetBank(900);
	Player player2 = Player("player2");
	player1.SetBank(600);
	Player player3 = Player("player3");
	player1.SetBank(1500);
	players.push_back(player1);
	players.push_back(player2);
	players.push_back(player3);

	InitPositions();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::InitPositions() {
	CRect rcInfo;
	rcInfo.left = 500;
	rcInfo.top = 40;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 300;
	rcInfo.top = 80;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 100;
	rcInfo.top = 150;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 10;
	rcInfo.top = 300;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 40;
	rcInfo.top = 480;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 200;
	rcInfo.top = 570;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 420;
	rcInfo.top = 630;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 1050;
	rcInfo.top = 630;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 1300;
	rcInfo.top = 570;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 1450;
	rcInfo.top = 480;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 1480;
	rcInfo.top = 300;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 1390;
	rcInfo.top = 150;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 1190;
	rcInfo.top = 80;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);

	rcInfo.left = 990;
	rcInfo.top = 40;
	rcInfo.right = rcInfo.left + 120;
	rcInfo.bottom = rcInfo.top + 60;
	posInfo.push_back(rcInfo);
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

void CClientDlg::ShowPlayerCards(Card card1, Card card2)
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

	Player player = players.back();

	for (int i = 0; i < posInfo.size(); i++) {
		CRect rcInfo = posInfo.at(i);
		dc->Rectangle(rcInfo);

		dc->TextOut(rcInfo.left + 5, rcInfo.top + 5, player.GetName());
		string bankStr = "Bank: " + to_string(player.GetBank());
		dc->TextOut(rcInfo.left + 5, rcInfo.top + 22, bankStr.c_str());
		string betStr = "Bet: " + to_string(player.GetBet());
		dc->TextOut(rcInfo.left + 5, rcInfo.top + 40, betStr.c_str());
	}

	
	

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
	ShowPlayerCards(userCards[0], userCards[1]);
	ShowPlayers();
}