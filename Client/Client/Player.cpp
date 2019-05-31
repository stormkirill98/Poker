#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}

Player::Player(CString ip, CString name)
	: m_Name(name), m_Ip(ip)
{
}


Player::~Player()
{
}

void Player::SetBet(int bet)
{
	m_Bet = bet;
}

void Player::SetBank(int bank)
{
	m_Bank = bank;
}

void Player::SetFold(bool fold)
{
	m_Fold = fold;
}

void Player::SetAction(CString action)
{
	m_Action = action;
}

CString Player::GetName()
{
	return m_Name;
}

int Player::GetBet()
{
	return m_Bet;
}

int Player::GetBank()
{
	return m_Bank;
}

bool Player::IsFold()
{
	return m_Fold;
}

CString Player::GetAction()
{
	return m_Action;
}
