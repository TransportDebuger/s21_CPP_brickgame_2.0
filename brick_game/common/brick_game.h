/*!
  \file
  \author provemet
  \version 2
  \date Декабрь 2024
  \brief Общие определения для реализации библиотек семейства brick games
  \details Файл содержит описания макросов, структур данных, описание функций используемых при реализации игр семейства brick game.
*/

#include <stdlib.h>
#include <stdbool.h>

#ifndef BRICK_GAME_H
#define BRICK_GAME_H

/*!
  \brief Максимальная высота доступного игрового поля.
  \details Макрос определяет максимальную высоту доступного игрового поля в игре. По умолчанию определено значение в 20 ячеек.
*/
#define MAX_HEIGNT 20

/*!
  \brief Максимальная ширина доступного игрового поля.
  \details Макрос определяет высоту доступного игрового поля в игре. По умолчанию определено значение в 10 ячеек.
*/
#define MAX_WIDTH 10

#define MIN_GAMEBLOCK_SIZE 1

/*!
  \brief Перечисление значения флага состояния хранения адреса локатора
*/
enum is_set { nset, set };

/*!
  \brief Перечисление действий пользователя.
  \details Перечисление кодов вызываемых действий пользователя.
*/
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

/*!
  \brief Структура состояния игры на игровом этапе (такте).
  \details Структура предназначена для хранения текущего состояния игрового процесса. 
*/
typedef struct GameInfo_t {
  int** field;  ///< Указатель на матрицу текущего состояний клеток (пикселей) поля в игре.
  int** next;   ///< Указатель на матрицу фигуры выпадающей следующей.
  int score;  ///< Значение полученных игроком очков во время игрвого процесса.
  int high_score;  ///< Значение лучшего результата (количество очков) полученных игроками ранее во время игрового процесса
  int level;  ///< Значение текущего уровня (модификатора скорости) игрового процесса.
  int speed;  ///< Целочисленное значение скорости игрового процесса.
  int pause;  ///< Целочисленное значение индикации паузы в игре.

} GameInfo_t;

/*!
  \brief Структура локатора адреса хранения структуры GameInfo_t 
  \details Структура предназначена для хранения текущего состояния игрового процесса. 
*/
typedef struct GameInfoLocator_t {
  GameInfo_t *gameinfo; ///< Указатель на адрес хранения структуры GameInfo_t
  char setval; ///< Состояние локатора {nset, set}
} GameInfoLocator_t;

/*!
  \brief Структура описания игровых блоков
  \details Структура предназначена для хранения сведений об игровом блоке (Тетрамино или пиксель для игр Tetris и Snake соответственно).
*/
typedef struct GameBlock_t {
  int* field; ///< Указатель на матрицу игровой структуры
  int size; ///< Размер сторон матрицы (квадратная матрица)
  int posX; ///< Координаты левого верхнего угла матрицы игрового блока по горизотали
  int posY; ///< Координаты левого верхнего угла матрицы игрвого блока по вертикали
} GameBlock_t;

/*!
  \defgroup Data_structure_management Функции управления структурами данных
  \brief Функции создают и уничтожают структуры
*/
GameInfo_t* createGameInfo();
void destroyGameInfo(GameInfo_t *gameinfo);
int** createMatrix(int rows, int cols);
bool removeMatrix(int rows, int cols, int** matrix);

/*!
  \defgroup Address_providers Функции поставщиков адресов.
  \brief Функции предназначенные для предоставления адресов структур данных.
*/
GameInfo_t* locateGameInfo(GameInfo_t *gameinfo);

GameInfo_t updateCurrentState();

void userInput(UserAction_t action, bool hold);

#endif