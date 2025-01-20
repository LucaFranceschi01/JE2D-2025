#pragma once

// TODO: EN LOS HEADERS INCLUSO LO INDISPENSBLE

// FORWARD DECLARATION

// la camara está en el stage y la pasa a los otros

// gamemap y player dentro de stage también

// el current es un stage abstracto

// hacer stages como punteros usando new, luego liberar usando delete

// inicializar map y player dentro de constructor de playstage

// cast stage dynamic_cast<PlayStage*>(current_stage)
// o
// static_cast<PlayStage*>(current_stage) (mejor si se puede)
// dynamic cast puede devolver nulo si está mal, entonces puedes hacer if (!play_stage) {return;}

// cuidado con los assert, nunca hacer assert(load(...)) porque en release se quitan los assert

// hasta collisions

#include "player.h"
#include "gameMap.h"
#include "math.h"
#include "input.h"

class Image;

class Stage
{
public:
	double time;
	Stage* current_stage;
	
	Stage();
	virtual void init() {}; // así lo haces cuando te de la gana
	virtual void render(Image* fb) {};
	virtual void update(double dt) {};

	virtual void camera_clamp(Vector2 fb_size) {};
	virtual bool is_valid(Vector2 target) { return true; };
	//virtual void handle_player_movement(double dt) {};

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

	void camera_clamp(Vector2 fb_size) override;
	/*bool is_valid(Vector2 target, Vector2 offset);
	bool is_player_grounded(Vector2 target, Vector2 offset);*/
	//void handle_player_movement(double dt) override;

	void onEnter() override;
	void onLeave() override;

	void onKeyDown(SDL_KeyboardEvent event) override;
	void onKeyUp(SDL_KeyboardEvent event) override;
};