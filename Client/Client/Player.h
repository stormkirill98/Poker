#pragma once
#include "Card.h"

class Player
{
public:
	Player();
	Player(CString name);
	~Player();

private:
	CString m_Name;
	int m_Bet = 0;
	int m_Bank = 0;
	
	bool m_Fold = true;

public:
	void SetBet(int bet);
	void SetBank(int bank);
	void SetFold(bool fold);

public:
	CString GetName();
	int GetBet();
	int GetBank();
	bool IsFold();
};