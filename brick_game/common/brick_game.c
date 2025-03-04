/*!
  \file
  \author provemet
  \version 2
  \date Декабрь 2024
  \brief Общие функции реализации группы игр Brick Games
  \details Файл содержит реализацию функций управления общими структурами и хранимыми ими данными группы игр Brick Games.
*/

#include "brick_game.h"

/*!
  \ingroup Data_structure_management Функции управления структурами данных
  \brief Функция (констурктор) создания структуры GameInfo_t.
  \return Указатель на область памяти, хранящую стркутру игровых данных GameInfo_t

  \details Функция предназначена для выделения области памяти для хранения структуры GameInfo_t.
  Функция возвращает указатель на выделенную область памяти.
  В случае ошибки выделения памяти функция возвращает NULL.
*/
GameInfo_t *createGameInfo() {
    GameInfo_t *gameinfo = NULL;

    if ((gameinfo = malloc(sizeof(GameInfo_t))) != NULL) {
        if ((gameinfo->field = createMatrix(MAX_HEIGNT, MAX_WIDTH)) == NULL) {
            free(gameinfo);
            gameinfo = NULL;
        } else {
            gameinfo->high_score = 0;
            gameinfo->level = 0;
            gameinfo->pause = 0;
            gameinfo->score = 0;
            gameinfo->speed = 0;
            gameinfo->next = NULL;
            locateGameInfo(gameinfo);
        }
    }
    
    return gameinfo;
}

/*!
  \ingroup Data_structure_management Функции управления структурами данных
  \brief Функция (деструктор) очищения памяти, занимаемой структурой GameInfo_t.
  \param [in] gameinfo Указатель на область памяти, занимаемой структурой
  GameInfo_t.

  \details Функция предназначена для освобождения области памяти выделенной для хранения
  структуры GameInfo_t.
*/
void destroyGameInfo(GameInfo_t *gameinfo) {
    if (gameinfo) {
        if (gameinfo->field) {
            removeMatrix(MAX_HEIGNT, MAX_WIDTH, gameinfo->field);
            gameinfo->field = NULL;
        }
        // Добавить проверку удаления next и в случае его наличия удалить.
        locateGameInfo(gameinfo);
        free(gameinfo);
        gameinfo = NULL;
    }
}

/*!
  \ingroup Address_providers Функции поставщиков адресов.
  \brief Функция локатор выделенных ресурсов игровой структуре GameInfo_t.
  \param [in] game Указатель на адрес структуры GameInfo_t.
  \return Указатель на область памяти, хранящую стркутру игровых данных GameInfo_t

  \details Функции устанавливают и возвращают указатели на структуры игровых данных. 
  Если адрес локатору не не передавался адрес структуры данных GameInfo_t и таковой не хранится в локаторе (адрес равен NULL), то осуществляется сохранение адреса в структуре локатора, состояние локатора меняется на 'установлен', функция возвращает указатель на переданный адрес. 
  Если передан NULL или указатель отличный от хранимого локатором, при этом локатор находится в состянии 'установлен', то функция возвращает значение ранее сохранненого адреса. 
  При передаче адреса игровой структуры GameInfo_t локатору соответствующему хранимому локатором, состояние локатора изменяется на 'не задан', адрес приводится в состояние NULL. Функция возвращает значение NULL;
*/
GameInfo_t *locateGameInfo(GameInfo_t *gameinfo) {
    static GameInfoLocator_t locator = {0};

    if (gameinfo == locator.gameinfo && locator.setval) {
        locator.gameinfo = NULL;
        locator.setval = nset;
    } else if (gameinfo != locator.gameinfo && !locator.setval) {
        locator.gameinfo = gameinfo;
        locator.setval = set;
    }

    return locator.gameinfo;
}

int** createMatrix(int rows, int cols) {
    int** matrix = NULL;
    int* values = NULL;
    if (rows > 0 && rows <= MAX_HEIGNT && cols > 0 && cols <= MAX_WIDTH ) {
        if ((matrix = (int **)malloc(sizeof(int*) * rows)) != NULL && 
            (values = (int *)malloc(sizeof(int) * rows * cols)) != NULL) {
            
            for (int i = 0; i < rows; i++) {
                *(matrix + i) = values + (i * cols);
            }
        }
    }

    return matrix;
}

bool removeMatrix(int rows, int cols, int** matrix) {
    // Preprocessing of internal parameters
    if (!matrix && (rows > 0 && rows <= MAX_HEIGNT && cols > 0 && cols <= MAX_WIDTH )) return false;

    free(*matrix);
    free(matrix);
    return true;
}

GameInfo_t updateCurrentState() {
    GameInfo_t gameinfo = {0};

    gameinfo.field = updateMatrix();
    gameinfo.high_score = 0;
    gameinfo.score = updateScore();
    gameinfo.level = getCurrentLevel();
    gameinfo.next = getNextFigure();
    gameinfo.pause = getPauseState();
    gameinfo.speed = getGameSpeed();

    return gameinfo;
}