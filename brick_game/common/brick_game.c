/**
 * @file brick_game.c
 * @brief Реализация общих функций для игр BrickGame.
 * @author provemet
 * @version 2.0
 * @date Декабрь 2024
 *
 * @warning Все функции не потокобезопасны. Используйте в однопоточных
 * приложениях.
 */

#include "brick_game.h"

#include "addr_locator.h"

GameInfo_t *createGameInfo() {
  GameInfo_t *gameinfo = NULL;

  if ((gameinfo = malloc(sizeof(GameInfo_t))) != NULL) {
    if ((gameinfo->field = createMatrix(FIELD_HEIGHT, FIELD_WIDTH)) == NULL) {
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

void destroyGameInfo(GameInfo_t *gameinfo) {
  if (gameinfo) {
    if (gameinfo->field) {
      removeMatrix(FIELD_HEIGHT, FIELD_WIDTH, gameinfo->field);
      gameinfo->field = NULL;
    }
    locateGameInfo(gameinfo);
    free(gameinfo);
    gameinfo = NULL;
  }
}

GameInfo_t *locateGameInfo(GameInfo_t *gameinfo) {
  static AddressLocator_t locator = {NULL, false};
  if (gameinfo == NULL) {
    return locator.is_set ? (GameInfo_t *)locator.address : NULL;
  }

  if (gameinfo == (GameInfo_t *)locator.address && locator.is_set) {
    locator.address = NULL;
    locator.is_set = false;
  } else if (!locator.is_set) {
    locator.address = (void *)gameinfo;
    locator.is_set = true;
  }

  return (GameInfo_t *)locator.address;
}

int **createMatrix(int rows, int cols) {
  int **matrix = NULL;
  int *values = NULL;
  if (rows > 0 && rows <= FIELD_HEIGHT && cols > 0 && cols <= FIELD_WIDTH) {
    if ((matrix = (int **)malloc(sizeof(int *) * rows)) != NULL &&
        (values = (int *)malloc(sizeof(int) * rows * cols)) != NULL) {
      for (int i = 0; i < rows; i++) {
        *(matrix + i) = values + (i * cols);
      }
    }
  }

  return matrix;
}

bool removeMatrix(int rows, int cols, int **matrix) {
  // Preprocessing of internal parameters
  if (!matrix &&
      (rows > 0 && rows <= FIELD_HEIGHT && cols > 0 && cols <= FIELD_WIDTH))
    return false;

  free(*matrix);
  free(matrix);
  return true;
}

GameInfo_t updateCurrentState() {
  GameInfo_t gameinfo = {0};

  // gameinfo.field = updateMatrix();
  // gameinfo.high_score = 0;
  // gameinfo.score = updateScore();
  // gameinfo.level = getCurrentLevel();
  // gameinfo.next = getNextFigure();
  // gameinfo.pause = getPauseState();
  // gameinfo.speed = getGameSpeed();

  return gameinfo;
}

int setGameBlockOrientation(GameBlock_t *gameblock,
                            gameBlockOrientation orientation) {
  int errval = -1;

  if (gameblock) {
    gameblock->orientation = orientation;
  }

  return errval;
}

int getGameBlockOrientation(GameBlock_t *gameblock) {
  int errval = -1;

  if (gameblock) {
    return gameblock->orientation;
  } else {
    return errval;
  }
}

GameBlockQueue_t *createGameBlockQueue() { 
  GameBlockQueue_t* bQueue = NULL;
  if ((bQueue = (GameBlockQueue_t*)malloc(sizeof(GameBlockQueue_t))) != NULL) {
    bQueue->first = NULL;
    bQueue->last = NULL;
    bQueue->size = 0;
  }

  locateGameBlockQueue(bQueue);

  return bQueue; 
}

GameBlockQueue_t *locateGameBlockQueue(GameBlockQueue_t* blockQueue) {
  static AddressLocator_t locator = {NULL, false};
  if (blockQueue == NULL) {
    return locator.is_set ? (GameBlockQueue_t*)locator.address : NULL;
  }

  if (blockQueue == (GameBlockQueue_t*)locator.address && locator.is_set) {
    locator.address = NULL;
    locator.is_set = false;
  } else if (!locator.is_set) {
    locator.address = (void *)gameinfo;
    locator.is_set = true;
  }

  return (GameBlockQueue_t*)locator->address;
}

void destroyGameBlockQueue(GameBlockQueue_t *blockQueue) {
  if (blockQueue) {
    if (blockQueue == locateGameBlockQueue(NULL)) {
      locateGameBlockQueue(blockQueue);
    }

    populateGameBlockQueue(blockQueue);
    free(blockQueue);
  }
}

int insertGameBlock(GameBlockQueue_t *blockQueue, GameBlock_t *gameBlock) {
  if (!blockQueue || !gameBlock) return 1;
  
  if (blockQueue->size == 0) {
    blockQueue->last = gameBlock;
  } 
  
  if (blockQueue->first) {
    gameBlock->nextBlock = blockQueue->first 
  }
  blockQueue->first = gameBlock;
  return 0;
}

int populateGameBlockQueue(GameBlockQueue_t *blockQueue) { 
  if (!blockQueue) return 1;

  while (blockQueue->size) {
    GameBlock_t* block = blockQueue->first;
    blockQueue->first = blockQueue->first->nextBlock;
    --blockQueue->size;
    if (block) deleteGameBlock(block);
  }

  return 0; 
}

GameBlock_t *createGameBlock(const int posX, const int posY, const int size, const gameBlockOrientation orientation, const int** blockMatrix) { 
  GameBlock_t* block = NULL;
  
  if ((block = (GameBlock_t*)malloc(GameBlock_t)) != NULL) {
    block->posX = posX;
    block->posY = posY;
    block->orientation = orientation;
    block->field = blockMatrix;
    block->nextBlock = NULL;
  }

  return block; 
}

void deleteGameBlock(GameBlock_t *gameBlock) {
  if (gameBlock) {
    if (gameBlock->field) {
      if (*gameBlock->field) {
        free(*gameBlock->field)
      }
      free(gameBlock->field);
    }
    free(gameBlock);
  }
}

void userInput(UserAction_t action, bool hold) {
//TO-DO hold, обработка зажатия клавиши.
  switch (action) {
    case Start:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_START_GAME);
      break;
    case Terminate:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_TERMINATE);
      break;
    case Pause:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_PAUSE);
      break;
    case Down:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_MOVE_DOWN);
      break;
    case Up:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_MOVE_UP);
      break;
    case Left:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_MOVE_LEFT);
      break;
    case Right:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_MOVE_RIGHT);
      break;
     case Action:
      fsm_processTrigger(locateFSM(NULL), TRIGGER_ROTATE);
    default:
      break;
  }
}