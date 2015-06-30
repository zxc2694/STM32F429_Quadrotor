#include "QuadCopterConfig.h"

typedef struct {
	unsigned int lenHigh;
	unsigned int lenLow;
	unsigned int d;
} Ultrasonic_t;

extern Ultrasonic_t Ultrasonic;

void print_us100_distance();