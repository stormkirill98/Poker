#include "stdafx.h"
#include "Card.h"

Card::Card()
{
}

Card::Card(int suit, int rank)
	: m_Suit(suit), m_Rank(rank)
{}

Card::~Card()
{}