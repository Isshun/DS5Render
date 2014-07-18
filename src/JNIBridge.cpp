#include <iostream>
#include "JNIBridge.hpp"
#include "Render.hpp"

JNIBridge::JNIBridge(JNIEnv *env, jobject *thisObj) {
	this->env = env;
	this->thisObj = thisObj;
}

int JNIBridge::refresh(int refresh) {
//	std::cout << "refresh" << std::endl;

	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "onRefresh", "(I)V");
	env->CallObjectMethod(*thisObj, mid, refresh);
}

int JNIBridge::update() {
//	std::cout << "update" << std::endl;

	// Update game
	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "onUpdate", "()V");
	env->CallObjectMethod(*thisObj, mid);

	// Change count
	jfieldID fidNumber = (env)->GetFieldID(thisClass, "number", "I");
	jint number = (env)->GetIntField(*thisObj, fidNumber);
	number++;
	(env)->SetIntField(*thisObj, fidNumber, number);
}

void JNIBridge::getItemInfos(Render* render) {
	std::cout << "getItemInfos" << std::endl;

	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "getItemInfos", "()Ljava/lang/Object;");
	jobject mydata = env->CallObjectMethod(*thisObj, mid);

	jclass clsArrayList = env->FindClass("java/util/ArrayList");
	jclass clsInteger = env->FindClass("java/lang/Integer");
	//jclass clsModel = env->FindClass("alone/in/deepspace/model/item/ItemInfo");
	jmethodID midArrayListSize = env->GetMethodID(clsArrayList, "size", "()I");
	jmethodID midArrayListGet = env->GetMethodID(clsArrayList, "get", "(I)Ljava/lang/Object;");
	jmethodID midIntegerIntValue = env->GetMethodID(clsInteger, "intValue", "()I");

	if((clsArrayList != 0) && (midArrayListSize != 0)){
		int arrayListSize = env->CallIntMethod(mydata,  midArrayListSize );//ok
		if((arrayListSize > 0)&& (midArrayListGet != 0) && (clsInteger != 0) && (midIntegerIntValue != 0)){

			for(int i=0; i<arrayListSize; i++){
				jobject arrayElement = env->CallObjectMethod(mydata,  midArrayListGet, i);//get: will contain 0 :(

				jclass arrayElementClass = (env)->GetObjectClass(arrayElement);
				int width = (env)->GetIntField(arrayElement, (env)->GetFieldID(arrayElementClass, "width", "I"));
				int height = (env)->GetIntField(arrayElement, (env)->GetFieldID(arrayElementClass, "height", "I"));
				int sprite = (env)->GetIntField(arrayElement, (env)->GetFieldID(arrayElementClass, "spriteId", "I"));

				jstring jstr = (jstring)env->GetObjectField(arrayElement, (env)->GetFieldID(arrayElementClass, "name", "Ljava/lang/String;"));
				const char *name = env->GetStringUTFChars(jstr, NULL);

				std::cout << "Add item info: " << name << " (#" << sprite << ", " << width << "x" << height << ")" << std::endl;

				render->addResource(sprite, name, width, height);

				env->ReleaseStringUTFChars(jstr, name);
			}
			std::cout << "Add " << arrayListSize << " items" << std::endl;
		}
	}
}

void JNIBridge::getItems(long ** items, long ** structures, long ** areas) {
//	std::cout << "getItems" << std::endl;

	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "getItems", "()Ljava/lang/Object;");
	jobject mydata = env->CallObjectMethod(*thisObj, mid);

	jclass clsArrayList = env->FindClass("java/util/ArrayList");
	jclass clsInteger = env->FindClass("java/lang/Integer");
	jmethodID midArrayListSize = env->GetMethodID(clsArrayList, "size", "()I");
	jmethodID midArrayListGet = env->GetMethodID(clsArrayList, "get", "(I)Ljava/lang/Object;");
	jmethodID midIntegerIntValue = env->GetMethodID(clsInteger, "intValue", "()I");

	if((clsArrayList != 0) && (midArrayListSize != 0)){
		int arrayListSize = env->CallIntMethod(mydata,  midArrayListSize );//ok
		if((arrayListSize > 0)&& (midArrayListGet != 0) && (clsInteger != 0) && (midIntegerIntValue != 0)){

			for(int i=0; i<arrayListSize; i++){
				jobject arrayElement = env->CallObjectMethod(mydata,  midArrayListGet, i);//get: will contain 0 :(

				jclass arrayElementClass = (env)->GetObjectClass(arrayElement);
				int sprite = (env)->GetIntField(arrayElement, (env)->GetFieldID(arrayElementClass, "sprite", "I"));
				int type = (env)->GetIntField(arrayElement, (env)->GetFieldID(arrayElementClass, "type", "I"));
				int x = (env)->GetIntField(arrayElement, (env)->GetFieldID(arrayElementClass, "x", "I"));
				int y = (env)->GetIntField(arrayElement, (env)->GetFieldID(arrayElementClass, "y", "I"));

				std::cout << "Add item: " << type << " (" << sprite << ", " << x << " x " << y << ")" << std::endl;

				if (type == 1) {
					items[x][y] = sprite;
				} else if (type == 2) {
					structures[x][y] = sprite;
				} else if (type == 4) {
					areas[x][y] = sprite;
				}
			}
			std::cout << "Add " << arrayListSize << " items" << std::endl;
		}
	}
}

void JNIBridge::getCharacters(long ** characters) {
	std::cout << "getCharacters" << std::endl;

	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "getCharacters", "()[I");
	jobject mydata = env->CallObjectMethod(*thisObj, mid);
	jintArray * arr = reinterpret_cast<jintArray*>(&mydata);
	long * data = env->GetIntArrayElements(*arr, NULL);

	for (int x = 0; x < 250; x++) {
		for (int y = 0; y < 250; y++) {
			characters[x][y] = data[x * 250 + y];
		}
	}

	env->ReleaseIntArrayElements(*arr, data, 0);
}

void JNIBridge::click(int x, int y) {
	std::cout << "click: " << x << "x" << y << std::endl;

	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "onClick", "(II)V");
	env->CallObjectMethod(*thisObj, mid, x, y);
}

void JNIBridge::getUI(std::list<View>* views) {
	views->clear();
	//std::cout << "getUI" << std::endl;

	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "getUI", "()Ljava/lang/Object;");
	jobject mydata = env->CallObjectMethod(*thisObj, mid);

	jclass clsArrayList = env->FindClass("java/util/ArrayList");
	jclass clsInteger = env->FindClass("java/lang/Integer");
	jclass clsTextView = env->FindClass("alone/in/deepspace/engine/ui/View");
	jmethodID midArrayListSize = env->GetMethodID(clsArrayList, "size", "()I");
	jmethodID midArrayListGet = env->GetMethodID(clsArrayList, "get", "(I)Ljava/lang/Object;");
	jmethodID midIntegerIntValue = env->GetMethodID(clsInteger, "intValue", "()I");
	jmethodID midGetPosX = env->GetMethodID(clsTextView, "getPosX", "()I");
	jmethodID midGetPosY = env->GetMethodID(clsTextView, "getPosY", "()I");
	jmethodID midGetWidth = env->GetMethodID(clsTextView, "getWidth", "()I");
	jmethodID midGetHeight = env->GetMethodID(clsTextView, "getHeight", "()I");
	jmethodID midGetColor= env->GetMethodID(clsTextView, "getColor", "()I");

	if((clsArrayList != 0) && (midArrayListSize != 0)){
		int arrayListSize = env->CallIntMethod(mydata,  midArrayListSize );//ok
		if((arrayListSize > 0)&& (midArrayListGet != 0) && (clsInteger != 0) && (midIntegerIntValue != 0)){

			for(int i=0; i<arrayListSize; i++){
				jobject arrayElement = env->CallObjectMethod(mydata,  midArrayListGet, i);//get: will contain 0 :(
				int x = env->CallIntMethod(arrayElement, midGetPosX);
				int y = env->CallIntMethod(arrayElement, midGetPosY);
				int width = env->CallIntMethod(arrayElement, midGetWidth);
				int height = env->CallIntMethod(arrayElement, midGetHeight);
				int rgb = env->CallIntMethod(arrayElement, midGetColor);

				sf::Color color = sf::Color((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, (rgb >> 0) & 0xff);
				views->push_back(View(x, y, width, height, color));
			}
		}
	}
}

int JNIBridge::longUpdate() {
	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "onLongUpdate", "()V");
	env->CallObjectMethod(*thisObj, mid);
}

void JNIBridge::getUIText(std::list<TextView>* views) {
	views->clear();
	//std::cout << "getUI" << std::endl;

	jclass thisClass = (env)->GetObjectClass(*thisObj);
	jmethodID mid = env->GetMethodID( thisClass, "getUITexts", "()Ljava/lang/Object;");
	jobject mydata = env->CallObjectMethod(*thisObj, mid);

	jclass clsArrayList = env->FindClass("java/util/ArrayList");
	jclass clsInteger = env->FindClass("java/lang/Integer");
	jclass clsTextView = env->FindClass("alone/in/deepspace/engine/ui/TextView");
	jmethodID midArrayListSize = env->GetMethodID(clsArrayList, "size", "()I");
	jmethodID midArrayListGet = env->GetMethodID(clsArrayList, "get", "(I)Ljava/lang/Object;");
	jmethodID midIntegerIntValue = env->GetMethodID(clsInteger, "intValue", "()I");
	jmethodID midGetString = env->GetMethodID(clsTextView, "getString", "()Ljava/lang/String;");
	jmethodID midGetPosX = env->GetMethodID(clsTextView, "getPosX", "()I");
	jmethodID midGetPosY = env->GetMethodID(clsTextView, "getPosY", "()I");
	jmethodID midGetCharacterSize = env->GetMethodID(clsTextView, "getCharacterSize", "()I");

	if((clsArrayList != 0) && (midArrayListSize != 0)){
		int arrayListSize = env->CallIntMethod(mydata,  midArrayListSize );//ok
		if((arrayListSize > 0)&& (midArrayListGet != 0) && (clsInteger != 0) && (midIntegerIntValue != 0)){

			for(int i=0; i<arrayListSize; i++){
				jobject arrayElement = env->CallObjectMethod(mydata,  midArrayListGet, i);//get: will contain 0 :(
				jstring mydata = (jstring) env->CallObjectMethod(arrayElement, midGetString);
				int x = env->CallIntMethod(arrayElement, midGetPosX);
				int y = env->CallIntMethod(arrayElement, midGetPosY);
				int size = env->CallIntMethod(arrayElement, midGetCharacterSize);
				if (mydata != NULL) {
					const char* text = env->GetStringUTFChars(mydata, NULL);
					views->push_back(TextView(text, x, y, size));
					env->ReleaseStringUTFChars(mydata, text);
				}
			}
		}
	}
}
