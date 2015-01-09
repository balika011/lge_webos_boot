#ifndef _BGTASK_POWSEQ_H_
#define _BGTASK_POWSEQ_H_


#define	CHECK_POWSEQ(_a,_p)				( (_p.next_seq		== (_a)) &&			\
										  (get_cur_time()	>= _p.next_time)	)
#define	SET_NEXT_POWSEQ(_a,_t,_p)		{ _p.next_seq 		= (_a);				\
										  _p.next_time		= get_cur_time()+(_t);}
#define	SET_NEXT_POWSEQ_BYTIME(_a,_t,_p) { _p.next_seq		= (_a);				\
										   _p.next_time		= (_t);				}
#define	CHECK_FORCED_DELAY(_e,_t,_p)	{										\
				if (_e == BGTASK_END && (_t = get_cur_time()) < _p.next_time)	\
					mdelay(_p.next_time - _t);									}

typedef unsigned int	UINT32;

typedef enum
{
	BGTASK_CONT					= 0,
	BGTASK_START				= 1,
	POWSEQ_PANEL				= 1,
	POWSEQ_PWM					= 2,
	POWSEQ_SCALER_SOE_POL		= 3,
	POWSEQ_GPIO_DPM_HCONV		= 4,
	POWSEQ_SCALER_VDD_EVEN_ODD	= 5,
	POWSEQ_SCALER_VST			= 6,
	POWSEQ_SCALER_GCLK			= 7,
	POWSEQ_MFC					= 8,
	POWSEQ_INVERTER				= 9,
	//
	BGTASK_END					= 100
} BGTASK_POWSEQ_FLAG;


typedef struct
{
	BGTASK_POWSEQ_FLAG		next_seq;
	UINT32					next_time;
} POWSEQ_TIME_t;


extern	POWSEQ_TIME_t	S7_PowerSeq;

void PRINT_POWSEQ(void);
void BG_PowerSeqTask(BGTASK_POWSEQ_FLAG eFlag);

#endif /* _BGTASK_POWSEQ_H_ */

