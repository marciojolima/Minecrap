#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <ctime>

class Minecrap{
public:
	Minecrap();
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_int_distribution<int> dist;

    int points, health;
    float velocity, x, obj_vel_max, obj_vel;
    bool pressed, gameOver, paused;

    // MUSICA E SONOPLASTIA( EFEITOS DE SOM OU FX )
    sf::SoundBuffer pop, gameover_buffer;
    sf::Sound pop_sound, gameover_sound;
    sf::Music music;

    sf::RenderWindow window;

    sf::Font font, jet;
    sf::Text score, life, gameOverText, f1, paused_text;

    sf::Texture bloco, bg, hammer, stop;
    sf::Sprite object, fundo, picareta, stop_sprite;

    sf::Vector2i pos_mouse_win, current_position; //posicao do mouse em relação aa janela
    sf::Vector2f pos_mouse_coord; //coordenadas mapeadas

    //OBJETOS
    std::vector<sf::Sprite> objs;
    size_t max_objs;

    // PROTOTIPOS DAS FUNCOES MEMBROS
    void code_game();
    void game_over();
    void pause_game();
    void loop_events();
    void run_game();
};

