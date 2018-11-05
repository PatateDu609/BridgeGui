#pragma once
#ifndef GAME_H
#define GAME_H

#include <array>
#include "Cards.h"

namespace game {
	enum Symbols { NONE = -1, SPADE = 0, HEART = 1, DIAMOND = 2, CLUB = 3, NT = 4 };

	typedef std::array<int, 3> Contract;
	typedef std::array<int, 2> Score;
	typedef std::map<Contract, cards::Sprite> ContractSprite;

	bool initContractSprite(cards::Symbols const& s, sf::Font const&, ContractSprite & cs);

	void showContract(sf::RenderWindow & w, cards::Symbols const& s, sf::Font const& f, game::Contract const& c);
	void showContract(sf::RenderTexture & w, cards::Symbols const& s, sf::Font const& f, game::Contract const& c);
	void showWhoPlay(sf::RenderWindow & w, int turn, sf::Vector2i ch);
	
	cards::Card compare(Symbols color, int contract, cards::Card const& a, cards::Card const& b);
	bool playable(Symbols & color, cards::Hand const& h, cards::Card const& c);
	void showBidding(sf::RenderWindow & w, sf::Font const& f, Symbols const& color);
}

#endif
