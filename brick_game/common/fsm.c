/**
 * @file fsm.c
 * @brief Реализация функций для работы с конечным автоматом (FSM).
 * @author provemet
 * @version 2.0
 * @date Декабрь 2024
 *
 * @warning Все функции не потокобезопасны. Используйте в однопоточных
 * приложениях.
 */
#include "fsm.h"

#include <stdbool.h>
#include <stdlib.h>

#include "addr_locator.h"

FiniteStateMachine* fsm_create(FSMState* states, int numStates, void* context) {
  if (!states || numStates <= 0 || locateFSM(NULL)) {
    return NULL;
  }

  FiniteStateMachine* fsm = malloc(sizeof(FiniteStateMachine));
  if (fsm) {
    fsm->states = states;
    fsm->numStates = numStates;
    fsm->currentState = 0;  // Начальное состояние
    fsm->context = context;
    locateFSM(fsm);
  }
  return fsm;
}

FiniteStateMachine* locateFSM(FiniteStateMachine* fsm) {
  static AddressLocator_t locator = {NULL, false};
  if (fsm == NULL) {
    return locator.is_set ? (FiniteStateMachine*)locator.address : NULL;
  }

  if (!locator.is_set) {
    locator.address = (void*)fsm;
    locator.is_set = true;
  } else if (fsm == (FiniteStateMachine*)locator.address && locator.is_set) {
    locator.address = NULL;
    locator.is_set = false;
  }

  return (FiniteStateMachine*)locator.address;
}

void fsm_destroy(FiniteStateMachine* fsm) {
  if (fsm) {
    if (locateFSM(NULL) == fsm) {
      locateFSM(fsm);
    }
    free(fsm);
  }
}

void fsm_processTrigger(FiniteStateMachine* fsm, int trigger) {
  FSMState* current = &fsm->states[fsm->currentState];
  for (int i = 0; i < current->numTransitions; i++) {
    Transition* t = &current->transitions[i];
    if (t->trigger == trigger) {
      if (current->onExit) current->onExit(fsm->context);
      fsm->currentState = t->targetState;
      if (t->onEnter) t->onEnter(fsm->context);
      break;
    }
  }
}

void fsm_update(FiniteStateMachine* fsm) {
  FSMState* current = &fsm->states[fsm->currentState];
  if (current->onUpdate) {
    current->onUpdate(fsm->context);
  }
}