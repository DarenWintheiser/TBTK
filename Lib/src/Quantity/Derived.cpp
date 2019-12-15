/* Copyright 2019 Kristofer Björnson
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

/** @file Derived.cpp
 *
 *  @author Kristofer Björnson
 */

#include "TBTK/Quantity/Derived.h"

using namespace std;

namespace TBTK{
namespace Quantity{

//Mass.
constexpr double kg_per_baseMass = 1.602176634e-19;
constexpr double u_per_baseMass = 2.99792458e8*2.99792458e8/9.31494095e8;
template<>
Mass::ConversionTable Mass::conversionTable({
	{Mass::Unit::kg,	{"kg",	kg_per_baseMass}},
	{Mass::Unit::g,		{"g",	kg_per_baseMass*1e3}},
	{Mass::Unit::mg,	{"mg",	kg_per_baseMass*1e6}},
	{Mass::Unit::ug,	{"ug",	kg_per_baseMass*1e9}},
	{Mass::Unit::ng,	{"ng",	kg_per_baseMass*1e12}},
	{Mass::Unit::pg,	{"pg",	kg_per_baseMass*1e15}},
	{Mass::Unit::fg,	{"fg",	kg_per_baseMass*1e18}},
	{Mass::Unit::ag,	{"ag",	kg_per_baseMass*1e21}},
	{Mass::Unit::ag,	{"u",	u_per_baseMass*1e21}}
});

//MagneticField.
constexpr double T_per_baseMagneticField = 1.602176634e-19;
constexpr double G_per_baseMagneticField = 1.602176634e-19*1e4;
template<>
MagneticField::ConversionTable MagneticField::conversionTable({
	{MagneticField::Unit::MT,	{"MT",	T_per_baseMagneticField*1e-6}},
	{MagneticField::Unit::kT,	{"kT",	T_per_baseMagneticField*1e-3}},
	{MagneticField::Unit::T,	{"T",	T_per_baseMagneticField}},
	{MagneticField::Unit::mT,	{"mT",	T_per_baseMagneticField*1e3}},
	{MagneticField::Unit::uT,	{"uT",	T_per_baseMagneticField*1e6}},
	{MagneticField::Unit::nT,	{"nT",	T_per_baseMagneticField*1e9}},
	{MagneticField::Unit::GG,	{"GG",	G_per_baseMagneticField*1e-9}},
	{MagneticField::Unit::MG,	{"MG",	G_per_baseMagneticField*1e-6}},
	{MagneticField::Unit::kG,	{"kG",	G_per_baseMagneticField*1e-3}},
	{MagneticField::Unit::G,	{"G",	G_per_baseMagneticField}},
	{MagneticField::Unit::mG,	{"mG",	G_per_baseMagneticField*1e3}},
	{MagneticField::Unit::uG,	{"uG",	G_per_baseMagneticField*1e6}}
});

//Voltage.
constexpr double V_per_baseVoltage = 1.602176634e-19;
template<>
Voltage::ConversionTable Voltage::conversionTable({
	{Voltage::Unit::GV,	{"GV",	V_per_baseVoltage*1e-9}},
	{Voltage::Unit::MV,	{"MV",	V_per_baseVoltage*1e-6}},
	{Voltage::Unit::kV,	{"kV",	V_per_baseVoltage*1e-3}},
	{Voltage::Unit::V,	{"V",	V_per_baseVoltage}},
	{Voltage::Unit::mV,	{"mV",	V_per_baseVoltage*1e3}},
	{Voltage::Unit::uV,	{"uV",	V_per_baseVoltage*1e6}},
	{Voltage::Unit::nV,	{"nV",	V_per_baseVoltage*1e9}},
});

//Velocity
template<>
Velocity::ConversionTable Velocity::conversionTable({});

//Planck
template<>
Planck::ConversionTable Planck::conversionTable({});

//Boltzmann
template<>
Boltzmann::ConversionTable Boltzmann::conversionTable({});

//Permeability
template<>
Permeability::ConversionTable Permeability::conversionTable({});

//Permittivity
template<>
Permittivity::ConversionTable Permittivity::conversionTable({});

//Magneton
template<>
Magneton::ConversionTable Magneton::conversionTable({});

};	//End of namespace Quantity
};	//End of namespace TBTK
