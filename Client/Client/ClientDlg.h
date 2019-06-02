
// ClientDlg.h : header file
//

#pragma once
#include "Card.h"
#include "Enums.h"
#include "Player.h"
#include <vector>

using namespace std;



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
	void InitPlayersTest();
	void InitPositions();
	DECLARE_MESSAGE_MAP()

public:


	bool m_IsConnected = false;
	SOCKET	m_sClient = NULL;

	std::string m_UserName;
	int m_MinBet = 0,
		m_MaxBet = 100,
		m_Bet = 0;
	int m_CashGain = 0;
	Card m_UserCards[2];

public:
	CEdit* timer;
	int time = 30;

	int countPlayers = 3;
	vector<Player> players;

	vector<CRect> posInfo;
	vector<CRect> posCards;

	CEdit m_BetText;
	CEdit m_CashGainText;
	CSliderCtrl m_SliderBet;

	void LoadCard(CImage* card, int suit, int rank);
	void LoadShirt(CImage* shirt);
	void DrawTable();
	void ShowUserCards(Card card1, Card card2);

	void ShowPlayers();
	void ShowPlayerCards(int index);

	double CountAngle(int x, int y);
	void RotateImg(Gdiplus::Graphics * graphics, int x, int y, int angle);
	void ImgToStream(CImage* img, IStream ** ppStream);

	void SetBet(int bet);
	void SetRangeBet(int min, int max);
	void SetCashGain(int cashGain);

	void StartCountDown(int playerIndex);

	void SendMsg(char* msg);
	void ShowError(int error);

	void StopTimer();

public:
	afx_msg void OnMenuExit();
	afx_msg void OnMenuMain();

	afx_msg void OnPaint();

	afx_msg void OnClickedCheck();
	afx_msg void OnClickedCall();
	afx_msg void OnClickedFold();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	static UINT RecvThread(PVOID lpParam);
	void ParseMsg(char * msg);
	void GetCards(const char * cards);
};