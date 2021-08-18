# http://urin.github.io/posts/2013/simple-makefile-for-clang
CC 		= g++
C_FLAGS = -Wall $(`sdl2-config --cflags --libs`)
L_FLAGS = 

COMPILER  = g++
CFLAGS    = -g -MMD -MP -Wall -Wextra -Winit-self -Wno-missing-field-initializers $(shell sdl2-config --cflags)
ifeq "$(shell getconf LONG_BIT)" "64"
  LDFLAGS =
else
  LDFLAGS =
endif
LIBS      = $(shell sdl2-config --libs) -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf -lSDL2_net
INCLUDE   = -I./include
TARGET    = ./bin/$(shell basename `readlink -f .`)
SRCDIR    = ./src
ifeq "$(strip $(SRCDIR))" ""
  SRCDIR  = .
endif
SOURCES   = $(wildcard $(SRCDIR)/*.cpp)
OBJDIR    = ./obj
ifeq "$(strip $(OBJDIR))" ""
  OBJDIR  = .
endif
OBJECTS   = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
DEPENDS   = $(OBJECTS:.o=.d)

$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	-mkdir -p $(OBJDIR)
	$(COMPILER) $(CFLAGS) $(INCLUDE) -o $@ -c $<

all: clean $(TARGET)

clean:
	-rm -f $(OBJECTS) $(DEPENDS) $(TARGET)

do:
	$(TARGET)

-include $(DEPENDS)