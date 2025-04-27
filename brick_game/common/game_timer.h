/**
 * @file game_timer.h
 * @author provemet
 * @version 2
 * @date Декабрь 2024
 * 
 * @brief Реализация таймера BrickGame (Tetris, Snake)
 *
 * @details Этот модуль реализует общие для игр BrickGame (Tetris, Snake)
 * основные компоненты и методы.
 */

#ifndef GAME_TIMER_H
#define GAME_TIMER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t last_update;  
    uint64_t interval;     
    bool paused;           
} GameTimer_t;

/**
 * @brief Инициализирует таймер с заданным интервалом.
 * @param timer Указатель на структуру таймера.
 * @param interval Интервал срабатывания в миллисекундах.
 */
void initTimer(GameTimer_t* timer, uint64_t interval);

/**
 * @brief Проверяет, истек ли интервал таймера.
 * @param timer Указатель на структуру таймера.
 * @return true, если интервал истек, иначе false.
 */
bool checkTimer(GameTimer_t* timer);

/**
 * @brief Приостанавливает или возобновляет работу таймера.
 * @param timer Указатель на структуру таймера.
 * @param state true для приостановки, false для возобновления.
 */
void pauseTimer(GameTimer_t* timer, bool state);

#endif