/*	Simple example tasks
 *	Copyright (c) 2020 by SECKC Software
 */
#include <stdio.h>
#include "tasks.h"

void task_a( void )
{
puts("TaskA\r\n");
pend_task( TASK_B );
}

void task_b( void )
{
puts("TaskB\r\n");
pend_task( TASK_A );
}