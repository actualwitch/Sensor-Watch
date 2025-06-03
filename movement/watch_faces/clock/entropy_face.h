/*
 * MIT License
 *
 * Copyright (c) 2025 Adelaide あで Fisher & Claude
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ENTROPY_FACE_H_
#define ENTROPY_FACE_H_

#include "movement.h"

/*
 * ENTROPY - A Glitch Art Watch Face
 *
 * This watch face oscillates between order and chaos, inspired by glitch art.
 * In the "order" state, it displays mostly correct time with intentional glitches.
 * In the "chaos" state, the display fills with trippy, wrong symbols and patterns
 * designed to evoke cognitive dissonance. The timing of transitions drifts,
 * creating an unpredictable rhythm between readable time and digital decay.
 *
 */

typedef enum {
    ENTROPY_STATE_ORDER,
    ENTROPY_STATE_TRANSITIONING_TO_CHAOS,
    ENTROPY_STATE_CHAOS,
    ENTROPY_STATE_TRANSITIONING_TO_ORDER
} entropy_state_t;

typedef struct {
    entropy_state_t state;
    uint8_t ticks;                     // General tick counter
    uint16_t state_ticks;              // Ticks in current state
    uint8_t transition_progress;       // 0-100 for smooth transitions
    uint32_t last_update;              // Last time display was updated
    uint8_t drift_offset;              // 0-59 seconds drift for order reveal
    uint8_t order_duration;            // How long to stay in order (varies)
    uint8_t chaos_duration;            // How long to stay in chaos (varies)
    uint16_t glitch_seed;              // Seed for pseudo-random glitches
    bool low_energy;                   // Track if we're in low energy mode
    uint8_t glitch_intensity;          // 0-100 intensity of glitches
    uint8_t displayed_digits[10];      // What's currently shown (for glitching)
    
    // Per-position chaos state for asynchronous motion
    struct {
        uint16_t phase;                // Current phase (0-65535)
        uint16_t frequency;            // How fast this position changes
        uint8_t pattern_type;          // Which pattern this position uses
        uint8_t intensity;             // Individual intensity
        char target_char;              // Character to converge to in order
    } position_state[10];
    
    // Global chaos parameters
    uint8_t chaos_layers;              // Number of active chaos layers
    uint16_t global_phase;             // Global phase for synchronization effects
} entropy_face_state_t;

void entropy_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr);
void entropy_face_activate(movement_settings_t *settings, void *context);
bool entropy_face_loop(movement_event_t event, movement_settings_t *settings, void *context);
void entropy_face_resign(movement_settings_t *settings, void *context);

#define entropy_face ((const watch_face_t){ \
    entropy_face_setup, \
    entropy_face_activate, \
    entropy_face_loop, \
    entropy_face_resign, \
    NULL, \
})

#endif // ENTROPY_FACE_H_