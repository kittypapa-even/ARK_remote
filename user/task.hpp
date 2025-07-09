#ifndef __TASK_HPP
#define __TASK_HPP

#include "Rocker.hpp"
#include "bsp_encode.hpp"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"
#include "adc.h"
#include "pack.h"
#include "usart.h"
#include "key.h"
#include "oled.h"

    void myinit();
    void myloop();
    void key_loop();

#ifdef __cplusplus
}
#endif

#endif




