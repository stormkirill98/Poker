#include "stdafx.h"
#include "DefineCombination.h"


DefineCombination::DefineCombination()
{
}


DefineCombination::~DefineCombination()
{
}

bool DefineCombination::ContainsCard(vector<Card> cards, int suit, int rank) {
	for (Card card : cards) {
		if (suit == -1) {
			if (card.m_Rank == rank) {
				return true;
			}
		}
		else {
			if (card.m_Suit == suit && card.m_Rank == rank) {
				return true;
			}
		}
	}

	return false;
}

bool DefineCombination::RoyalFlush(vector<Card> cards) {
	bool isRoyalFlush;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isRoyalFlush = true;

		if (card.m_Rank >= 8) {
			int suit = card.m_Suit;
			for (int rank = 8; rank <= 12; rank++) {
				if (!ContainsCard(cards, suit, rank)) {
					isRoyalFlush = false;
					break;
				}
			}

			if (isRoyalFlush) {
				return true;
			}
		}
	}

	return false;
}

int DefineCombination::StraightFlush(vector<Card> cards) {
	bool isStraightFlush;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isStraightFlush = true;

		int suit = card.m_Suit;
		for (int rank = card.m_Rank; rank <= card.m_Rank + 4; rank++) {
			if (!ContainsCard(cards, suit, rank)) {
				isStraightFlush = false;
				break;
			}
		}

		if (isStraightFlush) {
			return card.m_Rank + 4;
		}
	}

	return -1;
}

int DefineCombination::Kare(vector<Card> cards) {
	bool isKare;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isKare = true;

		int rank = card.m_Rank;
		for (int suit = 0; suit <= 3; suit++) {
			if (!ContainsCard(cards, suit, rank)) {
				isKare = false;
				break;
			}
		}

		if (isKare) {
			return rank;
		}
	}

	return -1;
}

int DefineCombination::FullHouse(vector<Card> cards) {
	int rankThree = -1;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		bool isThreee = true;

		rankThree = card.m_Rank;
		for (int suit = 0; suit <= 2; suit++) {
			if (!ContainsCard(cards, suit, rankThree)) {
				isThreee = false;
				rankThree = -1;
				break;
			}
		}

		if (isThreee) {
			break;
		}
	}

	if (rankThree == -1) {
		return -1;
	}

	int rankTwo = -1;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		bool isPair = true;

		rankTwo = card.m_Rank;
		if (rankTwo == rankThree) {
			rankTwo = -1;
			continue;
		}
		for (int suit = 0; suit <= 1; suit++) {
			if (!ContainsCard(cards, suit, rankTwo)) {
				isPair = false;
				rankTwo = -1;
				break;
			}
		}

		if (isPair) {
			break;
		}
	}

	if (rankTwo == -1) {
		return -1;
	}

	return rankThree;
}
int DefineCombination::Flush(vector<Card> cards) {
	int maxRank;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		maxRank = card.m_Rank;

		int suit = card.m_Suit;
		int countCardThisSuit = 0;
		for (int j = 0; j < cards.size(); j++) {
			if (j == i) {
				continue;
			}

			Card cardBuf = cards.at(j);
			if (cardBuf.m_Suit == suit) {
				countCardThisSuit++;
				if (cardBuf.m_Rank > maxRank) {
					maxRank = cardBuf.m_Rank;
				}
			}
		}

		if (countCardThisSuit == 4) {
			return maxRank;
		}
	}

	return -1;
}

int DefineCombination::Straight(vector<Card> cards) {
	bool isStraight;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isStraight = true;

		for (int rank = card.m_Rank; rank <= card.m_Rank + 4; rank++) {
			if (!ContainsCard(cards, -1, rank)) {
				isStraight = false;
				break;
			}
		}

		if (isStraight) {
			return card.m_Rank + 4;
		}
	}

	return -1;
}

int DefineCombination::Three(vector<Card> cards) {
	bool isThree;
	int rank1 = -1;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isThree = true;

		rank1 = card.m_Rank;
		for (int suit = 0; suit <= 2; suit++) {
			if (!ContainsCard(cards, suit, rank1)) {
				isThree = false;
				rank1 = -1;
				break;
			}
		}

		if (isThree) {
			break;
		}
	}

	if (rank1 == -1) {
		return -1;
	}

	int rank2 = -1;
	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isThree = true;

		rank2 = card.m_Rank;
		if (rank2 == rank1) {
			rank2 = -1;
			continue;
		}

		for (int suit = 0; suit <= 2; suit++) {
			if (!ContainsCard(cards, suit, rank2)) {
				isThree = false;
				rank2 = -1;
				break;
			}
		}

		if (isThree) {
			break;
		}
	}

	if (rank1 > rank2) {
		return rank1;
	}
	else {
		return rank2;
	}
}

int DefineCombination::TwoPair(vector<Card> cards) {
	bool isPair;
	int rank1 = -1;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isPair = true;

		rank1 = card.m_Rank;
		for (int suit = 0; suit <= 1; suit++) {
			if (!ContainsCard(cards, suit, rank1)) {
				isPair = false;
				rank1 = -1;
				break;
			}
		}

		if (isPair) {
			break;
		}
	}

	int rank2 = -1;
	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isPair = true;

		rank2 = card.m_Rank;
		if (rank2 == rank1) {
			rank2 = -1;
			continue;
		}

		for (int suit = 0; suit <= 1; suit++) {
			if (!ContainsCard(cards, suit, rank2)) {
				isPair = false;
				rank2 = -1;
				break;
			}
		}

		if (isPair) {
			break;
		}
	}

	int rank3 = -1;
	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isPair = true;

		rank3 = card.m_Rank;
		if (rank3 == rank1 || rank3 == rank2) {
			rank3 = -1;
			continue;
		}

		for (int suit = 0; suit <= 1; suit++) {
			if (!ContainsCard(cards, suit, rank3)) {
				isPair = false;
				rank3 = -1;
				break;
			}
		}

		if (isPair) {
			break;
		}
	}

	if (rank1 == -1 || rank2 == -1) {
		return -1;
	}

	if (rank1 > rank2) {
		if (rank3 > rank1) {
			return rank3;
		}
		return rank1;
	}
	else {
		if (rank3 > rank2) {
			return rank3;
		}
		return rank2;
	}
}

int DefineCombination::Pair(vector<Card> cards) {
	bool isPair;
	int rank = -1;

	for (int i = 0; i < cards.size(); i++) {
		Card card = cards.at(i);

		isPair = true;

		rank = card.m_Rank;
		for (int suit = 0; suit <= 1; suit++) {
			if (!ContainsCard(cards, suit, rank)) {
				isPair = false;
				rank = -1;
				break;
			}
		}

		if (isPair) {
			return rank;
		}
	}

	return -1;
}

int DefineCombination::HighCard(Card handCard1, Card handCard2) {
	if (handCard1.m_Rank > handCard2.m_Rank) {
		return handCard1.m_Rank;
	}
	else {
		return handCard2.m_Rank;
	}
}
