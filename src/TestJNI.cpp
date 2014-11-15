#include <stdio.h> 
#include "TestJNI.h" 
#include "Render.hpp"

#if DEBUG
#include "TCPBridge.hpp"
#else
#include <jni.h> 
#include "JNIBridge.hpp"
#endif


void display( long * data ) {
}

JNIEXPORT void JNICALL Java_alone_in_deepspace_Main_init(JNIEnv *env, jobject thisObj, jobjectArray stringArrays) 
{ 
#if DEBUG
  Bridge bridge = TCPBridge("127.0.0.1", 4242);
#else
  Bridge bridge = JNIBridge(env, &thisObj);
#endif

  Render render = Render(&bridge);

  bridge.getItemInfos(&render);

  int size = env->GetArrayLength(stringArrays);
  for (int i = 0; i < size; ++i)
  {
      jstring string = (jstring)env->GetObjectArrayElement(stringArrays, i);
      const char* myarray = env->GetStringUTFChars(string, 0);
      //.... do some work or copy it to a c++ array of char*....
      
      env->ReleaseStringUTFChars(string, myarray);
      env->DeleteLocalRef(string);
  }
 /* 
  const char *param[20];
  jsize stringCount = (*env).GetArrayLength(prdctini);

    for (int i=0; i<stringCount; i++) {
            jstring string = (jstring) (*env).GetObjectArrayElement( prdctini, i);
            param[i] = (*env).GetStringUTFChars( string, NULL);
    }
    cout<<"U_Id="<<param[0]<<endl;
    cout<<"aggregation="<<param[1]<<endl
  
    jmethodID mid = env->GetMethodID( thisClass, "getInfo", "()[I");
    jobject mydata = env->CallObjectMethod(*thisObj, mid);
    jintArray * arr = reinterpret_cast<jintArray*>(&mydata);
    long * data = env->GetIntArrayElements(*arr, NULL);
    
    for (int x = 0; x < 250; x++) {
      for (int y = 0; y < 250; y++) {
        map[x][y] = data[x * 250 + y];
      }
    }
    
    env->ReleaseIntArrayElements(*arr, data, 0);
*/

  
  render.init();

  return; 
}
