#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include <list>
#include "TextView.hpp"
#include "View.hpp"
#include "Render.hpp"

class Render;

class Bridge
{
public:
    virtual void refresh(int refresh) = 0;
    virtual void click(int x, int y) = 0;
    virtual void getItemInfos(Render* render) = 0;
    virtual void getItems(long** items, long ** structures, long ** areas) = 0;
    virtual void getCharacters(long** characters) = 0;
    virtual void getUI(std::list<View>* views) = 0;
    virtual void getUIText(std::list<TextView>* views) = 0;
};

#endif
