/* required for Linux clock_gettime */
#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif /* __STDC_VERSION__ */

#include <atto/app.h>
#include <time.h>

static struct timespec a__time_start = {0, 0};

ATimeMs aAppTime(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	if (a__time_start.tv_sec == 0 && a__time_start.tv_nsec == 0) a__time_start = ts;
	return
		(ts.tv_sec - a__time_start.tv_sec) * 1000 +
		(ts.tv_nsec - a__time_start.tv_nsec) / 1000000;
}
