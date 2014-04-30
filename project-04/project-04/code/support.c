/*
 * Josh Hursey
 * Nov. 2, 2012
 *
 * CS 441/541 : Memory Management Performance Template
 *
 */
#include "support.h"

int PAGESIZE;

static int initialized = FALSE;

#if defined(__APPLE__)
static double get_conversion();
#else
#ifdef CLOCK_REALTIME
static double get_timespec_diff_as_double( struct timespec start, struct timespec end);
static struct timespec get_timespec_diff_as_timespec( struct timespec start, struct timespec end);
static double timespec_to_double(struct timespec ctime);
#else
static struct timeval get_timeval_diff_as_timeval(struct timeval start, struct timeval end);
static double get_timeval_diff_as_double(struct timeval start, struct timeval *given_end);
static double timeval_to_double(struct timeval ctime);
#endif
#endif

int support_init(void) {
    int ret = 0;

    initialized = TRUE;

    // Note: If this line causes compilation problems
    // then replace the getpagesize() function call with
    // the value: 4096
    PAGESIZE = getpagesize();

    srandom(time(NULL));

    return ret;
}

int support_finalize(void) {
    int ret = 0;

    initialized = FALSE;

    return ret;
}

int allocate_matrix( mtype_t **matrix, int N ) {
    size_t alloc_size;

    // Page align the allocation
    alloc_size = sizeof(mtype_t) * N * N;
    if( alloc_size < PAGESIZE ) {
        alloc_size = PAGESIZE;
    } else {
        alloc_size = (alloc_size/PAGESIZE)*PAGESIZE + PAGESIZE;
    }

    /* Allocate Memory */
    *matrix  = (mtype_t*)malloc( alloc_size );
    if( NULL == *matrix ) {
        fprintf(stderr, "Error: Unable to allocate memory %d\n", __LINE__);
        return -1;
    }

    return 0;
}

void clear_matrix( mtype_t *matrix, int N) {
    int i, j;

    for(i = 0; i < N; ++i ) {
        for(j = 0; j < N; ++j ) {
            if( sizeof(mtype_t) == sizeof(double) ) {
                matrix[ GET_INDEX(i, j, N)] = (random() % 100) / 10.0;
            }
            else {
                matrix[ GET_INDEX(i, j, N)] = random() % 10;
            }
        }
    }
}

hptimer_t get_time() {
    hptimer_t t;
#if defined(__APPLE__)
    t.ctime = mach_absolute_time();
#else
#ifdef CLOCK_REALTIME
    clock_gettime(CLOCK_REALTIME, &t.ctime);
#else
    gettimeofday(&t.ctime, NULL);
#endif
#endif
    return t;
}

// Diff timers in seconds
double diff_timers(hptimer_t start, hptimer_t end) {
#if defined(__APPLE__)
    uint64_t difference = end.ctime - start.ctime;
    static double conversion = 0.0;

    if( 0.0 == conversion ) {
        conversion = get_conversion();
    }    

    return conversion * (double) difference;
#else
#ifdef CLOCK_REALTIME
    return get_timespec_diff_as_double(start.ctime, end.ctime);
#else
    return get_timeval_diff_as_double(start.ctime, &end.ctime);
#endif
#endif
}

double timer_to_double(hptimer_t ctime) {
#if defined(__APPLE__)
    static double conversion = 0.0;

    if( 0.0 == conversion ) {
        conversion = get_conversion();
    }

    return conversion * (double) ctime.ctime;
#else
#ifdef CLOCK_REALTIME
    return timespec_to_double(ctime.ctime);
#else
    return timeval_to_double(ctime.ctime);
#endif
#endif
}

char * human_readable(int bytes) {
    char * str = NULL;
    char letter = 'B';

    if( bytes >= 1024 ) {
        letter = 'K';
        bytes = bytes / 1024;
    }
    if( bytes >= 1024 ) {
        letter = 'M';
        bytes = bytes / 1024;
    }
    if( bytes >= 1024 ) {
        letter = 'G';
        bytes = bytes / 1024;
    }

    asprintf(&str, "%5d %c", bytes, letter);

    return str;
}

/********************* Local Support **************************/
#if defined(__APPLE__)
static double get_conversion() {
    static double conversion = 0.0;

    if( 0.0 == conversion ) {
        mach_timebase_info_data_t info;
        kern_return_t err = mach_timebase_info( &info );

        //Convert the timebase into seconds
        if( err == 0  ) {
            conversion = 1e-9 * (double) info.numer / (double) info.denom;
        }
    }

    return conversion;
}

#else
#ifdef CLOCK_REALTIME
static double timespec_to_double(struct timespec ctime) {
    if( FALSE == initialized ) {
        fprintf(stderr, "Warning: You forgot to call support_init() before calling timespec_to_double()\n");
        support_init();
    }

    return (ctime.tv_sec + (ctime.tv_nsec/(1.0 + TIME_NSEC)));
}

static struct timespec get_timespec_diff_as_timespec(struct timespec start, struct timespec end) {
    struct timespec loc_diff;

    if( FALSE == initialized ) {
        fprintf(stderr, "Warning: You forgot to call support_init() before calling get_timespec_diff_as_timespec()\n");
        support_init();
    }

    /* Check to see if we have to carry a second over */
    if( end.tv_nsec < start.tv_nsec ) {
        loc_diff.tv_nsec = (TIME_NSEC - start.tv_nsec) + end.tv_nsec;
        end.tv_sec -= 1;
    } else {
        loc_diff.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    loc_diff.tv_sec = end.tv_sec - start.tv_sec;

    return loc_diff;
}

static double get_timespec_diff_as_double(struct timespec start, struct timespec end) {
    struct timespec loc_diff;

    if( FALSE == initialized ) {
        fprintf(stderr, "Warning: You forgot to call support_init() before calling get_timespec_diff_as_double()\n");
        support_init();
    }

    loc_diff = get_timespec_diff_as_timespec(start, end);

    return timespec_to_double(loc_diff);
}

#else

static double timeval_to_double(struct timeval ctime) {
    if( FALSE == initialized ) {
        fprintf(stderr, "Warning: You forgot to call support_init() before calling timeval_to_double()\n");
        support_init();
    }

    return (ctime.tv_sec + (ctime.tv_usec/(1.0 + TIME_USEC)));
}

static struct timeval get_timeval_diff_as_timeval(struct timeval start, struct timeval end) {
    struct timeval loc_diff;

    if( FALSE == initialized ) {
        fprintf(stderr, "Warning: You forgot to call support_init() before calling get_timeval_diff_as_timeval()\n");
        support_init();
    }

    /* Check to see if we have to carry a second over */
    if( end.tv_usec < start.tv_usec ) {
        loc_diff.tv_usec = (TIME_USEC - start.tv_usec) + end.tv_usec;
        end.tv_sec -= 1;
    } else {
        loc_diff.tv_usec = end.tv_usec - start.tv_usec;
    }

    loc_diff.tv_sec = end.tv_sec - start.tv_sec;

    return loc_diff;
}

static double get_timeval_diff_as_double(struct timeval start, struct timeval *given_end) {
    struct timeval loc_diff, end;

    if( FALSE == initialized ) {
        fprintf(stderr, "Warning: You forgot to call support_init() before calling get_timeval_diff_as_double()\n");
        support_init();
    }

    if( NULL == given_end ) {
        gettimeofday(&end, NULL);
    } else {
        end.tv_sec  = given_end->tv_sec;
        end.tv_usec = given_end->tv_usec;
    }

    loc_diff = get_timeval_diff_as_timeval(start, end);

    return timeval_to_double(loc_diff);
}
#endif
#endif
