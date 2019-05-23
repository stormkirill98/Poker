
// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "Menu.h"
#include <string>

#include "atlimage.h"
#include "gdiplus.h"
using namespace Gdiplus;

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
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_MENU_EXIT, &CClientDlg::OnMenuExit)
	ON_COMMAND(ID_MENU_MAIN, &CClientDlg::OnMenuMain)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);

	ShowWindow(SW_MAXIMIZE);

	std::string temp = "player";
	strcpy_s(m_UserName, temp.c_str());

	/*CImage ci;
	LoadCard(&ci, DIAMONDS, ACE);
	CDC *dc = AfxGetMainWnd()->GetDC();
	HDC hdc = *dc;
	ci.Draw(hdc, 10, 10);*/
	DrawTable();
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

void CClientDlg::LoadCard(CImage* image, Suit suit, Rank rank) {
	CString path = "..\\images\\";
	switch(suit) {
		case CLUBS: path.Append("clubs\\"); break;
		case DIAMONDS: path.Append("diamonds\\"); break;
		case HEARTS: path.Append("hearts\\"); break;
		case SPADES: path.Append("spades\\"); break;
	}

	switch (rank) {
		case TWO: path.Append("2.gif"); break;
		case THREE: path.Append("3.gif"); break;
		case FOUR: path.Append("4.gif"); break;
		case FIVE: path.Append("5.gif"); break;
		case SIX: path.Append("6.gif"); break;
		case SEVEN: path.Append("7.gif"); break;
		case EIGHT: path.Append("8.gif"); break;
		case NINE: path.Append("9.gif"); break;
		case TEN: path.Append("10.gif"); break;
		case JACK: path.Append("j.gif"); break;
		case QUEEN: path.Append("q.gif"); break;
		case KING: path.Append("k.gif"); break;
		case ACE: path.Append("a.gif"); break;
	}

	image->Load(path);
}

void CClientDlg::LoadShirt(CImage* shirt) {
	shirt->Load((CString)"..\\images\\shirt.gif");
}

void CClientDlg::DrawTable() {
	CDC* dc = AfxGetMainWnd()->GetDC();

	CBrush MyBrush(RGB(20, 140, 10));
	dc->SelectObject(MyBrush);

	dc->Ellipse(100, 100, 1500, 650);
}