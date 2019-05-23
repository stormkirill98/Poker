#pragma once
class Card
{

public:
	enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };
	enum Rank { TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };
	
public:
	Card(Suit suit, Rank rank);
	~Card();

public:
	Suit m_Suit;
	Rank m_Rank;
};

