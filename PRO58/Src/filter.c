
#include "filter.h"
#include <assert.h>

int32_t averageFilter(int32_t *val, int size){
	int64_t res = 0;
	for(int i = 0; i<size; i++){
		res += val[i];
	}
	return res/size;
}

void expFilterInit(expFilter_t *filter, int gain, int precision){
	assert(gain<= precision);
	filter->gain=gain;
	filter->state=0;
	filter->precision=precision;
}
void expFilterUpdate(expFilter_t *filter, int64_t val){
	filter->state=((filter->precision - filter->gain)*filter->state + filter->gain*val)/filter->precision;
}



