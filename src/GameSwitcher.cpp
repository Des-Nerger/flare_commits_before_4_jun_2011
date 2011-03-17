/*
 * class GameSwitcher
 *
 * State machine handler between main game modes that take up the entire view/control
 *
 * Examples:
 * - the main gameplay (GameEngine class)
 * - title screen
 * - new game screen (character create)
 * - load game screen
 * - maybe full-video cutscenes
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */
 
#include "GameSwitcher.h"

GameSwitcher::GameSwitcher(SDL_Surface *_screen, InputState *_inp) {
	inp = _inp;
	screen = _screen;
	
	game_state = GAME_STATE_PLAY;
	eng = new GameEngine(screen, _inp);
}

void GameSwitcher::logic() {
	switch (game_state) {
		
		// title screen
		case GAME_STATE_TITLE:
		
		
			break;
			
		// main gameplay
		case GAME_STATE_PLAY:
		
			eng->logic();
			done = eng->done;
			break;
	
		// load game
		case GAME_STATE_LOAD:
		
			break;
			
		// new game
		case GAME_STATE_NEW:
		
			break;
	
		default:
			break;
	}
	
}

void GameSwitcher::render() {
	switch (game_state) {
		
		// title screen
		case GAME_STATE_TITLE:
		
		
			break;
			
		// main gameplay
		case GAME_STATE_PLAY:
		
			eng->render();
			break;
	
		// load game
		case GAME_STATE_LOAD:
		
			break;
			
		// new game
		case GAME_STATE_NEW:
		
			break;
	
		default:
			break;
	}

}

GameSwitcher::~GameSwitcher() {
	delete(eng);
}
