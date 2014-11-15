#ifndef TCPBRIDGE_HPP
#define TCPBRIDGE_HPP

#include <SFML/Network.hpp>
#include <list>
#include "TextView.hpp"
#include "View.hpp"
#include "Render.hpp"
#include "Bridge.hpp"

class TCPBridge : public Bridge
{
private:
    sf::TcpSocket* socket;
  
public:
    TCPBridge(std::string url, int port);
    void refresh(int refresh);
    void click(int x, int y);
    void getItemInfos(Render* render);
    void getItems(long** items, long ** structures, long ** areas);
    void getCharacters(long** characters);
    void getUI(std::list<View>* views);
    void getUIText(std::list<TextView>* views);
    void sendRequest(std::string cmd, std::string* response);
};

#endif
