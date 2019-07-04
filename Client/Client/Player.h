#pragma once
#include "Card.h"
#include <string>

class Player
{
public:
	Player();
	Player(int sct);
	~Player();

private:
	int m_Sct;

	int m_Bet = 0;
	int m_Bank = 0;
	
	bool m_Fold = true;

public:
	void SetBet(int bet);
	void SetBank(int bank);
	void SetFold(bool fold);

public:
	int GetSct();
	int GetBet();
	int GetBank();
	bool IsFold();
};