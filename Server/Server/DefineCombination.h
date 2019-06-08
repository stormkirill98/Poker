#pragma once
#include "Card.h"
#include <vector>

using namespace std;

class DefineCombination
{
public:
	DefineCombination();
	~DefineCombination();

public:
	static bool RoyalFlush(vector<Card> cards);
	static int StraightFlush(vector<Card> cards);
	static int Kare(vector<Card> cards);
	static int FullHouse(vector<Card> cards);
	static int Flush(vector<Card> cards);
	static int Straight(vector<Card> cards);
	static int Three(vector<Card> cards);
	static int TwoPair(vector<Card> cards);
	static int Pair(vector<Card> cards);
	static int HighCard(vector<Card> cards);

	static bool ContainsCard(vector<Card> cards, int suit, int rank);
};

