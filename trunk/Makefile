prefix = /usr

NAME = skylium

LIBNAME = libskylium
LIB = $(LIBNAME).so.1.0
LIB_A = $(LIB)
LIB_B = $(LIBNAME).so.1
LIB_C = $(LIBNAME).so

CXX = g++
#CXX = clang++
CXXFLAGS += -O2 -g -Wall -pipe -std=c++0x
CXXLFLAGS = -fPIC -shared

C = gcc
CFLAGS += -O2 -g -Wall -pipe
CLFLAGS = -fPIC -shared

CP = cp
LN = ln -sf
RM = rm -f
RMDIR = rm -rf

LIBOWN = root
LIBGRP = root
LIBMOD = 644

INSTALL_FILE = install -p -o $(LIBOWN) -g $(LIBGRP) -m $(LIBMOD)
INSTALL_DIR = install -p -o $(LIBOWN) -g $(LIBGRP) -d

DESTDIR = $(prefix)/lib
INCDIR = $(prefix)/include

OBJS = Camera.o FontBase.o Hud.o HudData.o Light.o Material.o MatricesManager.o \
	Mesh.o Object.o Scene.o SceneManager.o Shader.o ShaderDataHandler.o Skylium.o \
	Texture.o TextureManager.o Timer.o Vertex.o config.o imgUtils.o keysfunc.o utils.o

COBJS = stb_image.o

D = include

INCLUDES = $(D)/Camera.h $(D)/config.h $(D)/defines.h \
	$(D)/FontBase.h $(D)/Hud.h $(D)/HudData.h $(D)/keysdef.h \
	$(D)/keysfunc.h $(D)/Light.h $(D)/Material.h $(D)/Matrices.h \
	$(D)/MatricesManager.h $(D)/Mesh.h $(D)/Object.h $(D)/Scene.h $(D)/SceneManager.h \
	$(D)/Shader.h $(D)/ShaderDataHandler.h $(D)/Singleton.h $(D)/Skylium.h $(D)/Texture.h \
	$(D)/TextureManager.h $(D)/Timer.h $(D)/utils.h	$(D)/Vectors.h $(D)/Vertex.h

SKYLIUM_MAIN = skylium.h

LDLIBS = -lGL

LFLAGS = -Wl,-soname,$(LIB)


all: $(LIB)

$(LIB): $(OBJS) $(COBJS)
	$(CXX) $(CXXFLAGS) $(CXXLFLAGS) $(LFLAGS) $(LDLIBS) -o $(LIB) $^

$(OBJS): %.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $(CXXLFLAGS) -c $< -o $@

$(COBJS): %.o: src/%.c
	$(C) $(CFLAGS) $(CLFLAGS) -c $< -o $@

install:
	$(INSTALL_DIR) $(INCDIR)/$(NAME)
	$(INSTALL_FILE) $(INCLUDES) $(INCDIR)/$(NAME)
	$(INSTALL_FILE) $(SKYLIUM_MAIN) $(INCDIR)
	$(INSTALL_FILE) $(LIB) $(DESTDIR)
	$(LN) $(DESTDIR)/$(LIB_A) $(DESTDIR)/$(LIB_B)
	$(LN) $(DESTDIR)/$(LIB_A) $(DESTDIR)/$(LIB_C)

uninstall:
	$(RMDIR) $(INCDIR)/$(NAME)
	$(RM) $(INCDIR)/$(SKYLIUM_MAIN)
	$(RM) $(DESTDIR)/$(LIB_C)
	$(RM) $(DESTDIR)/$(LIB_B)
	$(RM) $(DESTDIR)/$(LIB_A)

disaster: clean
	$(RM) $(LIB) $(NAME)

sample:
	$(CXX) $(CXXFLAGS) main.cpp -o $(NAME) -lskylium $(LDLIBS)

clean:
	rm -f *.o

.PHONY: all install uninstall sample clean
