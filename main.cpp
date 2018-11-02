#include <iostream>
#include <array>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <list>
#include <SFML/Graphics.hpp>

//definition constantes
#define CARD_WIDTH 56
#define CARD_HEIGHT 74
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//definition type pour orientation du centre
enum Center {
	NORTH = 0,
	WEST = 1,
	SOUTH = 2,
	EAST = 3,
	NONE
};

//définition des typedef
typedef std::array<int, 2> Card;
typedef std::vector<Card> Pack;
typedef std::vector<Card> Hand;
typedef std::map<Card, sf::Sprite> CardSprite;
typedef std::array<sf::Sprite, 5> BackCardSprite;

//fonctions

//fonction qui renvoie la carte cliquée, prend en paramètre : 
// - la fenêtre
// - la main concernée
// - l'origine de dessin de la main
// - un booléen représentant si oui ou non la main est verticale
Card clicked(sf::RenderWindow const& w, Hand h, sf::Vector2i o, bool rotation = 0) {
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

//fonctions pour initialiser le dictionnaire de sprite
void initCards(sf::Texture const& t, CardSprite &cs) {
	sf::Sprite s;
	s.setTexture(t);

	for (int x = 0; x < 13; x++) {
		for (int y = 0; y < 4; y++) {
			s.setTextureRect(sf::IntRect(x * CARD_WIDTH, y * CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));
			cs[{y, x}] = s;
		}
	}
}

void initBackCards(sf::Texture const & t, BackCardSprite &bs) {
	sf::Sprite s;
	s.setTexture(t);

	for (int i = 0; i < 5; i++) {
		s.setTextureRect(sf::IntRect(i * CARD_WIDTH, 4 * CARD_HEIGHT, CARD_WIDTH, CARD_HEIGHT));
		bs[i] = s;
	}
}

//Distribue les cartes pour le jeu en cours
std::array<Hand, 4> deal() {
	std::srand(std::time(0));
	Pack pack, pack1;
	std::array<Hand, 4> hands;
	Card c;

	//Remplis le paquet
	for (int i = 0; i < 52; i++) {
		c[0] = i / 13;
		c[1] = i % 13;
		pack.push_back(c);
	}
	//Mélange
	for (int i = 51; i >= 0; i--) {
		std::vector<Card>::iterator it = pack.begin();
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
		std::sort(hands[i].begin(), hands[i].end(), std::greater<std::array<int,2>>());
	}
	return hands;
}

//affiche une main, prends en paramètre :
// - la fenêtre
// - la main à afficher
// - le dictionnaire de cartes
// - l'origine de dessin
// - est ce que la main subit une rotation ?
void showHand(sf::RenderWindow & w, Hand const& h, CardSprite const& cs, std::array<int, 2> origin, bool rotate = false) {
	sf::Sprite s;
	std::list<int> c[4];
	
	for (int i = 0; i < h.size(); i++) c[h[i][0]].push_back(h[i][1]);

	for (int x = 3, i = 0; x >= 0; x--) {
		for (std::list<int>::iterator j = c[x].begin(); j != c[x].end(); j++, i++) {
			s = cs.at({x, *j});
			s.setRotation(90.f * rotate);
			s.setPosition(sf::Vector2f(origin[0], origin[1]));
			s.move(sf::Vector2f(14 * i * !rotate, 14 * i * rotate));
			w.draw(s);
		}
	}
}

void showCenter(sf::RenderWindow & w, std::map<Center, Card> toDraw, CardSprite const& cs, Center draw) {
	sf::Sprite s = cs.at(toDraw[draw]);
	
	int gap;
	switch(draw) {
	case NORTH:
		gap = - CARD_HEIGHT / 2;
		break;
	case WEST:
		gap = - CARD_HEIGHT / 2;
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
	s.move(WINDOW_WIDTH / 2 + gap * (draw % 2), WINDOW_HEIGHT / 2 + gap * !(draw % 2));
	w.draw(s);
}

//renvoie les coordonnées de l'origine de la main, prends les mains et l'indice de la main voulue
sf::Vector2i coordHand(std::array<Hand , 4> const& hands, int i) {
	sf::Vector2i v;
	int x, y;
	switch (i) {
	case 0: //Nord
		v.x = (WINDOW_WIDTH - 14 * hands[0].size()) / 2;
		v.y = 50;
		break;
	case 1: //ouest
		v.x = 50 + CARD_WIDTH;
		v.y = (WINDOW_HEIGHT - 14 * hands[1].size()) / 2;
		break;
	case 2: //sud
		v.x = (WINDOW_WIDTH - 14 * hands[2].size()) / 2;
		v.y = WINDOW_HEIGHT - CARD_HEIGHT - 50;
		break;
	case 3: //est
		v.x = WINDOW_WIDTH - 50;
		v.y = (WINDOW_HEIGHT - 14 * hands[3].size()) / 2;
		break;
	}
	return v;
}

int main()
{
	//initialisation des variables
	std::vector<Center> draw;
	for(int i = 0; i < 4; i++) draw.push_back(NONE);
	bool d = false;
	std::array<Card, 4> c;
	std::map<Center, Card> toDraw;
	std::array<Hand, 4> hands = deal();
	sf::RenderWindow window;
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bridge");
	sf::Texture table, cards;
	sf::Sprite sprite;
	Card def = { -1, -1 };
	
	//importation des images (sous forme de texture)
	if (!table.loadFromFile("wood1.jpg")) {
		std::cout << "Image pas trouvee ?" << std::endl;
	}
	if (!cards.loadFromFile("cards.png")) {
		std::cout << "Image pas trouvee ??" << std::endl;
	}
	
	//paramétrage des textures
	table.setRepeated(true);
	table.setSmooth(true);
	cards.setSmooth(true);
	
	//on prépare les dictionnaires
	CardSprite cardsSprite;
	initCards(cards, cardsSprite);

	BackCardSprite backCardsackSprite;
	initBackCards(cards, backCardsackSprite);
	
	//on prépare le sprite du fond
	sprite.setTexture(table);
	sprite.setTextureRect(sf::IntRect(0,0, WINDOW_WIDTH, WINDOW_HEIGHT));
	
	//boucle principale de la fenêtre
	while (window.isOpen()) {
		//on efface la fenêtre à chaque fois et on redessine le fond
		window.clear(sf::Color::Black);
		window.draw(sprite);
		
		//dessin des mains
		for (int i = 0; i < 4; i++) {
			sf::Vector2i v = coordHand(hands, i);
			showHand(window, hands[i], cardsSprite, { v.x, v.y }, i % 2);
		}
		
		//boucle d'évènements
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			// vérifie si une main est cliquée et prépare l'affichage
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				for (int i = 0; i < 4; i++) {
					c[i] = clicked(window, hands[i], coordHand(hands, i), i % 2);

					if (c[i] != def) {
						if (i == 0) {
							draw[i] = NORTH;
						}
						else if (i == 1) {
							draw[i] = WEST;
						}
						else if (i == 2) {
							draw[i] = SOUTH;
						}
						else if (i == 3) {
							draw[i] = EAST;
						}
						toDraw[draw[i]] = c[i];
						break;
					}
					else {
						draw.push_back(NONE);
					}
				}
			}
		}
		
		//affiche la carte cliquée au centre
		for(int i = 0; i < 4; i++) {
			if(draw[i] != NONE || d) {
				showCenter(window, toDraw, cardsSprite, draw[i]);
				d = true;
			}
		}

		window.display();
	}

	return 0;
}
