/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2022-6-1       Wayne Lin     First version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include "rtconfig.h"

#if defined(SOC_SERIES_N9H30)
    //#define LV_USE_ANTI_TEARING             1
    //#define LV_DISP_DEF_REFR_PERIOD         30

    #ifndef BIT31
        #define BIT31    (0x80000000)       ///< Bit 31 mask of an 32 bit integer
    #endif

    #define IS_CACHEABLE_VRAM(addr)    !((uint32_t)addr & BIT31)
#endif

#define LV_COLOR_DEPTH                  BSP_LCD_BPP
#define LV_HOR_RES_MAX                  BSP_LCD_WIDTH
#define LV_VER_RES_MAX                  BSP_LCD_HEIGHT

#define LV_FONT_MONTSERRAT_12           1
#define LV_FONT_MONTSERRAT_16           1
//#define LV_USE_PERF_MONITOR             1
//#define CONFIG_LV_LOG_LEVEL LV_LOG_LEVEL_TRACE

#define LV_USE_DEMO_RTT_MUSIC         0
#if LV_USE_DEMO_RTT_MUSIC
    #define LV_COLOR_SCREEN_TRANSP        1
    #define LV_DEMO_RTT_MUSIC_AUTO_PLAY   1

    #define LV_USE_DEMO_MUSIC             1
    #define LV_DEMO_MUSIC_AUTO_PLAY       1
#endif


//theme black
#define LV_THEME_DEFAULT_INIT          lv_theme_night_init

/* Please comment LV_USE_DEMO_RTT_MUSIC declaration before un-comment below */
//#define LV_USE_DEMO_WIDGETS         1
//#define LV_USE_DEMO_BENCHMARK       1

#endif
