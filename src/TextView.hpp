#ifndef TEXTVIEW_HPP
#define TEXTVIEW_HPP

class TextView
{
private:
  std::string	text;
  int			posX;
  int			posY;
  int			size;
  
public:
    TextView(std::string text, int x, int y, int size);
    int	getPosX() { return posX; }
    int	getPosY() { return posY; }
    int	getSize() { return size; }
    std::string getString() { return text; }
};

#endif
