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

/** \file os/posix/code/os_time.c
 *  \brief Time management
 *
 * Implements posix based time management functions
 */

#include "os_report.h"

#include <time.h>
#include <sys/time.h>
#include <errno.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

static os_time (*clockGet)(void) = NULL;

/** \brief Get the current time
 *
 * \b os_timeGet gets the current time by calling 
 * \b clock_gettime with clock ID \b CLOCK_REALTIME
 * and converting the result in \b struct
 * \b timespec format into \b os_time format.
 */
os_time
os_timeGet (
    void)
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

/** \brief Set the user clock
 *
 * \b os_timeSetUserClock sets the current time source
 * get function.
 */
void
os_timeSetUserClock (
    os_time (*userClock)(void)
    )
{
    clockGet = userClock;
}

/** \brief Suspend the execution of the calling thread for the specified time
 *
 * \b os_nanoSleep suspends the calling thread for the required
 * time by calling \b nanosleep. First it converts the \b delay in
 * \b os_time definition into a time in \b struct \b timeval definition.
 * In case the \b nanosleep is interrupted, the call is re-entered with
 * the remaining time.
 */
os_result
os_nanoSleep (
    os_time delay)
{
    struct timespec t;
    struct timespec r;
    int result;
    os_result rv;

    assert (delay.tv_nsec >= 0);
    assert (delay.tv_nsec < 1000000000);
    if( delay.tv_sec >= 0 ) {
        t.tv_sec = delay.tv_sec;
        t.tv_nsec = delay.tv_nsec;
        result = nanosleep (&t, &r);
        while (result && errno == EINTR) {
            t = r;
            result = nanosleep (&t, &r);
        }
        if (result == 0) {
            rv = os_resultSuccess;
        } else {
            rv = os_resultFail;
        }
    } else {
        /* Negative time-interval gives an illegal param error in most posix implementations.
         * However, VxWorks casts it to an unsigned int, and waits for years.
         */
        rv = os_resultFail;
    }
    return rv;
}
