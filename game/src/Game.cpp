#include "Game.h"

Level::Level() {};

Level::~Level() {
    for (auto wall : walls_) {
        delete wall;
    }
    for (auto brick : bricks_) {
        delete brick;
    }
    delete ball_;
    delete platform_;
}

void Level::Load_lvl(std::string path, Object* input_config, Graphics* graphics) {
    auto& config = input_config->AsDict();
    Object level;
    level.Read(path);
    auto& lvl_map = level.AsDict();
    int speed = lvl_map["StartSpeed"]->AsInt();
    auto g_boost = config["FallAbs"]->AsVector();
    // creating ball
    auto& ball = config["BallImage"]->AsDict();
    auto ball_image = graphics->NewImage(ball["Path"]->AsString().c_str());
    auto ball_high = ball["High"]->AsInt();
    auto ball_widht = ball["Widht"]->AsInt();
    auto ball_pos = lvl_map["Ball"]->AsVector();
    ball_ = new GameObject(
        POINT(0, -speed), POINT(g_boost[0].AsInt(), g_boost[1].AsInt()),
        POINT(ball_pos[0].AsInt(), ball_pos[1].AsInt()),
        ball_image, ball_widht, ball_high
    );
    // creating platform
    auto& platform = config["PlatformImage"]->AsDict();
    auto platform_image = graphics->NewImage(platform["Path"]->AsString().c_str());
    auto platform_high = platform["High"]->AsInt();
    auto platform_widht = platform["Widht"]->AsInt();
    auto platform_pos = lvl_map["Platform"]->AsVector();
    platform_ = new GameObject(
        POINT(0, 0), POINT(0, 0),
        POINT(platform_pos[0].AsInt(), platform_pos[1].AsInt()),
        platform_image, platform_widht, platform_high
    );
    // creating walls
    auto& left_wall = config["LeftWall"]->AsDict();
    auto left_wall_image = graphics->NewImage(left_wall["Path"]->AsString().c_str());
    auto left_wall_high = left_wall["High"]->AsInt();
    auto left_wall_widht = left_wall["Widht"]->AsInt();
    auto left_wall_pos = lvl_map["LeftWall"]->AsVector();
    walls_.push_back(new GameObject(
        POINT(0, 0), POINT(0, 0),
        POINT(left_wall_pos[0].AsInt(), left_wall_pos[1].AsInt()),
        left_wall_image, left_wall_widht, left_wall_high
    ));
    auto& right_wall = config["RightWall"]->AsDict();
    auto right_wall_image = graphics->NewImage(right_wall["Path"]->AsString().c_str());
    auto right_wall_high = right_wall["High"]->AsInt();
    auto right_wall_widht = right_wall["Widht"]->AsInt();
    auto right_wall_pos = lvl_map["RightWall"]->AsVector();
    walls_.push_back(new GameObject(
        POINT(0, 0), POINT(0, 0),
        POINT(right_wall_pos[0].AsInt(), right_wall_pos[1].AsInt()),
        right_wall_image, right_wall_widht, right_wall_high
    ));
    auto& top_wall = config["TopWall"]->AsDict();
    auto top_wall_image = graphics->NewImage(top_wall["Path"]->AsString().c_str());
    auto top_wall_high = top_wall["High"]->AsInt();
    auto top_wall_widht = top_wall["Widht"]->AsInt();
    auto top_wall_pos = lvl_map["TopWall"]->AsVector();
    walls_.push_back(new GameObject(
        POINT(0, 0), POINT(0, 0),
        POINT(top_wall_pos[0].AsInt(), top_wall_pos[1].AsInt()),
        top_wall_image, top_wall_widht, top_wall_high
    ));
    // creating bricks
    auto& brick = config["BrickImage"]->AsDict();
    auto brick_image = graphics->NewImage(brick["Path"]->AsString().c_str());
    auto brick_high = brick["High"]->AsInt();
    auto brick_widht = brick["Widht"]->AsInt();
    auto& bricks = lvl_map["Bricks"]->AsVector();
    bricks_.reserve(bricks.size());
    for (auto& brick : bricks) {
        auto brick_pos = brick.AsVector();
        bricks_.push_back(new GameObject(
            POINT(0, 0), POINT(0, 0),
            POINT(brick_pos[0].AsInt(), brick_pos[1].AsInt()),
            brick_image, brick_widht, brick_high
        ));
    }
}

bool Level::IsLose() {
    return ball_->GetPos().y > platform_->GetPos().y + platform_->GetH();
}

bool Level::IsWin() {
    return bricks_.empty();
}

void MovePlatform(GameObject* platform, Direction dir) {
    switch(dir) {
    case Direction::kToLeft:
        platform->SetBoost(POINT(-2, 0));
        break;
    case Direction::kToRight:
        platform->SetBoost(POINT(2, 0));
        break;
    case Direction::kStay:
        platform->SetBoost(POINT(0, 0));
        break;
    }
    platform->Boost();
}

void Level::Process(Direction dir) {
    for (auto& wall : walls_) {
        auto inter = platform_->IsIntersec(wall);
        if (inter.has_value() && 
            ((*inter == ReflectDirection::kLeft && dir == Direction::kToLeft) ||
             (*inter == ReflectDirection::kRight && dir == Direction::kToRight))
        ) {
            dir = kStay;
        }
    }
    auto inter = ball_->IsIntersec(platform_);
    if (inter.has_value()) {
        ball_->ReflectBoost(*inter);
        ball_->AddBoost(platform_);
        MovePlatform(platform_, dir);
        ball_->Boost(*inter);
        return;
    }
    for (auto& wall : walls_) {
        auto inter = ball_->IsIntersec(wall);
        if (inter.has_value()) {
            ball_->ReflectBoost(*inter);
            MovePlatform(platform_, dir);
            ball_->Boost(*inter);
            return;
        }
    }

    for (auto it = bricks_.begin(); it != bricks_.end(); ++it) {
        auto inter = ball_->IsIntersec(*it);
        if (inter.has_value()) {
            ball_->ReflectBoost(*inter);
            MovePlatform(platform_, dir);
            ball_->Boost(*inter);
            bricks_.erase(it);
            return;
        }
    }
    MovePlatform(platform_, dir);
    ball_->Boost();
}

void Level::DrawGame(Graphics* graphics) const {
    for (auto& brick : bricks_) {
        graphics->DrawImage(brick->GetImage(), brick->GetPos().x, brick->GetPos().y);
    }
    for (auto& wall : walls_) {
        graphics->DrawImage(wall->GetImage(), wall->GetPos().x, wall->GetPos().y);
    }
    graphics->DrawImage(platform_->GetImage(), platform_->GetPos().x, platform_->GetPos().y);
    graphics->DrawImage(ball_->GetImage(), ball_->GetPos().x, ball_->GetPos().y);
    //std::cout << "ball: " << ball_->GetPos().x << ", " << ball_->GetPos().y << std::endl;
}

Menu::Menu() : lines_cnt(0), curr_line(0) {}

Menu::~Menu() {
    for (auto line : lines) {
        delete line;
    }
}

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
    size_t x = graphics->GetW() / 2 - 150;
    size_t y = graphics->GetH() / 2 - lines_cnt * 40;
    for (int i = 0; i < lines_cnt; ++i) {
        if (i == curr_line) {
            graphics->DrawImage(lines[i]->GetImage(), x, y + i * 80);
        } else {
            graphics->DrawImage(lines[i]->GetImageStatic(), x, y + i * 80);
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


Game::Game(Object* input_config){
    config = input_config;
    run = true;
    state = MainMenu;
    lvls_count = config->Get("LevelsCount").AsInt();
    delay = config->Get("Delay").AsInt();
    for (size_t i = 0; i < lvls_count; ++i) {
        std::string path = "../lvls/lvl-" + std::to_string(i) + ".json";
        Level* lvl = new Level;
        lvl->Load_lvl(path, input_config, graphics);
        lvls.push_back(lvl);
    }
    main_menu = new Menu;
    Unit* new_game_line = new Unit;
    GetMultipleImage(config->Get("NewGameImage"), new_game_line, graphics);
    main_menu->AddNewLine(new_game_line, GameLvl);
    Unit* exit_line = new Unit;
    GetMultipleImage(config->Get("ExitImage"), exit_line, graphics);
    main_menu->AddNewLine(exit_line, ExitState);

    game_menu = new Menu;
    Unit* continue_line = new Unit;
    GetMultipleImage(config->Get("ContinueImage"), continue_line, graphics);
    game_menu->AddNewLine(continue_line, GameLvl);
    Unit* main_menu_line = new Unit;
    GetMultipleImage(config->Get("MainMenuImage"), main_menu_line, graphics);
    game_menu->AddNewLine(main_menu_line, MainMenu);
}

Game::~Game() {
    delete graphics;
    delete main_menu;
    delete game_menu;
}

void Game::DrawGame() {
    lvls[curr_lvl]->DrawGame(graphics);
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
    } else {
        state = MainMenu;
    }
}

void Game::ProcessGame() {
    if (input->IsKeyDown(97)) {//a
        lvls[curr_lvl]->Process(Direction::kToLeft);
        while (run && !input->IsKeyUp(97)) {
            input->Update();
            lvls[curr_lvl]->Process(Direction::kToLeft);
            SDL_Delay(5);
            DrawScreen();
        }
        return;
    }
    if (input->IsKeyDown(100)) {//d
        lvls[curr_lvl]->Process(Direction::kToRight);
        while (run && !input->IsKeyUp(100)) {
            input->Update();
            lvls[curr_lvl]->Process(Direction::kToRight);
            SDL_Delay(5);
            DrawScreen();
        }
        return;
    }
    if (input->IsKeyDown(27)) {//esc
        while (run && !input->IsKeyUp(27)) {
            input->Update();
        }
        game_menu->Refresh();
        state = GameMenu;
        return;
    }
    if (input->IsExit()) {
        state = ExitState;
    }
    if (lvls[curr_lvl]->IsWin()) {
        NewLvl();
    }
    if (lvls[curr_lvl]->IsLose()) {
        state = ExitState;
    }
    lvls[curr_lvl]->Process(Direction::kStay);
    SDL_Delay(delay);
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
    DrawScreen();

    while (run) {
        input->Update();
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
    }


    delete graphics;
    delete input;
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