#include "Game.h"
#include <iostream>

void game::showContract(sf::RenderWindow & w, cards::Symbols const& s, sf::Font const& f, game::Contract const& c) {
	int width = w.getSize().x,
		height = w.getSize().y;

	sf::Text levees, owner;
	owner.setFont(f);
	owner.setCharacterSize(12);
	owner.setFillColor(sf::Color::White);
	owner.setStyle(sf::Text::Bold);
	owner.setPosition(sf::Vector2f(width * 0.99, height * 0.01));

	levees.setFont(f);
	levees.setCharacterSize(12);
	levees.setFillColor(sf::Color::White);
	levees.setStyle(sf::Text::Bold);
	levees.setPosition(sf::Vector2f(width * 0.99 - 512 * 0.02 - owner.getCharacterSize(), height * 0.01));

	switch (c[2]) {
	case cards::Center::EAST:
		owner.setString("E");
		break;
	case cards::Center::NORTH:
		owner.setString("N");
		break;
	case cards::Center::SOUTH:
		owner.setString("S");
		break;
	case cards::Center::WEST:
		owner.setString("W");
		break;
	}

	if (c[0] == game::Symbols::NT) {
		levees.setString("NT" + std::to_string(c[1]));
		w.draw(levees);
		w.draw(owner);
		return;
	}
	else levees.setString(std::to_string(c[1]));
	
	sf::Sprite s0 = s[c[0]];
	s0.setPosition(sf::Vector2f(width * 0.99 - 512 * 0.03, height * 0.01));
	w.draw(owner);
	w.draw(levees);
	w.draw(s0);
}

cards::Card game::compare(game::Symbols color, int contract, cards::Card const& a, cards::Card const& b) {
	cards::Card c;
	int x = a[0] - b[0], y = a[1] - b[1];
	bool ac = (a[0] == color),
		bc = (b[0] == color),
		acontract = (a[0] == contract),
		bcontract = (b[0] == contract);

	if (x == 0 && ac) {
		if (a[1] == 0 || b[1] == 0) { //règle le cas de l'as
			if (a[1] == 0) c = a;
			if (b[1] == 0) c = b;
		}
		else c = (y > 0) ? a : b;
	}
	else if (bc && !ac) {
		if (!acontract) c = b;
		else c = a;
	}
	else if (ac && !bc) {
		if (!bcontract) c = a;
		else c = b;
	}
	else if (x == 0 && !ac) {
		if (acontract) {
			if (a[1] == 0 || b[1] == 0) { //règle le cas de l'as
				if (a[1] == 0) c = a;
				if (b[1] == 0) c = b;
			}
			else c = (y > 0) ? a : b;
		}
	}

	return c;
}

bool game::playable(Symbols & color, cards::Hand const& h, cards::Card const& c) {
	if (color == game::Symbols::NONE) {
		switch (c[0]) {
		case 0:
			color = game::Symbols::SPADE;
			break;
		case 1:
			color = game::Symbols::HEART;
			break;
		case 2:
			color = game::Symbols::DIAMOND;
			break;
		case 3:
			color = game::Symbols::CLUB;
			break;
		}
		return true;
	}

	if (color == c[0]) return true;
	else {
		for (int i = 0; i < h.size(); i++) {
			if (h[i][0] == color) return false;
		}
		return true;
	}
}