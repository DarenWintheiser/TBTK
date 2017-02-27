/* Copyright 2016 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file SpinPolarizedLDOS.h
 *
 *  @author Kristofer Björnson
 */

#include "SpinPolarizedLDOS.h"

using namespace std;

namespace TBTK{
namespace Property{

SpinPolarizedLDOS::SpinPolarizedLDOS(
	int dimensions,
	const int *ranges,
	double lowerBound,
	double upperBound,
	int resolution
){
	this->dimensions = dimensions;
	this->ranges = new int[dimensions];
	for(int n = 0; n < dimensions; n++)
		this->ranges[n] = ranges[n];

	this->lowerBound = lowerBound;
	this->upperBound = upperBound;
	this->resolution = resolution;

	size = 4*resolution;
	for(int n = 0; n < dimensions; n++)
		size *= ranges[n];

	data = new complex<double>[size];
	for(int n = 0; n < size; n++)
		data[n] = 0.;
}

SpinPolarizedLDOS::SpinPolarizedLDOS(
	int dimensions,
	const int *ranges,
	double lowerBound,
	double upperBound,
	int resolution,
	const complex<double> *data
){
	this->dimensions = dimensions;
	this->ranges = new int[dimensions];
	for(int n = 0; n < dimensions; n++)
		this->ranges[n] = ranges[n];

	this->lowerBound = lowerBound;
	this->upperBound = upperBound;
	this->resolution = resolution;

	size = 4*resolution;
	for(int n = 0; n < dimensions; n++)
		size *= ranges[n];

	this->data = new complex<double>[size];
	for(int n = 0; n < size; n++)
		this->data[n] = data[n];
}

SpinPolarizedLDOS::SpinPolarizedLDOS(
	const SpinPolarizedLDOS &spinPolarizedLDOS
){
	dimensions = spinPolarizedLDOS.dimensions;
	ranges = new int[dimensions];
	for(int n = 0; n < dimensions; n++)
		ranges[n] = spinPolarizedLDOS.ranges[n];

	lowerBound = spinPolarizedLDOS.lowerBound;
	upperBound = spinPolarizedLDOS.upperBound;
	resolution = spinPolarizedLDOS.resolution;

	size = spinPolarizedLDOS.size;

	data = new complex<double>[size];
	for(int n = 0; n < size; n++)
		data[n] = spinPolarizedLDOS.data[n];
}

SpinPolarizedLDOS::SpinPolarizedLDOS(
	SpinPolarizedLDOS &&spinPolarizedLDOS
){
	dimensions = spinPolarizedLDOS.dimensions;
	ranges = spinPolarizedLDOS.ranges;
	spinPolarizedLDOS.ranges = nullptr;

	lowerBound = spinPolarizedLDOS.lowerBound;
	upperBound = spinPolarizedLDOS.upperBound;
	resolution = spinPolarizedLDOS.resolution;

	size = spinPolarizedLDOS.size;

	data = spinPolarizedLDOS.data;
	spinPolarizedLDOS.data = nullptr;
}

SpinPolarizedLDOS::~SpinPolarizedLDOS(){
	if(ranges != nullptr)
		delete [] ranges;
	if(data != nullptr)
		delete [] data;
}

SpinPolarizedLDOS& SpinPolarizedLDOS::operator=(const SpinPolarizedLDOS &rhs){
	dimensions = rhs.dimensions;
	ranges = new int[dimensions];
	for(int n = 0; n < dimensions; n++)
		ranges[n] = rhs.ranges[n];

	lowerBound = rhs.lowerBound;
	upperBound = rhs.upperBound;
	resolution = rhs.resolution;

	size = rhs.size;

	data = new complex<double>[size];
	for(int n = 0; n < size; n++)
		data[n] = rhs.data[n];

	return *this;
}

SpinPolarizedLDOS& SpinPolarizedLDOS::operator=(SpinPolarizedLDOS &&rhs){
	if(this != &rhs){
		dimensions = rhs.dimensions;
		ranges = rhs.ranges;
		rhs.ranges = nullptr;

		lowerBound = rhs.lowerBound;
		upperBound = rhs.upperBound;
		resolution = rhs.resolution;

		size = rhs.size;

		data = rhs.data;
		rhs.data = nullptr;
	}

	return *this;
}

};	//End of namespace Property
};	//End of namespace TBTK
