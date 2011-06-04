CXX=g++
CXXFLAGS=-g -Wall -pipe -D__DEBUG__ `sdl-config --cflags` -std=c++0x

LSOIL=-lSOIL
LSDL=`sdl-config --libs`
LGLEE=-lglee
LGL=-lGL
LGLU=-lGLU
LGLEW=-lGLEW

LFLAGS=$(LSOIL) $(LSDL) $(LGLEE) $(LGL) $(LGLU) $(LGLEW)

OBJS= Camera.o FaceComp.o FontBase.o Hud.o Light.o Material.o Mesh.o Object.o Scene.o SceneManager.o Shader.o Skylium.o Texture.o TextureManager.o Timer.o Vertex.o config.o


skylium: $(OBJS) main.o
	$(CXX) -o $@  $^ $(LFLAGS)

$(OBJS): %.o: src/%.cpp
	 $(CXX) -c $(CXXFLAGS) $< -o $@

main.o: %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o