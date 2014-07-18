#ifndef TEXTVIEW_HPP
#define TEXTVIEW_HPP

#include <SFML/Graphics.hpp>

class TextView
{
private:
	std::string	text;
	int			posX;
	int			posY;
	int			size;
	sf::Color		color;

public:
	TextView(std::string text, sf::Color color, int x, int y, int size);
	int	getPosX() { return posX; }
	int	getPosY() { return posY; }
	int	getSize() { return size; }
	sf::Color getColor() { return color; }
	std::string getString() { return text; }
};

#endif
