#pragma once

// cast stage dynamic_cast<PlayStage*>(current_stage)
// o
// static_cast<PlayStage*>(current_stage) (mejor si se puede)
// dynamic cast puede devolver nulo si est� mal, entonces puedes hacer if (!play_stage) {return;}

#include "player.h"
#include "gameMap.h"
#include "math.h"
#include "input.h"

//class Image;

enum {
	EMPTY_STAGE=-1,
	STAGE_ENTRY,
	STAGE_INTRO,
	STAGE_FORWARD,
	STAGE_TEMPLE,
	STAGE_TEMPLE_REVERSE,
	STAGE_REVERSE,
	STAGE_ENDING
};

constexpr auto PREVIOUS_STAGE = 515;
constexpr auto NEXT_STAGE = 516;
constexpr auto RELOAD_STAGE = 491;

class Stage
{
public:
	double time;
	
	Stage();
	virtual void render(Image* fb) {};
	virtual void update(double dt) {};

	virtual void onEnter(int previous_state = -1) {};
	virtual int onLeave() { return -1; };
	virtual int changeStage() { return -1; };

	virtual void onKeyDown(SDL_KeyboardEvent event) {};
	virtual void onKeyUp(SDL_KeyboardEvent event) {};

	virtual int get_stage() { return EMPTY_STAGE; }
};

class PlayStage : public Stage 
{
public:
	GameMap gameMap;
	Vector2 camera_position;
	Player player;
	Image minifont;

	PlayStage(const char* gameMap_filename, int collision_layer);

	void camera_clamp(Vector2 fb_size);
};

class IntroStage : public PlayStage
{
public:
	enum e_renderable_layer {
		FLOOR, WATER, TRAP, GRASS1, GRASS2, OBTACLES, OBJECTS, COLLISIONS, PROGRESS_LAYER
	};

	IntroStage();
	void render(Image* fb) override;
	void update(double dt) override;

	void onEnter(int previous_state = -1) override;
	int onLeave() override;
	int changeStage() override;

	void onKeyDown(SDL_KeyboardEvent event) override;
	void onKeyUp(SDL_KeyboardEvent event) override;

	int get_stage() override;
};

class ForwardStage : public PlayStage
{
public:
	enum e_renderable_layer {
		BACKGROUND, WATER, GRASS, PLATFORMS, DEBUG, COLLISIONS, WALKABLE, PROGRESS_LAYER
	};

	ForwardStage();
	ForwardStage(const char* gameMap_filename);
	void render(Image* fb) override;
	void update(double dt) override;

	void onEnter(int previous_state = -1) override;
	int onLeave() override;
	int changeStage() override;

	void onKeyDown(SDL_KeyboardEvent event) override;
	void onKeyUp(SDL_KeyboardEvent event) override;

	int get_stage() override;
};

class TempleStage : public ForwardStage {
public:
	TempleStage();

	void onEnter(int previous_state = -1) override;
	int onLeave() override;
	int changeStage() override;

	int get_stage() override;
};

class ReverseStage : public ForwardStage {
public:
	ReverseStage();
	ReverseStage(const char* gameMap_filename);

	void onEnter(int previous_state = -1) override;
	void update(double dt) override;
	int onLeave() override;
	int changeStage() override;

	int get_stage() override;
};

class TempleReverseStage : public ReverseStage {
public:
	TempleReverseStage();


	void onEnter(int previous_state = -1) override;
	int onLeave() override;
	int changeStage() override;

	int get_stage() override;
};

class EndingStage : public Stage {
public:
	Image font;
	Image minifont;
	Color bg;

	EndingStage();
	
	void render(Image* fb) override;
	void update(double dt) override;

	void onEnter(int previous_state = -1) override;
	int onLeave() override;
	int changeStage() override;

	int get_stage() override;
};

class EntryStage : public Stage {
public:
	Image font;
	Image minifont;
	Color bg;

	EntryStage();

	void render(Image* fb) override;
	void update(double dt) override;

	void onEnter(int previous_state = -1) override;
	int onLeave() override;
	int changeStage() override;

	int get_stage() override;
};