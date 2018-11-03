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
	for(int i = 0; i < 4; i++) draw.push_back(cards::Center::NONE);
	bool d = false;
	std::array<cards::Card, 4> c;
	std::map<cards::Center, cards::Card> toDraw;
	std::array<cards::Hand, 4> hands = cards::deal();
	sf::RenderWindow window;
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bridge");
	sf::Texture table, cards, symbols;
	sf::Sprite sprite;
	cards::Card def = { -1, -1 };
	game::Contract contract = { 5, 6 };
	sf::Font cl;
	sf::Text scoreText;

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
	
	//on prépare les dictionnaires
	cards::Sprite cardsSprite;
	cards::initCards(cards, cardsSprite);

	cards::BackSprite backCardSprite;
	cards::initBackCards(cards, backCardSprite);
	
	//on prépare le sprite du fond
	sprite.setTexture(table);
	sprite.setTextureRect(sf::IntRect(0,0, WINDOW_WIDTH, WINDOW_HEIGHT));

	//paramétrage du texte
	scoreText.setFont(cl);
	scoreText.setString("Nord/Sud : " + std::to_string(0) + "\nEst/Ouest : " + std::to_string(0));
	scoreText.setFillColor(sf::Color::White);
	scoreText.setCharacterSize(12);
	scoreText.setStyle(sf::Text::Bold);
	
	//boucle principale de la fenêtre
	while (window.isOpen()) {
		//on efface la fenêtre à chaque fois et on redessine le fond
		window.clear(sf::Color::Black);
		window.draw(sprite);
		
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
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
				for (int i = 0; i < 4; i++) {
					c[i] = cards::clicked(window, hands[i], cards::coordHand(window, hands, i), i % 2);

					if (c[i] != def && draw[i] == cards::Center::NONE) {
						if (i == 0) {
							draw[i] = cards::Center::NORTH;
						}
						else if (i == 1) {
							draw[i] = cards::Center::WEST;
						}
						else if (i == 2) {
							draw[i] = cards::Center::SOUTH;
						}
						else if (i == 3) {
							draw[i] = cards::Center::EAST;
						}
						toDraw[draw[i]] = c[i];
						break;
					}
					else {
						draw.push_back(cards::Center::NONE);
					}
				}
			}
		}
		
		//affiche la carte cliquée au centre
		for(int i = 0; i < 4; i++) {
			if(draw[i] != cards::Center::NONE || d) {
				showCenter(window, toDraw, cardsSprite, draw[i]);
				d = true;
			}
		}

		window.draw(scoreText);

		window.display();
	}

	return 0;
}
