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
	maxXY = x[0] = x0;
	maxXY = max(maxXY, y[0] = y0);

	for(int i=0; i+1<n; ++i) {
		x[i+1] = max(0.0, x[i] + h * ( -a*x[i] + c*d*x[i]*y[i] ));
		maxXY = max(maxXY, x[i+1]);
		y[i+1] = max(0.0, y[i] + h * ( b*y[i] - d*x[i]*y[i] ));
		maxXY = max(maxXY, y[i+1]);
	}
}

void RealLife::setA(double val) {
	this->a = val;
	calculate();
}
void RealLife::setB(double val) {
	this->b = val;
	calculate();
}
void RealLife::setC(double val) {
	this->c = val;
	calculate();
}
void RealLife::setD(double val) {
	this->d = val;
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
	delete [] x;
	x = new double[n];
	delete [] y;
	y = new double[n];
	calculate();
}
void RealLife::setH(double val) {
	this->h = val;
	calculate();
}
double RealLife::getX(int i) {
	return this->x[i];
}
double RealLife::getY(int i) {
	return this->y[i];
}
int RealLife::getN() {
	return n;
}
double RealLife::getMaxXY() {
	return this->maxXY;
}
