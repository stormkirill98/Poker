
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
	void InitPositions();
	DECLARE_MESSAGE_MAP()

public:
	bool m_IsConnected = false;
	SOCKET	m_sClient = NULL;
	string m_Ip = "";

	string m_UserName;
	int m_MinBet = 0,
		m_MaxBet = 100,
		m_Bet = 0,
		m_Bank = 0;
	int m_CashGain = 0;
	Card m_UserCards[2];

	CWinThread* recvThread;
	CWinThread* timerThread;
	CWinThread* parseThread;

public:
	CEdit* timer;
	int m_Time = 30;

	int countPlayers = 3;
	vector<Player> players;
	vector<Card> cards;

	vector<CRect> posInfo;
	vector<CRect> posCards;
	vector<CEdit*> timers;

	CEdit m_BetText;
	CEdit m_CashGainText;
	CSliderCtrl m_SliderBet;
	CEdit m_BankOutput;


	void LoadCard(CImage* card, int suit, int rank);
	void LoadShirt(CImage* shirt);
	void DrawTable();
	void ShowUserCards(Card card1, Card card2);

	void ShowPlayers();
	void ShowPlayerCards(int index);

	double CountAngle(int x, int y);
	void RotateImg(Gdiplus::Graphics * graphics, int x, int y, int angle);
	void ImgToStream(CImage* img, IStream ** ppStream);

	void SetFoldPlayers(bool isFold);

	void SetBet(int bet);
	void SetRangeBet(int min, int max);
	void SetCashGain(int cashGain);

	void SendMsg(char* msg);
	void ShowError(int error);

public:
	afx_msg void OnMenuExit();
	afx_msg void OnMenuMain();

	afx_msg void OnPaint();

	void ShowCards();

	afx_msg void OnClickedCheck();
	afx_msg void OnClickedCall();
	afx_msg void OnClickedFold();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnUpdateTimer(WPARAM wParam, LPARAM lParam);

	static UINT RecvThread(PVOID lpParam);

	static UINT ParseMsg(LPVOID lParam);

	void CommandBank(const char * str);

	void CommandMinBet(const char * str);

	void CommandNewCard(const char * msg);

	void SplitStr(const char * str, vector<string>* res, char separator = ' ');
	void GetCards(const char * cards);
	void CommandPlayer(const char * command);
	bool PlayerIsCreated(std::string ip);
	void CreatePlayer(std::string ip, int bank, int bet);
	Player * GetPlayer(std::string ip);
	int GetPlayerIndex(string ip);
	static void CommandWait(const char* command);

	CButton btn_Check;
	CButton btn_Call;
	CButton btn_Fold;
};