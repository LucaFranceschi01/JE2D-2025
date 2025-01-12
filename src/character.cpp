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
	fb->drawImage(this->sprite, this->position.x, this->position.y, Area(this->frame * CH_WIDTH, this->side * CH_HEIGHT, CH_WIDTH, CH_HEIGHT));
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
	Character(spritename, position, movement_speed, resting_side)
{
}

void Player::move(double dt, double time)
{
	this->velocity = { 0.0, 0.0 };
	this->side = this->resting_side;
	this->frame = RESTING_FRAME;

	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) //if key up
	{
		move_up();
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) //if key down
	{
		move_down();
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) //if key left
	{
		move_left();
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) //if key right
	{
		move_right();
	}

	if (this->velocity.length() > 0.0) {
		update_position(dt);
		this->frame = int(time * ANIMATION_SPEED) % FRAMES;
	}
}

NPC::NPC(const char* spritename, Vector2 position, int movement_speed, int resting_side) :
	Character(spritename, position, movement_speed, resting_side)
{
}

void NPC::move(double dt, double time)
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

