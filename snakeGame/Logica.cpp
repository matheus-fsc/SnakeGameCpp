#include "header.h"

int* Game::pontuacoes = nullptr; // Defini��o da vari�vel est�tica

Game::Game() {
    score = 0;
    int l = 0;
    int a = 0;
    std::cout << "Digite a largura do mapa: ";
    std::cin >> l;
    std::cout << "Digite a altura do mapa: ";
    std::cin >> a;

    srand(time(0)); // Inicializa o gerador de n�meros aleat�rios

    Map* mapa = new Map(l, a);
    mapa->draw();

    while (!mapa->getGameStatus()) {
        clearScreen();
        mapa->input();
        mapa->logic();
        mapa->draw();
        Sleep(200);
    }
    delete mapa;
}

void Game::clearScreen() {
    system("cls");
}

Game::~Game() {}

Snake::Snake(Map& m) {
    cobraOlhaoMapa = &m;
    x = 0;
    y = 0;
    nTail = 0;
    direction = 0;
    lastDirection = 0;
    tailX = new int[cobraOlhaoMapa->getAltura() - 2];
    tailY = new int[cobraOlhaoMapa->getLargura() - 2];

    for (int i = 0; i < cobraOlhaoMapa->getLargura() - 2; ++i)
        tailX[i] = 0;
    for (int i = 0; i < cobraOlhaoMapa->getAltura() - 2; ++i)
        tailY[i] = 0;
}

void Snake::move() {
    lastDirection = direction;

    // Armazena a posi��o anterior
    int prevX = x;
    int prevY = y;
    int prev2X, prev2Y;

    // Move a cabe�a da cobra
    switch (direction) {
    case 0: // Esquerda
        y--;
        break;
    case 1: // Direita
        y++;
        break;
    case 2: // Cima
        x--;
        break;
    case 3: // Baixo
        x++;
        break;
    }

    // Move o corpo da cobra
    for (int i = 0; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
}

void Snake::collision() {
    // Verifica se a cobra colidiu com a parede
    if (x < 1 || x >= cobraOlhaoMapa->getLargura() - 1 ||
        y < 1 || y >= cobraOlhaoMapa->getAltura() - 1) {
        cobraOlhaoMapa->setGameStatus(true);
    }
    // Verifica se a cobra colidiu com o pr�prio corpo
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            std::cout << "Game Over!" << std::endl;
            exit(0);
        }
    }
}

void Snake::eat() {
    // Verifica se a cobra comeu a fruta
    if (cobraOlhaoMapa->getMap()[x][y] == 2) {
        nTail++; // Aumenta o tamanho da cobra
        cobraOlhaoMapa->generateFruit(); // Gera uma nova fruta
    }
}

Snake::~Snake() {
    delete[] tailX;
    delete[] tailY;
}

Map::Map(int& largura, int& altura) {
    this->largura = largura >= 4 ? largura : 4;
    this->altura = altura >= 4 ? altura : 4;
    cobra = new Snake(*this);
    map = new int* [this->largura];
    for (int i = 0; i < this->largura; i++) {
        map[i] = new int[this->altura];
    }
    gameOver = false;
    clean();
    setMeio();
    initSnake();
    generateFruit(); // Gera a fruta no in�cio do jogo
}

int** Map::getMap() {
    return map;
}

void Map::logic() {
    cobra->move();
    cobra->collision();
    cobra->eat();
}

void Map::initSnake() {
    cobra->setPosX(meio[0]);
    cobra->setPosY(meio[1]);
    map[cobra->getPosX()][cobra->getPosY()] = 1; // Marca a posi��o da cabe�a da cobra
}

void Map::clean() {
    for (int i = 0; i < largura; i++) {
        for (int j = 0; j < altura; j++) {
            map[i][j] = 0; // Limpa o mapa 
        }
    }
}

void Map::draw() {
    clean(); // Limpa o mapa antes de desenhar
    map[cobra->getPosX()][cobra->getPosY()] = 1; // Marca a posi��o da cabe�a da cobra

    // Marca a posi��o do corpo da cobra
    for (int i = 0; i < cobra->getNTail(); i++) {
        map[cobra->getTailX(i)][cobra->getTailY(i)] = 3; // 3 representa o corpo da cobra
    }


    map[fruitX][fruitY] = 2; // Marca a posi��o da fruta

    for (int i = 0; i < largura; i++) {
        for (int j = 0; j < altura; j++) {
            if (i == 0 || i == largura - 1 || j == 0 || j == altura - 1) {
                std::cout << "#"; // Desenha as paredes
            }
            else if (map[i][j] == 1) {
                std::cout << "O"; // Desenha a cabe�a da cobra
            }
            else if (map[i][j] == 2) {
                std::cout << "*"; // Desenha a fruta
            }
            else if (map[i][j] == 3) {
                std::cout << "o"; // Desenha o corpo da cobra
            }
            else {
                std::cout << " "; // Espa�o vazio
            }
        }
        std::cout << std::endl;
    }
}

void Map::input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            cobra->setDirection(0); // Esquerda
            break;
        case 'd':
            cobra->setDirection(1); // Direita
            break;
        case 'w':
            cobra->setDirection(2); // Cima
            break;
        case 's':
            cobra->setDirection(3); // Baixo
            break;
        }
    }
}

void Map::setMeio() {
    meio[0] = largura / 2;
    meio[1] = altura / 2;
}

void Map::generateFruit() {
    do {
        fruitX = rand() % (largura - 2) + 1; // Gera posi��es aleat�rias para a fruta
        fruitY = rand() % (altura - 2) + 1;
    } while (map[fruitX][fruitY] != 0); // Garante que a fruta n�o apare�a em cima da cobra
    map[fruitX][fruitY] = 2; // Marca a posi��o da fruta
}

Map::~Map() {
    for (int i = 0; i < largura; i++)
        delete[] map[i];
    delete[] map;
    delete cobra;
}
