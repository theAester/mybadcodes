#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <getopt.h>
#include <locale.h>

#define MPROCESSTIME 0
#define MREALTIME 1
#define MCOUNT 2

int calc_count(const char* str,long long int* out){
	if(out == NULL) return -1; 
	errno =0;
	char* endstr;
	int temp = strtoll(str, &endstr, 0);
	if(errno == 0 && *endstr == '\0'){
		*out = temp;
		return 0;
	}
	return errno;
}

void print_usage(const char* appname){
	printf("Usage: %s [OPTIONS]\n"
		"OPTIONS:\n\t"
		"-r: realtime timer\n\t"
		"-p: process cpu time counter\n\t"
		"-n [count]: record time for \"count\" simple arithmetic instructtions\n\n"
		"A simple arithmetic instruction consists of 3 additions, 1 multipication, 1 comparison and 0-1 divisions\n"
		"More precisely:\n"
		"\n"
		"   accum += 7;\n"
		"   accum *= accum -3;\n"
		"   if(accum > 1000000007)\n"
		"      accum %%= 1000000007;\n"
		"   ++count;\n", appname);
	exit(0);
}

struct passed_data{
	timer_t* timerptr;
	long long int* instptr;
};

static void timer_end(int sig, siginfo_t* si, void* uc){
	timer_t timerid = *(((struct passed_data*)si->si_value.sival_ptr)->timerptr);
	long long int instcnt = *(((struct passed_data*)si->si_value.sival_ptr)->instptr);
	setlocale(LC_NUMERIC, "");
	printf("Results: ran %'lld simple arithmetic instructions in 1 second\n", instcnt);
	exit(0);
}

int main(int argc, char* argv[]){
	int c;
	int method = 0;
	long long int countlim = 0;
	while((c = getopt(argc, argv, "rphn:")) != -1){
		switch(c){
			case 'r':
				method = MREALTIME;
			break;
			case 'n':
				if(calc_count(optarg, &countlim) != 0){
					fprintf(stderr, "Error. %s is not a numeric value.\n See -h.\n", optarg);
					exit(1);
				}	
				method = MCOUNT;
			break;
			case 'h':
			case '?':
				print_usage(argv[0]);
				exit(0);
		}
	}
	int accum=0;
	if(method == MCOUNT){
		clock_t start, end;
		for(int i=0, start = clock(); i<countlim; ++i){
			accum += 7;
			accum *= accum -3;
			if(accum > 1000000007)
				accum %= 1000000007;
		}
		end = clock();
		double time = ((double)(end-start)/CLOCKS_PER_SEC);
		setlocale(LC_NUMERIC, "");
		printf("Results: performed %'lld simple arithmetic instructions in %lf seconds\n", countlim, time);
		exit(0);
	}else{
		timer_t timerid;
		long long int count;
		struct passed_data pd={
			.timerptr = &timerid,
			.instptr= &count
		};
		struct sigevent se={
			.sigev_notify = SIGEV_SIGNAL,
			.sigev_signo = SIGALRM,
			.sigev_value.sival_ptr = &pd,
		};
		const struct itimerspec ps={
			.it_interval = { .tv_sec = 0, .tv_nsec = 0},
			.it_value = { .tv_sec = 1, .tv_nsec = 0}
		};
		if(timer_create((method == MREALTIME ? CLOCK_REALTIME : CLOCK_PROCESS_CPUTIME_ID), 
				  &se, &timerid) == -1){
		exit(1);
		}
		struct sigaction sa={
			.sa_flags = SA_SIGINFO,
			.sa_sigaction = timer_end
		};
		if(sigaction(SIGALRM, &sa, NULL) == -1){
		exit(3);
		}
		if( timer_settime(timerid, 0, &ps, NULL) == -1){
			fprintf(stderr, "bruh\n");
		exit(1);
		}
		while(1){
			accum += 7;
			accum *= accum -3;
			if(accum > 1000000007)
				accum %= 1000000007;
			++count;
		}
	}
}
