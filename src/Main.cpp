#include <stdio.h> 
#include "Render.hpp"
#include "TCPBridge.hpp"

int main() 
{ 
  TCPBridge bridge = TCPBridge("127.0.0.1", 4242);

  Render render = Render(&bridge);

  bridge.getItemInfos(&render);

  /*
  int size = env->GetArrayLength(stringArrays);
  for (int i = 0; i < size; ++i)
  {
      jstring string = (jstring)env->GetObjectArrayElement(stringArrays, i);
      const char* myarray = env->GetStringUTFChars(string, 0);
      
      env->ReleaseStringUTFChars(string, myarray);
      env->DeleteLocalRef(string);
  }
  */
  
  render.init();

  return 0; 
}
