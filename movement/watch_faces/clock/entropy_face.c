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

#include <stdlib.h>
#include <string.h>
#include "entropy_face.h"
#include "watch.h"
#include "watch_utility.h"
#include "watch_private_display.h"

// Glitch characters - expanded set for more visual chaos
static const char glitch_chars[] = "AbCdEFGHIJLnoPqrStUvxy&@=><[]{}|-_^~123456789o0";
static const uint8_t num_glitch_chars = sizeof(glitch_chars) - 1;

// Different chaos pattern generators
typedef enum {
    CHAOS_PATTERN_NOISE,
    CHAOS_PATTERN_WAVE,
    CHAOS_PATTERN_SCATTER,
    CHAOS_PATTERN_PULSE,
    CHAOS_PATTERN_MORPH,
    CHAOS_PATTERN_STATIC,
    CHAOS_PATTERN_CASCADE,
    CHAOS_PATTERN_INTERFERENCE,
    CHAOS_PATTERN_COUNT
} chaos_pattern_t;

// Pseudo-random number generator (simple LCG)
static uint16_t entropy_rand(entropy_face_state_t *state) {
    state->glitch_seed = (state->glitch_seed * 1103515245 + 12345) & 0x7FFF;
    return state->glitch_seed;
}

// Generate a glitched version of a digit
static char glitch_digit(entropy_face_state_t *state, char original, uint8_t intensity) {
    uint16_t chance = entropy_rand(state) % 100;
    
    if (chance < intensity) {
        // Replace with glitch character
        return glitch_chars[entropy_rand(state) % num_glitch_chars];
    } else if (chance < (intensity * 3 / 2) && original >= '0' && original <= '9') {
        // Shift to nearby digit
        int shift = (entropy_rand(state) % 3) - 1; // -1, 0, or 1
        char new_digit = original + shift;
        if (new_digit < '0') new_digit = '9';
        if (new_digit > '9') new_digit = '0';
        return new_digit;
    }
    
    return original;
}

// Apply segment-level glitches by manipulating individual pixels
static void apply_segment_glitches(entropy_face_state_t *state, uint8_t position, uint8_t intensity) {
    if (intensity == 0) return;
    
    // Map positions to segment coordinates (simplified)
    // This is a bit hacky but works for creating glitch effects
    uint8_t com_start = 0;
    uint8_t seg_start = 0;
    
    // Rough mapping based on display positions
    if (position < 2) {
        // Weekday area
        seg_start = 0 + position * 2;
    } else if (position < 4) {
        // Day area  
        com_start = 0;
        seg_start = 4 + (position - 2) * 2;
    } else {
        // Time area
        seg_start = 8 + (position - 4) * 2;
    }
    
    // Random segment corruption
    for (int i = 0; i < 3; i++) {
        if ((entropy_rand(state) % 100) < intensity) {
            uint8_t com = (com_start + (entropy_rand(state) % 3)) % 3;
            uint8_t seg = (seg_start + (entropy_rand(state) % 4)) % 24;
            
            if (entropy_rand(state) % 2) {
                watch_set_pixel(com, seg);
            } else {
                watch_clear_pixel(com, seg);
            }
        }
    }
}

// Generate character for a specific position based on its pattern
static char generate_position_char(entropy_face_state_t *state, uint8_t pos) {
    uint16_t phase = state->position_state[pos].phase;
    uint8_t pattern = state->position_state[pos].pattern_type;
    uint8_t intensity = state->position_state[pos].intensity;
    
    // Add some global phase influence for occasional synchronization
    uint16_t combined_phase = phase + (state->global_phase >> 2);
    
    switch (pattern) {
        case CHAOS_PATTERN_NOISE:
            // Pure random noise
            if ((entropy_rand(state) % 100) < intensity) {
                return glitch_chars[entropy_rand(state) % num_glitch_chars];
            }
            return state->position_state[pos].target_char;
            
        case CHAOS_PATTERN_WAVE:
            // Sine-like wave of corruption
            {
                uint8_t wave = (combined_phase >> 8) & 0xFF;
                uint8_t threshold = (wave < 128) ? wave : (255 - wave);
                if ((entropy_rand(state) % 255) < threshold) {
                    return glitch_chars[(phase + pos) % num_glitch_chars];
                }
            }
            return ' ';
            
        case CHAOS_PATTERN_SCATTER:
            // Scattered bursts
            if ((phase & 0x1FF) < intensity) {
                return glitch_chars[(phase >> 9) % num_glitch_chars];
            }
            return (entropy_rand(state) % 4) ? ' ' : state->position_state[pos].target_char;
            
        case CHAOS_PATTERN_PULSE:
            // Rhythmic pulses with varying duty cycle
            {
                uint16_t duty = 0xFFFF / (2 + (pos % 3));
                return (phase % 0xFFFF < duty) ? '8' : ' ';
            }
            
        case CHAOS_PATTERN_MORPH:
            // Morphing between characters
            {
                uint8_t morph_idx = (phase >> 11) % num_glitch_chars;
                uint8_t next_idx = (morph_idx + 1) % num_glitch_chars;
                uint8_t blend = (phase >> 3) & 0xFF;
                return (blend < 128) ? glitch_chars[morph_idx] : glitch_chars[next_idx];
            }
            
        case CHAOS_PATTERN_STATIC:
            // TV static effect
            return (entropy_rand(state) % 100 < intensity) ? 
                   glitch_chars[entropy_rand(state) % num_glitch_chars] : ' ';
                   
        case CHAOS_PATTERN_CASCADE:
            // Cascading corruption
            {
                uint16_t cascade_pos = (state->global_phase >> 6) % 20;
                if (abs((int)cascade_pos - (int)pos) < 2) {
                    return glitch_chars[(phase + cascade_pos) % num_glitch_chars];
                }
            }
            return state->displayed_digits[pos];
            
        case CHAOS_PATTERN_INTERFERENCE:
            // Multiple frequency interference
            {
                uint16_t f1 = phase;
                uint16_t f2 = phase * 3;
                uint16_t f3 = phase * 7;
                uint8_t interference = ((f1 >> 8) ^ (f2 >> 7) ^ (f3 >> 9)) & 0xFF;
                if (interference > 200) {
                    return glitch_chars[interference % num_glitch_chars];
                }
            }
            return ' ';
            
        default:
            return '?';
    }
}

// Generate complete chaos display with multiple independent layers
static void generate_chaos_pattern(entropy_face_state_t *state) {
    char buf[11];
    
    // Update global phase
    state->global_phase += 73; // Prime number for interesting patterns
    
    // Update each position independently
    for (int i = 0; i < 10; i++) {
        // Update position phase based on its frequency
        state->position_state[i].phase += state->position_state[i].frequency;
        
        // Generate character for this position
        buf[i] = generate_position_char(state, i);
        
        // Occasionally swap pattern types for more variety
        if ((entropy_rand(state) % 1000) < 5) {
            state->position_state[i].pattern_type = entropy_rand(state) % CHAOS_PATTERN_COUNT;
        }
        
        // Modulate intensity
        if ((entropy_rand(state) % 100) < 10) {
            state->position_state[i].intensity = 30 + (entropy_rand(state) % 70);
        }
    }
    
    buf[10] = '\0';
    watch_display_string(buf, 0);
    
    // Apply aggressive segment-level glitches
    uint8_t glitch_count = 3 + (entropy_rand(state) % 8);
    for (int i = 0; i < glitch_count; i++) {
        uint8_t pos = entropy_rand(state) % 10;
        uint8_t intensity = 20 + (entropy_rand(state) % 60);
        apply_segment_glitches(state, pos, intensity);
    }
    
    // Randomly toggle colon for extra chaos
    if ((entropy_rand(state) % 100) < 40) {
        if (entropy_rand(state) % 2) {
            watch_clear_colon();
        } else {
            watch_set_colon();
        }
    }
    
    // Occasionally corrupt the indicators too
    if ((entropy_rand(state) % 100) < 15) {
        if (entropy_rand(state) % 2) {
            watch_clear_indicator(WATCH_INDICATOR_24H);
        } else {
            watch_set_indicator(WATCH_INDICATOR_24H);
        }
    }
}

// Display time with controlled glitches
static void display_glitched_time(entropy_face_state_t *state, watch_date_time date_time, uint8_t glitch_intensity) {
    char buf[11];
    
    // Format time in 24h mode
    sprintf(buf, "%s%2d%2d%02d%02d", 
            watch_utility_get_weekday(date_time),
            date_time.unit.day,
            date_time.unit.hour,
            date_time.unit.minute,
            date_time.unit.second);
    
    // Store target characters for convergence
    for (int i = 0; i < 10; i++) {
        state->position_state[i].target_char = buf[i];
    }
    
    // Apply glitches to each character
    for (int i = 0; i < 10; i++) {
        state->displayed_digits[i] = buf[i];
        buf[i] = glitch_digit(state, buf[i], glitch_intensity);
    }
    
    watch_display_string(buf, 0);
    
    // Apply segment-level glitches
    for (int i = 0; i < 10; i++) {
        apply_segment_glitches(state, i, glitch_intensity / 3);
    }
    
    // Glitch the colon occasionally
    if ((entropy_rand(state) % 100) < glitch_intensity) {
        if (entropy_rand(state) % 2) {
            watch_clear_colon();
        } else {
            watch_set_colon();
        }
    } else {
        watch_set_colon();
    }
}

// Converge from chaos to order
static void converge_to_order(entropy_face_state_t *state, watch_date_time date_time, uint8_t convergence_progress) {
    char buf[11];
    
    // Get current time as target
    sprintf(buf, "%s%2d%2d%02d%02d", 
            watch_utility_get_weekday(date_time),
            date_time.unit.day,
            date_time.unit.hour,
            date_time.unit.minute,
            date_time.unit.second);
    
    // Update target chars
    for (int i = 0; i < 10; i++) {
        state->position_state[i].target_char = buf[i];
    }
    
    // Generate display based on convergence progress
    for (int i = 0; i < 10; i++) {
        // Slow down frequencies as we converge
        uint16_t freq_scale = 100 - convergence_progress;
        state->position_state[i].phase += (state->position_state[i].frequency * freq_scale) / 100;
        
        // Increase chance of showing correct character as we converge
        if ((entropy_rand(state) % 100) < convergence_progress) {
            buf[i] = state->position_state[i].target_char;
        } else {
            buf[i] = generate_position_char(state, i);
        }
        
        // Reduce pattern intensity
        state->position_state[i].intensity = (state->position_state[i].intensity * (100 - convergence_progress)) / 100;
    }
    
    watch_display_string(buf, 0);
    
    // Reduce segment glitches
    uint8_t glitch_intensity = 80 - (convergence_progress * 3 / 4);
    for (int i = 0; i < 3; i++) {
        apply_segment_glitches(state, entropy_rand(state) % 10, glitch_intensity);
    }
    
    // Stabilize colon as we converge
    if (convergence_progress > 50 || (entropy_rand(state) % 100) < convergence_progress) {
        watch_set_colon();
    } else if ((entropy_rand(state) % 100) < 30) {
        watch_clear_colon();
    }
    
    // Restore 24H indicator
    if (convergence_progress > 70) {
        watch_set_indicator(WATCH_INDICATOR_24H);
    }
}

// Initialize position states with random parameters
static void init_position_states(entropy_face_state_t *state) {
    for (int i = 0; i < 10; i++) {
        // Random frequency for each position (some fast, some slow)
        state->position_state[i].frequency = 50 + (entropy_rand(state) % 500);
        
        // Random starting phase
        state->position_state[i].phase = entropy_rand(state);
        
        // Random pattern type
        state->position_state[i].pattern_type = entropy_rand(state) % CHAOS_PATTERN_COUNT;
        
        // Random intensity
        state->position_state[i].intensity = 40 + (entropy_rand(state) % 60);
        
        // Initialize target char to space
        state->position_state[i].target_char = ' ';
    }
}

void entropy_face_setup(movement_settings_t *settings, uint8_t watch_face_index, void ** context_ptr) {
    (void) settings;
    (void) watch_face_index;
    
    if (*context_ptr == NULL) {
        *context_ptr = malloc(sizeof(entropy_face_state_t));
        memset(*context_ptr, 0, sizeof(entropy_face_state_t));
        
        entropy_face_state_t *state = (entropy_face_state_t *)*context_ptr;
        state->state = ENTROPY_STATE_ORDER;
        state->glitch_seed = watch_rtc_get_date_time().reg; // Seed with current time
        state->drift_offset = 0;
        state->order_duration = 5; // Start with 5 seconds in order
        state->chaos_duration = 15; // 15 seconds in chaos
        
        // Initialize per-position chaos states
        init_position_states(state);
    }
}

void entropy_face_activate(movement_settings_t *settings, void *context) {
    (void) settings;
    entropy_face_state_t *state = (entropy_face_state_t *)context;
    
    // Request appropriate tick frequency
    if (state->low_energy) {
        // In low energy, we'll get EVENT_LOW_ENERGY_UPDATE every minute
    } else {
        movement_request_tick_frequency(8); // 8Hz for smooth animations
    }
    
    watch_set_indicator(WATCH_INDICATOR_24H); // Always 24h mode
}

bool entropy_face_loop(movement_event_t event, movement_settings_t *settings, void *context) {
    entropy_face_state_t *state = (entropy_face_state_t *)context;
    watch_date_time date_time;
    
    switch (event.event_type) {
        case EVENT_ACTIVATE:
            state->low_energy = false;
            movement_request_tick_frequency(8);
            break;
            
        case EVENT_TICK:
            state->ticks++;
            state->state_ticks++;
            
            date_time = watch_rtc_get_date_time();
            
            // State machine
            switch (state->state) {
                case ENTROPY_STATE_ORDER:
                    // Display time with minor glitches
                    display_glitched_time(state, date_time, state->glitch_intensity);
                    
                    // Check if it's time to transition to chaos
                    if (state->state_ticks >= state->order_duration * 8) {
                        state->state = ENTROPY_STATE_TRANSITIONING_TO_CHAOS;
                        state->state_ticks = 0;
                        state->transition_progress = 0;
                    }
                    break;
                    
                case ENTROPY_STATE_TRANSITIONING_TO_CHAOS:
                    // Gradually increase chaos while still showing time
                    state->transition_progress += 3; // Slightly slower
                    
                    // Start introducing chaos patterns gradually
                    if (state->transition_progress > 30) {
                        // Mix chaos and time display
                        char buf[11];
                        sprintf(buf, "%s%2d%2d%02d%02d", 
                                watch_utility_get_weekday(date_time),
                                date_time.unit.day,
                                date_time.unit.hour,
                                date_time.unit.minute,
                                date_time.unit.second);
                        
                        for (int i = 0; i < 10; i++) {
                            state->position_state[i].phase += state->position_state[i].frequency / 4;
                            
                            // Increasing chance of chaos taking over
                            if ((entropy_rand(state) % 100) < (state->transition_progress - 30)) {
                                buf[i] = generate_position_char(state, i);
                            } else {
                                buf[i] = glitch_digit(state, buf[i], state->transition_progress / 2);
                            }
                        }
                        watch_display_string(buf, 0);
                        
                        // Add increasing segment chaos
                        uint8_t chaos_glitches = (state->transition_progress / 20);
                        for (int i = 0; i < chaos_glitches; i++) {
                            apply_segment_glitches(state, entropy_rand(state) % 10, 
                                                 20 + (state->transition_progress / 2));
                        }
                    } else {
                        // Early transition - just increase regular glitches
                        state->glitch_intensity = state->transition_progress;
                        display_glitched_time(state, date_time, state->glitch_intensity);
                    }
                    
                    if (state->transition_progress >= 100) {
                        state->state = ENTROPY_STATE_CHAOS;
                        state->state_ticks = 0;
                    }
                    break;
                    
                case ENTROPY_STATE_CHAOS:
                    // Full chaos mode
                    generate_chaos_pattern(state);
                    
                    // Check if it's time to transition back
                    if (state->state_ticks >= state->chaos_duration * 8) {
                        state->state = ENTROPY_STATE_TRANSITIONING_TO_ORDER;
                        state->state_ticks = 0;
                        state->transition_progress = 100;
                    }
                    break;
                    
                case ENTROPY_STATE_TRANSITIONING_TO_ORDER:
                    // Use convergence animation instead of simple glitch reduction
                    state->transition_progress -= 2; // Slower for more dramatic effect
                    
                    // Map transition progress to convergence (reverse mapping)
                    uint8_t convergence = 100 - state->transition_progress;
                    converge_to_order(state, date_time, convergence);
                    
                    if (state->transition_progress <= 0) {
                        state->state = ENTROPY_STATE_ORDER;
                        state->state_ticks = 0;
                        
                        // Randomize durations for next cycle
                        state->order_duration = 3 + (entropy_rand(state) % 8); // 3-10 seconds
                        state->chaos_duration = 10 + (entropy_rand(state) % 20); // 10-29 seconds
                        
                        // Update drift offset
                        state->drift_offset = (state->drift_offset + 7 + (entropy_rand(state) % 11)) % 60;
                        
                        // Start with some base glitch intensity even in order
                        state->glitch_intensity = 5 + (entropy_rand(state) % 15); // 5-19%
                        
                        // Re-randomize position states for next chaos cycle
                        init_position_states(state);
                    }
                    break;
            }
            break;
            
        case EVENT_LOW_ENERGY_UPDATE:
            state->low_energy = true;
            date_time = watch_rtc_get_date_time();
            
            // In low energy mode, show mostly correct time with minimal glitches
            display_glitched_time(state, date_time, 10);
            
            // Every few updates, show a brief chaos burst
            if ((date_time.unit.minute % 5) == 0) {
                generate_chaos_pattern(state);
            }
            break;
            
        case EVENT_ALARM_BUTTON_UP:
            // Button press instantly reveals clean time for 2 seconds
            state->state = ENTROPY_STATE_ORDER;
            state->state_ticks = 0;
            state->glitch_intensity = 0;
            state->order_duration = 2;
            break;
            
        default:
            return movement_default_loop_handler(event, settings);
    }
    
    return true;
}

void entropy_face_resign(movement_settings_t *settings, void *context) {
    (void) settings;
    (void) context;
    
    // Clean up
    watch_set_colon();
}