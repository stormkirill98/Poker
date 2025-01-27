
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
	virtual BOOL OnInitDialog();
	void Init();
	DECLARE_MESSAGE_MAP()

public:
	bool m_IsConnected = false;
	SOCKET	m_sClient = NULL;
	string m_Ip = "";

	int m_MinBet = 0,
		m_MaxBet = 100,
		m_Bet = 0,
		m_Bank = 0;
	int m_CashGain = 0;
	Card m_UserCards[2];

	CWinThread* recvThread;
	CWinThread* parseThread;
public:
	CEdit* timer;

	vector<Player> players;
	vector<Card> cards;

	vector<CRect> posInfo;
	vector<CRect> posCards;
	vector<CEdit*> timers;

	CEdit m_BetText;
	CEdit m_CashGainText;
	CSliderCtrl m_SliderBet;
	CEdit m_BankOutput;
	CEdit m_CountBackStart;

	CButton btn_Check;
	CButton btn_Call;
	CButton btn_Fold;

	void LoadCard(CImage* card, int suit, int rank);
	void LoadShirt(CImage* shirt);

	void DrawTable();
	void ShowUserCards(Card card1, Card card2);
	void ShowCards();

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

	afx_msg void OnClickedCheck();
	afx_msg void OnClickedCall();
	afx_msg void OnClickedFold();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnUpdateTimer(WPARAM wParam, LPARAM lParam);

	static UINT RecvThread(PVOID lpParam);

	static UINT ParseMsg(LPVOID lParam);

	void CommandBank(const char * str);
	void CommandMinBet(const char * str);
	void CommandCards(const char * cards);
	void CommandPlayer(const char * command);
	void CommandNewCard(const char * msg);
	static void CommandWait(const char* command);
	void CommandStart(const char * command);
	void CommandDisconnect(const char * command);

	void SplitStr(const char * str, vector<string>* res, char separator = ' ');

	bool PlayerIsCreated(int sct);
	void CreatePlayer(int sct, int bank, int bet);

	Player * GetPlayer(int sct);
	int GetPlayerIndex(int sct);

	void ClearFields();
};