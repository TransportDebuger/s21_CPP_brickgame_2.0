/**
 * @file fsm.h
 * @brief Реализация универасального конечного автомата (Finite State Machine,
 * FSM) для управления состояниями игр.
 * @author provemet
 * @version 2.0
 * @date Декабрь 2024
 *
 * @details Этот модуль реализует гибкий конечный автомат, который может быть
 * использован для управления состояниями в различных приложениях (например,
 * играх). Состояния и переходы между ними настраиваются через callback-функции.
 *
 * @warning Все callback-функции должны обрабатывать NULL-контекст, если это
 * возможно.
 */

#ifndef FSM_H
#define FSM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup Callbacks
 * @brief Тип callback-функции, вызываемой при обработке состояния.
 * @param context Указатель на пользовательские данные (например, GameInfo_t).
 * @warning При реализации этой функции требуется явное приведение типа
 * `context` к ожидаемому типу данных внутри функции.
 * @code
 * void onEnterHandler(void* context) {
 *     GameInfo_t* game = (GameInfo_t*)context;
 *     // Логика инициализации...
 * }
 * @endcode
 *
 * @warning Контекст может быть NULL, если не был задан при создании FSM.
 */
typedef void (*handlerCallback)(void* context);

/**
 * @struct Transition
 * @brief Структура, описывающая переход между состояниями.
 *
 * @details Содержит триггер (событие), целевое состояние и callback, вызываемый
 * при переходе.
 */
typedef struct Transition {
  int trigger;  ///< Событие, вызывающее переход (например, действие
                ///< пользователя).
  int targetState;  ///< Идентификатор целевого состояния.
  handlerCallback onEnter;  ///< Callback, вызываемый при активации перехода
                            ///< (может быть NULL).
} Transition;

/**
 * @struct FSMState
 * @brief Структура, описывающая состояние FSM.
 *
 * @details Содержит идентификатор состояния, callback-функции и список
 * возможных переходов.
 * @note Все callback-функции (onEnter, onUpdate, onExit) могут быть NULL, если
 * не требуются.
 */
typedef struct FSMState {
  int id;  ///< Уникальный идентификатор состояния.
  handlerCallback onEnter;  ///< Callback при входе в состояние.
  handlerCallback onUpdate;  ///< Callback при обновлении состояния (вызывается
                             ///< каждый цикл).
  handlerCallback onExit;  ///< Callback при выходе из состояния.
  Transition* transitions;  ///< Массив переходов из этого состояния.
  int numTransitions;  ///< Количество переходов в массиве.
} FSMState;

/**
 * @struct FiniteStateMachine
 * @ingroup CoreStructs
 * @brief Основная структура FSM.
 * @details Управляет текущим состоянием, списком всех состояний и
 * пользовательским контекстом.
 */
typedef struct FiniteStateMachine {
  FSMState* states;  ///< Массив всех возможных состояний. Не может быть `NULL`.
  int numStates;  ///< Количество состояний в массиве. Должно быть > 0.
  int currentState;  ///< Индекс текущего состояния в массиве `states` (от 0 до
                     ///< `numStates-1`)
  void* context;  ///< Пользовательские данные, передаваемые в callback-функции.
} FiniteStateMachine;

/**
 * @defgroup FSMMethods Методы FSM
 * @brief Функции для создания, управления и уничтожения FSM.
 */

/**
 * @defgroup AddressProviders Поставщики адресов
 * @brief Структуры и методы для хранения указателей на ресурсы
 */

/**
 * @ingroup FSMMethods
 * @brief Создает новый экземпляр FSM.
 * @param states Массив состояний. Не может быть `NULL`.
 * @param numStates Количество состояний. Должно быть > 0.
 * @param context Пользовательские данные (могут быть `NULL`).
 * @return Указатель на созданный FSM. `NULL` в случае ошибки.
 * @warning Память для `states` должна быть выделена до вызова функции.
 * @code
 * FSMState states[] = {
 *     {STATE_IDLE, NULL, NULL, NULL, NULL, 0}
 * };
 * FiniteStateMachine* fsm = fsm_create(states, 1, gameInfo);
 * @endcode
 */
FiniteStateMachine* fsm_create(FSMState* states, int numStates, void* context);

/**
 * @ingroup FSMMethods
 * @brief Уничтожает экземпляр FSM.
 * @param fsm Указатель на FSM. Если `NULL`, функция ничего не делает.
 * @warning Не освобождает память для `states` и `context` — это обязанность
 * вызывающей стороны.
 */
void fsm_destroy(FiniteStateMachine* fsm);

/**
 * @ingroup FSMMethods
 * @brief Обрабатывает триггер и выполняет переход между состояниями.
 * @param fsm Указатель на FSM. Не может быть `NULL`.
 * @param trigger Идентификатор триггера (например, `TRIGGER_PAUSE`).
 * @details Если триггер найден в текущем состоянии:
 * 1. Вызывается `onExit` текущего состояния.
 * 2. Обновляется текущее состояние.
 * 3. Вызывается `onEnter` нового состояния.
 * @warning Индекс `targetState` должен быть валидным.
 */
void fsm_processTrigger(FiniteStateMachine* fsm, int trigger);

/**
 * @ingroup FSMMethods
 * @brief Обновляет логику текущего состояния.
 * @param fsm Указатель на FSM. Не может быть `NULL`.
 * @details Вызывает callback `onUpdate`, если он задан.
 */
void fsm_update(FiniteStateMachine* fsm);

/**
 * @ingroup AddressProviders
 * @brief Управляет локатором FSM.
 * @param fsm Указатель на FSM. Если `NULL`, возвращает текущий экземпляр.
 * @return Текущий FSM или `NULL`.
 * @details
 * - Если `fsm` не `NULL` и локатор пуст: сохраняет `fsm`.
 * - Если `fsm` совпадает с текущим в локаторе: сбрасывает локатор.
 * @warning Не передавайте адрес вручную — используйте только
 * `fsm_create`/`fsm_destroy`.
 *
 * @see FiniteStateMachine
 * @see AddressLocator_t
 */
FiniteStateMachine* locateFSM(FiniteStateMachine* fsm);

#ifdef __cplusplus
}
#endif

#endif