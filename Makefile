uild:
	gcc -Wfatal-errors \
	./src/*.cpp \
	-L"user32.lib" \
	-L"Gdi32.lib" \
	-lmingw32 \
	-lgdi32 \
	-lm \
	-o 2Drenderer.exe
run:
	2Drenderer.exe
