#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

typedef struct expFilter_s {
	int64_t state;
	int gain;
	int precision;
}expFilter_t;

int32_t averageFilter(int32_t *val, int size);
void expFilterInit(expFilter_t *filter, int gain, int precision);
void expFilterUpdate(expFilter_t *filter, int64_t val);

#endif
