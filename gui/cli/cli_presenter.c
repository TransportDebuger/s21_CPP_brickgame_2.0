#include "cli_presenter.h"

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
}

void freeNCurses() { endwin(); }

int initView(ConsoleView_t* view) {
  ConsoleView_t* consoleView;
  int errval = ERROR_FAULT;

  if ((consoleView = (ConsoleView_t*)malloc(sizeof(ConsoleView_t))) != NULL) {
    view = consoleView;
    view->size = 0;
    errval = ERROR_OK;
    locateView(view);
    initNcurses();
  }

  return errval;
}

void freeView(ConsoleView_t* view) {
  if (view) {
    for (int i = view->size - 1; i >= 0; i--) {
      deleteElement(view, i);
    }
    free(view);
    locateView(view);
    view = NULL;
    freeNCurses();
  }
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
  } else if (gameinfo != locator.view && !locator.setval) {
    locator.view = view;
    locator.setval = TRUE;
  }

  return locator.view;
}

int appendViewElement(ConsoleView_t* view, int top, int left, int height,
                      int width, char* label) {
  int errval = ERROR_FAULT;
  ConsoleElement_t* element;
  int newsize = 1;

  //Проверка условий исполнения.
  if (!view) return errval;

  newsize += view->size;

  if ((element = (ConsoleElement_t*)malloc(sizeof(ConsoleElement_t) *
                                           newsize)) != NULL) {
    for (int i = 0; i < newsize; i++) {
      if (i < view->size) {
        element[i] = view->element[i];
      } else {
        element[i].top = top;
        element[i].left = left;
        element[i].height = height;
        element[i].width = width;
        element[i].label = (char)malloc(sizeof(char) * (strlen(label) + 1));
        for (int j = 0; j <= strlen(label); j++) {
          element[i].label[j] = label[j];
        }
      }
    }

    errval = ERROR_OK;
  }

  return errval;
}

void deleteElement(ConsoleView_t* view, int index) {
  if (!view) return;
  if (index < 0 || index > view->size - 1) return;

  if (view->element[index]->label) free(label);
  free(view->element[index]);

  for (int i = index; i < view->size - 1; i++) {
    view.element[i] = view.element[i + 1];
  }
  view->element =
      realloc(view->element, sizeof(ConsoleElement_t) * (view->size - 1));
  --view->size;
}

void renderView(ConsoleView_t* view) {
  if (view) {
    for (int i = 0; i < view->size; i++) {
      renderElementFrame(view->element[i]);
    }
  }
}

void renderElementFrame(ConsoleElement_t* element) {
  if (element) {
    if (element->height < MIN_ELEMENT_DIM && element->width < MIN_ELEMENT_DIM)
      return;
    int top = element->top;
    int left = element->left;
    int height = element->height + 2;
    int width = element->width + 2;

    if (top < 0)
      top = 0;
    else if (top > MAX_COORDINATE_VALUE - height)
      top = MAX_COORDINATE_VALUE - height;
    if (left < 0)
      left = 0;
    else if (left > MAX_COORDINATE_VALUE - width)
      left = MAX_COORDINATE_VALUE - width;

    int labelLength = 0;

    if (element->label != NULL) {
      labelLength = (int)strlen(element->label);
    }

    if (labelLength > (width * 2) - 2) {
      labelLength = width * 2 - 2;
    }

    int labelpos = left + 1 + (width * 2 - 2 - labelLength) / 2;

    for (int i = left; i < left + width * 2; i++) {
      if (labelLength > 0 && i >= labelpos && i < labelpos + labelLength) {
        mvaddch(top, i, label[i - labelpos]);
      } else {
        mvaddch(top, i, ACS_HLINE);
      }
      mvaddch(top + height - 1, i, ACS_HLINE);
    }

    for (int j = top; j < top + height; j++) {
      mvaddch(j, left, ACS_VLINE);
      mvaddch(j, left + (width * 2) - 1, ACS_VLINE);
    }

    mvaddch(top, left, ACS_ULCORNER);
    mvaddch(top, left + (width * 2) - 1, ACS_URCORNER);
    mvaddch(top + height - 1, left, ACS_LLCORNER);
    mvaddch(top + height - 1, left + (width * 2) - 1, ACS_LRCORNER);
    refresh();
  }
}
