#include "character.h"
#include "math.h"
#include "image.h"
#include "input.h"

Character::Character(const char* spritename, Vector2 position, int movement_speed, int resting_side)
{
	this->spritename = spritename;
	this->position = position;
	this->movement_speed = movement_speed;
	this->resting_side = resting_side;
	this->frame = RESTING_FRAME;
	this->side = FACE_DOWN;
	this->velocity = { 0.0, 0.0 };
	this->sprite.loadTGA(this->spritename);
}

void Character::render(Image* fb)
{
	fb->drawImage(this->sprite, this->position.x - CH_WIDTH * 0.5, this->position.y - CH_HEIGHT + 2, Area(this->frame * CH_WIDTH, this->side * CH_HEIGHT, CH_WIDTH, CH_HEIGHT));
}

void Character::move_up()
{
	this->velocity.y -= 1.0;
	this->side = FACE_UP;
}

void Character::move_down()
{
	this->velocity.y += 1.0;
	this->side = FACE_DOWN;
}

void Character::move_left()
{
	this->velocity.x -= 1.0;
	this->side = FACE_LEFT;
}

void Character::move_right()
{
	this->velocity.x += 1.0;
	this->side = FACE_RIGHT;
}

void Character::update_position(double dt)
{
	this->velocity.normalize();
	this->position += this->velocity * this->movement_speed * dt;
}

Player::Player(const char* spritename, Vector2 position, int movement_speed, int resting_side) :
	Character(spritename, position, movement_speed, resting_side) {}

void Player::render(Image* fb)
{
	fb->drawImage(this->sprite, this->position.x - this->camera_position.x - CH_WIDTH * 0.5, this->position.y - this->camera_position.y - CH_HEIGHT + 2, Area(this->frame * CH_WIDTH, this->side * CH_HEIGHT, CH_WIDTH, CH_HEIGHT));
}

void Player::move(double dt, double time, Vector2 fb_size, Vector2 map_size)
{
	this->velocity = { 0.0, 0.0 };
	this->side = this->resting_side;
	this->frame = RESTING_FRAME;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP) || Input::isKeyPressed(SDL_SCANCODE_W)) //if key up
	{
		move_up();
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN) || Input::isKeyPressed(SDL_SCANCODE_S)) //if key down
	{
		move_down();
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT) || Input::isKeyPressed(SDL_SCANCODE_A)) //if key left
	{
		move_left();
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT) || Input::isKeyPressed(SDL_SCANCODE_D)) //if key right
	{
		move_right();
	}
	// TODO: ASK hay direcciones que tienen prioridad (no es la ultima direccion cuando hay 2 pulsadas a la vez)

	if (this->velocity.length() > 0.0) {
		update_position(dt, fb_size, map_size);
		this->frame = int(time * ANIMATION_SPEED) % FRAMES;
	}
}

void Player::update_position(double dt, Vector2 fb_size, Vector2 map_size)
{
	Character::update_position(dt);
	positionClamp(fb_size, map_size);
	cameraClamp(fb_size, map_size);
}

void Player::cameraClamp(Vector2 fb_size, Vector2 map_size)
{
	this->camera_position = this->position - fb_size * 0.5; // also update camera position
	this->camera_position.x = clamp(this->camera_position.x, 0.0f, map_size.x - fb_size.x);
	this->camera_position.y = clamp(this->camera_position.y, 0.0f, map_size.y - fb_size.y);
}

void Player::positionClamp(Vector2 fb_size, Vector2 map_size)
{
	this->position.x = clamp(this->position.x, CH_WIDTH*0.5-8.0, map_size.x - CH_WIDTH*0.5+8.0);
	this->position.y = clamp(this->position.y, CH_HEIGHT-6.0, map_size.y);
}

NPC::NPC(const char* spritename, Vector2 position, int movement_speed, int resting_side) :
	Character(spritename, position, movement_speed, resting_side)
{
	// TODO: CTOR
}

void NPC::move(double dt, double time, Vector2 fb_size, Vector2 map_size)

{
	this->velocity = { 0.0, 0.0 };
	this->side = this->resting_side;
	this->frame = RESTING_FRAME;

	// TODO: NPC MOVEMENTS

	if (this->velocity.length() > 0.0) {
		update_position(dt);
		this->frame = int(time * ANIMATION_SPEED) % FRAMES;
	}
}

