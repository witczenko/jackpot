
#include "JackPot.h"
#include "MsgDesc.h"
#include "RollerTile.h"
#include "Settings.h"

JackPot::JackPot(int argc, char* argv[]) :
window_height(WINDOW_HEIGHT),
window_width(WINDOW_WIDTH),
is_running(true),
current_state(States::MENU),
next_state(States::MENU),
initialized(false), 
full_screen(false),
coins(COINS_NUMBER),
bet(1),
win(0),
suprise_man(nullptr),
bg(nullptr),
show_a_man(false),
man_x_pos(WINDOW_WIDTH)
{
	app_logs.push_back(sayMessage.at(APP_INIT_START));

	if (!TakeProgramArgs(argc, argv)) {
		app_logs.push_back(sayMessage.at(APP_ARGS_ERROR));
	}

	if (!InitializeSDL()) {
		app_logs.push_back(sayMessage.at(SDL_INIT_ERROR));
		return;
	}

	if (!LoadResources()) {
		app_logs.push_back(sayMessage.at(RESOURCE_LOAD_ERROR));
		return;
	}

	initialized = true;
}

JackPot::~JackPot()
{
	ReleaseResources();
	ReleaseSDL();
}

bool JackPot::IsInitialized()
{
	return initialized;
}

void JackPot::Run()
{
	unsigned int dtime = 0, last_ticks = 0, current_ticks;

	if (initialized) {
		CreateRollers();

		while (is_running) {
			current_ticks = SDL_GetTicks();
			dtime = current_ticks - last_ticks;
			last_ticks = current_ticks;

			PollEvents();
			UpdateLogic(dtime);
			Render(dtime);
			SDL_Delay(16);
		}
	}
}

bool JackPot::TakeProgramArgs(int argc, char * argv[])
{
	//TODO: Implement parameters like screen resolution etc...  
	return true;
}

void JackPot::Render(unsigned int dtime)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, bg, NULL, NULL);
	for (auto roller : rollers) {
		roller->Render();
	}
	RenderHUD(dtime);
	SDL_RenderPresent(renderer);
}

void JackPot::RenderHUD(unsigned int dtime)
{
	static int which_color = 0;
	static unsigned int time_acc = 0;
	const int left_margin = window_width - 160, offset = 60, colors_count=3;
	const SDL_Color color_set[colors_count] = {
		{255, 255, 55, 255},
		{255, 55, 255, 255},
		{55, 255, 255, 255}
	};

	SDL_Rect dest = { left_margin, 20, 120, 30};
	SDL_Color color = { 255,155,255,255 };

	time_acc += dtime;

	std::string hud_str = std::string("COINS: ") + std::to_string(coins);
	font->Say(hud_str.c_str() , dest, color_set[which_color]);
	dest.y += offset;

	hud_str = std::string("BET: ") + std::to_string(bet);
	font->Say(hud_str.c_str(), dest, color_set[which_color]);
	dest.y += offset;

	hud_str = std::string("WIN: ") + std::to_string(win);
	font->Say(hud_str.c_str(), dest, color_set[which_color]);
	
	if (!show_a_man) {
		dest = { left_margin, window_height - 80, 110, 25 };
		hud_str = std::string("PUSH");
		font->Say(hud_str.c_str(), dest, color_set[which_color]);

		dest = { left_margin, window_height - 40, 110, 25 };
		hud_str = std::string("SPACE");
		font->Say(hud_str.c_str(), dest, color_set[which_color]);
	}
	else {
		dest = { man_x_pos, 180, 250, 300 };
		SDL_RenderCopy(renderer, suprise_man, NULL, &dest);
	}

	if (time_acc > 500) {
		which_color = which_color < colors_count-1 ? which_color + 1 : 0;
		time_acc = 0;
	}
}

void JackPot::UpdateLogic(unsigned int dtime)
{
	const float wait_time = 1;
	const std::map<int, int> win_table = {
		{0, 100},
		{1, 75},
		{2, 50},
		{3, 25},
		{4, 5}
	};
	static int which_roller = 0;
	static float wait = 0.0;
	float elapsed_time_sec = dtime / 1000.0f;

	current_state = next_state;

	switch (current_state) {
		case MENU: 
			break;
		
		case START:
			show_a_man = true;
			for (auto roller : rollers) {
				roller->StartSpin();
			}

			next_state = WAIT;

			if (coins >= bet) {
				coins -= bet;
			}
			break;
		case STOP_NEXT_ROLLER: 
		    rollers[which_roller]->StopSpin();
			next_state = WAIT;
			which_roller++;
			break;
		case WAIT:
			if (wait >= wait_time) {
				if (which_roller < rollers.size()) {
					next_state = STOP_NEXT_ROLLER;
				}
				else {
					next_state = REWARD;
					which_roller = 0;
				}
				wait = 0.0;
			}
			else {
				wait += elapsed_time_sec;
			}

			if (man_x_pos > 425) {
				man_x_pos -= 5;
			}

			break;

		case REWARD: 
			int win_id;
			bool line_all_match = true;
			show_a_man = false;
			man_x_pos = window_width;
			next_state = MENU;
		
			win_id = rollers[0]->GetMiddleLineID();
			for (int i = 1; i < rollers.size(); i++) {
				if (win_id != rollers[i]->GetMiddleLineID()) {
					line_all_match = false;
				}
			}

			if (line_all_match) {
				win += win_table.at(win_id) * bet;
			}
			break;
	}

	for (auto roller : rollers) {
		roller->Update(elapsed_time_sec);
	}
}

bool JackPot::InitializeSDL()
{
	SDL_Window *window;
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
		sayMessage.at(APP_TITLE).c_str(),                
		SDL_WINDOWPOS_UNDEFINED,           
		SDL_WINDOWPOS_UNDEFINED,          
		window_width,                    
		window_height,                  
		SDL_WINDOW_OPENGL
	);

	if (window == nullptr) {
		app_logs.push_back(std::string(SDL_GetError())+std::string("\n"));
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == nullptr) {
		app_logs.push_back(std::string(SDL_GetError())+std::string("\n"));
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	TTF_Init();

	return true;
}

void JackPot::ReleaseSDL()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
	TTF_Quit();
}

bool JackPot::LoadResources()
{
	std::vector<std::string> img_names = {
		IMG_PATH"pl.bmp",
		IMG_PATH"de.bmp",
		IMG_PATH"it.bmp",
		IMG_PATH"fr.bmp",
		IMG_PATH"es.bmp",
		IMG_PATH"man.bmp",
		IMG_PATH"bg.bmp"
	};

	for (auto name : img_names) {
		SDL_Surface *image = SDL_LoadBMP(name.c_str());
		if (nullptr != image) {
			SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 255, 0));

			if (name == std::string(IMG_PATH"bg.bmp")) {
				bg = SDL_CreateTextureFromSurface(renderer, image);
			}else if (name == std::string(IMG_PATH"man.bmp")) {
				suprise_man = SDL_CreateTextureFromSurface(renderer, image);
			}
			else {
				sdl_tile_textures.push_back(SDL_CreateTextureFromSurface(renderer, image));
			}

			SDL_FreeSurface(image);
		}
		else {
			app_logs.push_back(std::string(SDL_GetError())+std::string("\n"));
			return false;
		}
	}

	if (font = std::make_unique<Font>(renderer, FONTS_PATH"SUBWT___.ttf", 120); !font->IsInitilized()) {
			app_logs.push_back(std::string(TTF_GetError())+std::string("\n"));
			return false;
	};

	return true;
}

void JackPot::ReleaseResources()
{
	for (auto tile_img : sdl_tile_textures) {
		SDL_DestroyTexture(tile_img);
	}
}

void JackPot::CreateRollers()
{
	const int tileWidth = 150, tileHeight = 150, rollerCount = 3;
	int id = 0, x = 0, y = 0;

	for (int r = 0; r < rollerCount; r++) {
		std::shared_ptr<Roller> roller = std::make_shared<Roller>();

		for (auto tile_tex : sdl_tile_textures) {
			SDL_Rect dest = { r*tileWidth, y, tileWidth, tileHeight };
			roller->AddTile(std::make_shared<RollerTile>(id, dest, renderer, tile_tex));
			y += tileHeight;
			id++;
		}
		id = 0; y = 0;
		rollers.push_back(roller);
	}
}

void JackPot::PollEvents()
{
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT: is_running = false;
			break;
		case SDL_KEYUP:
			if (e.key.keysym.sym == SDLK_ESCAPE) is_running = false;
			break;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_SPACE) {
				if (current_state == MENU) next_state = START;
			}

			if (e.key.keysym.sym == SDLK_f) {
				if (full_screen) {
					full_screen = false;
					SDL_SetWindowFullscreen(window, 0);
				}
				else {
					full_screen = true;
					SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
				}
			}

			if (e.key.keysym.sym == SDLK_UP) {
				if (current_state == MENU) {
					bet = bet < MAX_BET ? bet + 1 : 1;
				}
			}
			if (e.key.keysym.sym == SDLK_DOWN) {
				if (current_state == MENU) {
					bet = bet > 1 ? bet - 1 : MAX_BET;
				}
			}
			break;

		}
	}
}

void JackPot::DumpLogsFile()
{
	//TODO:
}

void JackPot::DumpLogsToStream()
{
	for (auto msg : app_logs) {
		std::cout << msg;
	}
}

