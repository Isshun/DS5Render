#include <iostream>
#include "TextView.hpp"

TextView::TextView(std::string text, int x, int y, int size) {
	this->text = text;
	this->posX = x;
	this->posY = y;
	this->size = size;
}
