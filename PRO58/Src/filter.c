
#include "filter.h"

float averageFilter(int *val, int size){
	float res = 0;
	for(int i = 0; i<size; i++){
		res += val[i]/(float)size;
	}
	return res;
}

void expFilterInit(expFilter_t *filter, float gain){
	filter->gain=gain;
	filter->state=0;
}
void expFilterUpdate(expFilter_t *filter, int val){
	filter->state=(1-filter->gain)*filter->state + filter->gain*val;
}



