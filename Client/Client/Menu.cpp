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

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Исключение: страница свойств OCX должна возвращать значение FALSE
}

void Menu::OnClickedConnect()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void Menu::OnClickedDisconnect()
{
	// TODO: добавьте свой код обработчика уведомлений
}


void Menu::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}



