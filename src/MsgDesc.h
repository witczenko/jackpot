#ifndef MSGDESC_H 
#define MSGDESC_H 

#include <map>
#include <string>

enum MsgCode {
	APP_TITLE,
    APP_INIT_START, 
	APP_ARGS_ERROR,
	SDL_INIT_ERROR,
	RESOURCE_LOAD_ERROR,
	EXIT,
	GENERAL_ERROR
};

const std::map<MsgCode, std::string> sayMessage = 
{
	{MsgCode::APP_TITLE, "JACKPOT - lets Europe make great again!"},
	{MsgCode::APP_INIT_START, "Start initializig Jackpot\n"},
	{MsgCode::APP_ARGS_ERROR, "Cannot read program arguments: WARNING\n"},
	{MsgCode::SDL_INIT_ERROR, "Cannot initialize SDL library: FATAL ERROR\n"},
	{MsgCode::RESOURCE_LOAD_ERROR, "Cannot load resources: FATAL ERROR\n"},
	{MsgCode::EXIT, "JACKPOT says goodbye ;*\n"},
	{MsgCode::GENERAL_ERROR, "GENERAL ERROR OCCURED\n"},
};

#endif //MSGDESC_H 
