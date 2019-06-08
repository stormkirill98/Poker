#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mpr.lib")

#include "string"

class Menu : public CDialogEx
{
	DECLARE_DYNAMIC(Menu)

public:
	Menu(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~Menu();

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

	std::string	m_UserNameStr;
	bool	m_IsConnected;
	SOCKET	m_sClient;
	void	SetConnected(bool IsConnected);

	CEdit m_Port;
	CComboBox m_ServerCombo;
	CEdit m_UserName;
	virtual BOOL OnInitDialog();
	void GetListPCIp();
	afx_msg void OnClose();
};
