# Leave this line at the top of the file; it has all the watch library sources and includes.
TOP = ../..
include $(TOP)/make.mk

# If you add any other subdirectories with header files you wish to include, add them after ../
# Note that you will need to add a backslash at the end of any line you wish to continue, i.e.
# INCLUDES += \
#   -I../ \
#   -I../drivers/ \
#   -I../watch_faces/fitness/
INCLUDES += \
  -I../ \
  -I../watch_faces/ \
  -I../watch_faces/clock/ \
  -I../watch_faces/settings/ \
  -I../watch_faces/complication/ \
  -I../watch_faces/sensor/ \
  -I../watch_faces/demo/ \
  -I../../littlefs/ \
  -I../lib/chirpy_tx/ \
  -I../lib/TOTP/ \
  -I../lib/base32/ \
  -I../lib/sunriset/ \
  -I../lib/vsop87/ \
  -I../lib/astrolib/ \
  -I../lib/morsecalc/ \
  -I../lib/smallchesslib/ \

# If you add any other source files you wish to compile, add them after ../app.c
# Note that you will need to add a backslash at the end of any line you wish to continue, i.e.
# SRCS += \
#   ../movement.c \
#   ../drivers/lis2dh.c \
#   ../watch_faces/fitness/step_count_face.c
SRCS += \
  ../lib/chirpy_tx/chirpy_tx.c \
  ../lib/TOTP/sha1.c \
  ../lib/TOTP/sha256.c \
  ../lib/TOTP/sha512.c \
  ../lib/TOTP/TOTP.c \
  ../lib/base32/base32.c \
  ../lib/sunriset/sunriset.c \
  ../lib/vsop87/vsop87a_milli.c \
  ../lib/astrolib/astrolib.c \
  ../lib/morsecalc/calc.c \
  ../lib/morsecalc/calc_fns.c \
  ../lib/morsecalc/calc_strtof.c \
  ../lib/morsecalc/morsecalc_display.c \
  ../../littlefs/lfs.c \
  ../../littlefs/lfs_util.c \
  ../movement.c \
  ../filesystem.c \
  ../shell.c \
  ../shell_cmd_list.c \
  ../watch_faces/clock/simple_clock_face.c \
  ../watch_faces/clock/close_enough_clock_face.c \
  ../watch_faces/clock/clock_face.c \
  ../watch_faces/clock/world_clock_face.c \
  ../watch_faces/clock/beats_face.c \
  ../watch_faces/clock/weeknumber_clock_face.c \
  ../watch_faces/settings/preferences_face.c \
  ../watch_faces/settings/set_time_face.c \
  ../watch_faces/settings/set_time_hackwatch_face.c \
  ../watch_faces/sensor/thermistor_readout_face.c \
  ../watch_faces/sensor/thermistor_logging_face.c \
  ../watch_faces/sensor/thermistor_testing_face.c \
  ../watch_faces/demo/character_set_face.c \
  ../watch_faces/demo/voltage_face.c \
  ../watch_faces/demo/lis2dw_logging_face.c \
  ../watch_faces/demo/demo_face.c \
  ../watch_faces/demo/hello_there_face.c \
  ../watch_faces/complication/pulsometer_face.c \
  ../watch_faces/complication/day_one_face.c \
  ../watch_faces/complication/stopwatch_face.c \
  ../watch_faces/complication/totp_face.c \
  ../watch_faces/complication/totp_face_lfs.c \
  ../watch_faces/complication/sunrise_sunset_face.c \
  ../watch_faces/complication/countdown_face.c \
  ../watch_faces/complication/sailing_face.c \
  ../watch_faces/complication/counter_face.c \
  ../watch_faces/complication/blinky_face.c \
  ../watch_faces/complication/moon_phase_face.c \
  ../watch_faces/sensor/accelerometer_data_acquisition_face.c \
  ../watch_faces/clock/mars_time_face.c \
  ../watch_faces/complication/orrery_face.c \
  ../watch_faces/complication/astronomy_face.c \
  ../watch_faces/complication/tomato_face.c \
  ../watch_faces/complication/probability_face.c \
  ../watch_faces/complication/wake_face.c \
  ../watch_faces/demo/frequency_correction_face.c \
  ../watch_faces/complication/alarm_face.c \
  ../watch_faces/complication/ratemeter_face.c \
  ../watch_faces/complication/interval_face.c \
  ../watch_faces/complication/rpn_calculator_alt_face.c \
  ../watch_faces/complication/stock_stopwatch_face.c \
  ../watch_faces/complication/tachymeter_face.c \
  ../watch_faces/settings/nanosec_face.c \
  ../watch_faces/settings/finetune_face.c \
  ../watch_faces/complication/databank_face.c \
  ../watch_faces/complication/tempchart_face.c \
  ../watch_faces/complication/tally_face.c \
  ../watch_faces/complication/tarot_face.c \
  ../watch_faces/complication/morsecalc_face.c \
  ../watch_faces/complication/rpn_calculator_face.c \
  ../watch_faces/complication/activity_face.c \
  ../watch_faces/demo/chirpy_demo_face.c \
  ../watch_faces/complication/ships_bell_face.c \
  ../watch_faces/sensor/lightmeter_face.c \
  ../watch_faces/complication/discgolf_face.c \
  ../watch_faces/complication/habit_face.c \
  ../watch_faces/complication/planetary_time_face.c \
  ../watch_faces/complication/planetary_hours_face.c \
  ../watch_faces/complication/breathing_face.c \
  ../watch_faces/clock/repetition_minute_face.c \
  ../watch_faces/complication/timer_face.c \
  ../watch_faces/complication/invaders_face.c \
  ../watch_faces/clock/world_clock2_face.c \
  ../watch_faces/complication/time_left_face.c \
  ../watch_faces/complication/randonaut_face.c \
  ../watch_faces/complication/toss_up_face.c \
  ../watch_faces/complication/geomancy_face.c \
  ../watch_faces/clock/simple_clock_bin_led_face.c \
  ../watch_faces/complication/menstrual_cycle_face.c \
  ../watch_faces/complication/flashlight_face.c \
  ../watch_faces/clock/decimal_time_face.c \
  ../watch_faces/clock/wyoscan_face.c \
  ../watch_faces/settings/save_load_face.c \
  ../watch_faces/clock/day_night_percentage_face.c \
  ../watch_faces/complication/simple_coin_flip_face.c \
  ../watch_faces/complication/solstice_face.c \
  ../watch_faces/complication/couch_to_5k_face.c \
  ../watch_faces/clock/minute_repeater_decimal_face.c \
  ../watch_faces/clock/entropy_face.c \
  ../watch_faces/complication/tuning_tones_face.c \
  ../watch_faces/sensor/minmax_face.c \
  ../watch_faces/complication/kitchen_conversions_face.c \
  ../watch_faces/complication/butterfly_game_face.c \
  ../watch_faces/complication/wareki_face.c \
  ../watch_faces/complication/wordle_face.c \
  ../watch_faces/complication/endless_runner_face.c \
  ../watch_faces/complication/periodic_face.c \
  ../watch_faces/complication/deadline_face.c \
  ../watch_faces/complication/higher_lower_game_face.c \
  ../watch_faces/clock/french_revolutionary_face.c \
  ../watch_faces/clock/minimal_clock_face.c \
  ../watch_faces/complication/simon_face.c \
  ../watch_faces/complication/simple_calculator_face.c \
  ../watch_faces/sensor/alarm_thermometer_face.c \
  ../watch_faces/demo/beeps_face.c \
  ../watch_faces/sensor/accel_interrupt_count_face.c \
  ../watch_faces/complication/metronome_face.c \
  ../watch_faces/complication/smallchess_face.c \
# New watch faces go above this line.

# Leave this line at the bottom of the file; it has all the targets for making your project.
include $(TOP)/rules.mk
