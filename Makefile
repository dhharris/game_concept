CC=clang

BIN=demo

INCS= -I./raylib/src

CFLAGS= $(INCS) -std=c11 -Wall -Werror -Wno-unused -O2 -g
C_FILES= $(wildcard src/*.c)
OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))

PLATFORM = $(shell uname)

ifeq ($(findstring Linux,$(PLATFORM)),Linux)
	OUT=$(BIN)
	# FIXME
	LFLAGS= -lcorange -lGL -lSDL2 -lSDL2_net -lSDL2_mixer -lm
endif

ifeq ($(findstring Darwin,$(PLATFORM)),Darwin)
	OUT=$(BIN)
	LFLAGS= -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL raylib/src/libraylib.a
endif

ifeq ($(findstring MINGW,$(PLATFORM)),MINGW)
	OUT=$(BIN).exe
	# FIXME
	LFLAGS= ../../corange.res -lcorange -lmingw32 -lSDL2main -lSDL2 -lSDL2_Mixer -lSDL2_Net -lopengl32
endif

$(OUT): $(OBJ_FILES)
	$(CC) -g $(OBJ_FILES) $(LFLAGS) -o $@

obj/%.o: src/%.c | obj
	$(CC) $< -c $(CFLAGS) -o $@

obj:
	mkdir obj
	
lib:
	mkdir lib
	
clean:
	$(RM) $(OUT) $(OBJ_FILES)

