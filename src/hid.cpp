// (C) komunre 2025 LGPL-2.1 License

#include <cstddef>
#include <iostream>
#include <hidapi.h>
#include "hid.h"

static int res;
static hid_device *g_wheel_handle;

bool open_wheel_hid()
{
    // Initialize the library
    res = hid_init();

    // Open the steering wheel!
    // using VID, PID (vendor id, product id)
    // without serial number (NULL)
    g_wheel_handle = hid_open(0x46d, 0xc24f, NULL);
    if (!g_wheel_handle)
    {
        std::cerr << "Unable to open device!\n";
        hid_exit();
        return false;
    }

    std::cout << "Steering wheel found and has beeb opened!\n";

    return true;
}

bool change_state(unsigned char desired_state)
{
    static unsigned char current_state = LED_NONE;

    if (desired_state == current_state)
    {
        return true;
    }

    static unsigned char buf[7];

    buf[0] = 0xf8;
    buf[1] = 0x12;
    buf[2] = desired_state;
    buf[3] = 0x00;
    buf[4] = 0x00;
    buf[5] = 0x00;
    buf[6] = 0x00;

    res = hid_write(g_wheel_handle, buf, 7);

    if (res != LED_SUCCESS)
    {
        std::cerr << "Error in setting LED status: " << res << std::endl;
        return false;
    }

    current_state = desired_state;
    return true;
}

void end_led_control()
{
    hid_exit();
}