#ifndef JNIBRIDGE_HPP
#define JNIBRIDGE_HPP

#include <jni.h>
#include <list>
#include "TextView.hpp"
#include "View.hpp"
#include "Render.hpp"
#include "Bridge.hpp"

class JNIBridge
{
private:
  JNIEnv*     env;
  jobject*    thisObj;
  JNIBridge*  bridge;
  
public:
    JNIBridge(JNIEnv *env, jobject *thisObj);
    void refresh(int refresh);
    void click(int x, int y);
    void getItemInfos(Render* render);
    void getItems(long** items, long ** structures, long ** areas, int x, int y, int width, int height);
    void getCharacters(long** characters);
    void getUI(std::list<View>* views);
    void getUIText(std::list<TextView>* views);
};

#endif
