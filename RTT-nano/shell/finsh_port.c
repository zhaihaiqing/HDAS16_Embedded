/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <rthw.h>
#include <rtconfig.h>
#include "main.h"

#ifndef RT_USING_FINSH
#error Please uncomment the line <#include "finsh_config.h"> in the rtconfig.h 
#endif

#ifdef RT_USING_FINSH

RT_WEAK char rt_hw_console_getchar(void)
{
    /* Note: the initial value of ch must < 0 */
    int ch = -1;

		if (__HAL_UART_GET_FLAG(&huart8, UART_FLAG_RXNE) != RESET)
    {
        ch = huart8.Instance->RDR & 0xff;
    }
    else
    {
        if(__HAL_UART_GET_FLAG(&huart8, UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&huart8);
        }
        rt_thread_mdelay(10);
    }

    return ch;
}

#endif /* RT_USING_FINSH */

