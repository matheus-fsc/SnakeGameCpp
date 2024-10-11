#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib> 
#include <ctime>   

class Game {
    static int* pontuacoes;
    int score;
public:
    Game();
    void clearScreen();
    ~Game();
};

class Map;

class Snake {
private:
    int x;
    int y;
    int* tailX;
    int* tailY;
    int nTail;
    int direction;
    int lastDirection;
    Map* cobraOlhaoMapa;

public:
    Snake(Map&);
    Map* getCobraOlhaoMapa() { return cobraOlhaoMapa; }
    void setPosX(int x) { this->x = x; }
    void setPosY(int y) { this->y = y; }
    int getPosX() { return x; }
    int getPosY() { return y; }
    int getNTail() { return nTail; }
    int getTailX(int i) { return tailX[i]; }
    int getTailY(int i) { return tailY[i]; }
    void setDirection(int direction) { this->direction = direction; }
    int getDirection() { return direction; }
    void move();
    void collision();
    void eat();
    ~Snake();
};

class Map {
private:
    int largura;
    int altura;
    int** map;
    int fruitX;
    int fruitY;
    int direction;
    int meio[2];
    Snake* cobra;
    bool gameOver;

public:
    Map(int&, int&);
    void setGameStatus(bool gameOver) { this->gameOver = gameOver; }
    bool getGameStatus() { return gameOver; }
    int getAltura() { return altura; }
    int getLargura() { return largura; }
    int getDirection() { return direction; }
    int* getMeio() { return meio; }
    void setPos(int& x, int& y, int value) { map[x][y] = value; }
    Map pointerToMapObj() { return *this; }
    int** getMap();
    void draw();
    void clean();
    void input();
    void logic();
    void setMeio();
    void initSnake();
    void generateFruit();
    ~Map();
};

#endif // !GAME_H
