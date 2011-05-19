CXX=g++
CXXFLAGS=-g -Wall -pipe -D__DEBUG__

LSOIL=SOIL
LSDL=SDL
LGLEE=GLee
LGL=GL
LGLU=GLU

LFLAGS=-l$(LSOIL) -l$(LSDL) -l$(LGLEE) -l$(LGL) -l$(LGLU)

OBJS=Box.o Camera.o Hud.o Light.o Material.o Object.o ObjectObjParser.o PolygonGroup.o Scene.o SceneManager.o sFont.o Shader.o Skylium.o Timer.o


skylium: $(OBJS) main.o
	$(CXX) -o $@  $^ $(LFLAGS)

$(OBJS): %.o: src/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

main.o: %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o
