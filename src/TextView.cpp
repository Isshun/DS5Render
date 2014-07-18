#include <iostream>
#include "TextView.hpp"

TextView::TextView(std::string text, sf::Color color, int x, int y, int size) {
	this->color = color;
	this->text = text;
	this->posX = x;
	this->posY = y;
	this->size = size;
}
