
#include "utils.h"

float fastLog(float x) {
	union { float f; unsigned int i; } vx = { x };
	float y = vx.i;
	y *= 8.2629582881927490e-8f;
	return y - 87.989971088f;
}

float fastInvSqrt(float x) {
	float xhalf = x * 0.5f;
	int i = *(int*)& x;// get bits for floating value
	i = 0x5f3759df - (i >> 1); // gives initial guess
	x = *(float*)& i; // convert bits back to float
	x = x * (1.5f - xhalf * x * x); // Newton step
	x = x * (1.5f - xhalf * x * x); // Newton step
	return x;
}
