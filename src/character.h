/*	by Luca Franceschi
	This class encapsulates a character and all the things related to rendering one (e.g.: main character or NPC)
*/

#pragma once

#include "math.h"
#include "image.h"

constexpr auto CH_WIDTH = 16;
constexpr auto CH_HEIGHT = 16;
constexpr auto FRAMES = 2;
constexpr auto RESTING_FRAME = 1;
constexpr auto ANIMATION_SPEED = 0.2;

enum {
	FACE_RIGHT,
	FACE_LEFT
};

class Player
{
public:
	int side;
	int frame;
	Vector2 position;
	Vector2 velocity;
	int movement_speed;
	const char* spritename;
	Image sprite;

	Player();

	Player(const char* spritename, Vector2 position, int movement_speed);

	void set_velocity(Vector2 velocity);
	void set_side(int side, double time);

	void render(Image* fb, Vector2 camera_position);
	
	void move(double dt, double time);
};
