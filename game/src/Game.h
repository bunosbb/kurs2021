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

#include "Audio.h"
class Audio;

#include "Parser.h"
class Object;

#include "util.h"


class Mob {
public:
    void Draw(Graphics* graphics);

private:
    int x;
    int y;
    Unit* skin;
};

class Level {
public:
    Level();
    void Load_lvl(std::string path, Graphics* graphics);

    std::vector<std::string> GetField();
    std::vector<POINT> GetRocks();
    size_t GetW();
    size_t GetH();
    size_t GetStartX();
    size_t GetStartY();
    size_t GetEndX();
    size_t GetEndY();
    size_t CharPosX();
    size_t CharPosY();

    void SetCharPos();
    void SetCharPos(size_t x, size_t y);
    void MoveRock(int k, int x, int y);

    void ChangeRockPos(std::vector<Object>& positions);

    void GetRockPos(std::vector<Object>& positions);

    std::string TryMove(int x, int y);


    bool CheckWall(int x, int y);
    std::string GetWallStr(int x, int y);

    bool IsWin();

private:
    std::vector<std::string> field;
    std::vector<POINT> rocks;
    size_t corn_x;
    size_t corn_y;
    size_t w;
    size_t h;
    size_t ch_x;
    size_t ch_y;
    size_t start_x;
    size_t start_y;
    size_t end_x;
    size_t end_y;
};

class Menu {
public:
    Menu();

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
    Logger* log;
};


class Game {
public:
    Game(Object& input_config);

    void DrawGame();
    void DrawGameFloor();
    void DrawGameWall();
    void DrawGameRockExc(int k, int ch_y, int dir);
    void DrawGameFloor(int alpha);
    void DrawGameWall(int alpha);
    void DrawGameWallAround(int x, int y);
    void DrawGameWallAround(int x, int y, int alpha);
    void DrawGameRockExc(int k, int ch_y, int dir, int alpha);

    void CharMove(int x_ch, int y_ch);
    void CharMoveRock(int x_ch, int y_ch);


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
    std::unordered_map<std::string, Audio*> auds;
    Unit* main_ch;
    Unit* wall;
    Unit* start;
    Unit* end;
    Unit* floor;
    Unit* rock;
    std::vector<Level*> lvls;
    size_t curr_lvl;
    size_t lvls_count;

    Menu* main_menu;
    Menu* game_menu;

    size_t screen_w;
    size_t screen_h;

    Input* input;
    Object& config;

    Logger* log;
};

#endif