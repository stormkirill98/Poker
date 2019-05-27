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

public:
	void SetBet(int bet);
	void SetBank(int bank);

public:
	CString GetName();
	int GetBet();
	int GetBank();
};