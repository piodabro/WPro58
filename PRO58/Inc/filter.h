#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

typedef struct expFilter_s {
	float state;
	float gain;
}expFilter_t;

float averageFilter(int *val, int size);
void expFilterInit(expFilter_t *filter, float gain);
void expFilterUpdate(expFilter_t *filter, int val);

#endif
