rem g++ -c src/JNIBridge.cpp src/Render.cpp src/TestJNI.cpp src/TextView.cpp src/View.cpp -I C:\SFML\include -I C:\java\include -I C:\java\include\win32
rem g++ -shared -o Debug/render.dll JNIBridge.o Render.o TestJNI.o TextView.o View.o TestJNI.def -L C:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system

g++ -DDEBUG -c src/TCPBridge.cpp src/Render.cpp src/Main.cpp src/TextView.cpp src/View.cpp -I C:\SFML\include -Ilibs\rapidxml-1.13
echo "Compile OK"

g++ -o Debug/render.exe TCPBridge.o Render.o Main.o TextView.o View.o -L C:\SFML\lib -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system
echo "Link OK"

pause

rem g++ TestJNI.o -o app -L C:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-system
