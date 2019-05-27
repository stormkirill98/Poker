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
	Card m_Cards[2];
	int m_Bet = 0;
	int m_Bank = 0;

public:
	void SetCards(Card card1, Card card2);
	void SetBet(int bet);
	void SetBank(int bank);

public:
	CString GetName();
	Card* GetCards();
	int GetBet();
	int GetBank();
};