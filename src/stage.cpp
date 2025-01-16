#include "stage.h"
#include "gameMap.h"
#include "character.h"
#include "math.h"
#include "input.h"

enum {
	FLOOR, OBJECTS, FURNITURE, PROPS, WALL_FACE, WALL_TOP, COLLISIONS
} layers; // TODO: name?

Stage::Stage()
{
	this->current_stage = this;
	this->time = 0;
}

IntroStage::IntroStage()
{
	this->gameMap = GameMap("data/tileset.tga");
	bool good = this->gameMap.loadGameMap("data/map.json");
	assert(good);
	Vector2 player_initial_position = Vector2{ this->gameMap.size.x * 0.4f, this->gameMap.size.y * 0.95f };
	this->player = Player("data/johnnysilverhand.tga", player_initial_position, 100);
}

void IntroStage::render(Image* fb)
{
	current_stage->camera_clamp(Vector2(fb->width, fb->height));

	gameMap.render(fb, this->camera_position, FLOOR);
	gameMap.render(fb, this->camera_position, OBJECTS);
	gameMap.render(fb, this->camera_position, FURNITURE);
	gameMap.render(fb, this->camera_position, PROPS);

	if (player.position.y < 240.0) {
		player.render(fb, this->camera_position);
		gameMap.render(fb, this->camera_position, WALL_FACE);
	}
	else {
		gameMap.render(fb, this->camera_position, WALL_FACE);
		player.render(fb, this->camera_position);
	}

	gameMap.render(fb, this->camera_position, WALL_TOP);
	//gameMap.render(fb, this->camera_position, COLLISIONS);
}

void IntroStage::update(double dt)
{
	Vector2 offset = { 0.0, 0.0 };
	Vector2 velocity = { 0.0, 0.0 };
	player.frame = RESTING_FRAME;

	if (Input::isKeyPressed(SDL_SCANCODE_UP) || Input::isKeyPressed(SDL_SCANCODE_W)) //if key up
	{
		offset.y = 2; // just two pixels above // TODO: CHANGE
		velocity.y -= 1.0;
		player.set_side(FACE_UP, time);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN) || Input::isKeyPressed(SDL_SCANCODE_S)) //if key down
	{
		offset.y -= 1; // just one pixel below
		velocity.y += 1.0;
		player.set_side(FACE_DOWN, time);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT) || Input::isKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		offset.x = 0.2 * CH_WIDTH;
		velocity.x -= 1.0;
		player.set_side(FACE_LEFT, time);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT) || Input::isKeyPressed(SDL_SCANCODE_D)) //if key right
	{
		offset.x = -0.2 * CH_WIDTH;
		velocity.x += 1.0;
		player.set_side(FACE_RIGHT, time);
	}

	Vector2 target = this->player.position + velocity;

	if (this->is_valid(target, offset)) {
		player.set_velocity(velocity);
	}
	else if (this->is_valid(Vector2(target.x, this->player.position.y), offset)) {
		player.set_velocity(Vector2(velocity.x, 0.0));
	}
	else if (this->is_valid(Vector2(this->player.position.x, target.y), offset)) {
		player.set_velocity(Vector2(0.0, velocity.y));
	}
	else {
		player.set_velocity(Vector2(0.0, 0.0));
	}

	player.move(dt, this->time);

	this->time++;
}

void IntroStage::camera_clamp(Vector2 fb_size)
{
	this->camera_position = this->player.position - fb_size * 0.5; // also update camera position
	this->camera_position.x = clamp(this->camera_position.x, 0.0f, this->gameMap.size.x - fb_size.x);
	this->camera_position.y = clamp(this->camera_position.y, 0.0f, this->gameMap.size.y - fb_size.y);
}

bool IntroStage::is_valid(Vector2 target, Vector2 offset)
{
	int cx = floor((target.x - offset.x) / this->gameMap.tile_width);
	int cy = floor((target.y - offset.y) / this->gameMap.tile_height);

	int cell_type = this->gameMap.getCell(cx, cy, COLLISIONS).type;

	if (cell_type == WALL) {
		return false;
	}
	return true;
}