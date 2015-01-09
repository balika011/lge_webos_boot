
#ifndef _CFE_POLLTIMER_H_
#define _CFE_POLLTIMER_H_

#define MAX_POLL_TIMER	4


struct poll_timer
{
	unsigned char		used;
	unsigned int	timeout;
	unsigned int	interval;	/* 1ms unit */
	unsigned int	oneshot;
	void		(*func)(void*);
	void		*arg;
};

static struct poll_timer timer_list[MAX_POLL_TIMER];


extern int add_timer(unsigned int interval, unsigned int oneshot, void (*func)(void*), void *arg);
extern int del_timer(int id);
extern void polling_timer(void);
extern void wait_oneshot_timer(void);
extern void del_timer_all(void);

#endif
