#include "player.h"
#include "math.h"
#include "image.h"
#include "input.h"
#include "gameMap.h"

//Image debug_font; // TODO: remove

Player::Player()
{
	this->spritename = NULL;
	this->movement_speed = NULL;
	this->frame = RESTING_FRAME;
	this->side = FACE_RIGHT;
	this->position = { 0.0, 0.0 };
	this->velocity = { 0.0, 0.0 };
	this->offset = { 0.0, 0.0 };
	this->grounded = false;
	this->time = 0.0;
	//debug_font.loadTGA("data/mini-font-white-4x6.tga");
}

Player::Player(const char* spritename)
{
	this->spritename = spritename;
	this->movement_speed = 100;
	this->frame = RESTING_FRAME;
	this->side = FACE_RIGHT;
	this->position = { 0.0, 0.0 };
	this->velocity = { 0.0, 0.0 };
	this->offset = { 0.0, 0.0 };
	bool good = this->sprite.loadTGA(this->spritename);
	assert(good);
	this->grounded = false;
	this->time = 0.0;
	//debug_font.loadTGA("data/mini-font-white-4x6.tga");
}

Player::Player(const char* spritename, Vector2 position, int movement_speed)
{
	this->spritename = spritename;
	this->position = position;
	this->movement_speed = movement_speed;
	this->frame = RESTING_FRAME;
	this->side = FACE_RIGHT;
	this->velocity = { 0.0, 0.0 };
	this->offset = { 0.0, 0.0 };
	bool good = this->sprite.loadTGA(this->spritename);
	assert(good);
	this->grounded = false;
	this->time = 0.0;
	//debug_font.loadTGA("data/mini-font-white-4x6.tga");
}

void Player::set_velocity(Vector2 velocity)
{
	this->velocity = velocity;
}

void Player::set_side(int side)
{
	this->side = side;
	this->frame = int(time * ANIMATION_SPEED) % FRAMES;
}

void Player::render(Image* fb, Vector2 camera_position)
{
	fb->drawImage(this->sprite, this->position.x - camera_position.x - CH_WIDTH * 0.5, this->position.y - camera_position.y - CH_HEIGHT + 2, Area(this->frame * CH_WIDTH, this->side * CH_HEIGHT, CH_WIDTH, CH_HEIGHT));
	
#ifdef RENDER_DEBUG
	Color red(255, 0, 0); // TODO: remove debug code
	std::string result = grounded ? "TRUE" : "FALSE";

	fb->drawRectangle(this->position.x - offset.x - camera_position.x, this->position.y - offset.y - camera_position.y, 1, 1, red);
	//fb->drawText(result, this->position.x - camera_position.x, this->position.y - camera_position.y, debug_font, 4, 6);
#endif
}

void Player::update(double dt)
{
	this->position += this->velocity * this->movement_speed * dt;
	
	velocity.x = clamp(velocity.x, -2.0, 2.0);
	velocity.y = clamp(velocity.y, -2.5, 1.5);
	
	this->time += dt;
}

Vector2 Player::handle_horizontal_input()
{
	frame = RESTING_FRAME;
	
	Vector2 velocity = this->velocity;

	if (Input::isKeyPressed(SDL_SCANCODE_LEFT) || Input::isKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		velocity.x = -std::abs(velocity.x) - 0.1;
		set_side(FACE_LEFT);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT) || Input::isKeyPressed(SDL_SCANCODE_D)) //if key right
	{
		velocity.x = std::abs(velocity.x) + 0.1;
		set_side(FACE_RIGHT);
	}

	return velocity;
}

void Player::is_grounded(GameMap* map)
{
	int cx = floor((position.x - offset.x) / map->tile_width);
	int cy = floor((position.y - offset.y) / map->tile_height);

#ifdef RENDER_DEBUG
	map->add_debug_cell(cx, cy);
#endif

	int cell_type = map->getCell(cx, cy, map->ground_layer).type;

	grounded = false;
	if (cell_type == FLOOR) {
		grounded = true;
	}
}

void Player::move(GameMap* map, Vector2 target)
{
	Vector2 velocity2 = target - position;

	if (is_valid_target(map, target)) {
		set_velocity(velocity2);
	}
	else if (is_valid_target(map, Vector2(target.x, position.y))) {
		set_velocity(Vector2(velocity2.x, 0.0));
	}
	else if (is_valid_target(map, Vector2(position.x, target.y))) {
		set_velocity(Vector2(0.0, velocity2.y));
	}
	else {
		set_velocity(Vector2(0.0, 0.0));
	}
}

bool Player::is_valid_target(GameMap* map, Vector2 target)
{
	Vector2 velocity2 = target - position;
	
	if (velocity2.y < 0.0) {
		offset.y = 0.3 * CH_HEIGHT;
	}
	else {
		offset.y = -0.2 * CH_HEIGHT;
	}

	if (velocity2.x < 0.0) {
		offset.x = 0.2 * CH_WIDTH;
	}
	else {
		offset.x = -0.2 * CH_WIDTH;
	}

	int cx = floor((target.x - offset.x) / map->tile_width);
	int cy = floor((target.y - offset.y) / map->tile_height);

#ifdef RENDER_DEBUG
	map->add_debug_cell(cx, cy);
#endif

	int cell_type = map->getCell(cx, cy, map->collision_layer).type;

	if (cell_type == COLLISION) {
		return false;
	}
	return true;
}
