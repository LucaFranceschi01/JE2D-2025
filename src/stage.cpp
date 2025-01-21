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

PlayStage::PlayStage()
{
	this->gameMap = GameMap("data/tileset.tga");
	bool good = this->gameMap.loadGameMap("data/map_forward.json");
	assert(good);
	this->player = Player("data/bicho.tga");
	onEnter();
}

void PlayStage::render(Image* fb)
{
	camera_clamp(Vector2(fb->width, fb->height));

	gameMap.render(fb, this->camera_position, BACKGROUND);
	gameMap.render(fb, this->camera_position, WATER);
	gameMap.render(fb, this->camera_position, GRASS);
	gameMap.render(fb, this->camera_position, PLATFORMS);
	gameMap.render(fb, this->camera_position, DEBUG);

	//gameMap.render(fb, this->camera_position, COLLISIONS);
	//gameMap.render(fb, this->camera_position, WALKABLE);

	player.render(fb, this->camera_position);
}

void PlayStage::update(double dt)
{
	player.is_grounded(&gameMap); // first of all check state, grounded or not

	Vector2 target = player.handle_input(); // handle input, returns which is the desired new position
	
	player.move(&gameMap, target); // gameMap checks which is the closest valid position to the target

	player.update(dt); // player is updated

	gameMap.update(dt);

	time += dt;
}

void PlayStage::onEnter()
{
	player.position = Vector2(20.0, 20.0);
}

void PlayStage::onLeave()
{
	// TODO
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

IntroStage::IntroStage()
{
	this->gameMap = GameMap("data/tileset.tga");
	bool good = this->gameMap.loadGameMap("data/map_forward.json");
	assert(good);

	this->player = Player("data/bicho.tga");
	onEnter();
}

void IntroStage::render(Image* fb)
{
	player.render(fb);
}

void IntroStage::update(double dt)
{
}

void IntroStage::onEnter()
{
	player.position = Vector2(20.0, 20.0);
}

void IntroStage::onLeave()
{
}

void IntroStage::onKeyDown(SDL_KeyboardEvent event)
{
}

void IntroStage::onKeyUp(SDL_KeyboardEvent event)
{
}