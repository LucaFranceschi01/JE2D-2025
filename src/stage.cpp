#include "stage.h"
#include "gameMap.h"
#include "player.h"
#include "math.h"
#include "input.h"

Stage::Stage()
{
	//this->current_stage = this;
	this->time = 0;
}

PlayStage::PlayStage(const char* gameMap_filename, int collision_layer)
{
	this->gameMap = GameMap(gameMap_filename, collision_layer);
	this->player = Player("data/bicho.tga");
}

ForwardStage::ForwardStage() : PlayStage("data/map_forward.json", COLLISIONS)
{
	gameMap.ground_layer = WALKABLE;
	//onEnter();
}

ForwardStage::ForwardStage(const char* gameMap_filename) : PlayStage(gameMap_filename, COLLISIONS)
{
	gameMap.ground_layer = WALKABLE;
}

void ForwardStage::render(Image* fb)
{
	camera_clamp(Vector2(fb->width, fb->height));

	gameMap.render(fb, this->camera_position, BACKGROUND);
	gameMap.render(fb, this->camera_position, WATER);
	gameMap.render(fb, this->camera_position, GRASS);
	gameMap.render(fb, this->camera_position, PLATFORMS);
	gameMap.render(fb, this->camera_position, DEBUG);

	// gameMap.render(fb, this->camera_position, COLLISIONS);
	// gameMap.render(fb, this->camera_position, WALKABLE);
	// gameMap.render(fb, this->camera_position, PROGRESS_LAYER);

	player.render(fb, this->camera_position);
}

void ForwardStage::update(double dt)
{
	player.velocity.x *= 0.9; // friction if not pressing any button
	
	player.is_grounded(&gameMap); // first of all check state, grounded or not

	Vector2 velocity = player.handle_horizontal_input(); // handle input, returns which is the desired new position

	// check if able to jump and add gravity
	if (player.grounded) {
		velocity.y = 0.0;
		if (Input::isKeyPressed(SDL_SCANCODE_UP) || Input::isKeyPressed(SDL_SCANCODE_W)) {
			velocity.y = -2.5;
		}
	}
	else {
		velocity.y += 0.1;
	}

	Vector2 target = player.position + velocity;
	
	player.move(&gameMap, target); // gameMap checks which is the closest valid position to the target

	player.update(dt); // player is updated

	gameMap.update(dt);

	time += dt;
}

void ForwardStage::onEnter(int previous_state)
{
	if (previous_state == STAGE_INTRO) {
		player.position = Vector2(40.0, 20.0);
	}
	else if (previous_state == STAGE_TEMPLE) {
		player.position = Vector2((gameMap.width-1)*gameMap.tile_width-1, 13.0 * gameMap.tile_height);
	}
	else if (previous_state == STAGE_FORWARD) {
		player.position = Vector2(3.0 * gameMap.tile_width, 17.0 * gameMap.tile_height);
	}
}

int ForwardStage::onLeave()
{
	player.velocity = Vector2(0.0, 0.0);
	return STAGE_FORWARD;
}

int ForwardStage::changeStage()
{
	int cx = floor(player.position.x / gameMap.tile_width);
	int cy = floor(player.position.y / gameMap.tile_height);

	int cell_type = gameMap.getCell(cx, cy, PROGRESS_LAYER).type;

	if (cell_type == NEXT_STAGE) {
		return STAGE_TEMPLE;
	}
	else if (cell_type == RELOAD_STAGE) {
		return STAGE_FORWARD;
	}
	return -1;
}

void ForwardStage::onKeyDown(SDL_KeyboardEvent event)
{
}

void ForwardStage::onKeyUp(SDL_KeyboardEvent event)
{
}

void PlayStage::camera_clamp(Vector2 fb_size)
{
	this->camera_position = this->player.position - fb_size * 0.5; // also update camera position
	this->camera_position.x = clamp(this->camera_position.x, 0.0f, this->gameMap.size.x - fb_size.x);
	this->camera_position.y = clamp(this->camera_position.y, 0.0f, this->gameMap.size.y - fb_size.y);
}

IntroStage::IntroStage() : PlayStage("data/map_intro.json", COLLISIONS)
{
	onEnter();
}

void IntroStage::render(Image* fb)
{
	camera_clamp(Vector2(fb->width, fb->height));

	gameMap.render(fb, this->camera_position, FLOOR);
	gameMap.render(fb, this->camera_position, WATER);
	gameMap.render(fb, this->camera_position, TRAP);
	gameMap.render(fb, this->camera_position, GRASS1);
	gameMap.render(fb, this->camera_position, GRASS2);
	gameMap.render(fb, this->camera_position, OBTACLES);
	gameMap.render(fb, this->camera_position, OBJECTS);

	//gameMap.render(fb, this->camera_position, COLLISIONS);

	player.render(fb, this->camera_position);
}

void IntroStage::update(double dt)
{
	player.velocity *= 0.9; // friction if not pressing any button

	Vector2 velocity = player.handle_horizontal_input(); // handle input, returns which is the desired new position

	if (Input::isKeyPressed(SDL_SCANCODE_UP) || Input::isKeyPressed(SDL_SCANCODE_W)) //if key left
	{
		velocity.y = -std::abs(velocity.y) - 0.1;
		//set_side(FACE_LEFT);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN) || Input::isKeyPressed(SDL_SCANCODE_S)) //if key right
	{
		velocity.y = std::abs(velocity.y) + 0.1;
		//set_side(FACE_RIGHT);
	}

	Vector2 target = player.position + velocity;

	player.move(&gameMap, target); // gameMap checks which is the closest valid position to the target

	player.update(dt); // player is updated

	gameMap.update(dt);

	time += dt;
}

void IntroStage::onEnter(int previous_state)
{
	player.position = Vector2(50.0, 100.0);
}

int IntroStage::onLeave()
{
	player.velocity = Vector2(0.0, 0.0);
	return STAGE_INTRO;
}

int IntroStage::changeStage()
{
	int cx = floor(player.position.x / gameMap.tile_width);
	int cy = floor(player.position.y / gameMap.tile_height);

	int cell_type = gameMap.getCell(cx, cy, PROGRESS_LAYER).type;

	if (cell_type == NEXT_STAGE) {
		return STAGE_FORWARD;
	}
	return -1;
}

void IntroStage::onKeyDown(SDL_KeyboardEvent event)
{
}

void IntroStage::onKeyUp(SDL_KeyboardEvent event)
{
}

TempleStage::TempleStage() : ForwardStage("data/map_temple.json")
{
	//onEnter();
}

void TempleStage::onEnter(int previous_state)
{
	
	if (previous_state <= STAGE_FORWARD) {
		player.position = Vector2(gameMap.tile_width + 1, 13.0*gameMap.tile_height);
	}
	else if (previous_state == STAGE_TEMPLE_REVERSE) {
		player.position = Vector2((gameMap.width-1)*gameMap.tile_width, 9.0 * gameMap.tile_height);
	}
	else if (previous_state == STAGE_TEMPLE) {
		player.position = Vector2(gameMap.tile_width*3.0, 13.0 * gameMap.tile_height);
	}
}

int TempleStage::onLeave()
{
	player.velocity = Vector2(0.0, 0.0);
	return STAGE_TEMPLE;
}

int TempleStage::changeStage()
{
	int cx = floor(player.position.x / gameMap.tile_width);
	int cy = floor(player.position.y / gameMap.tile_height);

	int cell_type = gameMap.getCell(cx, cy, PROGRESS_LAYER).type;

	if (cell_type == NEXT_STAGE) {
		return STAGE_REVERSE;
	}
	else if (cell_type == RELOAD_STAGE) {
		return STAGE_TEMPLE;
	}
	else if (cell_type == PREVIOUS_STAGE) {
		return STAGE_FORWARD;
	}
	return -1;
}
