//Dustin Gehm

#include <math.h>

#include "AI.h"
#include "Utils.h"

using std::pow;

double sigmoid(double value){
	int k = 2;

	return 1.0 / (1.0 + pow(EULER, (-k * value)));
}

double sigmoidDeriv(double value){
	return sigmoid(value) * (1 - sigmoid(value));
}

int crispify(double value){
	if (value > .5){
		return 1;
	}
	else{
		return 0;
	}
}