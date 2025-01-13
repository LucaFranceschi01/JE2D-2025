#include "gameMap.h"
#include "image.h"

GameMap::GameMap(const char* filename) {
	this->layers = nullptr;
	this->width = 0;
	this->height = 0;
    this->numLayers = 0;
	this->tile_width = 0;
	this->tile_height = 0;
    this->map_size = { 0.0, 0.0 }; // resolution "on the framebuffer"
    this->tileset.loadTGA(filename);
}

GameMap::GameMap(const char* filename, int w, int h) {
	this->layers = nullptr;
	this->width = w;
	this->height = h;
    this->numLayers = 0;
    this->tile_width = 0;
    this->tile_height = 0;
    this->map_size = { 0.0, 0.0 };
    this->tileset.loadTGA(filename);
}

sCell& GameMap::getCell(int x, int y, int l)
{
	return this->layers[l].data[x + y * width];
}

void GameMap::render(Image* fb, Vector2 camera_pos)
{
    int num_tiles_x = this->tileset.width / this->tile_width;
    int num_tiles_y = this->tileset.height / this->tile_height;


    for (int layer_id = 0; layer_id < this->numLayers; layer_id++)
    {
        // For every cell
        for (int x = 0; x < this->width; ++x)
        {
            for (int y = 0; y < this->height; ++y)
            {
                // Get cell info
                sCell& cell = getCell(x, y, layer_id);
                if (cell.type == -1)
                    continue;
                int type = (int)cell.type;

                // Compute tile pos in tileset image
                int tilex = (type % num_tiles_x) * this->tile_width;
                int tiley = floor(type / num_tiles_y) * this->tile_height;

                // Create tile area
                Area area(tilex, tiley, this->tile_width, this->tile_height);

                // Screen position with camera offset
                int screenx = x * this->tile_width - camera_pos.x;
                int screeny = y * this->tile_height - camera_pos.y;

                // Avoid rendering out of screen stuff
                if (screenx < -this->tile_width || screenx >(int)fb->width ||
                    screeny < -this->tile_height || screeny >(int)fb->height)
                    continue;

                // Draw region of tileset inside framebuffer
                fb->drawImage(this->tileset, screenx, screeny, area);
            }
        }
    }
}

GameMap* loadGameMap(const char* filename, const char* tileset_filename)
{
    using json = nlohmann::json;
    std::ifstream f(filename);
    if (!f.good())
        return nullptr;
    json jData = json::parse(f);

    int w = jData["width"];
    int h = jData["height"];
    int numLayers = jData["layers"].size();

    GameMap* map = new GameMap(tileset_filename, w, h);
    // Allocate memory each layer
    map->layers = new sLayer[numLayers];
    map->numLayers = numLayers;
    map->tile_width = jData["tilewidth"];
    map->tile_height = jData["tileheight"];

    for (int l = 0; l < numLayers; l++) {
        // Allocate memory for data inside each layer
        map->layers[l].data = new sCell[w * h];
        json layer = jData["layers"][l];
        for (int x = 0; x < map->width; x++) {
            for (int y = 0; y < map->height; y++) {
                int index = x + y * map->width;
                int type = layer["data"][index].get<int>();
                map->getCell(x, y, l).type = eCellType(type - 1);
            }
        }
    }

    map->map_size.x = map->tile_width * map->width;
    map->map_size.y = map->tile_height * map->height;

    return map;
}