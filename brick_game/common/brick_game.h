/**
 * @file brick_game
 * @author provemet
 * @version 2
 * @date Декабрь 2024
 * @brief Реализация общих компонент модели игр BrickGame (Tetris, Snake)
 *
 * @details Этот модуль реализует общие для игр BrickGame (Tetris, Snake)
 * основные компоненты и методы.
 */

#include <stdbool.h>
#include <stdlib.h>

#include "fsm.h"

#ifndef BRICK_GAME_H
#define BRICK_GAME_H

/**
 * @def FIELD_HEIGHT
 * @brief Макрос, определяющий высоту игрового поля
 * @details Используется, для определения размерности матрицы игрового поля.
 * Высота поля задана значением 20.
 */
#define FIELD_HEIGHT 20

/**
 * @def FIELD_WIDTH
 * @brief Макрос, определяющий ширину игрового поля
 * @details Используется, для определения размерности матрицы игрового поля.
 * Ширина поля определена значением 10.
 */
#define FIELD_WIDTH 10

/**
 * @def MIN_GAMEBLOCK_SIZE
 * @brief Макрос, определяющий минимальный размер игрового блока
 * @details Макрос ограничивает минимальный размер игрового блока по одному
 * измерению. Значение взято из сопоставления размеров игровых блоков
 * (GameBlock_t) для игры Tetris (размерности Tetrinoes от 2 до 4) и Snake
 * (размерность 'еды' и элемента змейки - 1).
 */
#define MIN_GAMEBLOCK_SIZE 1

/**
 * @def MAX_GAMEBLOCK_SIZE
 * @brief Макрос, определяющий максимальный размер игрового блока
 * @details Макрос ограничивает максимальный размер игрового блока по одному
 * измерению. Значение взято из сопоставления размеров игровых блоков
 * (GameBlock_t) для игры Tetris (размерности Tetrinoes от 2 до 4) и Snake
 * (размерность 'еды' и элемента змейки - 1).
 */
#define MAX_GAMEBLOCK_SIZE 4

/**
 * @enum UserAction_t
 * @brief Перечисление кодов действий пользователя.
 * @details Перечисление кодов вызываемых действий пользователя.
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

/**
 * @enum StateID
 * @brief Перечисление состояний игры.
 * @details Перечислени состояний игры, которые определяют контекст,
 *          в которых обрабатываются триггеры FSM.
 */
typedef enum {
  STATE_IDLE,       ///< Ожидание начала игры
  STATE_START,      ///< Начало игры
  STATE_TERMINATE,  ///< Завершение работы
  STATE_SPAWN,      ///< Генерация новой фигуры
  STATE_MOVE_DOWN,  ///< Движение фигуры вниз
  STATE_MOVE_UP,    ///< Движение фигуры вверх
  STATE_MOVE_LEFT,  ///< Движение фигуры влево
  STATE_MOVE_RIGHT, ///< Движение фигуры вправо
  STATE_ROTATE,
  STATE_PAUSE,      ///< Пауза
  STATE_GAME_OVER,  ///< Конец игры
  NUM_STATES        ///< Количество состояний
} StateID;

/**
 * @enum TriggerID
 * @brief Перечисление триггеров (событий) иницирующих изменение состояния игры.
 * @details Триггеры (события).Абстрагируют действия пользователя и внутренние
 * события, превращая их в единые сигналы для FSM.
 */
typedef enum {
  TRIGGER_INIT,  ///< Триггер инициации начального состояния
  TRIGGER_START_GAME,  ///< Начало игры
  TRIGGER_TERMINATE,   ///< Заврешение приложения
  TRIGGER_SPAWN,       ///< Отрисовка фигуры
  TRIGGER_MOVE_DOWN,   ///< Движение вниз
  TRIGGER_MOVE_UP,     ///< Движение вверх
  TRIGGER_MOVE_LEFT,   ///< Движение влево
  TRIGGER_MOVE_RIGHT,  ///< Движение вправо
  TRIGGER_COLLISION,   ///< Столкновение
  TRIGGER_PAUSE,       ///< Пауза
  TRIGGER_RESUME,      ///< Возобновление
  TRIGGER_ROTATE,      ///< Поворот фигуры
  TRIGGER_FIX,         ///< Фиксация фигуры
  TRIGGER_GAME_OVER,   ///< Поражение
  TRIGGER_EXIT,        ///< Выход
  NUM_TRIGGERS         ///< Количество триггеров
} TriggerID;

/**
 * @enum gameBlockOrientation
 * @brief Перечисление направлений ориентации фигуры.
 * @details Перечисление направлений ориентации фигуры.
 */
typedef enum { 
  ToTop,     ///< Вверх
  ToRight,   ///< Направо
  ToBottom,  ///< Вниз
  ToLeft     ///< Влево
} gameBlockOrientation;

/**
 * @struct  GameInfo_t
 * @brief Структура состояния игры на игровом этапе (такте).
 * @details Структура предназначена для хранения текущего состояния игрового
 * процесса.
 */
typedef struct GameInfo_t {
  int** field;  ///< Указатель на матрицу текущего состояний клеток (пикселей)
                ///< поля в игре.
  int** next;   ///< Указатель на матрицу фигуры выпадающей следующей.
  int score;    ///< Значение полученных игроком очков во время игрвого процесса.
  int high_score;  ///< Значение лучшего результата (количество очков)
                   ///< полученных игроками ранее во время игрового процесса
  int level;    ///< Значение текущего уровня (модификатора скорости) игрового
                ///< процесса.
  int speed;    ///< Целочисленное значение скорости игрового процесса.
  int pause;    ///< Целочисленное значение индикации паузы в игре.

} GameInfo_t;

/**
 * @struct GameField_t
 * @brief Структура хранения состяния поля
 * @details Структура предназначена для хранения состояния поля.
 * 
 * @see GameInfo_t
 */
typedef struct GameField_t {
  int** filed; ///< Матрица поля
  int width;   ///< Ширина поля
  int height;  ///< Высота поля
} GameInfo_t;

/**
 * @struct GameBlock_t
 * @brief Структура описания игровых блоков
 * @details Структура предназначена для хранения сведений об игровом блоке
 * (Тетрамино или пиксель для игр Tetris и Snake соответственно).
 * 
 * @see gameBlockOrientation
 */
typedef struct GameBlock_t {
  int** field;  ///< Указатель на матрицу игровой структуры
  int size;     ///< Размер сторон матрицы (квадратная матрица)
  int posX;     ///< Координаты левого верхнего угла матрицы игрового блока по
                ///< горизотали
  int posY;     ///< Координаты левого верхнего угла матрицы игрвого блока по
                ///< вертикали
  gameBlockOrientation orientation;  ///< Ориентация игрового блока.
  GameBlock_t* nextBlock; ///< Указатель на следующий блок, для организации очереди.
} GameBlock_t;


/**
 * @struct GameBlockQueue_t
 * @brief Структура организации очереди игровых блоков.
 * @details Структура предназначена для организации очереди игровых блоков. Например для тетерис это текущая и следующая фигура.
 * Для Snake это цепочка одиночных фрагментов змейки.
 */
typedef struct GameBlockQueue_t {
  GameBlock_t* first;  ///< Ссылка на первый элемент списка
  GameBlock_t* last;   ///< Ссылка на последний элемент списка
  int size;            ///< Размер списка
} GameBlockQueue_t;

/**
 * @defgroup Data_structure_management Функции управления структурами данных
 * @brief Функции создают и уничтожают структуры
 */

 /**
  * @ingroup Data_structure_management Функции управления структурами данных
  * @brief Функция (констурктор) создания структуры GameInfo_t.
  * @return Указатель на область памяти, хранящую стркутру игровых данных
  * GameInfo_t
  * 
  * @details Функция предназначена для выделения области памяти для хранения
  * структуры GameInfo_t. Функция возвращает указатель на выделенную область
  * памяти. В случае ошибки выделения памяти функция возвращает NULL.
*/
GameInfo_t* createGameInfo();

/**
 * @ingroup Data_structure_management Функции управления структурами данных
 * @brief Функция (деструктор) очищения памяти, занимаемой структурой GameInfo_t.
 * @param gameinfo Указатель на область памяти, занимаемой структурой
 * GameInfo_t.
 * 
 * @details Функция предназначена для освобождения области памяти выделенной для
 * хранения структуры GameInfo_t.
*/
void destroyGameInfo(GameInfo_t* gameinfo);


/**
 * @ingroup Data_structure_management Функции управления структурами данных
 * @brief Функция (конструктор) создания матрицы.
 * @param rows Количество строк в матрице.
 * @param cols Количество столбцов в матрице.
 * @return Указатель на матрицу.
 * @details Функция предназначена для освобождения области памяти выделенной для
 * хранения структуры GameInfo_t.
*/
int** createMatrix(int rows, int cols);
bool removeMatrix(int rows, int cols, int** matrix);

/**
 * @defgroup AddressProviders Поставщики адресов
 * @brief Структуры и методы для хранения указателей на ресурсы
 */

 /**
  * @ingroup AddressProviders
  * @brief Функция локатор выделенных ресурсов игровой структуре GameInfo_t.
  * @param game Указатель на адрес структуры (GameInfo_t*).
  * @return Указатель на область памяти, хранящую стркутру игровых данных
  * GameInfo_t
  * 
  * @details Функции устанавливают и возвращают указатели на структуры игровых
  * данных. Если адрес локатору не не передавался адрес структуры данных GameInfo_t
  * и таковой не хранится в локаторе (адрес равен NULL), то осуществляется
  * сохранение адреса в структуре локатора, состояние локатора меняется на
  * 'установлен', функция возвращает указатель на переданный адрес. Если передан
  * NULL или указатель отличный от хранимого локатором, при этом локатор находится
  * в состянии 'установлен', то функция возвращает значение ранее сохранненого
  * адреса. При передаче адреса игровой структуры GameInfo_t локатору
  * соответствующему хранимому локатором, состояние локатора изменяется на 'не
  * задан', адрес приводится в состояние NULL. Функция возвращает значение NULL;
  * 
  * @warning Не передавайте адрес вручную — используйте только
  * `createGameInfo`/`destroyGameInfo`.
*/
GameInfo_t* locateGameInfo(GameInfo_t* gameinfo);

/**
 * @brief Функция обновления текущего состояния игрового процесса
 * @return Возвращает вычесленное состяние игрового процесса на основе изменений
 * произошедших в модели.
 */
GameInfo_t updateCurrentState();

/**
 * @defgroup User_interaction_handlers Функции обработки действий пользователя
 * @brief Функции предназначенные для обработки действий пользователей
 * @param action Код действия пользователя (UserAction_t)
 * @param hold Признак удержания "зажатия" клавиши пользователем
 */
void userInput(UserAction_t action, bool hold);

/**
 * @defgroup BlockManipulationRoutines Функции манипуляции игровыми блоками
 * @brief Функции предназначенные для изменения 
 */

/** 
 * @brief Функция измениния ориентации блока на поле.
 */
int setGameBlockOrientation(GameBlock_t* gameblock,
  gameBlockOrientation orientation);

/**
 * @brief Функция получения информации о направлении (ориентации) игрового блока на поле.
 */
int getGameBlockOrientation(GameBlock_t* gameblock);

/**
 * @brief Функция создания очереди блоков.
 */
GameBlockQueue_t* createGameBlockQueue();

/**
 * @brief Функция ллокатора очереди блоков 
 */
GameBlockQueue_t* locateGameBlockQueue(GameBlockQueue_t* blockQueue);

/**
 * @brief Функция уничтожения очереди блоков
 */
void destroyGameBlockQueue(GameBlockQueue_t* blockQueue);

/**
 * @brief Функция вставки игрового блока в очередь
 */
int insertGameBlock(GameBlockQueue_t* blockQueue, GameBlock_t* gameBlock);

/**
 * @brief Функция очистки очереди блоков.
 */
int populateGameBlockQueue(GameBlockQueue_t* blockQueue);

//int popGameBlock(GameBlock_t* block);
//int pushBackGameBlock(GameBlockQueue_t* blockQueue, GameBlock_t* gameBlock);
//int popBackGameBlock(GameBlockQueue_t* blockQueue);

GameBlock_t *createGameBlock(const int posX, const int posY, const int size, const gameBlockOrientation orientation, const int** blockMatrix);
void deleteGameBlock(GameBlock_t *gameBlock);

#endif