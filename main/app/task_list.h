/**
 ******************************************************************************
 * @author: VietDung
 * @date:   31/10/2024
 ******************************************************************************
 **/
#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "eos.h"

enum {
    /*Shell task*/
    TASK_SHELL_ID,
    
    /*Device task*/
    TASK_DEVICE_ID,

    /*End task*/
    EOS_TASK_END_ID,
};

extern eos_task_t table_task[];

#endif