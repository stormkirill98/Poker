#pragma once


// Диалоговое окно Menu

class Menu : public CDialogEx
{
	DECLARE_DYNAMIC(Menu)

public:
	Menu(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~Menu();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENU };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedConnect();
	afx_msg void OnClickedDisconnect();
	afx_msg void OnBnClickedCancel();

	bool	m_IsConnected;
	SOCKET	m_sClient;
	void	SetConnected(bool IsConnected);

	CEdit m_Port;
	CEdit m_Server;
	CEdit m_UserName;
	virtual BOOL OnInitDialog();
};
