#pragma once
#ifndef CARDS_H
#define CARDS_H

#include <array>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

namespace cards {
	enum Center {NORTH, WEST, SOUTH, EAST, NONE};

	typedef std::array<int, 2> Card;
	typedef std::vector<Card> Pack;
	typedef std::vector<Card> Hand;
	typedef std::map<Card, sf::Sprite> Sprite;
	typedef std::array<sf::Sprite, 5> BackSprite;
	typedef std::array<sf::Sprite, 4> Symbols;

	void initCards(sf::Texture const& t, Sprite &cs);
	void initBackCards(sf::Texture const & t, BackSprite &bs);
	void initSymbols(sf::Texture const& t, Symbols &s);

	Card clicked(sf::RenderWindow const& w, Hand h, sf::Vector2i o, bool rotation);

	std::array<Hand, 4> deal();

	void showHand(sf::RenderWindow & w, Hand const& h, Sprite const& cs, std::array<int, 2> origin, bool rotate);
	void showCenter(sf::RenderWindow & w, std::map<Center, Card> toDraw, Sprite const& cs, Center draw);
	sf::Vector2i coordHand(sf::RenderWindow const& w, std::array<Hand, 4> const& hands, int i);
}

#endif