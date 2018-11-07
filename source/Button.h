#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Button {
public:
	Button();
	Button(sf::Vector2u & s);
	Button(sf::Text & t);
	Button(sf::Text & t, sf::Vector2u & s);

	sf::Text getText();
	void setText(sf::Text t);

	sf::Vector2u getSize();
	void setSize(sf::Vector2u s);

	~Button();

private:
	sf::Vector2u * size;
	sf::Text * text;
};

#endif