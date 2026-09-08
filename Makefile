milestone1:
	gcc milestone1.c dijkstra.c -o dijkstra
milestone2:
	gcc milestone2.c dijkstra.c -o sim -I raylib/src raylib/src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
milestone3:
	gcc milestone3.c dijkstra.c -o sim -I raylib/src raylib/src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
milestone4:
	gcc milestone4.c dijkstra.c graphics.c travelers.c -o sim -I raylib/src raylib/src/libraylib.a -lGL -lm -lpthread -ldl -lrt -lX11
clean:
	rm -f dijkstra sim *.o
