#include "game_timer.h"
#include <sys/time.h>

uint64_t getCurrentTimeMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(tv.tv_sec) * 1000 + (uint64_t)(tv.tv_usec) / 1000;
}

void initTimer(GameTimer_t* timer, uint64_t interval) {
    timer->interval = interval;
    timer->last_update = getCurrentTimeMs();
    timer->paused = false;
}

bool checkTimer(GameTimer_t* timer) {
    if (timer->paused) return false;
    uint64_t now = getCurrentTimeMs();
    if (now - timer->last_update >= timer->interval) {
        timer->last_update = now;
        return true;
    }
    return false;
}

void pauseTimer(GameTimer_t* timer, bool state) {
    timer->paused = state;
}