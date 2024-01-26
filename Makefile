all:
	g++ -o game main.cpp Player.cpp Arrow.cpp defaultEnemy.cpp Boomerang.cpp boomEnemy.cpp Bomb.cpp mentalEnemy.cpp mentalHardEnemy.cpp Wolf.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf && ./game
