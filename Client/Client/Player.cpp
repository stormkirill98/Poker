#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}

Player::Player(CString name)
	: m_Name(name)
{
	m_Cards[0] = Card(-1, -1);
	m_Cards[1] = Card(-1, -1);
}


Player::~Player()
{
}

void Player::SetCards(Card card1, Card card2)
{
	m_Cards[0] = card1;
	m_Cards[1] = card2;
}

void Player::SetBet(int bet)
{
	m_Bet = bet;
}

void Player::SetBank(int bank)
{
	m_Bank = bank;
}

CString Player::GetName()
{
	return m_Name;
}

Card * Player::GetCards()
{
	return m_Cards;
}

int Player::GetBet()
{
	return m_Bet;
}

int Player::GetBank()
{
	return m_Bank;
}
