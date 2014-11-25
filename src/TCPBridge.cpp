#include <iostream>
#include "TCPBridge.hpp"
#include "Render.hpp"
#include "rapidxml.hpp"
#include <stdio.h>
#include <string.h>
#include <sstream>

using namespace rapidxml;

#define BUFFER_SIZE 4096

TCPBridge::TCPBridge(std::string url, int port) {
    this->socket = new sf::TcpSocket();
    sf::Socket::Status status = this->socket->connect(url, port);
    if (status != sf::Socket::Done) {
        std::cout << "unable to connect" << std::endl;
    }
}

void TCPBridge::refresh(int refresh) {
	std::cout << "refresh" << std::endl;
}

void TCPBridge::getItemInfos(Render* render) {
	std::cout << "getItemInfos" << std::endl;
    
    std::string str;
    sendRequest("getItemInfos\r\n", &str);
    
    int size = str.size();
    char buffer[size+1];
    strcpy(buffer, str.c_str());
    buffer[size] = 0;
    
    std::cout << "copy " << std::endl;

    xml_document<> doc;    // character type defaults to char
    doc.parse<0>(buffer);    // 0 means default parse flags

    std::cout << "parse " << buffer << std::endl;

    xml_node<> *ui_node = doc.first_node();
    for (xml_node<> *view_node = ui_node->first_node(); view_node; view_node = view_node->next_sibling()) {
        int width = atoi(view_node->first_attribute("width")->value());
        int height = atoi(view_node->first_attribute("height")->value());
        int sprite = atoi(view_node->first_attribute("sprite")->value());
        char* name = view_node->first_attribute("name")->value();
        
        std::cout << "parse " << name << std::endl;

        render->addResource(sprite, name, width, height);
    }
    
    std::cout << "done " << std::endl;
}

void TCPBridge::getItems(long ** items, long ** structures, long ** areas, int x, int y, int width, int height) {
	std::cout << "getItems" << std::endl;
    
    std::string str;
    std::stringstream stream;
    
    stream << "getItems\r\n" << x << "\r\n" << y << "\r\n" << width << "\r\n" << height << "\r\n";
    sendRequest(stream.str().c_str(), &str);
    
    std::cout << "size " << str.size() << std::endl;
    
    int size = str.size();
    char* buffer = new char[size+1];
    strcpy(buffer, str.c_str());
    buffer[size] = 0;
    
    std::cout << "copy " << std::endl;

    xml_document<> doc;    // character type defaults to char
    doc.parse<0>(buffer);    // 0 means default parse flags

    std::cout << "parse " << buffer << std::endl;

    xml_node<> *ui_node = doc.first_node();
    for (xml_node<> *view_node = ui_node->first_node(); view_node; view_node = view_node->next_sibling()) {
        int x = atoi(view_node->first_attribute("x")->value());
        int y = atoi(view_node->first_attribute("y")->value());
        int sprite = atoi(view_node->first_attribute("sprite")->value());
        int type = atoi(view_node->first_attribute("type")->value());
        
        if (type == 1) {
            items[x][y] = sprite;
        } else if (type == 2) {
            structures[x][y] = sprite;
        } else if (type == 4) {
            areas[x][y] = sprite;
        }
    }
    
    delete buffer;
    
    std::cout << "done " << std::endl;
}

void TCPBridge::getCharacters(long ** characters) {
	std::cout << "getCharacters" << std::endl;
}

void TCPBridge::click(int x, int y) {
	std::cout << "click: " << x << "x" << y << std::endl;
}

void TCPBridge::getUI(std::list<View>* views) {
	std::cout << "getUI" << std::endl;
	views->clear();

    std::string str;
    sendRequest("getUI\r\n", &str);
    
    int size = str.size();
    char buffer[size+1];
    strcpy(buffer, str.c_str());
    buffer[size] = 0;
    
    std::cout << "copy " << std::endl;

    xml_document<> doc;    // character type defaults to char
    doc.parse<0>(buffer);    // 0 means default parse flags

    std::cout << "parse " << buffer << std::endl;

    xml_node<> *ui_node = doc.first_node();
    for (xml_node<> *view_node = ui_node->first_node(); view_node; view_node = view_node->next_sibling()) {
        int x = atoi(view_node->first_attribute("x")->value());
        int y = atoi(view_node->first_attribute("y")->value());
        int width = atoi(view_node->first_attribute("width")->value());
        int height = atoi(view_node->first_attribute("height")->value());
        int rgb = atoi(view_node->first_attribute("rgb")->value());
        sf::Color color = sf::Color::Transparent;
        if (rgb != -1) {
            color = sf::Color((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, (rgb >> 0) & 0xff);
        }
        
        views->push_back(View(x, y, width, height, color));
    }
    
    std::cout << "done " << std::endl;
}

void TCPBridge::getUIText(std::list<TextView>* views) {
	std::cout << "getUIText" << std::endl;
	views->clear();

    std::string str;
    sendRequest("getUIText\r\n", &str);
    
    int size = str.size();
    char buffer[size+1];
    strcpy(buffer, str.c_str());
    buffer[size] = 0;
    
    std::cout << "copy " << std::endl;

    xml_document<> doc;    // character type defaults to char
    doc.parse<0>(buffer);    // 0 means default parse flags

    std::cout << "parse " << buffer << std::endl;

    xml_node<> *ui_node = doc.first_node();
    for (xml_node<> *view_node = ui_node->first_node(); view_node; view_node = view_node->next_sibling()) {
        int x = atoi(view_node->first_attribute("x")->value());
        int y = atoi(view_node->first_attribute("y")->value());
        int size = atoi(view_node->first_attribute("size")->value());
        char* text = view_node->first_attribute("y")->value();
        //int width = atoi(view_node->first_attribute("width")->value());
        //int height = atoi(view_node->first_attribute("height")->value());
        int rgb = atoi(view_node->first_attribute("rgb")->value());
        sf::Color color = sf::Color::Transparent;
        if (rgb != -1) {
            color = sf::Color((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, (rgb >> 0) & 0xff);
        }
        
        views->push_back(TextView(text, color, x, y, size));
    }
    
    std::cout << "done " << std::endl;
}

void TCPBridge::sendRequest(std::string cmd, std::string* response) {
    if (socket->send(cmd.c_str(), cmd.size()) != sf::Socket::Done) {
        std::cout << "getUI Error" << std::endl;
    }
    
    char data[BUFFER_SIZE];
    std::size_t received;
    do {
        if (socket->receive(data, BUFFER_SIZE, received) != sf::Socket::Done) {
            std::cout << "getUI Error" << std::endl;
        }
        response->append(data, received);
        //std::cout << "Received " << received << "bytes" << std::endl;
    } while (received == BUFFER_SIZE);
    
    //std::cout << "Received " << *response << std::endl;
}

