// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once


#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1

#define USB_VBUS_PIN GP29

// Mod-tap settings
// #define TAPPING_TERM 200 // Maximum time for a tap (vs hold)
// #define QUICK_TAP_TERM 10 // Time to re-hold mod-tap key for repeat (vs mod-activation)

// Oneshot settings
#define ONESHOT_TAP_TOGGLE 3
#define ONESHOT_TIMEOUT 5000
