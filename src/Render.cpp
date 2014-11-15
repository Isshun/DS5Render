#include <stdio.h> 
#include <sstream>
#include <string>
#include <iostream>
#include "Render.hpp"
#include <sys/time.h>
#include <unistd.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1200

Render::Render(Bridge* bridge) {
    _frame = 0;
	_bridge = bridge;
	_map = std::map<int, sf::Sprite*>();
	_pressX = 0;
	_pressY = 0;
	_maxId = 0;
	_count = 0;
	_invalid = 0;
	_viewX = 130;
	_viewY = 130;
	_viewWidth = 100;
	_viewHeight = 80;
	lastX = 0;
	lastY = 0;
	_isRightClick = false;

	_window = NULL;
	_transform = new sf::Transform();
	_font = new sf::Font();
	_sprite1 = new sf::Sprite();
	_sprite2 = new sf::Sprite();
	_sprite3 = new sf::Sprite();
	_texture1 = new sf::RenderTexture();
	_texture2 = new sf::RenderTexture();
	_texture3 = new sf::RenderTexture();

	_mapItems = new long*[250];
	_mapAreas = new long*[250];
	_mapStructures = new long*[250];
	_mapCharacters = new long*[250];
	for (int i = 0; i < 250; i++) {
		_mapItems[i] = new long[250];
		_mapAreas[i] = new long[250];
		_mapStructures[i] = new long[250];
		_mapCharacters[i] = new long[250];
		for (int j = 0; j < 250; j++) {
			_mapItems[i][j] = 0;
			_mapAreas[i][j] = 0;
			_mapStructures[i][j] = 0;
			_mapCharacters[i][j] = 0;
		}
	}
}

Render::~Render() {
	delete _window;
	delete _renderState;
	delete _texture1;
	delete _texture2;
	delete _texture3;
	delete _sprite1;
	delete _sprite2;
	delete _sprite3;
	delete _font;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

void Render::addResource(int id, const char* name, int width, int height) {
	if (id > _maxId) {
		_maxId = id;
	}

	sf::Texture *texture = new sf::Texture();
	std::vector<std::string> vector;
	split(name, '.', vector);

	texture->loadFromFile("data/items/"+vector[1]+".png");
	_map[id] = new sf::Sprite();
	_map[id]->setTexture(*texture);
	_map[id]->setTextureRect(sf::IntRect(0, 0, 32 * width, 32 * height));
	std::cout << "addResource: " << "data/items/"+vector[1]+".png" << " (" << id << ") " << std::endl;
}

void Render::init() {
	_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	if (!_font->loadFromFile("arial.ttf")) {
		// erreur...
	}

	_transform->translate(-_viewX * 32, -_viewY * 32);

	_texture1->create(250 * 32, 250 * 32);
	_texture1->setSmooth(true);
	_texture1->display();

	_texture2->create(250 * 32, 250 * 32);
	_texture2->setSmooth(true);
	_texture2->display();

	_texture3->create(250 * 32, 250 * 32);
	_texture3->setSmooth(true);
	_texture3->display();

	struct timeval start, end;

	while (_window->isOpen()) {
		gettimeofday(&start, NULL);

		// Events
		//		std::cout << "handleEvents" << std::endl;
		handleEvents();

		// Render
		//		std::cout << "handleRenders" << std::endl;
		handleRenders();

		// Updates
		//		std::cout << "handleUpdate" << std::endl;
		handleUpdates();

		// Sleep
		gettimeofday(&end, NULL);
		long mtime = ((end.tv_sec  - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) + 0.5;
		//printf("Main loop: %ld ms\n", mtime);
		if (mtime < 10) {
			sf::sleep(sf::milliseconds(10 - mtime));
		}
	}
}

void Render::handleUpdates() {
	if (_count % 60 == 0) {
		_invalid = 0;
		//		gettimeofday(&start, NULL);
		if (!_isRightClick) {
			_bridge->getItems(_mapItems, _mapStructures, _mapAreas);
		}
		//		gettimeofday(&end, NULL);
		//		mtime = ((end.tv_sec  - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) + 0.5;
		//printf("update: %ld milliseconds\n", mtime);
	}

	if (_count % 10 == 0) {
		//		gettimeofday(&start, NULL);
		_bridge->getCharacters(_mapCharacters);
		//		gettimeofday(&end, NULL);
		//		mtime = ((end.tv_sec  - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) + 0.5;
		//printf("update char: %ld milliseconds\n", mtime);
	}

	_count++;
}

void Render::handleEvents() {
	sf::Event event;

	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			_window->close();
		}
		if (event.type == sf::Event::MouseLeft) {
			releaseRightButton(lastX, lastY);
		}
		if (event.type == sf::Event::MouseMoved && _isRightClick) {
			_transform->translate(-(lastX - event.mouseMove.x), -(lastY - event.mouseMove.y));
			lastX = event.mouseMove.x;
			lastY = event.mouseMove.y;
		}
		if (event.mouseButton.button == sf::Mouse::Left) {
			if (event.type == sf::Event::MouseButtonReleased) {
				_bridge->click(event.mouseButton.x, event.mouseButton.y);
				_bridge->getUIText(&list);
				_bridge->getUI(&listViews);
			}
		}
		if (event.mouseButton.button == sf::Mouse::Right) {
			if (event.type == sf::Event::MouseButtonPressed) {
				_isRightClick = true;
				lastX = _pressX = event.mouseButton.x;
				lastY = _pressY = event.mouseButton.y;
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				releaseRightButton(event.mouseButton.x, event.mouseButton.y);
			}
		}
	}
}

void Render::releaseRightButton(int x, int y) {
	if (_isRightClick) {
		_isRightClick = false;
		_viewX += (_pressX - x) / 32;
		_viewY += (_pressY - y) / 32;
		_invalid = 0;
	}
}

void Render::handleRenders() {
	struct timeval start, end;

	_window->clear();

	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(1, 1));

	if (_invalid < 3) {
		if (_invalid == 0) {
			_texture1->clear(sf::Color::Transparent);
		} else if (_invalid == 1) {
			_texture2->clear(sf::Color::Transparent);
		} else if (_invalid == 2) {
			_texture3->clear(sf::Color::Transparent);
		}

		gettimeofday(&start, NULL);
		for (int x = _viewX-_viewWidth; x < _viewX + _viewWidth*2; x++) {
			for (int y = _viewY-_viewHeight; y < _viewY + _viewHeight*2; y++) {
				if (x >= 0 && x < 250 && y >= 0 && y < 250) {
					// Refresh area
					if (_invalid == 0) {
						int id = _mapAreas[x][y];
						if (id != 0 && id <= _maxId && _map[id] != NULL) {
							_map[id]->setPosition(x * 32, y * 32);
							_texture1->draw(*_map[id]);
						}
					}

					// Refresh structures
					if (_invalid == 1) {
						int id = _mapStructures[x][y];
						if (id != 0 && id <= _maxId && _map[id] != NULL) {
							_map[id]->setPosition(x * 32, y * 32);
							_texture2->draw(*_map[id]);
						}
					}

					// Refresh items
					if (_invalid == 2) {
						int id = _mapItems[x][y];
						if (id != 0 && id <= _maxId && _map[id] != NULL) {
							_map[id]->setPosition(x * 32, y * 32);
							_texture3->draw(*_map[id]);
						}
					}
				}
			}
		}
		gettimeofday(&end, NULL);
		//mtime = ((end.tv_sec  - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) + 0.5;
		//printf("update sprites #%d: %ld milliseconds\n", invalid, mtime);
	}


	if (_invalid < 3) {
		std::cout << "text" << std::endl;
		if (_invalid == 0) {
			_sprite1->setTexture(_texture1->getTexture());
		} else if (_invalid == 1) {
			_sprite2->setTexture(_texture2->getTexture());
		} else if (_invalid == 2) {
			_sprite3->setTexture(_texture3->getTexture());
		}
		_invalid++;
	}

	sf::RenderStates renderState = sf::RenderStates(*_transform);

	_window->draw(*_sprite1, renderState);
	_window->draw(*_sprite2, renderState);
	_window->draw(*_sprite3, renderState);

	//	for (int x = _viewX; x < _viewX + _viewWidth; x++) {
	//		for (int y = _viewY; y < _viewY + _viewHeight; y++) {
	//			int id = _mapItems[x][y];
	//			std::stringstream ss;
	//			ss << id;
	//			sf::Text text;
	//			text.setColor(sf::Color::Black);
	//			text.setFont(*_font);
	//			text.setString(ss.str());
	//			text.setCharacterSize(12);
	//			text.setPosition((x-_viewX) * 32, (y-_viewY ) *32);
	//			_window->draw(text);
	//		}
	//	}

	//	std::cout << "drawCharacters" << std::endl;
	drawCharacters(renderState);

	//	std::cout << "drawUI" << std::endl;
	drawUI();

	_window->display();
}

void Render::drawCharacters(sf::RenderStates renderState) {
	for (int x = _viewX; x < _viewX + _viewWidth; x++) {
		for (int y = _viewY; y < _viewY + _viewHeight; y++) {
			if (x >= 0 && x < 250 && y >= 0 && y < 250) {
				int id = _mapCharacters[x][y];
				if (id != 0) {
					_map[id]->setPosition(x * 32, y * 32);
					_window->draw(*_map[id], renderState);
				}
			}
		}
	}
}

void Render::drawUI() {
	if (_count % 20 == 0) {
		//		gettimeofday(&start, NULL);
		_bridge->getUIText(&list);
		_bridge->getUI(&listViews);
		//		gettimeofday(&end, NULL);
		//		mtime = ((end.tv_sec  - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000.0) + 0.5;
		//printf("update UI: %ld milliseconds\n", mtime);
	}

	// Shapes
	for (std::list<View>::iterator it = listViews.begin(); it != listViews.end(); ++it) {
		sf::RectangleShape shape;
		shape.setSize(sf::Vector2f((*it).getWidth(), (*it).getHeight()));
		shape.setFillColor((*it).getColor());
		shape.setPosition((*it).getPosX(), (*it).getPosY());
		_window->draw(shape);
	}

	// Texts
	for (std::list<TextView>::iterator it = list.begin(); it != list.end(); ++it) {
		std::stringstream ss;
		ss << (*it).getString();
		sf::Text text;
		text.setFont(*_font);
		text.setString(ss.str());
		text.setColor((*it).getColor());
		text.setCharacterSize((*it).getSize());
		text.setPosition((*it).getPosX(), (*it).getPosY());
		_window->draw(text);
	}
    
    std::stringstream ss;
    ss << _frame++;
    sf::Text text;
    text.setFont(*_font);
    text.setString(ss.str());
    //text.setColor((*it).getColor());
    //text.setCharacterSize((*it).getSize());
    text.setPosition(10, 10);
    _window->draw(text);
}
