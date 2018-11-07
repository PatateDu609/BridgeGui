#include "Button.h"

Button::Button() {
	sf::Font cl;
	if (!cl.loadFromFile("C:/Users/azuz/source/repos/SFML_test/SFML_test/Font/CL.ttf")) {}

	sf::Text t;
	t.setFont(cl);
	t.setFillColor(sf::Color::White);
	t.setCharacterSize(24);
	t.setStyle(sf::Text::Bold);

	text = &t;
}

Button::Button(sf::Vector2u & s) : size(&s) {

}

Button::Button(sf::Text & t) : text(&t) {

}

Button::Button(sf::Text & t, sf::Vector2u & s) : text(&t), size(&s) {

}

sf::Text Button::getText() {
	return *text;
}

void Button::setText(sf::Text t) {
	text = &t;
}

sf::Vector2u Button::getSize() {
	return *size;
}

void Button::setSize(sf::Vector2u s) {
	if (s.x > 0 && s.y > 0) size = &s;
}

Button::~Button() {
	delete size;
	delete text;
}