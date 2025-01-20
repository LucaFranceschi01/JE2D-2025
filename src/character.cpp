#include "character.h"
#include "math.h"
#include "image.h"
#include "input.h"

Player::Player()
{
	this->spritename = NULL;
	this->movement_speed = NULL;
	this->frame = RESTING_FRAME;
	this->side = FACE_RIGHT;
	this->position = { 0.0, 0.0 };
	this->velocity = { 0.0, 0.0 };
}

Player::Player(const char* spritename, Vector2 position, int movement_speed)
{
	this->spritename = spritename;
	this->position = position;
	this->movement_speed = movement_speed;
	this->frame = RESTING_FRAME;
	this->side = FACE_RIGHT;
	this->velocity = { 0.0, 0.0 };
	this->sprite.loadTGA(this->spritename);
}

void Player::set_velocity(Vector2 velocity)
{
	this->velocity = velocity;
}

void Player::set_side(int side, double time)
{
	this->side = side;
	this->frame = int(time * ANIMATION_SPEED) % FRAMES;
}

void Player::render(Image* fb, Vector2 camera_position)
{
	//fb->drawImage(this->sprite, this->position.x - CH_WIDTH * 0.5, this->position.y - CH_HEIGHT + 2, Area(this->frame * CH_WIDTH, this->side * CH_HEIGHT, CH_WIDTH, CH_HEIGHT));
	//fb->drawImage(this->sprite, this->position.x - camera_position.x - CH_WIDTH * 0.5, this->position.y - camera_position.y - CH_HEIGHT + 2, Area(this->frame * CH_WIDTH, this->side * CH_HEIGHT, CH_WIDTH, CH_HEIGHT));
	fb->drawImage(this->sprite, this->position.x - camera_position.x - CH_WIDTH * 0.5, this->position.y - camera_position.y - CH_HEIGHT + 2, Area(this->frame * CH_WIDTH, this->side * CH_HEIGHT, CH_WIDTH, CH_HEIGHT));
	Color red(255, 0, 0); // TODO: remove debug code
	fb->drawRectangle(this->position.x - camera_position.x, this->position.y - camera_position.y, 1, 1, red);
}

void Player::move(double dt, double time)
{
	if (this->velocity.length() > 0.0) {
		this->velocity.normalize();
		this->position += this->velocity * this->movement_speed * dt;
	}
}
