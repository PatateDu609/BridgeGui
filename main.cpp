//definition constantes
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
	//initialisation des variables
	std::vector<cards::Center> draw;
	bool d = false;
	std::array<cards::Card, 4> c;
	std::map<cards::Center, cards::Card> toDraw;
	std::array<cards::Hand, 4> hands = cards::deal();
	sf::RenderWindow window;
	sf::Texture table, cards, symbols;
	sf::Sprite spriteBackground;
	cards::Card def = { -1, -1 };
	game::Contract contract = { game::Symbols::DIAMOND, 6, cards::Center::NORTH };
	game::Score score = {0, 0};
	sf::Font cl;
	sf::Text scoreText;
	cards::Card winner, temp;
	int iWinner;
	int turn = 0, k = 0;
	game::Symbols color = game::Symbols::NONE;

	for (int i = 0; i < 4; i++) draw.push_back(cards::Center::NONE);

	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bridge");

	//importation des images (sous forme de texture)
	if (!table.loadFromFile("wood1.jpg")) {
		std::cout << "Image pas trouvee ?" << std::endl;
	}
	if (!cards.loadFromFile("cards.png")) {
		std::cout << "Image pas trouvee ??" << std::endl;
	}
	if (!symbols.loadFromFile("symbols.png")) {

	}

	//importation des polices
	if (!cl.loadFromFile("Font/CL.ttf")) {
		std::cout << "Police pas trouvee ?" << std::endl;
	}
	
	//paramétrage des textures
	table.setRepeated(true);
	table.setSmooth(true);
	cards.setSmooth(true);
	symbols.setSmooth(true);

	//on prépare le sprite du fond
	spriteBackground.setTexture(table);
	spriteBackground.setTextureRect(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT));
	
	//on prépare les dictionnaires
	cards::Sprite cardsSprite;
	cards::initCards(cards, cardsSprite);

	cards::BackSprite backCardSprite;
	cards::initBackCards(cards, backCardSprite);

	cards::Symbols s;
	cards::initSymbols(symbols, s);

	//paramétrage du texte
	scoreText.setFont(cl);
	scoreText.setString("Nord/Sud : " + std::to_string(0) + "\nEst/Ouest : " + std::to_string(0));
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(12);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition(sf::Vector2f(WINDOW_WIDTH * 0.01, WINDOW_HEIGHT * 0.01));
	
	//boucle principale de la fenêtre
	while (window.isOpen()) {
		//on efface la fenêtre à chaque fois et on redessine le fond
		window.clear(sf::Color::Black);
		window.draw(spriteBackground);

		game::showContract(window, s, cl, contract);
		
		for (int coups = 0; coups < 13; coups++) {
			//dessin des mains
			for (int i = 0; i < 4; i++) {
				sf::Vector2i v = cards::coordHand(window, hands, i);
				cards::showHand(window, hands[i], cardsSprite, { v.x, v.y }, i % 2);
			}

			//boucle d'évènements
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
				// vérifie si une main est cliquée et prépare l'affichage
				// interdit l'affichage si la main est non jouable et si ce n'est pas le tour du joueur
				if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
					for (int i = 0; i < 4; i++) {
						c[i] = cards::clicked(window, hands[i], cards::coordHand(window, hands, i), i % 2);

						if (c[i] != def && draw[i] == cards::Center::NONE && turn == i) {
							if (game::playable(color, hands[i], c[i])) {
								if (i == 0) draw[i] = cards::Center::NORTH;
								else if (i == 1) draw[i] = cards::Center::WEST;
								else if (i == 2) draw[i] = cards::Center::SOUTH;
								else if (i == 3) draw[i] = cards::Center::EAST;

								toDraw[draw[i]] = c[i];

								cards::Hand::iterator h = std::find(std::begin(hands[i]), std::end(hands[i]), c[i]);
								hands[i].erase(h);

								--turn;
								turn = (turn < 0) ? 3 : turn;

								++k;
								if (k == 1) {
									winner = c[i];
									iWinner = i;
								}
								temp = game::compare(color, contract[0], winner, c[i]);

								if (temp != winner) iWinner = i;

								winner = temp;

								break;
							}
						}
						else {
							draw.push_back(cards::Center::NONE);
						}
					}
				}
			}

			//affiche la carte cliquée au centre
			for (int i = 0; i < 4; i++) {
				if (draw[i] != cards::Center::NONE || d) {
					showCenter(window, toDraw, cardsSprite, draw[i]);
					d = true;
				}
			}

			if (k == 4) {
				++score[iWinner % 2];
				scoreText.setString("Nord/Sud : " + std::to_string(score[0]) + "\nEst/Ouest : " + std::to_string(score[1]));
				turn = iWinner;
				k = 0;
				d = 0;
				color = game::Symbols::NONE;
				for (int i = 0; i < 4; i++) draw[i] = cards::Center::NONE;
			}

			window.draw(scoreText);
		}
		window.display();
	}

	return 0;
}
