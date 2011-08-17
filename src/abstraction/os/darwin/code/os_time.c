/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2010 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */

/** \file os/linux/code/os_time.c
 *  \brief Linux time management
 *
 * Implements time management for Linux
 * by including the common services and
 * the SVR4 os_timeGet implementation and
 * the POSIX os_nanoSleep implementation
 */

#include "../common/code/os_time.c"
#include "../posix/code/os_time.c"

/** \brief Translate calendar time into readable string representation
 *
 * ctime_r provides a re-entrant translation function.
 * ctime_r function adds '\n' to the string which must be removed.
 */
char *
os_ctime_r (
    os_time *t,
    char *buf)
{
    time_t tt = t->tv_sec;
    if (buf) {
        ctime_r(&tt, buf);
    }
    return buf;
}

/** \brief Get high resolution relative time
 *
 */
os_time
os_hrtimeGet (
    void
    )
{
  os_time rt;
	#ifdef __MACH__
	    // TODO: Check these mach return values
  		clock_serv_t cclock;
  	  mach_timespec_t mts;
  	  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  	  clock_get_time(cclock, &mts);
  	  mach_port_deallocate(mach_task_self(), cclock);
  	  rt.tv_sec = mts.tv_sec;
  	  rt.tv_nsec = mts.tv_nsec;
	#else
	    struct timespec t;
	    int result;

	    if (clockGet) {
	        rt = clockGet ();
	    } else {
	        result = clock_gettime (CLOCK_REALTIME, &t);
	        if (result == 0) {
		    rt.tv_sec = t.tv_sec;
		    rt.tv_nsec = t.tv_nsec;
	        } else {
		    OS_REPORT_1 (OS_WARNING, "os_timeGet", 1, "clock_gettime failed with error %d", errno);
		    rt.tv_sec = 0;
		    rt.tv_nsec = 0;
	        } 
	    } 
	#endif
  return rt;
}
