CC := gcc
CFLAGS := -Wall -Werror -Wextra -std=c11 -pedantic 
CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -std=c++17

PROJECT := brick_game
PROJECT_VERSION := 2.0
#Install path can defines with command line parameters.
INSTALL_PATH ?= .
INSTALL_PATH := $(shell realpath $(INSTALL_PATH))
INSTALL_PATH := ${INSTALL_PATH}/build/${PROJECT}
LIB_BIN_PATH := "${INSTALL_PATH}/lib/bin/"
LIB_INLUDES_PATH := "${INSTALL_PATH}/lib/includes/"
#Library type can defines with CLI parameters
LIB_TYPE ?= static
INSTALL_PATH := "${INSTALL_PATH}"
# for defining command-line interface change GUI_TYPE parameter here
# or define it in command line
VALID_GUI_TYPES = desktop cli
GUI_TYPE ?= cli

ifeq (${GUI_TYPE},desktop) 
    PRESENTER_LIB_PATH := ./gui/desktop
else ifeq ($(GUI_TYPE),cli)
    PRESENTER_LIB_PATH := ./gui/cli
else
    ${error Unknown interface type: $(GUI_TYPE). Valid types: $(VALID_GUI_TYPES)}
endif

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

DEPENDENCIES := 
TEST_DEPENDENCIES := clang-format cppcheck check lcov libgtest-dev libgmock-dev

.DEFAULT_GOAL: all
.PHOMY: all build install uninstall check-dependencies check-test-dependencies check-folders tests linter clean

all: build

build: check-dependencies
	@${MAKE} --directory=${PRESENTER_LIB_PATH} build LIB_TYPE=${LIB_TYPE} && ${MAKE} --directory=${PRESENTER_LIB_PATH} install BIN_PATH=${LIB_BIN_PATH} INCLUDE_PATH=${LIB_INLUDES_PATH} LIB_TYPE=${LIB_TYPE}

install: check-folders

uninstall:
	@${MAKE} --directory=${PRESENTER_LIB_PATH} uninstall BIN_PATH=${LIB_BIN_PATH} INCLUDE_PATH=${LIB_INLUDES_PATH} LIB_TYPE=${LIB_TYPE}

tests: check-test-dependencies linter

linter:
	@printf "\e[40;33m\n";
	@echo Linter tests started
	@tput sgr0
	@${MAKE} --directory=${PRESENTER_LIB_PATH} linter

check-dependencies:
	@printf "\e[40;32m\n";
	@echo "Checking dependancies for ${PROJECT} project."
	@for dep in $(DEPENDENCIES); do \
		if [ "$(PACKAGE_MANAGER)" != "unknown" ]; then \
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
		fi; \
    done
	@tput sgr0

check-test-dependencies:
	@printf "\e[40;33m\n";
	@echo "Checking dependancies for testing ${PROJECT} project."
	@printf "\e[40;32m\n";
	@for dep in $(TEST_DEPENDENCIES); do \
		if [ "$(PACKAGE_MANAGER)" != "unknown" ]; then \
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
		fi; \
    done
	@tput sgr0

check-folders:
	@printf "\e[40;32m\n";
	@echo "Checking existance of installation folder for ${PROJECT}."
	@if [ ! -d "${INSTALL_PATH}" ]; then \
		echo "Folder ${INSTALL_PATH} does not exists. Creating folder."; \
		mkdir -p "${INSTALL_PATH}"; \
	else \
		echo "Installation folder exists."; \
	fi
	@if [ ! -d "${INSTALL_PATH}" ]; then \
		printf "\e[40;31m\n"; \
		echo "Error: Can't create installation dir."; \
		tput sgr0; \
		exit 1; \
	fi
	@tput sgr0

clean:
	@${MAKE} uninstall INSTALL_PATH=${INSTALL_PATH}
	@rm -rf ${INSTALL_PATH}
	@printf "\e[40;33m\n";
	@echo "--- Project ${PROJECT} uninstalled. ---"
	@tput sgr0