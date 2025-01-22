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

	void (Player::*offset_calculation)(Vector2);
	//void (Player::*is_grounded)(GameMap*);
	bool (Player::* is_valid_target)(GameMap*, Vector2);

	Player();

	Player(const char* spritename);

	Player(const char* spritename, Vector2 position, int movement_speed);

	void set_velocity(Vector2 velocity);
	void set_side(int side);

	void render(Image* fb, Vector2 camera_position = { 0.0, 0.0 });
	void update(double dt);
	Vector2 handle_horizontal_input();
	void move(GameMap* map, Vector2 target);

	void is_grounded_forward(GameMap* map);
	void is_grounded_reverse(GameMap* map);
	bool is_valid_target_forward(GameMap* map, Vector2 target);
	bool is_valid_target_reverse(GameMap* map, Vector2 target);

	void offset_calculation_forward(Vector2 target);
	void offset_calculation_reverse(Vector2 target);
};