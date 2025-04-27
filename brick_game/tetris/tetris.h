#ifndef TETRIS_H
#define TETRIS_H

#include "../common/brick_game.h"

#ifdef __cplusplus
extern "C" {
#endif

// Callbacks для FSM. TODO Доделать обработчики состояний.
void onIdleEnter(void* context);
void onIdleUpdate(void* context);
void onIdleExit(void* context);
void onStartEnter(void* context);
void onStartUpdate(void* context);
void onStartExit(void* context);
void onTerminateEnter(void* context);
void onSpawnEnter(void* context);
void onSpawnUpdate(void* context);
void onSpawnExit(void* context);
void onMoveDownEnter(void* context);
void onMoveDownUpdate(void* context);
void onMoveDownExit(void* context);
void onMoveUpEnter(void* context);
void onMoveUpUpdate(void* context);
void onMoveUpExit(void* context);
void onMoveLeftEnter(void* context);
void onMoveLeftUpdate(void* context);
void onMoveLeftExit(void* context);
void onMoveRightEnter(void* context);
void onMoveRightUpdate(void* context);
void onMoveRightExit(void* context);
void onRotateEnter(void* context);
void onRotateUpdate(void* context);
void onRotateExit(void* context);
void onPauseEnter(void* context);
void onPauseUpdate(void* context);
void onPauseExit(void* context);

// Матрица состояний Tetris. TODO Доделать матрицу
const FSMState gameStates[] = {
    // Состояние: Ожидание (IDLE)
    {
        .id = STATE_IDLE,
        .onEnter = onIdleEnter, // Ожидание начала пользовательских действий (начала игры)
        .onUpdate = onIdleUpdate,
        .onExit = onIdleExit,
        .transitions = (Transition[]){
            {TRIGGER_START_GAME, STATE_START, NULL}, // Старт игры
            {TRIGGER_TERMINATE, STATE_TERMINATE, NULL}  //Выход из приложения 
        },
        .numTransitions = 2
    },

    // Состояние: Старт (START)
    {
        .id = STATE_START,
        .onEnter = onStartEnter, // Инициализация игры
        .onUpdate = onStartUpdate,
        .onExit = onStartExit,
        .transitions = (Transition[]){
            {TRIGGER_SPAWN, STATE_SPAWN, NULL} // Переход к генерации фигуры
        },
        .numTransitions = 1
    },

    // Состояние: Завершение работы программы (TERMINATE)
    {
        .id = STATE_TERMINATE,
        .onEnter = onTerminateEnter, 
        .onUpdate = NULL,
        .onExit = NULL,
        .transitions = {},
        .numTransitions = 0
    },

    // Состояние: Генерация фигуры (SPAWN)
    {
        .id = STATE_SPAWN,
        .onEnter = onSpawnEnter, // Создать новую фигуру
        .onUpdate = onSpawnUpdate, // Проверить столкновение
        .onExit = onSpawnExit,
        .transitions = (Transition[]){
            {TRIGGER_MOVE_DOWN, STATE_MOVE_DOWN, NULL}, // Начать движение вниз
            {TRIGGER_MOVE_UP, STATE_MOVE_UP, NULL}, // Начать движение вниз
            {TRIGGER_MOVE_LEFT, STATE_MOVE_LEFT, NULL},
            {TRIGGER_MOVE_RIGHT, STATE_MOVE_RIGHT, NULL}
        },
        .numTransitions = 2
    },

    // Состояние: Движение (MOVE)
    {
        .id = STATE_MOVE_DOWN,
        .onEnter = onMoveDownEnter,
        .onUpdate = onMoveDownUpdate, // Движение вниз по таймеру
        .onExit = onMoveDownExit,
        .transitions = (Transition[]){
            {TRIGGER_ROTATE, STATE_ROTATE, NULL}, // Вращение
            {TRIGGER_MOVE_LEFT, STATE_MOVE_LEFT, onMoveLeft}, // Движение влево
            {TRIGGER_MOVE_RIGHT, STATE_MOVE_RIGHT, onMoveRight}, // Движение вправо
            {TRIGGER_MOVE_DOWN, STATE_MOVE_DOWN, NULL},
            {TRIGGER_MOVE_UP, STATE_MOVE_UP, NULL},
            {TRIGGER_PAUSE, STATE_PAUSE, NULL}, // Пауза
            {TRIGGER_COLLISION, STATE_SPAWN, onFixFigure} // Фиксация фигуры
        },
        .numTransitions = 5
    },

    // Состояние: Вращение (ROTATE)
    {
        .id = STATE_ROTATE,
        .onEnter = onRotateEnter, // Поворот фигуры
        .onUpdate = onRotateUpdate,
        .onExit = onRotateExit,
        .transitions = (Transition[]){
            {TRIGGER_MOVE_DOWN, STATE_ROTATE, NULL} // Возврат к движению
        },
        .numTransitions = 1
    },

    // Состояние: Пауза (PAUSE)
    {
        .id = STATE_PAUSE,
        .onEnter = onPauseEnter, // Показать меню паузы
        .onUpdate = onPauseUpdate,
        .onExit = onPauseExit,
        .transitions = (Transition[]){
            {TRIGGER_RESUME, STATE_MOVE, NULL}, // Продолжить игру
            {TRIGGER_EXIT, STATE_TERMINATE, NULL} // Выход
        },
        .numTransitions = 2
    },

    // Состояние: Конец игры (GAME_OVER)
    {
        .id = STATE_GAME_OVER,
        .onEnter = onIdleEnter, // Показать результат
        .onUpdate = NULL,
        .onExit = NULL,
        .transitions = (Transition[]){
            {TRIGGER_INIT, STATE_START, NULL}, // Рестарт
            {TRIGGER_EXIT, STATE_IDLE, NULL} // Выход
        },
        .numTransitions = 2
    }
};

#ifdef __cplusplus
}
#endif

#endif