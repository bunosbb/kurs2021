#ifndef _GAME_H_
#define _GAME_H_

enum GameStates
{
    MainMenu,
    GameMenu,
    GameLvl,
    ExitState
};

#include "Project.h"
#include "Graphics.h"
class Graphics;
class Unit;

#include "Input.h"
class Input;

#include "Parser.h"
class Object;

#include "GameObject.hpp"
class GameObject;

#include "util.h"

enum Direction {
    kStay,
    kToLeft,
    kToRight
};

class Level {
public:
    Level();
    ~Level();

    void Load_lvl(std::string path, Object* input_config, Graphics* graphics);

    Graphics* GetGraphics() const;

    void Process(Direction dir);

    void DrawGame(Graphics* graphics) const;

    bool IsLose();

    bool IsWin();

private:
    std::vector<GameObject*> walls_;
    std::vector<GameObject*> bricks_;
    GameObject* ball_;
    GameObject* platform_;
};

class Menu {
public:
    Menu();
    ~Menu();

    void AddNewLine(Unit* line, GameStates action);

    void Move(int x);

    GameStates ChooseLine();

    void Refresh();

    void DrawMenu(Graphics* graphics);

private:
    int lines_cnt;
    int curr_line;
    std::vector<Unit*> lines;
    std::vector<GameStates> lines_action;
    SDL_Surface* surf;
};


class Game {
public:
    Game(Object* input_config);
    ~Game();

    void DrawGame();

    void DrawMenu(Menu* menu);
    void DrawScreen();
    void NewLvl();
    bool MoveLvl(int dir);
    void ProcessGame();
    void ProcessMenu(Menu* menu);
    int Execute(int width, int height, size_t lvl_number);
    Graphics* GetGraphics();
    Input* GetInput();

    void Exit();
private:
    bool run;
    GameStates state;

    Graphics* graphics;
    std::vector<Level*> lvls;
    size_t curr_lvl;
    size_t lvls_count;

    size_t delay;

    Menu* main_menu;
    Menu* game_menu;

    size_t screen_w;
    size_t screen_h;

    Input* input;
    Object* config;
};

#endif