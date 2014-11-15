#ifndef RENDER_HPP
#define RENDER_HPP

#include <iostream>
#include <list>
#include <SFML/Graphics.hpp>
#include "TextView.hpp"
#include "View.hpp"
#include "Bridge.hpp"

class Render
{
private:
	Bridge* _bridge;
	std::map<int, sf::Sprite*>  _map;
	int	_maxId;

	int _count;
	sf::Font* _font;

	long** _mapItems;
	long** _mapAreas;
	long** _mapStructures;
	long** _mapCharacters;

	sf::RenderWindow*	_window;
	sf::RenderTexture* _texture1;
	sf::RenderTexture* _texture2;
	sf::RenderTexture* _texture3;
	sf::RenderStates*  _renderState;
	sf::Transform*		_transform;
	sf::Sprite* _sprite1;
	sf::Sprite* _sprite2;
	sf::Sprite* _sprite3;

	int _invalid;
	bool	_isRightClick;

	int _pressX;
	int _pressY;

	int	lastX;
	int	lastY;

	int _frame;
	int _viewX;
	int _viewY;
	int _realX;
	int _realY;
	int _viewWidth;
	int _viewHeight;

	std::list<TextView> list;
	std::list<View> listViews;

public:
	Render(Bridge* bridge);
	~Render();
	void handleEvents();
	void handleUpdates();
	void handleRenders();
	void releaseRightButton(int x, int y);
	void init();
	void addResource(int id, const char* name, int width, int height);
	void drawCharacters(sf::RenderStates renderState);
	void drawUI();
};

#endif
