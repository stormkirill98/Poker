#pragma once
#include "Card.h"

class Player
{
public:
	Player();
	Player(CString ip, CString name);
	~Player();

private:
	CString m_Ip;

	CString m_Name;
	int m_Bet = 0;
	int m_Bank = 0;
	
	CString m_Action = "";

	bool m_Fold = true;

public:
	void SetBet(int bet);
	void SetBank(int bank);
	void SetFold(bool fold);
	void SetAction(CString action);

public:
	CString GetName();
	int GetBet();
	int GetBank();
	bool IsFold();
	CString GetAction();
};