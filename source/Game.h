#pragma once
#ifndef GAME_H
#define GAME_H

#include "Cards.h"

namespace game {
	enum Symbols { NONE = -1, NT = 0, SPADE = 1, HEART = 2, DIAMOND = 3, CLUB = 4 };

	typedef std::array<int, 3> Contract;
	typedef std::array<int, 2> Score;
	typedef std::map < std::array<int, 2>, sf::Texture> ContractTexture;

	bool initContractSprite(cards::Symbols const& s, sf::Font const&, ContractTexture & cs);

	void showContract(sf::RenderWindow & w, cards::Symbols const& s, sf::Font const& f, game::Contract const& c);
	void showWhoPlay(sf::RenderWindow & w, int turn, sf::Vector2i ch);
	
	cards::Card compare(Symbols color, int contract, cards::Card const& a, cards::Card const& b);
	bool playable(Symbols & color, cards::Hand const& h, cards::Card const& c);
	void showBidding(sf::RenderWindow & w, sf::Font const& f, Symbols const& color, ContractTexture const& cs);
	
	bool checkValidity(Contract const& first, Contract const& second);
}

#endif
