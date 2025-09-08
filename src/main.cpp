// (C) komunre 2025 LGPL-2.1 License

#include <stdio.h>
#include <iostream>
#include <signal.h>
#include "hid.h"
#include "outgauge.h"

#define MAX_RPM 7000

static volatile sig_atomic_t g_keep_running = 1;

void handle_sigint(int)
{
    g_keep_running = 0;
}

int main()
{
    // Set SIGINT handler for graceful exit
    struct sigaction sigint_handler;
    sigint_handler.sa_handler = handle_sigint;
    if (sigemptyset(&sigint_handler.sa_mask) != 0)
    {
        std::cerr << "sigiemptyset has failed\n";
    }
    sigint_handler.sa_flags = 0;

    if (sigaction(SIGINT, &sigint_handler, NULL) == -1)
    {
        std::cerr << "Failed to set SIGINT handler\n";
    }

    // Wheel stuff starts here
    if (!open_wheel_hid())
    {
        return 1;
    }

    // Reset LED state.
    // Technically does nothing, since change_state 
    // by default assumes LED_NONE to be current state
    // and ignores an attempt to send it again
    change_state(LED_NONE);

    // Open UPD server socket
    open_socket();
    // Update loop
    while (g_keep_running)
    {
        // Attempt to receive data from the socket
        Outgauge_t* data = receive_data();
        // avoid messing with invalid/unhcanged data
        // if receive ended in timeout or error
        if (data == NULL)
        {
            continue;
        }

        static float max_rpm = MAX_RPM;
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

    // Graceful exit

    std::cout << "Exitting...\n";

    // Not sending this signal could leave LEDs turned on
    change_state(LED_NONE);

    end_led_control();

    return 0;
}