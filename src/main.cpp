// (C) komunre 2025 LGPL-2.1 License

#include <stdio.h>
#include <iostream>
#include "hid.h"
#include "outgauge.h"

int main()
{
    if (!open_wheel_hid())
    {
        return 1;
    }

    change_state(LED_NONE);

    open_socket();
    while (true)
    {
        Outgauge_t* data = receive_data();

        static float max_rpm = 6000;
        float percent = data->rpm / max_rpm;
        if (percent < 0.20)
        {
            change_state(LED_NONE);
        }
        else if (percent >= 0.20 && percent < 0.40)
        {
            change_state(LED_ONE);
        }
        else if (percent >= 0.40 && percent < 0.60)
        {
            change_state(LED_TWO);
        }
        else if (percent >= 0.60 && percent < 0.70)
        {
            change_state(LED_THREE);
        }
        else if (percent >= 0.70 && percent < 0.90)
        {
            change_state(LED_FOUR);
        }
        else if (percent >= 0.90)
        {
            change_state(LED_FIVE);
        }
    }

    std::cout << "Exitting...\n";

    end_led_control();

    return 0;
}