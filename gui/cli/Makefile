CC ?= gcc
CFLAGS ?= -Wall -Werror -Wextra -std=c11 -pedantic -c -fPIC

PROJECT_NAME = libbgamecli
LIB_TYPE ?= static
VALID_LIB_TYPES = static dynamic
LIB_SOURCES_DIR = .
LIB_SOURCES = $(wildcard ${LIB_SOURCES_DIR}/*.c)
LIB_HEADERS = $(wildcard ${LIB_SOURCES_DIR}/*.h)
LIB_OBJECTS = $(patsubst %.c, %.o, ${LIB_SOURCES})

ifeq ($(LIB_TYPE),static)
    LIB_NAME = ${PROJECT_NAME}.a
else ifeq ($(LIB_TYPE),dynamic)
    LIB_NAME = ${PROJECT_NAME}.so
else
    $(error Unknown type of library: ${LIB_TYPE}. Valid library types: ${VALID_LIB_TYPES})
endif

BIN_PATH ?= ./lib/bin/
INCLUDE_PATH ?= ./lib/includes/

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    ifeq ($(shell which apt-get 2>/dev/null),/usr/bin/apt-get)
        PACKAGE_MANAGER := apt-get
        CHECK_COMMAND = dpkg -l | grep -wq
        INSTALL_COMMAND = sudo apt-get install -y
    else ifeq ($(shell which yum 2>/dev/null),/usr/bin/yum)
        PACKAGE_MANAGER := yum
        CHECK_COMMAND = rpm -q
        INSTALL_COMMAND = sudo yum install -y
    else ifeq ($(shell which dnf 2>/dev/null),/usr/bin/dnf)
        PACKAGE_MANAGER := dnf
        CHECK_COMMAND = rpm -q
        INSTALL_COMMAND = sudo dnf install -y
    else ifeq ($(shell which pacman 2>/dev/null),/usr/bin/pacman)
        PACKAGE_MANAGER := pacman
        CHECK_COMMAND = pacman -Q
        INSTALL_COMMAND = sudo pacman -S --noconfirm
    else
        $(error Unknown package manager)
    endif
else ifeq ($(UNAME_S),Darwin)
    ifeq ($(shell which brew 2>/dev/null),/usr/local/bin/brew)
        CHECK_PACKAGE = brew list
        INSTALL_COMMAND = brew install
    else
        $(error Homebrew not installed.)
    endif
endif

DEPENDENCIES := libncurses-dev
DEP_FLAGS := -lncurses

CLFORMAT = clang-format
CLFLAGS = --style=Google --dry-run

.PHONY: all install uninstall build check-dependancies check-folders linter tests

all: build

install: check-folders
	@printf "\e[40;33m\n";
	@echo "--- Installing library ${PROJECT_NAME} to the destination folder ---"
	@cp ${LIB_NAME} "${BIN_PATH}"
	@cp ${LIB_HEADERS} "${INCLUDE_PATH}/${PROJECT_NAME}.h"
	@rm ${LIB_NAME}
	@tput sgr0

uninstall:
	@printf "\e[40;33m\n";
	@rm -f "${BIN_PATH}${PROJECT_NAME}.a"
	@rm -f "${BIN_PATH}${PROJECT_NAME}.so"
	@rm -f "${INCLUDE_PATH}${PROJECT_NAME}.h"
	@echo "--- Library ${PROJECT_NAME} uninstalled ---"
	@tput sgr0

build: check-dependancies ${LIB_NAME}
	@printf "\e[40;33m\n";
	@echo "${LIB_NAME} compilation complete"
	@tput sgr0

check-dependancies:
	@printf "\e[40;33m\n";
	@echo "Checking dependancies for ${PROJECT_NAME} project."
	@printf "\e[40;32m\n";
	@for dep in $(DEPENDENCIES); do \
		echo "Checking $$dep installation."; \
		if ! $(CHECK_COMMAND) $$dep > /dev/null 2>&1; then \
            echo "$$dep not installed. Installtion attempt..."; \
            if $(INSTALL_COMMAND) $$dep; then \
                echo "OK: $$dep intalled successfuly."; \
            else \
				printf "\e[40;31m\n"; \
                echo "Error: $$dep not installed."; \
                echo "$$dep is needed. Please install package by yourself."; \
				tput sgr0; \
                exit 1; \
            fi; \
        else \
            	echo "OK: $$dep presents in system."; \
        fi; \
    done
	@tput sgr0

check-folders:
	@printf "\e[40;33m\n";
	@echo "Checking existance of installation folder for binary of ${PROJECT_NAME}."
	@printf "\e[40;32m\n";
	@if [ ! -d "${BIN_PATH}" ]; then \
		echo "Folder ${BIN_PATH} does not exists. Creating folder."; \
		mkdir -p "${BIN_PATH}"; \
	else \
		echo "Installation folder exists."; \
	fi
	@if [ ! -d "${BIN_PATH}" ]; then \
		printf "\e[40;31m\n"; \
		echo "Error: Can't create installation dir."; \
		tput sgr0; \
		exit 1; \
	fi
	@echo "Checking existance of installation folder for includes of ${PROJECT_NAME}."
	@if [ ! -d "${INCLUDE_PATH}" ]; then \
		echo "Folder ${INCLUDE_PATH} does not exists. Creating folder."; \
		mkdir -p "${INCLUDE_PATH}"; \
	else \
		echo "Installation folder exists."; \
	fi
	@if [ ! -d "${INCLUDE_PATH}" ]; then \
		printf "\e[40;31m\n"; \
		echo "Error: Can't create installation dir."; \
		tput sgr0; \
		exit 1; \
	fi
	@tput sgr0

tests: linter
	@printf "\e[40;32m\n";
	@echo "--- Tests for ${PROJECT_NAME} PASSED ---"
	@tput sgr0

linter: ${CLFORMAT}
	
${CLFORMAT}:	
	@printf "\e[40;33m\n";
	@echo '$@ test for ${PROJECT_NAME} started'
	@printf "\e[40;32m\n";
	@for src in ${LIB_SOURCES} ${LIB_HEADERS} ; do \
		var=`$@ ${CLFLAGS} $$src 2>&1 | wc -l`; \
		if [ $$var -ne 0 ] ; then \
		    printf "\e[40;31m\n"; \
			echo "$$src style test [FAULT]." ; \
			tput sgr0; \
			exit 1 ; \
		else \
			echo "$$src style test [PASS]"; \
		fi ; \
	done
	@tput sgr0

%.o: %.c
	@${CC} ${CFLAGS} $< ${DEP_FLAGS}


${LIB_NAME}: ${LIB_OBJECTS}
	@if [ "${LIB_TYPE}" = "static" ]; then \
	    ar rc $@ ${LIB_OBJECTS}; \
	    ranlib $@; \
	else \
		${CC} -shared ${LIB_OBJECTS} -o $@; \
	fi
	@rm -f ${LIB_OBJECTS}
