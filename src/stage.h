#pragma once

// cast stage dynamic_cast<PlayStage*>(current_stage)
// o
// static_cast<PlayStage*>(current_stage) (mejor si se puede)
// dynamic cast puede devolver nulo si está mal, entonces puedes hacer if (!play_stage) {return;}

#include "player.h"
#include "gameMap.h"
#include "math.h"
#include "input.h"

class Image;

class Stage
{
public:
	double time;
	//Stage* current_stage;
	
	Stage();
	//virtual void init() {}; // así lo haces cuando te de la gana
	virtual void render(Image* fb) {};
	virtual void update(double dt) {};

	virtual void onEnter() {};
	virtual void onLeave() {};

	virtual void onKeyDown(SDL_KeyboardEvent event) {};
	virtual void onKeyUp(SDL_KeyboardEvent event) {};
};

class PlayStage : public Stage 
{
public:
	GameMap gameMap;
	Vector2 camera_position;
	Player player;

	PlayStage();
	void render(Image* fb) override;
	void update(double dt) override;

	void camera_clamp(Vector2 fb_size);

	void onEnter() override;
	void onLeave() override;

	void onKeyDown(SDL_KeyboardEvent event) override;
	void onKeyUp(SDL_KeyboardEvent event) override;
};

class IntroStage : public Stage
{
public:
	GameMap gameMap;
	Player player;

	IntroStage();
	void render(Image* fb) override;
	void update(double dt) override;

	//void camera_clamp(Vector2 fb_size) override;

	void onEnter() override;
	void onLeave() override;

	void onKeyDown(SDL_KeyboardEvent event) override;
	void onKeyUp(SDL_KeyboardEvent event) override;
};