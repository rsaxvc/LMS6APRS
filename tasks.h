/*	Simple task management
 *	Copyright (c) 2020 by SECKC Software
 */
#ifndef TASKS_H
#define TASKS_H

/*	Macros
 */
/*Task IDs are numbers 0-7*/
#define TASK_ID_A 0
#define TASK_ID_B 1

/*Tasks are a bitfield of IDs*/
#define TASK_A (1<<(TASK_ID_A))
#define TASK_B (1<<(TASK_ID_B))

/*In this way, a bit-list of tasks can be pended for execution*/
void pend_task( unsigned char new_tasks );
void task_a( void );
void task_b( void );

#endif
