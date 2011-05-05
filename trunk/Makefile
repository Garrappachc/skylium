CXX=g++
CXXFLAGS=-g -Wall -ansi -pedantic -D__DEBUG__
LFLAGS=-lSOIL -lSDL -lglee -lGL -lGLU

OBJS=Box.o Camera.o Light.o Material.o Object.o PolygonGroup.o Scene.o SceneManager.o Shader.o Skylium.o

skylium: $(OBJS) main.o
	$(CXX) -o $@  $^ $(LFLAGS)

$(OBJS): %.o: src/%.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

main.o: %.o: %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
