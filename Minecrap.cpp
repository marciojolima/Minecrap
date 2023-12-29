#include "Minecrap.h"

Minecrap::Minecrap() {
    mt.seed(rd());

    // WINDOW
    window.create(sf::VideoMode(1280, 720), "Minecrap - 0.1", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i{ 400,200 });
    window.setMouseCursorVisible(false);

    // MUSICA E SONOPLASTIA( EFEITOS DE SOM OU FX )
    pop.loadFromFile("./assets/sound/pop.wav");
    gameover_buffer.loadFromFile("./assets/sound/game-over-sound.wav");
    music.openFromFile("./assets/sound/soundtrack.wav");
    pop_sound.setBuffer(pop);
    gameover_sound.setBuffer(gameover_buffer);

    // MAIN VARIAVEIS
    points = 0;
    health = 5;
    velocity = 5.f;
    pressed = false;
    gameOver = false;
    paused = false;

    font.loadFromFile("./assets/fonts/Minecraft.ttf");
    jet.loadFromFile("./assets/fonts/jetbrains.ttf");

    score.setFont(font);
    score.setString("Pontos: " + std::to_string(points));
    score.setFillColor(sf::Color::White);
    score.setPosition(5.f, 5.f);
    life.setFont(font);
    life.setString("Vidas: " + std::to_string(health));
    life.setFillColor(sf::Color::Yellow);
    life.setPosition(1125.f, 5.f);
    gameOverText.setFont(font);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setString("GAME OVER!");
    gameOverText.setPosition(350, 350);
    gameOverText.setCharacterSize(100);
    f1.setFont(jet);
    f1.setFillColor(sf::Color::Yellow);
    f1.setPosition(600, 10);
    f1.setString("Pressione F1 para um novo jogo.");
    paused_text.setFont(jet);
    paused_text.setFillColor(sf::Color::White);
    paused_text.setString("PAUSED");
    paused_text.setPosition(350, 300);
    paused_text.setCharacterSize(180);

    bloco.loadFromFile("./assets/images/minecrap.png");
    bg.loadFromFile("./assets/images/fundo.jpg");
    hammer.loadFromFile("./assets/images/hammer.png");
    stop.loadFromFile("./assets/images/paused.jpg");
    
    object.setTexture(bloco);
    fundo.setTexture(bg);
    picareta.setTexture(hammer);
    stop_sprite.setTexture(stop);

    dist = std::uniform_int_distribution<int>(10, (int)(window.getSize().x - bloco.getSize().x - 10));
    x = static_cast<float>(dist(mt));
    object.setPosition(x, 10.f);

    max_objs = 3;
    obj_vel_max = 10.f;
    obj_vel = obj_vel_max;
}

void Minecrap::code_game() {
    // CODIGO DO JOGO
    //Adicionar objectos ao nosso vector com atrasos
    if (objs.size() < max_objs) {
        if (obj_vel >= obj_vel_max) {
            x = static_cast<float>(dist(mt));
            object.setPosition(x, 0.f);
            objs.push_back(object);
            obj_vel = 0.f;
        }
        else {
            obj_vel += 1.f;
        }
    }

    //mover e deletar os objetos
    for (size_t i{}; i < objs.size(); ++i) {
        bool del = false;
        objs[i].move(0.f, velocity);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed) {
            if (objs[i].getGlobalBounds().contains(pos_mouse_coord)) {
                pop_sound.play();
                del = true;
                points += 10;
                score.setString("Pontos: " + std::to_string(points));
                pressed = true;
                //aumentando a dificuldade
                if (points % 100 == 0) {
                    velocity += 0.1f;
                    //std::cout << "Velocidade aumentada: " << velocity << '\n';
                }
                //bonificacao
                if (points % 200 == 0) {
                    ++health;
                }
                //std::cout << "Acertou miseravel: " << '\n';
                //std::cout << "Pontos : " << points << '\n';
            }
        }
        if (objs[i].getPosition().y > window.getSize().y) {
            --health;
            life.setString("Vidas: " + std::to_string(health));
            del = true;
            if (health <= 0) {
                gameover_sound.play();
                gameOver = true;
                //std::cout << "Game Over - Voce perdeu!" << '\n';
                //std::cout << "PONTOS FEITOS: " << points << '\n';
                //window.close();
            }
        }

        if (del) {
            objs.erase(objs.begin() + i);
        }
    }

    window.clear();
    window.draw(fundo);
    window.draw(score);
    window.draw(life);
    for (auto& e : objs) {
        window.draw(e);
    }
    window.draw(picareta);
    window.display();
}

void Minecrap::game_over() {
    music.stop();
    window.setMouseCursorVisible(true);
    window.clear(sf::Color::Black);
    window.draw(score);
    window.draw(f1);
    window.draw(gameOverText);
    window.display();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
        points = 0;
        score.setString("Pontos: " + std::to_string(points));
        health = 3;
        life.setString("Pontos: " + std::to_string(health));
        velocity = 5.f;
        objs.clear();
        gameOver = false;
        window.setMouseCursorVisible(false);
    }
}

void Minecrap::pause_game() {
    music.pause();
    window.clear();
    window.draw(stop_sprite);
    window.draw(paused_text);
    window.display();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        paused = false;
    }
    picareta.setPosition(static_cast<sf::Vector2f>(current_position));
}

void Minecrap::loop_events() {
    sf::Event event;

    if (music.getStatus() == sf::Music::Stopped || music.getStatus() == sf::Music::Paused) {
        music.play();
    }

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::Resized) {
            sf::FloatRect visible_area(0.f, 0.f, (float)event.size.width, (float)event.size.height);
            window.setView(sf::View(visible_area));
        }

        if (event.type == event.MouseButtonPressed) {
            pressed = false;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            paused = true;
        }

        pos_mouse_win = sf::Mouse::getPosition(window);
        pos_mouse_coord = window.mapPixelToCoords(pos_mouse_win);
        picareta.setPosition((sf::Vector2f)pos_mouse_win);
    }
}

void Minecrap::run_game() {
    while (window.isOpen()) {
        this->loop_events();
        if (gameOver) {
            this->game_over();
        }
        else {
            if (paused) {
                this->pause_game();
            }
            else {
                this->code_game();
            }
        }
    }
}