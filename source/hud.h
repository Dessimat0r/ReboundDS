#ifndef __hud_h__
#define __hud_h__

class Game;

class HUD {
	public:
		HUD();
		~HUD();
		void update();

		Game *pGame;
};

#endif
