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

//#define RENDER_DEBUG

#ifdef RENDER_DEBUG
struct s_debug_data {
	struct s_debug_cell {
		int cx;
		int cy;
		double time;
	};
	std::vector<s_debug_cell> cells;
};
#endif

enum eCellType : uint64_t {
	EMPTY, START, COLLISION=490, FLOOR=455, END
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
	int ground_layer;
	int collision_layer;

	GameMap() {}; // TODO: ASK: REMOVE AND SEE ERROR

	GameMap(const char* gameMap_filename, int collision_layer);

    sCell& getCell(int x, int y, int l);

	void render(Image* fb, Vector2 camera_pos, int layer_id);
	
	bool loadGameMap(const char* filename);

	// TODO: poner getStartPosition

	void update(double dt);
	void add_debug_cell(int cx, int cy, double time = 1.0);
};