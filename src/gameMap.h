/*
	The code from the slides but a bit refactored
*/

#pragma once

#include "json.hpp"
#include "image.h"
#include <cstdint> // uint8_t, uint16_t, uint32_t, uint64_t
#include <fstream>

// Don’t use uint8 for tilesets bigger
// than 16x16, since you will have more than
// 256 different tiles

constexpr auto SPACING = 1;

enum eCellType : uint64_t {
	EMPTY, START, COLLISION=106, DOOR, CHEST
};

enum eItemType : uint64_t {
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
	Vector2 size;

	Image tileset;

    sLayer* layers;

	GameMap() {}; // TODO: ASK: REMOVE AND SEE ERROR

	GameMap(const char* tileset_filename);

    sCell& getCell(int x, int y, int l);

	void render(Image* fb, Vector2 camera_pos, int layer_id);
	
	bool loadGameMap(const char* filename); // TODO: PONER DENTRO DE LA CLASE

	// TODO: poner getStartPosition
};