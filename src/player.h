/*	by Luca Franceschi
	This class encapsulates a player
*/

#pragma once

#include "math.h"
#include "image.h"

class GameMap;

constexpr auto CH_WIDTH = 16;
constexpr auto CH_HEIGHT = 16;
constexpr auto FRAMES = 5;
constexpr auto RESTING_FRAME = 1;
constexpr auto ANIMATION_SPEED = 10.0;

enum {
	FACE_RIGHT,
	FACE_LEFT,
	JUMP_RIGHT,
	JUMP_LEFT
};

class Player
{
public:
	int side;
	int frame;
	Vector2 position;
	Vector2 velocity;
	Vector2 offset;
	int movement_speed;
	const char* spritename;
	bool grounded;
	Image sprite;
	double time;

	Player();

	Player(const char* spritename);

	Player(const char* spritename, Vector2 position, int movement_speed);

	void set_velocity(Vector2 velocity);
	void set_side(int side);

	void render(Image* fb, Vector2 camera_position = { 0.0, 0.0 });
	void update(double dt);
	Vector2 handle_input();

	void is_grounded(GameMap* map);
	void move(GameMap* map, Vector2 target);
	bool is_valid_target(GameMap* map, Vector2 target);
};