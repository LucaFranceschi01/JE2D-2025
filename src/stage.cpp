#include "stage.h"
#include "gameMap.h"
#include "player.h"
#include "math.h"
#include "input.h"

Stage::Stage()
{
	this->time = 0;
}

PlayStage::PlayStage(const char* gameMap_filename, int collision_layer) : Stage()
{
	this->gameMap = GameMap(gameMap_filename, collision_layer);
	this->player = Player("data/bicho.tga");
	this->player.offset_calculation = &Player::offset_calculation_forward;
	//this->player.is_grounded = &Player::is_grounded_forward;
	this->player.is_valid_target = &Player::is_valid_target_forward;
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
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

	if (get_stage() == STAGE_FORWARD) {
		if (time < 1.5) {
			fb->drawText("AAAHH", player.position.x - camera_position.x - CH_WIDTH*2, player.position.y - camera_position.y - CH_HEIGHT*2, minifont, 4, 6);
		}
		else if (time < 5.0) {
			fb->drawText("I have to exit this creepy place...", player.position.x - camera_position.x - CH_WIDTH * 2, player.position.y - camera_position.y - CH_HEIGHT * 2, minifont, 4, 6);
		}
		else if (time < 10.0) {
			fb->drawText("But I cannot climb up to the exit...", player.position.x - camera_position.x - CH_WIDTH * 2, player.position.y - camera_position.y - CH_HEIGHT * 2, minifont, 4, 6);
		}
	}

	player.render(fb, this->camera_position);
}

void ForwardStage::update(double dt)
{
	player.velocity.x *= 0.9; // friction if not pressing any button
	
	player.is_grounded_forward(&gameMap); // first of all check state, grounded or not

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
	if (previous_state <= STAGE_INTRO) {
		player.position = Vector2(40.0, 20.0);
	}
	else if (previous_state >= STAGE_TEMPLE) {
		player.position = Vector2((gameMap.width-1)*gameMap.tile_width-1, 13.0 * gameMap.tile_height);
	}
	else if (previous_state == STAGE_FORWARD) {
		player.position = Vector2(5.0 * gameMap.tile_width, 17.0 * gameMap.tile_height);
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
	return EMPTY_STAGE;
}

void ForwardStage::onKeyDown(SDL_KeyboardEvent event)
{
}

void ForwardStage::onKeyUp(SDL_KeyboardEvent event)
{
}

int ForwardStage::get_stage()
{
	return STAGE_FORWARD;
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

	fb->drawText("Ughh, water seems to be toxic", 2 * gameMap.tile_width - camera_position.x, 4 * gameMap.tile_height - camera_position.y, minifont, 4, 6);
	fb->drawText("I'm soooo hungry...", 3 * gameMap.tile_width - camera_position.x, 5 * gameMap.tile_height - camera_position.y, minifont, 4, 6);
	fb->drawText("I hope I could find some food...", 4 * gameMap.tile_width - camera_position.x, 6 * gameMap.tile_height - camera_position.y, minifont, 4, 6);

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
	return EMPTY_STAGE;
}

void IntroStage::onKeyDown(SDL_KeyboardEvent event)
{
}

void IntroStage::onKeyUp(SDL_KeyboardEvent event)
{
}

int IntroStage::get_stage()
{
	return STAGE_INTRO;
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
	else if (previous_state >= STAGE_TEMPLE_REVERSE) {
		player.position = Vector2((gameMap.width-2)*gameMap.tile_width, 9.0 * gameMap.tile_height);
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
		return STAGE_TEMPLE_REVERSE;
	}
	else if (cell_type == RELOAD_STAGE) {
		return STAGE_TEMPLE;
	}
	else if (cell_type == PREVIOUS_STAGE) {
		return STAGE_FORWARD;
	}
	return EMPTY_STAGE;
}

int TempleStage::get_stage()
{
	return STAGE_TEMPLE;
}

ReverseStage::ReverseStage() : ForwardStage("data/map_reverse.json")
{
	this->player = Player("data/bicho_reverse.tga"); // overwrite, not optimal but it is what it is
	this->player.offset_calculation = &Player::offset_calculation_reverse;
	//this->player.is_grounded = &Player::is_grounded_reverse;
	this->player.is_valid_target = &Player::is_valid_target_reverse;
}

ReverseStage::ReverseStage(const char* gameMap_filename) : ForwardStage(gameMap_filename)
{
	this->player = Player("data/bicho_reverse.tga"); // overwrite, not optimal but it is what it is
	this->player.offset_calculation = &Player::offset_calculation_reverse;
	//this->player.is_grounded = &Player::is_grounded_reverse;
	this->player.is_valid_target = &Player::is_valid_target_reverse;
}

void ReverseStage::update(double dt)
{
	player.velocity.x *= 0.9; // friction if not pressing any button

	player.is_grounded_reverse(&gameMap); // first of all check state, grounded or not

	Vector2 velocity = player.handle_horizontal_input(); // handle input, returns which is the desired new position

	// check if able to jump and add gravity
	if (player.grounded) {
		velocity.y = 0.0;
		if (Input::isKeyPressed(SDL_SCANCODE_DOWN) || Input::isKeyPressed(SDL_SCANCODE_S)) {
			velocity.y = +2.5;
		}
	}
	else {
		velocity.y -= 0.1;
	}

	Vector2 target = player.position + velocity;

	player.move(&gameMap, target); // gameMap checks which is the closest valid position to the target

	player.update(dt); // player is updated

	gameMap.update(dt);

	time += dt;
}

void ReverseStage::onEnter(int previous_state)
{
	if (previous_state <= STAGE_TEMPLE_REVERSE) {
		player.position = Vector2(38.0*gameMap.tile_width, 12.0*gameMap.tile_height);
	}
	else if (previous_state >= STAGE_REVERSE) {
		player.position = Vector2(37.0 * gameMap.tile_width, 12.0 * gameMap.tile_height);
	}
}

int ReverseStage::onLeave()
{
	player.velocity = Vector2(0.0, 0.0);
	return STAGE_REVERSE;
}

int ReverseStage::changeStage()
{
	int cx = floor(player.position.x / gameMap.tile_width);
	int cy = floor(player.position.y / gameMap.tile_height);

	int cell_type = gameMap.getCell(cx, cy, PROGRESS_LAYER).type;

	if (cell_type == NEXT_STAGE) {
		return STAGE_ENDING;
	}
	else if (cell_type == RELOAD_STAGE) {
		return STAGE_REVERSE;
	}
	else if (cell_type == PREVIOUS_STAGE) {
		return STAGE_TEMPLE_REVERSE;
	}
	return EMPTY_STAGE;
}

int ReverseStage::get_stage()
{
	return STAGE_REVERSE;
}

TempleReverseStage::TempleReverseStage() : ReverseStage("data/map_temple_reverse.json")
{
	//onEnter();
}

void TempleReverseStage::onEnter(int previous_state)
{
	if (previous_state <= STAGE_TEMPLE) {
		player.position = Vector2(29.0*gameMap.tile_width, 8.0 * gameMap.tile_height);
	}
	else if (previous_state >= STAGE_REVERSE) {
		player.position = Vector2(gameMap.tile_width + 1, 12.0 * gameMap.tile_height);
	}
	else if (previous_state == STAGE_TEMPLE_REVERSE) {
		player.position = Vector2(28.0 * gameMap.tile_width, 8.0 * gameMap.tile_height);
	}
}

int TempleReverseStage::onLeave()
{
	player.velocity = Vector2(0.0, 0.0);
	return STAGE_TEMPLE_REVERSE;
}

int TempleReverseStage::changeStage()
{
	int cx = floor(player.position.x / gameMap.tile_width);
	int cy = floor(player.position.y / gameMap.tile_height);

	int cell_type = gameMap.getCell(cx, cy, PROGRESS_LAYER).type;

	if (cell_type == NEXT_STAGE) {
		return STAGE_REVERSE;
	}
	else if (cell_type == RELOAD_STAGE) {
		return STAGE_TEMPLE_REVERSE;
	}
	else if (cell_type == PREVIOUS_STAGE) {
		return STAGE_TEMPLE;
	}
	return EMPTY_STAGE;
}

int TempleReverseStage::get_stage()
{
	return STAGE_TEMPLE_REVERSE;
}

EndingStage::EndingStage() : Stage()
{
	font.loadTGA("data/bitmap-font-white.tga"); //load bitmap-font image
	minifont.loadTGA("data/mini-font-white-4x6.tga"); //load bitmap-font image
	bg = Color(128, 73, 104);
}

void EndingStage::render(Image* fb)
{
	fb->fillBlend(bg);
	fb->drawText("THE END", (fb->width*time-fb->width/2)*0.05, fb->height / 2, font);
	fb->drawText("You have successfully exited the cave! :)", (fb->width/2 - fb->width * time * 0.1), 3 * fb->height / 4, minifont, 4, 6);
}

void EndingStage::update(double dt)
{
	time += dt;
}

void EndingStage::onEnter(int previous_state)
{
	time = 0.0;
}

int EndingStage::onLeave()
{
	return STAGE_ENDING;
}

int EndingStage::changeStage()
{
	return EMPTY_STAGE;
}

int EndingStage::get_stage()
{
	return STAGE_ENDING;
}
