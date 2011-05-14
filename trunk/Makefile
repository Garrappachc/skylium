CXX=g++
CXXFLAGS=-g -Wall -pipe -D__DEBUG__
LFLAGS=-lSOIL -lSDL -lglee -lGL -lGLU

OBJS=Box.o Camera.o Light.o Material.o Object.o ObjectObjParser.o PolygonGroup.o Scene.o SceneManager.o Shader.o Skylium.o Timer.o

skylium: $(OBJS) main.o
	$(CXX) -o $@  $^ $(LFLAGS)

$(OBJS): %.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

main.o: %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o
