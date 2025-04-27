#include "cli_presenter.h"

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
}

void freeNCurses() { endwin(); }

ConsoleView_t* initView() {
  ConsoleView_t* consoleView = NULL;

  if ((consoleView = (ConsoleView_t*)malloc(sizeof(ConsoleView_t))) != NULL) {
    initNcurses();
    consoleView->size = (size_t)0;
    consoleView->element = NULL;
  }

  return consoleView;
}

void freeView(ConsoleView_t* view) {
  if (view) {
    for (int i = view->size - 1; i >= 0; i--) {
      deleteViewElement(view, i);
    }
    free(view);
    locateView(view);
    view = NULL;
  }
  freeNCurses();
}

/*!
  \ingroup Address_providers Функции поставщиков адресов.
  \brief Функция локатор выделенных ресурсов структуре хранения элементов
  интерфейса (ConsoleView_t). \param [in] view Указатель на адрес структуры
  ConsoleView_t. \return Указатель на область памяти, хранящую стркутру хранения
  элементов интерфейса (ConsoleView_t).

  \details Функция устанавливает и возвращает указатель на структуру хранения
  элементов интерфейса (ConsoleView_t). Если адрес локатору не передавался адрес
  структуры данных (ConsoleView_t) и таковой не хранится в локаторе (NULL), то
  осуществляется сохранение адреса в структуре локатора, состояние локатора
  меняется на 'установлен', функция возвращает указатель на переданный адрес.
  Если передан NULL или указатель отличный от хранимого локатором, при этом
  локатор находится в состянии 'установлен', то функция возвращает значение
  ранее сохранненого адреса. При передаче адреса структуры хранения элементов
  интерфейса (ConsoleView_t) локатору соответствующему хранимому локатором,
  состояние локатора изменяется на 'не задан', адрес приводится в состояние
  NULL. Функция возвращает значение NULL;
*/
ConsoleView_t* locateView(ConsoleView_t* view) {
  static ViewLocator_t locator = {0};

  if (view == locator.view && locator.setval) {
    locator.view = NULL;
    locator.setval = FALSE;
  } else if (view != locator.view && !locator.setval) {
    locator.view = view;
    locator.setval = TRUE;
  }

  return locator.view;
}

int appendViewElement(ConsoleView_t* view, int type, int top, int left,
                      int height, int width, char* label) {
  int errval = ERROR_FAULT;

  //Проверка условий исполнения функции.
  if (!view) return errval;

  size_t newsize = view->size + 1;
  ConsoleElement_t* elements;
  elements = (ConsoleElement_t*)realloc(view->element,
                                        newsize * sizeof(ConsoleElement_t));

  if (elements) {
    elements[newsize - 1].top = top;
    elements[newsize - 1].left = left;
    elements[newsize - 1].height = height;
    elements[newsize - 1].width = width;
    if (label) {
      elements[newsize - 1].label =
          (char*)malloc(sizeof(char) * (int)(strlen(label) + 1));
      for (int j = 0; j <= strlen(label); j++) {
        elements[newsize - 1].label[j] = label[j];
      }
    } else {
      elements[newsize - 1].label = NULL;
    }

    view->element = elements;
    view->size = newsize;
    errval = ERROR_OK;
  }

  return errval;
}

void deleteViewElement(ConsoleView_t* view, int index) {
  if (!view) return;
  if (!view->element) return;
  if (index < 0 || index > view->size - 1) return;

  if (view->element[index].label) free(view->element[index].label);

  for (int i = index; i < view->size - 1; i++) {
    view->element[i] = view->element[i + 1];
  }

  --view->size;
  if (view->size > 0) {
    view->element = (ConsoleElement_t*)realloc(
        view->element, sizeof(ConsoleElement_t) * (view->size));
  } else {
    free(view->element);
  }
}

void renderElementFrame(const ConsoleElement_t element) {
  if (element.height < MIN_ELEMENT_DIM || element.width < MIN_ELEMENT_DIM)
    return;

  int top = element.top;
  int left = element.left;
  int height = element.height + 2;
  int width = element.width + 2;

  if (top < 0)
    top = 0;
  else if (top > MAX_COORDINATE_VALUE - height)
    top = MAX_COORDINATE_VALUE - height;
  if (left < 0)
    left = 0;
  else if (left > MAX_COORDINATE_VALUE - width)
    left = MAX_COORDINATE_VALUE - width;

  int labelLength = 0;

  if (element.label != NULL) {
    labelLength = (int)strlen(element.label);
  }

  if (labelLength > (width * PIXEL_WIDTH) - 2) {
    labelLength = width * PIXEL_WIDTH - 2;
  }

  int labelpos = left + 1 + (width * PIXEL_WIDTH - 2 - labelLength) / 2;

  for (int i = left; i < left + width * PIXEL_WIDTH; i++) {
    if (labelLength > 0 && i >= labelpos && i < labelpos + labelLength) {
      mvaddch(top, i, element.label[i - labelpos]);
    } else {
      mvaddch(top, i, ACS_HLINE);
    }
    mvaddch(top + height - 1, i, ACS_HLINE);
  }

  for (int j = top; j < top + height; j++) {
    mvaddch(j, left, ACS_VLINE);
    mvaddch(j, left + (width * PIXEL_WIDTH) - 1, ACS_VLINE);
  }

  mvaddch(top, left, ACS_ULCORNER);
  mvaddch(top, left + (width * PIXEL_WIDTH) - 1, ACS_URCORNER);
  mvaddch(top + height - 1, left, ACS_LLCORNER);
  mvaddch(top + height - 1, left + (width * 2) - 1, ACS_LRCORNER);
  refresh();
}

void renderView(const ConsoleView_t* view) {
  if (view) {
    for (int i = 0; i < view->size; i++) {
      renderElementFrame(view->element[i]);
    }
  }
}

void refreshViewElement(ConsoleView_t* view, int index, int type, void* data) {
  if (!view || !data) return;
  if (index < 0 || index >= view->size) return;
  if (view->element[index].type != type) return;

  int top = view->element[index].top + ELEMENT_DATA_OFFSET;
  int left = view->element[index].left + ELEMENT_DATA_OFFSET;

  switch (type) {
    case DATA_TYPE_INT: {
      int value = *(int*)data;
      mvprintw(top, left, "%i", value);
      break;
    }
    case DATA_TYPE_INT2D: {
      int** matrix = (int**)data;
      for (int i = 0; i < view->element[index].height; i++) {
        for (int j = 0; j < view->element[index].width; j++) {
          if (matrix[i][j] == 0) {
            mvprintw(top + i, left + j * PIXEL_WIDTH, "%s", "  ");
          } else {
            mvprintw(top + i, left + j * PIXEL_WIDTH, "%s", "[]");
          }
        }
      }
      break;
    }
    case DATA_TYPE_CHAR: {
      mvprintw(top, left, "%c", *(char*)data);
      break;
    }
    case DATA_TYPE_STR: {
      mvprintw(top, left, "%s", (char*)data);
      break;
    }
    default:
      break;
  }
  refresh();
}