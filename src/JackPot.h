#ifndef JACKPOT_H
#define JACKPOT_H

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Roller.h"
#include "Font.h"


class JackPot
{
public:
	JackPot(int argc, char * argv[]);
	~JackPot();

	bool IsInitialized();
	void Run();
	void DumpLogsFile();
	void DumpLogsToStream();
private:
	enum States {
		MENU = 0,
		START, 
		STOP_NEXT_ROLLER,
		WAIT, 
		REWARD	
	};

	States current_state, next_state;

	int window_width;
	int window_height;
	bool is_running;
	bool start_spinning;
	bool initialized;
	bool full_screen;
	int coins, bet, win;
	int show_a_man;
	int man_x_pos;

	SDL_Window *window;
	SDL_Renderer *renderer;

	std::vector<std::string> app_logs;
	std::vector<SDL_Texture *> sdl_tile_textures;
	std::vector<std::shared_ptr<Roller>> rollers;
	SDL_Texture *suprise_man, *bg;

	std::unique_ptr<Font> font;

	bool TakeProgramArgs(int argc, char* argv[]);
	void Render(unsigned int dtime);
	void RenderHUD(unsigned int dtime);
	void UpdateLogic(unsigned int dtime);

	bool InitializeSDL();
	void ReleaseSDL();
	bool LoadResources();
	void ReleaseResources();
	void CreateRollers();
	void PollEvents();
};

#endif //JACKPOT_H
