/*
	The code from the slides but a bit refactored
*/

#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "json.hpp"
#include "image.h"
#include <cstdint> // uint8_t, uint16_t, uint32_t, uint64_t
#include <fstream>

// Don’t use uint8 for tilesets bigger
// than 16x16, since you will have more than
// 256 different tiles

enum eCellType : uint16_t {
	EMPTY, START, WALL, DOOR, CHEST
};

enum eItemType : uint16_t {
	NOTHING, SWORD, POTION
};

struct sCell {
	eCellType type;
	eItemType item;
};

struct sLayer {
	sCell* data;
};

class GameMap {

public:

	int width;
	int height;
    int tile_width;
    int tile_height;
	int numLayers;
	Vector2 map_size;

	Image tileset;

    sLayer* layers;

    GameMap(const char* filename);

    GameMap(const char* filename, int w, int h);

    sCell& getCell(int x, int y, int l);

	void render(Image* fb, Vector2 camera_pos);
};

GameMap* loadGameMap(const char* filename, const char* tileset_filename);

#endif