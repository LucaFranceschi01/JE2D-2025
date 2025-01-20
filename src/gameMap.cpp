#include "gameMap.h"
#include "image.h"

 #define RENDER_DEBUG

#ifdef RENDER_DEBUG
struct s_debug_data {
    struct s_debug_cell {
        int cx;
        int cy;
        double time;
    };
    std::vector<s_debug_cell> cells;
};

s_debug_data debug_data = {};
#endif

GameMap::GameMap(const char* tileset_filename) {
	this->layers = nullptr;
	this->width = 0;
	this->height = 0;
    this->numLayers = 0;
	this->tile_width = 0;
	this->tile_height = 0;
    this->size = Vector2( 0.0, 0.0 ); // resolution "on the framebuffer"
    this->tileset.loadTGA(tileset_filename);
}

sCell& GameMap::getCell(int x, int y, int l)
{
	return this->layers[l].data[x + y * width];
}

void GameMap::render(Image* fb, Vector2 camera_pos, int layer_id)
{
    int num_tiles_x = (this->tileset.width+SPACING) / (this->tile_width+SPACING);
    int num_tiles_y = (this->tileset.height+SPACING) / (this->tile_height+SPACING);

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
            int tilex = (type % num_tiles_x) * (this->tile_width + SPACING);
            int tiley = floor(type / num_tiles_x) * (this->tile_height + SPACING); // Fixed divisor

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
#ifdef RENDER_DEBUG
    Color red(255, 0, 0);
    for (const auto& dc : debug_data.cells) {
        fb->drawRectangle(dc.cx * this->tile_width - camera_pos.x, dc.cy * this->tile_height - camera_pos.y, this->tile_width, this->tile_height, red);
    }
#endif
}

bool GameMap::loadGameMap(const char* filename)
{
    using json = nlohmann::json;
    std::ifstream f(filename);
    if (!f.good())
        return false;
    json jData = json::parse(f);

    int w = jData["width"];
    int h = jData["height"];
    int numLayers = jData["layers"].size();

    //GameMap* map = new GameMap(tileset_filename, w, h);
    // Allocate memory each layer
    this->width = w;
    this->height = h;
    this->layers = new sLayer[numLayers];
    this->numLayers = numLayers;
    this->tile_width = jData["tilewidth"];
    this->tile_height = jData["tileheight"];

    for (int l = 0; l < numLayers; l++) {
        // Allocate memory for data inside each layer
        this->layers[l].data = new sCell[w * h];
        json layer = jData["layers"][l];
        for (int x = 0; x < this->width; x++) {
            for (int y = 0; y < this->height; y++) {
                int index = x + y * this->width;
                int type = layer["data"][index].get<int>();
                this->getCell(x, y, l).type = eCellType(type - 1);
            }
        }
    }

    this->size.x = this->tile_width * this->width;
    this->size.y = this->tile_height * this->height;

    return true;
}

void GameMap::update(double dt)
{
#ifdef RENDER_DEBUG
    for (auto& dc : debug_data.cells) {
        dc.time -= dt;

        if (dc.time < 0.0f) {
            debug_data.cells.erase(debug_data.cells.begin());
        }
    }
#endif
}

void GameMap::add_debug_cell(int cx, int cy, double time)
{
#ifdef RENDER_DEBUG
    s_debug_data::s_debug_cell cell{cx, cy, time};
    debug_data.cells.push_back(cell);
#endif
}