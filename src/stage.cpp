#include "stage.h"
#include "gameMap.h"
#include "character.h"
#include "math.h"
#include "input.h"

enum {
	BACKGROUND, WATER, GRASS, PLATFORMS, DEBUG, COLLISIONS, WALKABLE
};

Stage::Stage()
{
	this->current_stage = this;
	this->time = 0;
}

PlayStage::PlayStage()
{
	this->gameMap = GameMap("data/tileset2.tga");
	bool good = this->gameMap.loadGameMap("data/map1.json");
	assert(good);
	//Vector2 player_initial_position = Vector2{ this->gameMap.size.x * 0.4f, this->gameMap.size.y * 0.95f };
	this->player = Player("data/bicho.tga");
	onEnter();
}

void PlayStage::render(Image* fb)
{
	current_stage->camera_clamp(Vector2(fb->width, fb->height));

	gameMap.render(fb, this->camera_position, BACKGROUND);
	gameMap.render(fb, this->camera_position, WATER);
	gameMap.render(fb, this->camera_position, GRASS);
	gameMap.render(fb, this->camera_position, PLATFORMS);

	/*if (player.position.y < 240.0) {
		player.render(fb, this->camera_position);
		gameMap.render(fb, this->camera_position, WALL_FACE);
	}
	else {
		gameMap.render(fb, this->camera_position, WALL_FACE);
	}*/
	player.render(fb, this->camera_position);

	gameMap.render(fb, this->camera_position, DEBUG);
	//gameMap.render(fb, this->camera_position, COLLISIONS);
}

void PlayStage::update(double dt)
{
	handle_player_movement(dt);

	gameMap.update(dt);

	this->time++;
}

void PlayStage::handle_player_movement(double dt)
{
	Vector2 offset = { 0.0, 2.0 };
	Vector2 velocity = player.velocity;
	player.frame = RESTING_FRAME;

	/*if (Input::isKeyPressed(SDL_SCANCODE_UP) || Input::isKeyPressed(SDL_SCANCODE_W)) //if key up
	{
		offset.y = 2; // just two pixels above // TODO: CHANGE
		velocity.y -= 1.0;
		//player.set_side(, time);
		//player.jump();
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
	}*/

	if (Input::isKeyPressed(SDL_SCANCODE_LEFT) || Input::isKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		offset.x = 0.2 * CH_WIDTH;
		velocity.x -= 0.1;
		player.set_side(FACE_LEFT, time);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT) || Input::isKeyPressed(SDL_SCANCODE_D)) //if key right
	{
		offset.x = -0.2 * CH_WIDTH;
		velocity.x += 0.1;
		player.set_side(FACE_RIGHT, time);
	}

	bool is_grounded = this->is_player_grounded(player.position, offset);

	if (!is_grounded) {
		velocity.y += 0.2;
	}

	if (is_grounded && (Input::isKeyPressed(SDL_SCANCODE_UP) || Input::isKeyPressed(SDL_SCANCODE_W))) //if key up
	{
		velocity.y -= 1.0;
		this->player.jump();
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
}

void PlayStage::onEnter()
{
	player.position = Vector2(20.0, 20.0);
}

void PlayStage::onLeave()
{
}

void PlayStage::onKeyDown(SDL_KeyboardEvent event)
{
}

void PlayStage::onKeyUp(SDL_KeyboardEvent event)
{
}


void PlayStage::camera_clamp(Vector2 fb_size)
{
	this->camera_position = this->player.position - fb_size * 0.5; // also update camera position
	this->camera_position.x = clamp(this->camera_position.x, 0.0f, this->gameMap.size.x - fb_size.x);
	this->camera_position.y = clamp(this->camera_position.y, 0.0f, this->gameMap.size.y - fb_size.y);
}

bool PlayStage::is_valid(Vector2 target, Vector2 offset)
{
	int cx = floor((target.x - offset.x) / this->gameMap.tile_width);
	int cy = floor((target.y - offset.y) / this->gameMap.tile_height);

	//this->gameMap.add_debug_cell(cx, cy);
	
	int cell_type = this->gameMap.getCell(cx, cy, COLLISIONS).type;

	if (cell_type == COLLISION) {
		return false;
	}
	return true;
}

bool PlayStage::is_player_grounded(Vector2 position, Vector2 offset)
{
	int cx = floor((position.x - offset.x) / this->gameMap.tile_width);
	int cy = floor((position.y - offset.y) / this->gameMap.tile_height);

	this->gameMap.add_debug_cell(cx, cy);

	int cell_type = this->gameMap.getCell(cx, cy, WALKABLE).type;

	if (cell_type == FLOOR) {
		return true;
	}
	return false;
}