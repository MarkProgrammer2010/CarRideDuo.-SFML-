#include <SFML/Graphics.hpp>
#include <ctime>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

using namespace sf;
using namespace std;

// v1.0

// управление первого игрока на A и D
// управление второго игрока на ← и →
// всё что нужно знать

// самое важное что нужно реализовать, это сделать зависимость от время исполнения процессора чтобы время у всех примерно одинакого шло.

// на данный момент игра по большей части не оптимизированна. Написание и архитектура кода в некоторых местах может вызвать рвотный рефлекс.
// Названия с моего взгляда адекватные и интуитивно понятные. Логика более менее. Просто сыграть сойдёт.
// Писал быстро. С++ пока что только учу. На момент реализации мне (было) 13 лет. 
// Согласен многие уже в этом возрасте взламывают пентагон 😁.

const int WIDE = 1300; // ширина окна
const int HEIGHT = 800; // высота окна

const float carSPEED = 1 + (WIDE / 500); // скорость
float obstacleSPEED = 0.7;
float SlowedSPEED = obstacleSPEED / 5; // уменьшеная скорость

float SCOREF = 0; // добавочный счёт 
const int BALLS = WIDE / 400; // количесвто мячей
const int carWIDE = 50; // ширина машины
const int carHEIGHT = 180; // высота машины
const int obstacleDIAMETER = 15; // диаметр препядствия
const int coinDIAMETER = 30; // димаетр монетки

int SCORE1 = 0; // счёт 1
int highSCORE1 = 0; // счёт 1
int SOLID1 = 50; // прочность машины 1
int previosX1 = WIDE/2;
int currentX1 = WIDE/2;

int SCORE2 = 0; // счёт 2
int highSCORE2 = 0; // счёт 2
int SOLID2 = 50; // прочность машины 2
int previosX2 = WIDE/2;
int currentX2 = WIDE/2;
bool secondCarKeyboardORmouse = true;

int SCORE = (int)((SCORE1 + SCORE2) / 2);

const string fileNameHighScore1 = "highScore1.txt"; // путь к файлу со счётом 1
const string fileNameHighScore2 = "highScore2.txt"; // путь к файлу со счётом 2
const string fileNameCar1 = "carPicture1.png";
const string fileNameCar2 = "carPicture2.png";
const string fileNameObstacle = "obstacle.png";
const string fileNameCoin = "coin.png";

void RefreshFileHighScores() {
    string highScore1 = to_string(highSCORE1);
    ofstream fileHighScore1_2(fileNameHighScore1, ios::out);
    fileHighScore1_2 << highScore1;
    fileHighScore1_2.close();

    string highScore2 = to_string(highSCORE2);
    ofstream fileHighScore2_2_2(fileNameHighScore2, ios::out);
    fileHighScore2_2_2 << highScore2;
    fileHighScore2_2_2.close();
}

void RefreshVariableFromFile() {
    // обновления рекордного счёта 1 с прошлой игры
    string bufferS1 = "";
    ifstream fileHighScore1(fileNameHighScore1, ifstream::in);
    if (fileHighScore1.is_open()) {
        getline(fileHighScore1, bufferS1);
        highSCORE1 = stoi(bufferS1);
        fileHighScore1.close();
    }
    // обновления рекордного счёта 2 с прошлой игры
    string bufferS2 = "";
    ifstream fileHighScore2(fileNameHighScore2, ifstream::in);
    if (fileHighScore2.is_open()) {
        getline(fileHighScore2, bufferS2);
        highSCORE2 = stoi(bufferS2);
        fileHighScore2.close();
    }
}

void MoveCar(bool* keyboardORmouse, Sprite* carSprite1, RenderWindow* window) {
    if (*keyboardORmouse) {
        if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D)) {
            carSprite1->move(-0.5 * (carSPEED / 2), 0);
            if (carSprite1->getRotation() > 358 - carSPEED * 3 || carSprite1->getRotation() == 0 || carSprite1->getRotation() < 100) {
                carSprite1->rotate(-1);
                carSprite1->move(0, 0.5);
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A)) {
            carSprite1->move(0.5 * (carSPEED / 2), 0);
            if (carSprite1->getRotation() < 2 + carSPEED * 3 || carSprite1->getRotation() == 0 || carSprite1->getRotation() > 300) {
                carSprite1->rotate(1);
                carSprite1->move(0, -0.5);
            }
        }
        else
        {
            if (carSprite1->getRotation() < 359 && carSprite1->getRotation() != 0 && carSprite1->getRotation() > 300) {
                carSprite1->rotate(0.1);
            }
            else if (carSprite1->getRotation() < 30 && carSprite1->getRotation() != 0) {
                carSprite1->rotate(-0.1);
            }
            else {
                carSprite1->setRotation(0);
            }
            carSprite1->setPosition(carSprite1->getPosition().x, 630);
        }
    }
    else if (!*keyboardORmouse) {
        if (secondCarKeyboardORmouse) {
            if (Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Right)) {
                carSprite1->move(-0.5 * (carSPEED / 2), 0);
                if (carSprite1->getRotation() > 358 - carSPEED * 3 || carSprite1->getRotation() == 0 || carSprite1->getRotation() < 100) {
                    carSprite1->rotate(-1);
                    carSprite1->move(0, 0.5);
                }
            }
            else if (Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Left)) {
                carSprite1->move(0.5 * (carSPEED / 2), 0);
                if (carSprite1->getRotation() < 2 + carSPEED * 3 || carSprite1->getRotation() == 0 || carSprite1->getRotation() > 300) {
                    carSprite1->rotate(1);
                    carSprite1->move(0, -0.5);
                }
            }
            else
            {
                if (carSprite1->getRotation() < 359 && carSprite1->getRotation() != 0 && carSprite1->getRotation() > 300) {
                    carSprite1->rotate(0.1);
                }
                else if (carSprite1->getRotation() < 30 && carSprite1->getRotation() != 0) {
                    carSprite1->rotate(-0.1);
                }
                else {
                    carSprite1->setRotation(0);
                }
                carSprite1->setPosition(carSprite1->getPosition().x, 630);
            }
        }
        else {
            if (Mouse::getPosition(*window).x < carSprite1->getPosition().x + carWIDE / 2 - carWIDE / 3) {
                carSprite1->move(-0.5 * (carSPEED / 2), 0);
                if (carSprite1->getRotation() > 358 - carSPEED * 3 || carSprite1->getRotation() == 0 || carSprite1->getRotation() < 100) {
                    carSprite1->rotate(-1);
                    carSprite1->move(0, 0.5);
                }
            }
            else if (Mouse::getPosition(*window).x > carSprite1->getPosition().x + carWIDE / 2 + carWIDE / 3) {
                carSprite1->move(0.5 * (carSPEED / 2), 0);
                if (carSprite1->getRotation() < 2 + carSPEED * 3 || carSprite1->getRotation() == 0 || carSprite1->getRotation() > 300) {
                    carSprite1->rotate(1);
                    carSprite1->move(0, -0.5);
                }
            }
            else
            {
                if (carSprite1->getRotation() < 359 && carSprite1->getRotation() != 0 && carSprite1->getRotation() > 300) {
                    carSprite1->rotate(0.1);
                }
                else if (carSprite1->getRotation() < 30 && carSprite1->getRotation() != 0) {
                    carSprite1->rotate(-0.1);
                }
                else {
                    carSprite1->setRotation(0);
                }
                carSprite1->setPosition(carSprite1->getPosition().x, 630);
            }
        }
    }
}

bool CheckCollisionWithCar(Sprite *carSprite1, Sprite *carSprite2, bool firstORsecond) {
    if(carSprite1->getGlobalBounds().intersects(carSprite2->getGlobalBounds())){
        if (firstORsecond) {
            if (Keyboard::isKeyPressed(Keyboard::D) && carSprite1->getPosition().x < carSprite2->getPosition().x) {
                carSprite2->move(0.5 * (carSPEED / 2), 0);
                return true;
            }
            if (Keyboard::isKeyPressed(Keyboard::A) && carSprite1->getPosition().x > carSprite2->getPosition().x) {
                carSprite2->move(-0.5 * (carSPEED / 2), 0);
                return true;
            }
        }
        else {
            if (Keyboard::isKeyPressed(Keyboard::Right) && carSprite1->getPosition().x < carSprite2->getPosition().x) {
                carSprite2->move(0.5 * (carSPEED / 2), 0);
                return true;
            }
            if (Keyboard::isKeyPressed(Keyboard::Left) && carSprite1->getPosition().x > carSprite2->getPosition().x) {
                carSprite2->move(-0.5 * (carSPEED / 2), 0);
                return true;
            }
        }
    }
    return false;
}

void CheckCarCollisionWithFrame(Sprite *carSprite) {
    // обработчик выхода машинки за границу окна
    if (carSprite->getPosition().x < -45) {
        carSprite->setPosition(WIDE - 21, 630);
    }
    else if (carSprite->getPosition().x > WIDE - 20) {
        carSprite->setPosition(-35, 630);
    }
}

void DrawAll(Sprite* carSprite1, Sprite* carSprite2, Sprite obstacle[], Sprite* coinSprite, bool* CollisionObstacle, Text* scoreInt1, Text* solidText, Text* solidInt1, Text* scoreText, Text* highScoreText, Text* highScoreInt1, Text* gameOver, Text *scoreInt2, Text *highScoreInt2, Text *solidInt2, RenderWindow* window, bool drawGameOver, Sprite* blastSprite) {
    scoreInt1->setString(to_string(SCORE1));
    solidInt1->setString(to_string(SOLID1));
    highScoreInt1->setString(to_string(highSCORE1));
    scoreInt2->setString(to_string(SCORE2));
    solidInt2->setString(to_string(SOLID2));
    highScoreInt2->setString(to_string(highSCORE2));
    window->clear(Color(105, 105, 105));
    window->draw(*carSprite1);
    window->draw(*carSprite2);
    window->draw(*coinSprite);
    for (int i = 0; i < BALLS; i++) {
        window->draw(obstacle[i]);
    }
    scoreText->setPosition(30, 15);
    scoreText->setFillColor(Color::Magenta);
    window->draw(*scoreText);
    scoreText->setPosition(WIDE - 300, 15);
    scoreText->setFillColor(Color::Cyan);
    window->draw(*scoreText);
    window->draw(*scoreInt1);
    window->draw(*scoreInt2);
    highScoreText->setPosition(30, 60);
    highScoreText->setFillColor(Color::Magenta);
    window->draw(*highScoreText);
    highScoreText->setPosition(WIDE-300, 60);
    highScoreText->setFillColor(Color::Cyan);
    window->draw(*highScoreText);
    window->draw(*highScoreInt1);
    window->draw(*highScoreInt2);
    solidText->setPosition(30, 105);
    solidText->setFillColor(Color::Magenta);
    window->draw(*solidText);
    solidText->setPosition(WIDE -300, 105);
    solidText->setFillColor(Color::Cyan);
    window->draw(*solidText);
    window->draw(*solidInt1);
    window->draw(*solidInt2);
    if (drawGameOver) {
        window->draw(*blastSprite);
        window->draw(*gameOver);
    }
    window->display();
}

void CheckCollisionWithBall(Sprite* carSprite1, Sprite* carSprite2, Sprite obstacle[], Sprite* coinSprite, bool* CollisionObstacle, Text* scoreInt1, Text* solidText, Text* solidInt1, Text* scoreText, Text* highScoreText, Text* highScoreInt1, Text* gameOver, Text* scoreInt2, Text* highScoreInt2, Text* solidInt2, RenderWindow* window, int *i, Sprite *blastSprite) {
    if (*i < BALLS) {
        if (obstacle[*i].getPosition().y >= HEIGHT - carHEIGHT && obstacle[*i].getPosition().y <= HEIGHT - obstacleDIAMETER) {
            if (carSprite1->getGlobalBounds().intersects(obstacle[*i].getGlobalBounds())){
                if (SCORE1 - 2 >= 0) {
                    SCORE1 -= 2;
                }
                if (SOLID1 - 1 > 0) {
                    SOLID1--;
                }
                else {
                    gameOver->setCharacterSize(120);
                    gameOver->move(-40, -10);
                    gameOver->setFillColor(Color::Magenta);
                    RefreshFileHighScores();
                    blastSprite->setPosition(carSprite1->getPosition().x, carSprite1->getPosition().y-carHEIGHT);
                    while (blastSprite->getScale().x < 1.6 && blastSprite->getScale().y < 1.6) {
                        blastSprite->scale(1.11, 1.09);
                        blastSprite->move(-20, -20);
                        DrawAll(carSprite1, carSprite2, obstacle, coinSprite, CollisionObstacle, scoreInt1, solidText, solidInt1, scoreText, highScoreText, highScoreInt1, gameOver, scoreInt2, highScoreInt2, solidInt2, window, true, blastSprite);
                        Sleep(100);
                    }
                    Sleep(1000);
                    exit(0);
                }
                DrawAll(carSprite1, carSprite2, obstacle, coinSprite, CollisionObstacle, scoreInt1, solidText, solidInt1, scoreText, highScoreText, highScoreInt1, gameOver, scoreInt2, highScoreInt2, solidInt2, window, false, blastSprite);
                *CollisionObstacle = false;
                obstacle[*i].setPosition(rand() % WIDE, rand() % 5);
            }
            else if (carSprite2->getGlobalBounds().intersects(obstacle[*i].getGlobalBounds())) {
                if (SCORE2 - 2 >= 0) {
                    SCORE2 -= 2;
                }
                if (SOLID2 - 1 > 0) {
                    SOLID2--;
                }
                else {
                    gameOver->setCharacterSize(120);
                    gameOver->move(-40, -10);
                    gameOver->setFillColor(Color::Cyan);
                    RefreshFileHighScores();
                    blastSprite->setPosition(carSprite2->getPosition().x, carSprite2->getPosition().y-carHEIGHT);
                    while (blastSprite->getScale().x < 1.6 && blastSprite->getScale().y < 1.6) {
                        blastSprite->scale(1.11, 1.09);
                        blastSprite->move(-20, -20);
                        DrawAll(carSprite1, carSprite2, obstacle, coinSprite, CollisionObstacle, scoreInt1, solidText, solidInt1, scoreText, highScoreText, highScoreInt1, gameOver, scoreInt2, highScoreInt2, solidInt2, window, true, blastSprite);
                        Sleep(100);
                    }
                    Sleep(1000);
                    exit(0);
                }
                DrawAll(carSprite1, carSprite2, obstacle, coinSprite, CollisionObstacle, scoreInt1, solidText, solidInt1, scoreText, highScoreText, highScoreInt1, gameOver, scoreInt2, highScoreInt2, solidInt2, window, false, blastSprite);
                *CollisionObstacle = false;
                obstacle[*i].setPosition(rand() % WIDE, rand() % 5);
            }
        }
    }
    // обработчик столкновения монетки
    else {
        if (coinSprite->getPosition().y >= HEIGHT - carHEIGHT && coinSprite->getPosition().y <= HEIGHT - coinDIAMETER) {
            if (carSprite1->getGlobalBounds().intersects(coinSprite->getGlobalBounds())) {
                SCORE += 5;
                if (SCORE1 >= highSCORE1) {
                    highSCORE1 = SCORE1;
                    highScoreInt1->setString(to_string(highSCORE1));
                    RefreshFileHighScores();
                }
                DrawAll(carSprite1, carSprite2, obstacle, coinSprite, CollisionObstacle, scoreInt1, solidText, solidInt1, scoreText, highScoreText, highScoreInt1, gameOver, scoreInt2, highScoreInt2, solidInt2, window, false, blastSprite);
                *CollisionObstacle = false;
                coinSprite->setPosition(rand() % WIDE, 0);
            }
            else if (carSprite2->getGlobalBounds().intersects(coinSprite->getGlobalBounds())) {
                if (SCORE2 >= highSCORE2) {
                    highSCORE2 = SCORE2;
                    highScoreInt2->setString(to_string(highSCORE2));
                    RefreshFileHighScores();
                }
                DrawAll(carSprite1, carSprite2, obstacle, coinSprite, CollisionObstacle, scoreInt1, solidText, solidInt1, scoreText, highScoreText, highScoreInt1, gameOver, scoreInt2, highScoreInt2, solidInt2, window, false, blastSprite);
                *CollisionObstacle = false;
                coinSprite->setPosition(rand() % WIDE, 0);
            } 
        }
    }
}

void CheckCollisionOutOfWindow(Sprite obstacle[], int* i, Sprite* coinSprite, int* randomX, int* previosrandomX, int* random) {
    // обработчик выхода за окно
    if (obstacle[*i].getPosition().y > HEIGHT + 20 || coinSprite->getPosition().y > HEIGHT + 20) {
        *randomX = rand() % WIDE;

        // создание уникального рандома
        for (int o = *previosrandomX - 2; o < *previosrandomX + 2; o++) {
            for (int u = 0; u < BALLS; u++) {
                while (o == *randomX || o == obstacle[u].getPosition().y)
                {
                    *randomX = NULL;
                    *randomX = rand() % WIDE;
                    if (o == *randomX || o == obstacle[u].getPosition().y) {
                        break;
                    }
                }
            }
        }
        // респавн препядствия
        if (*i < BALLS) {
            obstacle[*i].setPosition(*randomX - obstacleDIAMETER, 0);
            *random = rand() % 4;
        }
        // респавн монетки
        else if (*i == BALLS) {
            coinSprite->setPosition(*randomX - coinDIAMETER, 0);
        }
        *previosrandomX = *randomX;
    }
}

void CheckSpecialKeys(bool *hardcore, bool *godmode, Text *scoreInt1, Text *highScoreInt1, Text *scoreInt2, Text *highScoreInt2) {
    // обработчик специальных клавиш

    // режим хардкор
    if (Keyboard::isKeyPressed(Keyboard::H) && hardcore) {
        obstacleSPEED *= 3;
        *hardcore = false;
    }
    // режим бога
    if (Keyboard::isKeyPressed(Keyboard::G) && godmode) {
        obstacleSPEED /= 1.5;
        *godmode = false;
    }
    // обнуление счёта
    if (Keyboard::isKeyPressed(Keyboard::R)) {
        SCORE1 = 0;
        SCORE2 = 0;
    }
    // обнуление рекордного счёта и счёта
    if (Keyboard::isKeyPressed(Keyboard::R) && (Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))) {
        SCORE1 = 0;
        scoreInt1->setString(to_string(SCORE1));
        SCORE2 = 0;
        scoreInt2->setString(to_string(SCORE2));
        highSCORE1 = 0;
        highScoreInt1->setString(to_string(highSCORE1));
        highSCORE2 = 0;
        highScoreInt2->setString(to_string(highSCORE2));

        RefreshFileHighScores();
    }
}

int main()
{
    srand(time(0));

    // нужные переменные для работы логики (перемные с заданным рандомом и переменные флаги)
    int random;
    int randomX;
    int previosrandomX = 0;
    bool CollisionObstacle = true;
    bool hardcore = true;
    bool godmode = true;
    bool keyboardORmouse1 = true;
    bool keyboardORmouse2 = false;
    bool whoKickWho = true;
    //-----------------

    // секундомер для добавочного счёта
    Clock clock;
    clock.restart();
    int timePrevios = clock.getElapsedTime().asSeconds();
    int timeCurrent = timePrevios;
    //------------

    //1

    // обновления рекордного счёта 1 и рекордного счёта 2 с прошлой игры
    RefreshVariableFromFile();

    // создание окна
    RenderWindow window(VideoMode(WIDE, HEIGHT), "LandSpace", Style::Close, ContextSettings(2, 6, 6));
    window.clear(Color(105, 105, 105));
    //----------

    // подгрузка взрыва машинки при 0 прочности
    Image blastImage;
    Texture blastTexture;
    Sprite blastSprite;
    blastImage.loadFromFile("blast.png");
    blastTexture.loadFromImage(blastImage);
    blastSprite.setTexture(blastTexture);
    blastSprite.setPosition(WIDE / 2, HEIGHT/2);
    blastSprite.setScale(0.2, 0.2);

    // создание машинки1
    Image carImage1;
    Texture carTexture1;
    Sprite carSprite1;
    carImage1.loadFromFile("carPicture1.png");
    carTexture1.loadFromImage(carImage1);
    carSprite1.setTexture(carTexture1);
    carSprite1.setPosition(WIDE / 3 - carWIDE / 2, 632);
    //-----------------

    // создание машинки2
    Image carImage2;
    Texture carTexture2;
    Sprite carSprite2;
    carImage2.loadFromFile("carPicture2.png");
    carTexture2.loadFromImage(carImage2);
    carSprite2.setTexture(carTexture2);
    carSprite2.setPosition(WIDE / 1.5 - carWIDE / 2, 630);
    //-----------------

    // создание монетки
    Image coinImage;
    Texture coinTexture;
    Sprite coinSprite;
    coinImage.loadFromFile("coin.png");
    coinTexture.loadFromImage(coinImage);
    coinSprite.setTexture(coinTexture);
    //---------------

    // создание препядствий
    Image obstacleImage;
    Texture obstacleTexture;
    obstacleImage.loadFromFile(fileNameObstacle);
    obstacleTexture.loadFromImage(obstacleImage);
    // создание BALLS количество препядствий
    Sprite obstacle[BALLS];
    for (int i = 0; i < BALLS; i++) {
        obstacle[i] = Sprite();
        obstacle[i].setTexture(obstacleTexture);
    }
    //---------------

    // создание текста с GAME OVER
    Font font;
    font.loadFromFile("font.otf");
    Text gameOver;
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(90);
    gameOver.setFont(font);
    gameOver.setFillColor(Color::Red);
    gameOver.setPosition(WIDE / 3.6, HEIGHT / 3);
    //--------------

    // создание текста с Score
    Text scoreText;
    scoreText.setString("Score: ");
    scoreText.setCharacterSize(34);
    scoreText.setFont(font);
    scoreText.setFillColor(Color::Magenta);
    scoreText.setPosition(30, 15);
    //--------------

    // создание текста со значением счёта 1 (Score)
    Text scoreInt1;
    scoreInt1.setString(to_string(SCORE1));
    scoreInt1.setCharacterSize(34);
    scoreInt1.setFont(font);
    scoreInt1.setFillColor(Color::Magenta);
    scoreInt1.setPosition(110, 15);
    //--------------

    // создание текста со значением счёта 2 (Score)
    Text scoreInt2;
    scoreInt2.setString(to_string(SCORE2));
    scoreInt2.setCharacterSize(34);
    scoreInt2.setFont(font);
    scoreInt2.setFillColor(Color::Cyan);
    scoreInt2.setPosition(WIDE - 200, 15);
    //--------------

    // создание текста с highScore
    Text highScoreText;
    highScoreText.setString("High Score: ");
    highScoreText.setCharacterSize(34);
    highScoreText.setFont(font);
    highScoreText.setFillColor(Color::Magenta);
    highScoreText.setPosition(30, 60);
    //--------------

    // создание текста со значением счёта 1 (highScore)
    Text highScoreInt1;
    highScoreInt1.setString(to_string(highSCORE1));
    highScoreInt1.setCharacterSize(34);
    highScoreInt1.setFont(font);
    highScoreInt1.setFillColor(Color::Magenta);
    highScoreInt1.setPosition(170, 60);
    //--------------

    // создание текста со значением счёта 2 (highScore)
    Text highScoreInt2;
    highScoreInt2.setString(to_string(highSCORE2));
    highScoreInt2.setCharacterSize(34);
    highScoreInt2.setFont(font);
    highScoreInt2.setFillColor(Color::Cyan);
    highScoreInt2.setPosition(WIDE - 150, 60);
    //--------------

    // создание текста с solid
    Text solidText;
    solidText.setString("Solid: ");
    solidText.setCharacterSize(34);
    solidText.setFont(font);
    solidText.setFillColor(Color::Magenta);
    solidText.setPosition(30, 105);
    //--------------

    // создание текста со значением прочности 1
    Text solidInt1;
    solidInt1.setString(to_string(SOLID1));
    solidInt1.setCharacterSize(34);
    solidInt1.setFont(font);
    solidInt1.setFillColor(Color::Magenta);
    solidInt1.setPosition(110, 105);
    //--------------

    // создание текста со значением прочности 2
    Text solidInt2;
    solidInt2.setString(to_string(SOLID2));
    solidInt2.setCharacterSize(34);
    solidInt2.setFont(font);
    solidInt2.setFillColor(Color::Cyan);
    solidInt2.setPosition(WIDE - 200, 105);
    //--------------

    //игра
    while (window.isOpen() && !Keyboard::isKeyPressed(Keyboard::Q)) { // завершение игры по нажатию кнопки Q
        srand(time(0));
        Event event;
        if (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        //цикл от 0 до количество BALLS + количество монеток (1)
        for (int i = 0; i < BALLS + 1; i++) {
            // обработчик столкновения препядствия
            CheckCollisionWithBall(&carSprite1, &carSprite2, obstacle, &coinSprite, &CollisionObstacle, &scoreInt1, &solidText, &solidInt1, &scoreText, &highScoreText, &highScoreInt1, &gameOver, &scoreInt2, &highScoreInt2, &solidInt2, &window, &i, &blastSprite);
            CollisionObstacle = true;
            scoreInt1.setString(to_string(SCORE1));
            scoreInt2.setString(to_string(SCORE2));

            // обработчик выхода за границу окна
            CheckCollisionOutOfWindow(obstacle, &i, &coinSprite, &randomX, &previosrandomX, &random);

            // обработчик движения препядствия
            if (i < BALLS) {
                obstacle[i].move(0, obstacleSPEED + ((float)SCORE / 1000) + (rand() % 10) / 5);
                obstacle[i].scale(obstacle[i].getScale().x+(rand() % 1 - 2) / 10, obstacle[i].getScale().y + (rand() % 1 - 2) / 10);
                if ((0 + rand() % 4) - 1 == i) {
                    obstacle[i].move((rand() % 10) / 10, SlowedSPEED);
                }
            }
            // обработчик движения монетки
            else if (i == BALLS) {
                coinSprite.move(0, obstacleSPEED);
            }
        }

        // машинка
        // обработчик выхода машинки за границу окна
        CheckCarCollisionWithFrame(&carSprite1);
        CheckCarCollisionWithFrame(&carSprite2);
        // управление машинкой и анимация поворота
        if (CheckCollisionWithCar(&carSprite1, &carSprite2, true) && whoKickWho) {
            MoveCar(&keyboardORmouse1, &carSprite1, &window);
            whoKickWho = false;
        }
        if (CheckCollisionWithCar(&carSprite2, &carSprite1, false) && !whoKickWho) {
            MoveCar(&keyboardORmouse2, &carSprite2, &window);
            whoKickWho = true;
        }
        else {
            MoveCar(&keyboardORmouse1, &carSprite1, &window);
            MoveCar(&keyboardORmouse2, &carSprite2, &window);
        }

        // обработчик специальных клавиш
        CheckSpecialKeys(&hardcore, &godmode, &scoreInt1, &highScoreInt1, &scoreInt2, &highScoreInt2);

        // постановление игры на паузу (неработатет)
        //--

        //отрисовка всего этого добра
        DrawAll(&carSprite1, &carSprite2, obstacle, &coinSprite, &CollisionObstacle, &scoreInt1, &solidText, &solidInt1, &scoreText, &highScoreText, &highScoreInt1, &gameOver, &scoreInt2, &highScoreInt2, &solidInt2, &window, false, &blastSprite);

        //clock.restart();
        timeCurrent = clock.getElapsedTime().asSeconds();
        //обработчик изменения секундомерa
        if (timePrevios != timeCurrent) {
            SCORE1++;
            SCORE2++;
            scoreInt1.setString(to_string(SCORE1));
            scoreInt2.setString(to_string(SCORE2));
        }

        // перезапись рекордного счёта
        if (SCORE1 > highSCORE1) {
            highSCORE1 = SCORE1;
            highScoreInt1.setString(to_string(highSCORE1));
            RefreshFileHighScores();
        }
        if (SCORE2 > highSCORE2) {
            highSCORE2 = SCORE2;
            highScoreInt2.setString(to_string(highSCORE2));
            RefreshFileHighScores();
        }

        timePrevios = timeCurrent;
    }
    //процедуры после конца игры
    window.clear();
    window.close();

    RefreshFileHighScores();

    std::cout << "Good Game!" << std::endl;

    return 0;
    // конец.
}