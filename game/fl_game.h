/*
 * 	fl_game.h
 *
 * 	Class to hold all core (post-initialization) game classes/logic.
 *
 */

#ifndef FL_GAME_H_
#define FL_GAME_H_

class FLEnvironment;

class FLGame {
  public:
	static FLGame &instance() {
		static FLGame instance;
		return instance;
	}

	void init();
	void start();

	FLEnvironment *environment();

  protected:
	FLEnvironment *_environment;

  private:
	FLGame(){};
};

#endif
