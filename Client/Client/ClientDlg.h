
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
	enum Suit {CLUBS, DIAMONDS, HEARTS, SPADES};
	enum Rank {TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE};

	bool m_IsConnected = false;
	SOCKET	m_sClient;
	char m_UserName[256];

	void LoadCard(CImage* image, Suit suit, Rank rank);
	void LoadShirt(CImage* shirt);

	afx_msg void OnMenuExit();
	afx_msg void OnMenuMain();
};
