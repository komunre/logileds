// (C) komunre 2025 LGPL-2.1 License

#ifndef LOGI_HID_H
#define LOGI_HID_H

#define LED_NONE 0
#define LED_ONE 1
#define LED_TWO 3
#define LED_THREE 7
#define LED_FOUR 15
#define LED_FIVE 31

#define LED_SUCCESS 7

bool open_wheel_hid();
bool change_state(unsigned char desired_state);
void end_led_control();

#endif