#include <iostream>
#include "View.hpp"

View::View(int x, int y, int width, int height, sf::Color color) {
	this->color = color;
	this->posX = x;
	this->posY = y;
	this->width = width;
	this->height = height;
}
