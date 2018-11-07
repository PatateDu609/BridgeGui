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

void game::showWhoPlay(sf::RenderWindow & w, int turn, sf::Vector2i ch) {
	int width = 20, height = 40, gap = 5, hTriangle = height * 0.50f, x, y;
	auto size = w.getSize();
	sf::ConvexShape triangle;
	sf::RectangleShape rect(sf::Vector2f(width, height));
	triangle.setPointCount(3);
	triangle.setPoint(0, sf::Vector2f(width / 2 + gap, 0.f));
	triangle.setPoint(1, sf::Vector2f(0.f, hTriangle));
	triangle.setPoint(2, sf::Vector2f(width + 2 * gap, hTriangle));
	
	switch(turn) {
	case 0:
		x = (size.x + 2*gap + width) / 2;
		y = ch.y + 20 + CARD_HEIGHT;
		rect.setPosition(sf::Vector2f(x + gap, y + hTriangle));
		break;
	case 1:
		x = ch.x + 20 + CARD_WIDTH;
		y = (size.y + height + hTriangle) / 2;
		rect.setPosition(sf::Vector2f(x + hTriangle, y - gap));
		break;
	case 2:
		x = (size.x + 2*gap + width) / 2;
		y = ch.y - 20;
		rect.setPosition(sf::Vector2f(x - gap, y - hTriangle));
		break;
	case 3:
		x = ch.x - 20 - CARD_HEIGHT;
		y = (size.y + height + hTriangle) / 2;
		rect.setPosition(sf::Vector2f(x - hTriangle, y + gap));
		break;
	}
	
	triangle.setFillColor(sf::Color(232, 133, 118));
	rect.setFillColor(sf::Color(232, 133, 118));
	
	triangle.setRotation(-90.f * turn);
	rect.setRotation(-90.f * turn);
	triangle.setPosition(sf::Vector2f(x, y));
	
	w.draw(triangle);
	w.draw(rect);
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

void game::showBidding(sf::RenderWindow & w, sf::Font const& f, Symbols const& color, ContractTexture const& cs) {
	sf::Vector2u size = w.getSize();
	sf::Texture t;

	std::array<sf::Text, 4> players;
	players[0].setString("West");
	players[1].setString("North");
	players[2].setString("East");
	players[3].setString("South");

	for (int i = 0; i < 4; i++) {
		players[i].setFont(f);
		players[i].setCharacterSize(18);
		players[i].setFillColor(sf::Color::White);
		players[i].setStyle(sf::Text::Bold);

		sf::FloatRect playersRect = players[i].getLocalBounds();
		players[i].setOrigin(playersRect.left + playersRect.width / 2, playersRect.top + playersRect.height / 2);
		players[i].setPosition(sf::Vector2f((i * size.x / 8) + (size.x * 0.04 + 18 + size.x / 4), size.y / 4 + 18));
		w.draw(players[i]);
	}

	sf::Sprite sp;
	for (int i = 0; i < 7; i++)
		for (int j = 0; j < 5; j++) {
			t = cs.at({ i, j });
			sp = sf::Sprite(t);
			sp.setPosition(sf::Vector2f((i * size.x / 16) + ((10 * size.x / 33)), (j * size.y / 16) +(size.x / 4) + 18));
			w.draw(sp);
		}


}

bool game::initContractSprite(cards::Symbols const& s, sf::Font const & f, game::ContractTexture & cs) {
	sf::RenderTexture texture;

	sf::Text levees, nt;
	levees.setFillColor(sf::Color::Black);
	levees.setFont(f);
	levees.setCharacterSize(12);
	levees.setStyle(sf::Text::Bold);

	nt.setFillColor(sf::Color::Black);
	nt.setFont(f);
	nt.setCharacterSize(12);
	nt.setStyle(sf::Text::Bold);
	nt.setString("NT");

	sf::FloatRect rect;
	sf::Sprite symbol;

	for (int l = 1; l < 8; l++) {
		levees.setString(std::to_string(l));

		rect = levees.getGlobalBounds();
		if (!texture.create(rect.left + rect.width + 0.03 * 512, rect.top + rect.height + 0.03 * 512)) return 0;
		for(int si = 0; si < 5; si++) {
			texture.clear(sf::Color::Transparent);
			if (si < 4) {
				switch (si) {
				case 0:
					symbol = s[0];
					symbol.setColor(sf::Color::Black);
					break;
				case 1:
					symbol = s[3];
					symbol.setColor(sf::Color::Red);
					break;
				case 2:
					symbol = s[2];
					break;
				case 3:
					symbol = s[1];
					break;
				}
				symbol.setPosition(sf::Vector2f(rect.left + rect.width, 0));
				texture.draw(symbol);
			}
			else {
				nt.setPosition(sf::Vector2f(rect.left + rect.width, 0));
				texture.draw(nt);
			}
			texture.draw(levees);
			texture.display();
			cs[{l - 1, si}] = texture.getTexture();
		}
	}

	return 1;
}
