/*	by Luca Franceschi
	This class encapsulates a character and all the things related to rendering one (e.g.: main character or NPC)
*/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "math.h"
#include "image.h"

constexpr auto CH_WIDTH = 32;
constexpr auto CH_HEIGHT = 32;
constexpr auto FRAMES = 3;
constexpr auto RESTING_FRAME = 1;
constexpr auto ANIMATION_SPEED = 10;

enum {
	FACE_DOWN,
	FACE_LEFT,
	FACE_RIGHT,
	FACE_UP
};

class Character
{
public:
	int side;
	int frame;
	Vector2 position;
	Vector2 velocity;
	int movement_speed;
	const char* spritename;
	Image sprite;
	int resting_side;

	Character(const char* spritename, Vector2 position, int movement_speed, int resting_side);

	void render(Image* fb);

	void move_up();
	void move_down();
	void move_left();
	void move_right();

	void update_position(double dt);
	
	// TODO: ASK, ya que ambos Player y NPC van a tener move tengo que poner la signature de move aquí?
	// virtual void move(double dt, double time);
};

class Player : public Character
{
public:
	Vector2 camera_position;

	Player(const char* spritename, Vector2 position, int movement_speed, int resting_side);

	void render(Image* fb);
	
	void move(double dt, double time, Vector2 fb_size, Vector2 map_size);
	
	void update_position(double dt, Vector2 fb_size, Vector2 map_size);

	void cameraClamp(Vector2 fb_size, Vector2 map_size);
};

class NPC : public Character 
{
public:
	NPC(const char* spritename, Vector2 position, int movement_speed, int resting_side);

	void move(double dt, double time, Vector2 fb_size, Vector2 map_size);
};

#endif
