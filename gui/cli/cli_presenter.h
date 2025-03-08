/*!
  \file cli_presenter.h
  \author provemet
  \version 2
  \date Февраль 2025
  \brief Заголовочный файл реализации консольного представления серии игр
  BrickGames

  \details Данный файл содержит определения макросов, структур данных, функций
  необходимых для реализации консольного интерфейса игр BrickGames.
*/

#ifndef CLI_PRESENTER_H
#define CLI_PRESENTER_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_OK 0
#define ERROR_FAULT 1

#define TIMEOUT 10
#define SCORE_LABEL "SCORE"
#define NEXT_LABLE "NEXT"
#define TOPSCORE_LABEL "TOP SCORE"
#define LEVEL_LABEL "LEVEL"
#define SPEED_LABEL "SPEED"
#define HINTS_LABEL "HINTS"
#define GAME_STATUS_LABEL "GAME STATUS"

#define MIN_COORDINATE_VALUE 1;
#define MAX_COORDINATE_VALUE 42;
#define MIN_ELEMENT_DIM 2;

#ifdef __cplusplus
extern "C" {
#endif

/*!
    \brief Структура представления элемента интерфейса.
    \details Данныя структура предназначена для хранения размерности, координат
   расположения элемента интерфейса.
*/
typedef struct ConsoleElement_t {
  int top;  ///< Координата расположения по вертикали экрана левого верхнего
            ///< угла элемента.
  int left;  ///< Координата расположения по горизонтали экрана левого верхнего
             ///< угла элемента.
  int width;   ///< Горизонтальный размер элемента
  int height;  ///< Вертикальный размер элемента
  bool is_changes;  //Пока пусть будет.
  char* label;  ///< Строка надписи (метки) элемента
} ConsoleElement_t;

/*!
    \brief Структура хранения элементов (ConsoleElement_t) интерфейса.
    \details Данная структура предназначена для хранения массива элементов
   интерфейса представления.
*/
typedef struct ConsoleView_t {
  ConsoleElement_t* element;  ///< Массив элементов интерфейса.
  size_t size;  ///< Размерность массива элементов интерфейса.
} ConsoleView_t;

/*!
    \brief Структура локатора адреса представления
    \details Данная структура предназначена для храненения указателя на адрес
   хранения структуры ConsoleView_t
*/
typedef struct ViewLocator_t {
  ConsoleView_t*
      view;  ///< Указатель на адрес памяти хранящий структуру хранения
             ///< элементов графического интерфейса (ConsoleView_t).
  bool setval;  ///< Признак занятости адреса хранимой структурой.
} ViewLocator_t;

/*!
  \defgroup Data_structure_management Функции управления структурами данных
  \brief Функции предназначенные для создания и уничтожения структур данных.
*/
int initView(ConsoleView_t* view);
void freeView(ConsoleView_t* view);

/*!
  \defgroup Address_providers Функции поставщиков адресов.
  \brief Функции предназначенные для предоставления адресов структур данных.
*/
ConsoleView_t* locateView(ConsoleView_t* view);

/*!
    \defgroup Data_structure_manipulation Функции манипуляции данными структур
    \brief Функции предназначенные для управления данными, хранимых в
   структурах.
*/
int appendViewElement(ConsoleView_t* view, int top, int left, int height,
                      int width, char* label);
void deleteViewElement(ConsoleView_t* view, int index);

/*!
    \details Graphic_functions Функции визуализации интерфейса.
    \brief Функции предназначенные для визуализации интерфейса и его изменения
   на экране.
*/
void renderView(ConsoleView_t* view);
//Добавить рендеринг обновленного состояния элемента

#ifdef __cplusplus
}
#endif

#endif