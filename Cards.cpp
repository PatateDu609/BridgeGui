#include "Cards.h"
#include <list>

#define CARD_WIDTH 56
#define CARD_HEIGHT 74

//fonctions

//fonction qui renvoie la carte cliquée, prend en paramètre : 
// - la fenêtre
// - la main concernée
// - l'origine de dessin de la main
// - un booléen représentant si oui ou non la main est verticale
cards::Card cards::clicked(sf::RenderWindow const& w, cards::Hand h, sf::Vector2i o, bool rotation = 0) {
	sf::Vector2i pos = sf::Mouse::getPosition(w); //récupère la position de la souris par rapport à la fenêtre
	bool x = 0, y = 0;
	if (!rotation) {
		if ((o.x <= pos.x) && ((o.x + (h.size() - 1) * 14 + CARD_WIDTH) >= pos.x)) x = 1;
		if (o.y <= pos.y && pos.y <= (o.y + CARD_HEIGHT)) y = 1;
	}
	else {
		if (o.x >= pos.x && pos.x >= (o.x - CARD_HEIGHT)) x = 1;
		if ((o.y <= pos.y) && (pos.y <= (o.y + (h.size() - 1) * 14 + CARD_WIDTH))) y = 1;
	}

	if (x && y) { // si la main se trouve bien dans les bornes 
		int i = -1;
		if (!rotation) {
			int x0 = o.x;
			while (x0 <= pos.x) {
				x0 += 14;
				++i;
			}
			if (i >= h.size()) i = h.size() - 1;
		}
		else {
			int y0 = o.y;
			while (y0 <= pos.y) {
				y0 += 14;
				++i;
			}
			if (i >= h.size()) i = h.size() - 1;
		}

		return h[i];
	}
	else { // renvoi par défaut
		Card c;
		c[0] = -1;
		c[1] = -1;
		return c;
	}
}

//fonctions pour initialiser les dictionnairesde sprite
void cards::initCards(sf::Texture const& t, cards::Sprite &cs) {
	sf::Sprite s;
	s.setTexture(t);

	for (int x = 0; x < 13; x++) {
		for (int y = 0; y < 4; y++) {
			s.setTextureRect(sf::IntRect(x * CARD_WIDTH, y * CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));
			cs[{y, x}] = s;
		}
	}
}

void cards::initBackCards(sf::Texture const & t, cards::BackSprite &bs) {
	sf::Sprite s;
	s.setTexture(t);

	for (int i = 0; i < 5; i++) {
		s.setTextureRect(sf::IntRect(i * CARD_WIDTH, 4 * CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));
		bs[i] = s;
	}
}

//Distribue les cartes pour le jeu en cours
std::array<cards::Hand, 4> cards::deal() {
	std::srand(std::time(0));
	cards::Pack pack, pack1;
	std::array<cards::Hand, 4> hands;
	cards::Card c;

	//Remplis le paquet
	for (int i = 0; i < 52; i++) {
		c[0] = i / 13;
		c[1] = i % 13;
		pack.push_back(c);
	}
	//Mélange
	for (int i = 51; i >= 0; i--) {
		std::vector<cards::Card>::iterator it = pack.begin();
		int j = (i != 0) ? (rand() % i) : 0;
		pack1.push_back(pack.at(j));
		pack.erase(it + j);
	}
	pack = pack1;

	//Distribue
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			hands[i].push_back(pack.back());
			pack.pop_back();
		}
		std::sort(hands[i].begin(), hands[i].end(), std::greater<std::array<int, 2>>());
	}
	return hands;
}

//affiche une main, prends en paramètre :
// - la fenêtre
// - la main à afficher
// - le dictionnaire de cartes
// - l'origine de dessin
// - est ce que la main subit une rotation ?
void cards::showHand(sf::RenderWindow & w, cards::Hand const& h, cards::Sprite const& cs, std::array<int, 2> origin, bool rotate = false) {
	sf::Sprite s;
	std::list<int> c[4];

	for (int i = 0; i < h.size(); i++) c[h[i][0]].push_back(h[i][1]);

	for (int x = 3, i = 0; x >= 0; x--) {
		for (std::list<int>::iterator j = c[x].begin(); j != c[x].end(); j++, i++) {
			s = cs.at({ x, *j });
			s.setRotation(90.f * rotate);
			s.setPosition(sf::Vector2f(origin[0], origin[1]));
			s.move(sf::Vector2f(14 * i * !rotate, 14 * i * rotate));
			w.draw(s);
		}
	}
}

void cards::showCenter(sf::RenderWindow & w, std::map<cards::Center, cards::Card> toDraw, cards::Sprite const& cs, cards::Center draw) {
	sf::Sprite s = cs.at(toDraw[draw]);

	int width = w.getSize().x,
		height = w.getSize().y;

	int gap;
	switch (draw) {
	case NORTH:
		gap = -CARD_HEIGHT / 2;
		break;
	case WEST:
		gap = -CARD_HEIGHT / 2;
		break;
	case SOUTH:
		gap = CARD_HEIGHT / 2;
		break;
	case EAST:
		gap = CARD_HEIGHT / 2;
		break;
	}

	s.rotate(90 * draw);
	s.setOrigin(CARD_WIDTH / 2, CARD_HEIGHT / 2);
	s.move(width / 2 + gap * (draw % 2), height / 2 + gap * !(draw % 2));
	w.draw(s);
}

//renvoie les coordonnées de l'origine de la main, prends les mains et l'indice de la main voulue
sf::Vector2i cards::coordHand(sf::RenderWindow const& w, std::array<cards::Hand, 4> const& hands, int i) {
	sf::Vector2i v;
	int x, y,
		width = w.getSize().x,
		height = w.getSize().y;
	switch (i) {
	case 0: //Nord
		v.x = (width - 14 * hands[0].size()) / 2;
		v.y = 50;
		break;
	case 1: //ouest
		v.x = 50 + CARD_WIDTH;
		v.y = (height - 14 * hands[1].size()) / 2;
		break;
	case 2: //sud
		v.x = (width - 14 * hands[2].size()) / 2;
		v.y = height - CARD_HEIGHT - 50;
		break;
	case 3: //est
		v.x = width - 50;
		v.y = (height - 14 * hands[3].size()) / 2;
		break;
	}
	return v;
}