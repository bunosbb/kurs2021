#include "Game.h"



void Mob::Draw(Graphics* graphics) {
    return;
}


Level::Level() {};

void Level::Load_lvl(std::string path, Graphics* graphics) {
    std::ifstream in(path);
    std::string s;
    while (in >> s) {
        if (field.size() == 0) {
            field.resize(s.size());
        }
        for (size_t i = 0; i < s.size(); ++i) {
            field[i] += s[i];
        }
    }
    h = field.size();
    if (h == 0) return;
    w = field[0].size();
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            if (field[i][j] == 's') {
                start_x = i;
                start_y = j;
            }
            if (field[i][j] == 'e') {
                end_x = i;
                end_y = j;
            }
            if (field[i][j] == 'r') {
                POINT pos(0, 0);
                pos.x = i;
                pos.y = j;
                rocks.push_back(pos);
                field[i][j] = 'f';
            }
        }
    }
    corn_x = 1;
    corn_y = 1;
}

std::vector<std::string> Level::GetField(){ return field; }
std::vector<POINT> Level::GetRocks(){ return rocks; }

size_t Level::GetW(){ return w; }
size_t Level::GetH(){ return h; }

size_t Level::GetStartX(){ return start_x; }
size_t Level::GetStartY(){ return start_y; }

size_t Level::GetEndX(){ return end_x; }
size_t Level::GetEndY(){ return end_y; }

size_t Level::CharPosX() { return ch_x; }
size_t Level::CharPosY() { return ch_y; }

void Level::SetCharPos() {
    ch_x = start_x;
    ch_y = start_y;
}
void Level::SetCharPos(size_t x, size_t y) {
    ch_x = x;
    ch_y = y;
}

void Level::MoveRock(int k, int x, int y) {
    rocks[k].x += x;
    rocks[k].y += y;
}

void Level::ChangeRockPos(std::vector<Object>& positions) {
    rocks.resize(positions.size());
    for (auto i = 0; i < positions.size(); ++i) {
        POINT pos;
        pos.x = positions[i].Get(0).AsInt();
        pos.y = positions[i].Get(1).AsInt();
        rocks[i] = pos;
    }
}

void Level::GetRockPos(std::vector<Object>& positions) {
    positions.resize(rocks.size());
    for (auto i = 0; i < rocks.size(); ++i) {
        POINT pos;
        positions[i].SetType(Vector);
        positions[i].AsVector().resize(2);
        positions[i].AsVector()[0] = rocks[i].x;
        positions[i].AsVector()[1] = rocks[i].y;
    }
}

std::string Level::TryMove(int x, int y) {
    int n_x = ch_x + x, n_y = ch_y + y;
    if (n_x < 0 || n_x >= field.size() ||
        n_y < 0 || n_y >= field[0].size()) {
        return "false";
    }
    for (int i = 0; i < rocks.size(); ++i) {
        if (rocks[i].x == n_x && rocks[i].y == n_y) {
            if (n_x + x >= 0 && n_x + x < field.size() &&
                n_y + y >= 0 && n_y + y < field[0].size() &&
                (field[n_x + x][n_y + y] == 'f' || field[n_x + x][n_y + y] == 's' || field[n_x + x][n_y + y] == 'e')) {
                for (int j = 0; j < rocks.size(); ++j) {
                    if (rocks[j].x == n_x + x && rocks[j].y == n_y + y) {
                        return "false";
                    }
                }
                return "true_rock";
            } else {
                return "false";
            }
        }
    }
    if (field[ch_x + x][ch_y + y] != 'w') {
        ch_x += x;
        ch_y += y;
        return "true";
    }
    return "false";
}

bool Level::CheckWall(int x, int y) {
    return (0 <= x && x < field.size() &&
            0 <= y && y < field[0].size() &&
            field[x][y] == 'w');
}
std::string Level::GetWallStr(int x, int y) {
    std::string res = "tblr";// tblr
    if (CheckWall(x, y - 1)) {
        res[0] = '0';
    }
    if (CheckWall(x, y + 1)) {
        res[1] = '0';
    }
    if (CheckWall(x - 1, y)) {
        res[2] = '0';
    }
    if (CheckWall(x + 1, y)) {
        res[3] = '0';
    }
    return res;
}

bool Level::IsWin() {
    return ch_x == end_x && ch_y == end_y;
}


Menu::Menu() : lines_cnt(0) {}

void Menu::AddNewLine(Unit* line, GameStates action) {
    lines.push_back(line);
    lines_action.push_back(action);
    ++lines_cnt;
    line->Move(50, 60 * (lines_cnt));
}

void Menu::Move(int x) {
    curr_line = (curr_line + x + lines_cnt) % lines_cnt;
}

GameStates Menu::ChooseLine() {
    return lines_action[curr_line];
}

void Menu::Refresh() {
    curr_line = 0;
}

void Menu::DrawMenu(Graphics* graphics) {
    (*log)("Menu", "DrawMenu", "start with " + std::to_string(lines_cnt));
    size_t x = graphics->GetW() / 2 - 150;
    size_t y = graphics->GetH() / 2 - lines_cnt * 40;
    for (int i = 0; i < lines_cnt; ++i) {
        if (i == curr_line) {
            graphics->DrawImage(lines[i]->GetImage(), x, y + i * 80);
            (*log)("Menu", "DrawMenu", "draw moving");
        } else {
            graphics->DrawImage(lines[i]->GetImageStatic(), x, y + i * 80);
            (*log)("Menu", "DrawMenu", "draw static");
        }
    }
}


void GetMultipleImage(Object& info, Unit* unit, Graphics* graphics) {
    for (auto i = 0; i < info.Get("Count").AsInt(); ++i) {
        unit->AddImage(graphics->NewImage((info.Get("Path").AsString() + std::to_string(i) + info.Get("FileType").AsString()).c_str()));
    }
}

void GetMapImages(Object& info, Unit* unit, Graphics* graphics) {
    for (auto i = 0; i < info.AsVector().size(); ++i) {
        unit->AddImage(graphics->NewImage((info.Get(i).Get("Path").AsString() + info.Get(i).Get("FileType").AsString()).c_str()),
            info.Get(i).Get("ImageCode").AsString());
    }
}


Game::Game(Object& input_config) : config(input_config) {
    config = input_config;
    run = true;
    state = MainMenu;
    lvls_count = config.Get("LevelsCount").AsInt();
    main_ch = new Unit;
    GetMultipleImage(config.Get("DemonImage"), main_ch, graphics);
    wall = new Unit;
    GetMapImages(config.Get("WallImage"), wall, graphics);
    floor = new Unit;
    floor->AddImage(graphics->NewImage("../imgs/floor-0.bmp"));
    start = new Unit;
    start->AddImage(graphics->NewImage("../imgs/start-0.bmp"));
    end = new Unit;
    end->AddImage(graphics->NewImage("../imgs/end-0.bmp"));
    rock = new Unit;
    rock->AddImage(graphics->NewImage("../imgs/rock-0.bmp"));
    for (size_t i = 0; i < lvls_count; ++i) {
        std::string path = "../lvls/lvl-" + std::to_string(i) + ".txt";
        Level* lvl = new Level;
        lvl->Load_lvl(path, graphics);
        lvls.push_back(lvl);
    }
    main_menu = new Menu;
    Unit* new_game_line = new Unit;
    GetMultipleImage(config.Get("NewGameImage"), new_game_line, graphics);
    main_menu->AddNewLine(new_game_line, GameLvl);
    Unit* exit_line = new Unit;
    GetMultipleImage(config.Get("ExitImage"), exit_line, graphics);
    main_menu->AddNewLine(exit_line, ExitState);

    game_menu = new Menu;
    Unit* continue_line = new Unit;
    GetMultipleImage(config.Get("ContinueImage"), continue_line, graphics);
    game_menu->AddNewLine(continue_line, GameLvl);
    Unit* main_menu_line = new Unit;
    GetMultipleImage(config.Get("MainMenuImage"), main_menu_line, graphics);
    game_menu->AddNewLine(main_menu_line, MainMenu);

    /*Audio* choose = new Audio("audio\\choose.wav");
    auds["choose"] = choose;
    Audio* roll = new Audio("audio\\roll.wav");
    auds["roll"] = roll;
    Audio* rock_move = new Audio("audio\\rock_move.wav");
    auds["rock_move"] = rock_move;
    Audio* pause = new Audio("audio\\pause-loop.wav");
    auds["pause_loop"] = pause;*/
}

void Game::DrawGame() {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    DrawGameFloor();
    DrawGameWall();
    DrawGameRockExc(-1, main_ch->GetY(), 1);
    graphics->DrawImage(main_ch->GetImage(), x + main_ch->GetX() * 50, y + main_ch->GetY() * 50 - 30);
    DrawGameRockExc(-1, main_ch->GetY(), -1);
    DrawGameWallAround(main_ch->GetX(), main_ch->GetY());
}

void Game::DrawGameFloor() {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto curr = lvls[curr_lvl]->GetField();
    size_t h = lvls[curr_lvl]->GetH();
    size_t w = lvls[curr_lvl]->GetW();
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            if (curr[i][j] == 'f')
                graphics->DrawImage(floor->GetImage(), x + i * 50, y + j * 50);
            if (curr[i][j] == 's')
                graphics->DrawImage(start->GetImage(), x + i * 50, y + j * 50);
            if (curr[i][j] == 'e')
                graphics->DrawImage(end->GetImage(), x + i * 50, y + j * 50);
        }
    }
}

void Game::DrawGameFloor(int alpha) {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto curr = lvls[curr_lvl]->GetField();
    size_t h = lvls[curr_lvl]->GetH();
    size_t w = lvls[curr_lvl]->GetW();
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            if (curr[i][j] == 'f')
                graphics->DrawImage(floor->GetImage(alpha), x + i * 50, y + j * 50);
            if (curr[i][j] == 's')
                graphics->DrawImage(start->GetImage(alpha), x + i * 50, y + j * 50);
            if (curr[i][j] == 'e')
                graphics->DrawImage(end->GetImage(alpha), x + i * 50, y + j * 50);
        }
    }
}

void Game::DrawGameWall() {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto curr = lvls[curr_lvl]->GetField();
    size_t h = lvls[curr_lvl]->GetH();
    size_t w = lvls[curr_lvl]->GetW();
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            if (curr[i][j] == 'w')
                graphics->DrawImage(wall->GetImage(lvls[curr_lvl]->GetWallStr(i, j)), x + i * 50, y + j * 50 - 35);
        }
    }
}

void Game::DrawGameWall(int alpha) {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto curr = lvls[curr_lvl]->GetField();
    size_t h = lvls[curr_lvl]->GetH();
    size_t w = lvls[curr_lvl]->GetW();
    for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
            if (curr[i][j] == 'w')
                graphics->DrawImage(wall->GetImage(alpha, lvls[curr_lvl]->GetWallStr(i, j)), x + i * 50, y + j * 50 - 35);
        }
    }
}

void Game::DrawGameWallAround(int i, int j) {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto field = lvls[curr_lvl]->GetField();
    if (lvls[curr_lvl]->CheckWall(i, j + 1)) {
        graphics->DrawImage(wall->GetImage(255, lvls[curr_lvl]->GetWallStr(i, j + 1)), x + i * 50, y + (j + 1) * 50 - 35, 0, 0, 50, 35);
    }
}

void Game::DrawGameWallAround(int i, int j, int alpha) {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto field = lvls[curr_lvl]->GetField();
    if (0 <= i && i < field.size() && 0 <= j + 1 && j + 1 < field[0].size() &&
        field[i][j + 1] == 'w') {
        graphics->DrawImage(wall->GetImage(alpha, lvls[curr_lvl]->GetWallStr(i, j + 1)), x + i * 50, y + (j + 1) * 50 - 35, 0, 0, 50, 35);
    }
}

void Game::DrawGameRockExc(int k, int ch_y, int dir) {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto rocks = lvls[curr_lvl]->GetRocks();
    for (int i = 0; i < rocks.size(); ++i) {
        if (i != k && rocks[i].y * dir <= ch_y * dir) {
            graphics->DrawImage(rock->GetImage(), x + rocks[i].x * 50, y + rocks[i].y * 50 - 30);
            DrawGameWallAround(rocks[i].x, rocks[i].y);
        }
    }
}

void Game::DrawGameRockExc(int k, int ch_y, int dir, int alpha) {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    auto rocks = lvls[curr_lvl]->GetRocks();
    for (int i = 0; i < rocks.size(); ++i) {
        if (i != k && rocks[i].y * dir <= ch_y * dir) {
            graphics->DrawImage(rock->GetImage(alpha), x + rocks[i].x * 50, y + rocks[i].y * 50 - 30);
            DrawGameWallAround(rocks[i].x, rocks[i].y, alpha);
        }
    }
}

void Game::CharMove(int x_ch, int y_ch) {
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    for (int i = 0; i < 50; i += 2) {
        DrawGameFloor();
        DrawGameWall();
        DrawGameRockExc(-1, main_ch->GetY(), 1);
        graphics->DrawImage(main_ch->GetImage(), x + main_ch->GetX() * 50 + x_ch * (i + 1), y + main_ch->GetY() * 50 + y_ch * (i + 1) - 30);
        DrawGameRockExc(-1, main_ch->GetY(), -1);
        DrawGameWallAround(main_ch->GetX(), main_ch->GetY());
        DrawGameWallAround(main_ch->GetX() + x_ch, main_ch->GetY() + y_ch);
        graphics->Update();
        SDL_Delay(2);
    }
    main_ch->Move(x_ch, y_ch);
}

void Game::CharMoveRock(int x_ch, int y_ch) {
    auto rocks = lvls[curr_lvl]->GetRocks();
    int moving_rock;
    for (int i = 0; i < rocks.size(); ++i) {
        if (rocks[i].x == main_ch->GetX() + x_ch && rocks[i].y == main_ch->GetY() + y_ch) {
            moving_rock = i;
            break;
        }
    }
    size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
    size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
    //auds["rock_move"]->Play(0);
    for (int i = 0; i < 50; i += 2) {
        DrawGameFloor();
        DrawGameWall();
        DrawGameRockExc(moving_rock, main_ch->GetY(), 1);
        graphics->DrawImage(rock->GetImage(), x + rocks[moving_rock].x * 50 + x_ch * (i + 1), y + rocks[moving_rock].y * 50 + y_ch * (i + 1),
                0, 30, 50, 80);
        graphics->DrawImage(main_ch->GetImage(), x + main_ch->GetX() * 50 + x_ch * (i + 1), y + main_ch->GetY() * 50 + y_ch * (i + 1) - 30);
        graphics->DrawImage(rock->GetImage(), x + rocks[moving_rock].x * 50 + x_ch * (i + 1), y + rocks[moving_rock].y * 50 + y_ch * (i + 1) - 30,
                0, 0, 50, 30);
        DrawGameRockExc(moving_rock, main_ch->GetY(), -1);
        DrawGameWallAround(main_ch->GetX(), main_ch->GetY());
        DrawGameWallAround(main_ch->GetX() + x_ch, main_ch->GetY() + y_ch);
        DrawGameWallAround(main_ch->GetX() + 2 * x_ch, main_ch->GetY() + 2 * y_ch);
        graphics->Update();
        SDL_Delay(1);
    }
    lvls[curr_lvl]->SetCharPos(main_ch->GetX() + x_ch, main_ch->GetY() + y_ch);
    lvls[curr_lvl]->MoveRock(moving_rock, x_ch, y_ch);
    main_ch->Move(x_ch, y_ch);
    //auds["rock_move"]->Stop();
}

void Game::DrawMenu(Menu* menu) {
    menu->DrawMenu(graphics);
}

void Game::DrawScreen() {
    graphics->Clear();
    switch (state) {
    case GameLvl:
        DrawGame();
        break;
    case MainMenu:
        DrawMenu(main_menu);
        break;
    case GameMenu:
        DrawMenu(game_menu);
        break;
    }
    graphics->Update();
}

void Game::NewLvl() {
    if (curr_lvl + 1 < lvls_count) {
        ++curr_lvl;
        main_ch->Move(lvls[curr_lvl]->GetStartX() - main_ch->GetX(), lvls[curr_lvl]->GetStartY() - main_ch->GetY());
        lvls[curr_lvl]->SetCharPos();
    } else {
        state = MainMenu;
    }
}

bool Game::MoveLvl(int dir) { //������� � �������
    int new_lvl = static_cast<int>(curr_lvl) + dir;
    if (new_lvl < 0 || new_lvl >= lvls_count)
        return false;
    int x_change = (static_cast<int>(lvls[new_lvl]->GetW()) - static_cast<int>(lvls[curr_lvl]->GetW())) / 2;
    int y_change = (static_cast<int>(lvls[new_lvl]->GetH()) - static_cast<int>(lvls[curr_lvl]->GetH())) / 2;
    lvls[new_lvl]->SetCharPos(lvls[curr_lvl]->CharPosX(), lvls[curr_lvl]->CharPosY());
    auto try_result = lvls[new_lvl]->TryMove(x_change, y_change);
    if (try_result == "true") {
        size_t x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
        size_t y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
        for (int i = 250; i >= 0; i -= 5) {
            graphics->Clear();
            DrawGameFloor(i);
            DrawGameWall(i);
            DrawGameRockExc(-1, main_ch->GetY(), 1, i);
            graphics->DrawImage(main_ch->GetImage(), x + main_ch->GetX() * 50, y + main_ch->GetY() * 50 - 30);
            DrawGameRockExc(-1, main_ch->GetY(),-1, i);
            DrawGameWallAround(main_ch->GetX(), main_ch->GetY(), i);
            graphics->Update();
            SDL_Delay(1);
        }
        curr_lvl = new_lvl;
        main_ch->Move(x_change, y_change);
        x = screen_w / 2 - lvls[curr_lvl]->GetH() * 25;
        y = screen_h / 2 - lvls[curr_lvl]->GetW() * 25;
        for (int i = 0; i <= 250; i += 5) {
            graphics->Clear();
            DrawGameFloor(i);
            DrawGameWall(i);
            DrawGameRockExc(-1, main_ch->GetY(), 1, i);
            graphics->DrawImage(main_ch->GetImage(), x + main_ch->GetX() * 50, y + main_ch->GetY() * 50 - 30);
            DrawGameRockExc(-1, main_ch->GetY(), -1, i);
            DrawGameWallAround(main_ch->GetX(), main_ch->GetY(), i);
            graphics->Update();
            SDL_Delay(1);
        }
    }
}

void Game::ProcessGame() {
    if (input->IsKeyDown(119)) {//w
        auto try_result = lvls[curr_lvl]->TryMove(0, -1);
        /*while(!input->IsKeyUp(97) && try_result == "true") {
            if (try_result == "true") {
                CharMove(0, -1);
            }
            if (try_result == "true_rock") {
                CharMoveRock(0, -1);
            }
            try_result = "";
            try_result = lvls[curr_lvl]->TryMove(0, -1);
            input->Update();
        }*/
        if (try_result == "true") {
            CharMove(0, -1);
        }
        if (try_result == "true_rock") {
            CharMoveRock(0, -1);
        }
    }
    if (input->IsKeyDown(97)) {//a
        auto try_result = lvls[curr_lvl]->TryMove(-1, 0);
        if (try_result == "true") {
            CharMove(-1, 0);
        }
        if (try_result == "true_rock") {
            CharMoveRock(-1, 0);
        }
    }
    if (input->IsKeyDown(115)) {//s
        auto try_result = lvls[curr_lvl]->TryMove(0, 1);
        if (try_result == "true") {
            CharMove(0, 1);
        }
        if (try_result == "true_rock") {
            CharMoveRock(0, 1);
        }
    }
    if (input->IsKeyDown(100)) {//d
        auto try_result = lvls[curr_lvl]->TryMove(1, 0);
        if (try_result == "true") {
            CharMove(1, 0);
        }
        if (try_result == "true_rock") {
            CharMoveRock(1, 0);
        }
    }
    if (input->IsKeyDown(113)) {//q - back
        MoveLvl(-1);
    }
    if (input->IsKeyDown(101)) {//e - forward
        MoveLvl(1);
    }
    if (input->IsKeyDown(114)) {//r - restart curr lvl
        --curr_lvl;
        NewLvl();
    }
    if (input->IsKeyDown(27)) {//esc
        (*log)("Game", "ProcessGame", "ESC pressed");
        while (run && !input->IsKeyUp(27)) {
            input->Update();
        }
        (*log)("Game", "ProcessGame", "ESC is up");
        game_menu->Refresh();
        state = GameMenu;
        (*log)("Game", "ProcessGame", "ESC process is end");
        return;
    }
    if (input->IsExit()) {
        state = ExitState;
    }
    if (lvls[curr_lvl]->IsWin()) {
        NewLvl();
    }
}

void Game::ProcessMenu(Menu* menu) {
    if (input->IsKeyDown(119)) {//w
        //auds["roll"]->Play();
        menu->Move(-1);
    }
    if (input->IsKeyDown(115)) {//s
        //auds["roll"]->Play();
        menu->Move(1);
    }
    if (input->IsKeyDown(13)) {//enter
        while (run && !input->IsKeyUp(13)) {
            input->Update();
        }
        //auds["choose"]->Play();
        state = menu->ChooseLine();
        SDL_Delay(100);
    }
    if (input->IsKeyDown(27)) {//esc
        while (run && !input->IsKeyUp(27)) {
            input->Update();
        }
        if (state == MainMenu) {
            state = ExitState;
        } else {
            state = GameLvl;
        }
    }
    if (input->IsExit()) {
        state = ExitState;
    }
}

int Game::Execute(int width, int height, size_t lvl_number) {
    screen_w = width;
    screen_h = height;
    curr_lvl = lvl_number;
    graphics = new Graphics(width, height);
    input = new Input();
    /*Object save;
    save.Read(config.Get("SavePath").AsString());
    if (!save.IsNull()) {
        curr_lvl = save.Get("LvlNumber").AsInt();
        for (auto i = 0; i < save.Get("RocksPositions").AsVector().size(); ++i) {
            lvls[i]->ChangeRockPos(save.Get("RocksPositions").Get(i).AsVector());
        }
    }*/

    lvls[curr_lvl]->SetCharPos();
    main_ch->Move(lvls[curr_lvl]->GetStartX() - main_ch->GetX(), lvls[curr_lvl]->GetStartY() - main_ch->GetY());
    DrawScreen();

    while (run) {
        input->Update();
        if (state == GameLvl) {
            //auds["pause_loop"]->Stop();
        } else {
            //auds["pause_loop"]->Play(0);
        }
        switch (state) {
        case MainMenu:
            ProcessMenu(main_menu);
            break;
        case GameMenu:
            ProcessMenu(game_menu);
            break;
        case GameLvl:
            ProcessGame();
            break;
        case ExitState:
            Exit();
            break;
        }
        DrawScreen();
        (*log)("Game", "Execute", "screen drawed");
    }


    delete graphics;
    delete input;

    /*save.SetType(Dict);

    save.Get("LvlNumber") = curr_lvl;
    save.Get("RocksPositions").SetType(Vector);
    for (auto i = 0; i <= curr_lvl; ++i) {
        if (save.Get("RocksPositions").AsVector().size() <= i) {
            save.Get("RocksPositions").AsVector().push_back(Object());
        }
        save.Get("RocksPositions").AsVector()[i].SetType(Vector);
        lvls[i]->GetRockPos(save.Get("RocksPositions").AsVector()[i].AsVector());        
    }
    save.Print(config.Get("SavePath").AsString());*/
    SDL_Quit();
    return 0;
}

Graphics* Game::GetGraphics() {
    return graphics;
}

Input* Game::GetInput() {
    return input;
}

void Game::Exit() {
    run = false;
}