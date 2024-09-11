#include "core/time/clock.h"

#define MAX_ELAPSED_TIME (24.0 * 60.0 * 60.0) // 24 hours in seconds

Clock clock_create() {
    auto now = std::chrono::high_resolution_clock::now();
    return {
        .start_time = now,
        .previous_time = now,
        .elapsed = 0,
        .delta_t = 0,
        .time_scale = 1,
        .days = 0,
    };
}

void clock_update(Clock *clock) {
    auto now = std::chrono::high_resolution_clock::now();
    auto frame_duration = std::chrono::duration<f64>(now - clock->previous_time).count();
    clock->delta_t = frame_duration * clock->time_scale;

    clock->elapsed += clock->delta_t;
    if (clock->elapsed >= MAX_ELAPSED_TIME) {
        clock->elapsed -= MAX_ELAPSED_TIME;
        clock->days++;
    }

    clock->previous_time = now;
}

void global_clock_init() {
    global_clock = clock_create();
}
