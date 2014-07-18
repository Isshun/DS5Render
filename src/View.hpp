#ifndef VIEW_HPP
#define VIEW_HPP

#include <SFML/Graphics.hpp>

class View
{
private:
	sf::Color		color;
	  int			posX;
	  int			posY;
	  int			width;
	  int			height;
  
public:
    View(int x, int y, int width, int height, sf::Color);
    sf::Color getColor() { return color; }
    int	getPosX() { return posX; }
    int	getPosY() { return posY; }
    int	getWidth() { return width; }
    int	getHeight() { return height; }
};

#endif
