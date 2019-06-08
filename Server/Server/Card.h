#pragma once
class Card
{
public:
	Card();
	Card(int suit, int rank);
	~Card();

public:
	int m_Suit;
	int m_Rank;
};