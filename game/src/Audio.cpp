#include "Audio.h"

Audio::Audio(std::string new_path) : path{new_path} {
    sound = Mix_LoadWAV(new_path.c_str());
    if (sound == NULL) {
        std::cout << Mix_GetError() << "\n";
    }
    channel = -1;
}

void Audio::Play() {
    if (channel != -1) {
        return;
    }
    channel = Mix_PlayChannel(-1, sound, 0);
    if (channel == -1) {
        std::cout << Mix_GetError() << " on " << path << std::endl;
    }
    channel = -1;
}

void Audio::Play(int count) {
    if (channel != -1) {
        return;
    }
    channel = Mix_PlayChannel(-1, sound, count - 1);
    if (channel == -1) {
        std::cout << Mix_GetError() << " on " << path << std::endl;
    }
}

void Audio::Stop() {
    if (channel == -1) {
        return;
    }
    Mix_HaltChannel(channel);
    channel = -1;
}