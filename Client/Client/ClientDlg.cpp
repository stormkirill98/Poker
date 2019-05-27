
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "Menu.h"
#include <string>

#include "Enums.h"

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

	return TRUE;  // return TRUE  unless you set the focus to a control
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

void CClientDlg::ShowPlayerCards(int suit1, int rank1, int suit2, int rank2)
{
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

}

void CClientDlg::OnClickedTest()
{
	DrawTable();

	ShowPlayerCards(Suit::CLUBS, Rank::ACE, Suit::DIAMONDS, Rank::ACE);
}

void CClientDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (HTCAPTION != nHitTest)
	{
		CDialogEx::OnNcLButtonDown(nHitTest, point); // CDialog или что у тебя там уже сам подставишь
	}
}

void CClientDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	if (HTCAPTION != nHitTest)
	{
		CDialogEx::OnNcLButtonDown(nHitTest, point); // CDialog или что у тебя там уже сам подставишь
	}
}


void CClientDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: добавьте свой код обработчика сообщений
					   // Не вызывать CDialogEx::OnPaint() для сообщений рисования

	DrawTable();
}
