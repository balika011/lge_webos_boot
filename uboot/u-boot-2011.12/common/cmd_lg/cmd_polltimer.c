/****************************************************************************************
 *   System IC Center, LG ELECTRONICS INC., SEOUL, KOREA
 *   COPYRIGHT(c) 2011 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work covered by this copyright hereon
 *   may be reproduced, stored in a retrieval system, in any form
 *   or by any means, electronic, mechanical, photocopying, recording
 *   or otherwise, without the prior written  permission of LG Electronics.
 ***************************************************************************************/


/*
 * 2011.03.09 - ks.hyun@lge.com
 * We can't create task or thread in lxboot,
 * so this timer have to be polled periodically !!!
 */



#include <common.h>
#include <malloc.h>
#include <command.h>
#include "cmd_polltimer.h"

int add_timer(unsigned int interval, unsigned int oneshot, void (*func)(void*), void *arg)
{
	int id;
	struct poll_timer *timer = NULL;

	for(id=0; id<MAX_POLL_TIMER; id++)
	{
		if(timer_list[id].used == 0)
		{
			timer = &timer_list[id];
			break;
		}
	}

	if(timer == NULL)
		return -1;

	timer->interval = interval;
	timer->timeout = readMsTicks() + interval;
	timer->oneshot = oneshot;
	timer->func = func;
	timer->arg = arg;
	timer->used = 1;

	return id;
}

int del_timer(int id)
{
	if(id >=0 && id < MAX_POLL_TIMER)
	{
		if(timer_list[id].used)
		{
			memset(&timer_list[id], 0, sizeof(struct poll_timer));
			return 0;
		}
	}

	return -1;
}

void del_timer_all(void)
{
	int id;
	for(id = 0; id < MAX_POLL_TIMER; id++)
	{
		memset(&timer_list[id], 0, sizeof(struct poll_timer));
	}
}

void polling_timer(void)
{
	int id;

	for(id=0; id<MAX_POLL_TIMER; id++)
	{
		if(timer_list[id].used)
		{
			unsigned int current = readMsTicks();

			if(current >= timer_list[id].timeout)
			{
				timer_list[id].func(timer_list[id].arg);
				
				if(timer_list[id].oneshot)
				{
					// if timer is expired in oneshot mode, delete it
					del_timer(id);
				}
				else
				{
					// next timeout setting
					timer_list[id].timeout = readMsTicks() + timer_list[id].interval;
				}
			}
	
		}
	}
}

/*
If oneshot timer remained in timer list, we will wait until the timer is expired.
This function will be used before jumping to other program like kernel
*/


void wait_oneshot_timer(void)
{
	int id;
	int remained;

	while(1)
	{
		remained = 0;

		for(id=0; id<MAX_POLL_TIMER; id++)
		{
			if(timer_list[id].used && timer_list[id].oneshot)
			{
				unsigned int current = readMsTicks();

				if(current >= timer_list[id].timeout)
				{
					timer_list[id].func(timer_list[id].arg);
					del_timer(id);
				}
				remained = 1;
			}
		}

		if(!remained)
			break;

		udelay(1000);
	}

}

