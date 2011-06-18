/*
 * RealLife.cpp
 *
 *  Created on: 18-06-2011
 *      Author: konrad
 */

#include "RealLife.h"

template<class T> T min(T arg1, T arg2) { return arg1<arg2?arg1:arg2; }
template<class T> T max(T arg1, T arg2) { return arg1<arg2?arg2:arg1; }

RealLife::RealLife() {
	d = 0.008;
	c = 0.07;
	b = 0.1;
	a = 0.1;
	y0 = 1000;
	x0 = 1;
	n = 10000;
	h = 0.1;

	x = new double[n];
	y = new double[n];
}

RealLife::~RealLife() {
	if(x) delete [] x;
	if(y) delete [] y;
	x = y = 0;
}

void RealLife::calculate() {
	xmax = x[0] = x0;
	y[0] = y0;

	for(int i=0; i+1<n; ++i) {
		x[i+1] = max(0.0, x[i] + h * ( -a*x[i] + c*d*x[i]*y[i] ));
		xmax = max(xmax, x[i+1]);
		y[i+1] = max(0.0, y[i] + h * ( b*y[i] - d*x[i]*y[i] ));
		xmax = max(xmax, y[i+1]);
	}
}

void RealLife::setA(int val) {
	this->a = (double)val/1000.0;
	calculate();
}
void RealLife::setB(int val) {
	this->b = (double)val/1000.0;
	calculate();
}
void RealLife::setC(int val) {
	this->c = (double)val/1000.0;
	calculate();
}
void RealLife::setD(int val) {
	this->d = (double)val/1000.0;
	calculate();
}
void RealLife::setX0(int val) {
	this->x0 = val;
	calculate();
}
void RealLife::setY0(int val) {
	this->y0 = val;
	calculate();
}
void RealLife::setN(int val) {
	this->n = val;
	calculate();
}
void RealLife::setH(int val) {
	this->h = (double)val/1000.0;
	calculate();
}
