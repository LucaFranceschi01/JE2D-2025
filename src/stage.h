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

class Image;

class Stage
{
public:
	virtual void init() {}; // así lo haces cuando te de la gana
	virtual void render(Image* fb) {};

	// ...
};

class PlayStage : public Stage 
{
public:
	void render(Image* fb) override;
};