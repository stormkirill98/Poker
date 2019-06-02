#pragma once
#include "Card.h"
#include <string>

class Player
{
public:
	Player();
	Player(std::string ip);
	~Player();

private:
	std::string m_Ip;

	int m_Bet = 0;
	int m_Bank = 0;
	
	bool m_Fold = true;

public:
	void SetBet(int bet);
	void SetBank(int bank);
	void SetFold(bool fold);

public:
	std::string GetIp();
	int GetBet();
	int GetBank();
	bool IsFold();
};