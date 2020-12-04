build:
	gcc -Wfatal-errors \
	./src/*.cpp \
	-std=c99 \
	-lmingw32 \
	-lgdi32 \
	-lkernel32 \
	-luser32 \
	-lm \
	-o 2Drenderer.exe
run:
	2Drenderer.exe
