LIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
CFLAGS=-Os -Wall -DPLATFORM_WEB
EMCC=docker run --rm -v $(shell pwd):/src -u $(shell id -u):$(shell id -g) emscripten/emsdk emcc
CC=gcc
EMAR=docker run --rm -v $(shell pwd):/src -u $(shell id -u):$(shell id -g) emscripten/emsdk emar

all: 
	$(CC) src/*.c -o bin/main.exe $(LIBS)

wa_lib: $(LIB_OBJS_PATH)
	$(EMAR) rcs lib/libraylib.a $(LIB_OBJS_PATH)

wasm: 
	$(EMCC) src/*.c -o web/pong.html -Wall -I. -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 -s FORCE_FILESYSTEM=1 --preload-file assets -DPLATFORM_WEB

# %.o:%.c
# 	$(CC) -c $^ $(CFLAGS) $(if $(filter $@, $(NOGL_LIB_OBJS_PATH)), , -DGRAPHICS_API_OPENGL_ES2) -o $@

clean:
	rm web/*
	rm bin/*