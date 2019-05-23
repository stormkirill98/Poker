
// ClientDlg.h : header file
//

#pragma once


// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	bool m_IsConnected = false;
	SOCKET	m_sClient;
	char m_UserName[256];

	int countPlayers = 2;	

	void LoadCard(CImage* card, int suit, int rank);
	void LoadShirt(CImage* shirt);
	void DrawTable();
	void ShowPlayerCards(int suit1, int rank1, int suit2, int rank2);

	afx_msg void OnMenuExit();
	afx_msg void OnMenuMain();
	afx_msg void OnClickedTest();
};
