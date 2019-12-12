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

/** @file UnitHandler.cpp
 *
 *  @author Kristofer Björnson
 */

#include "TBTK/Streams.h"
#include "TBTK/TBTKMacros.h"
#include "TBTK/UnitHandler.h"

#include <iostream>
#include <sstream>

#ifdef M_E	//Avoid name clash with math.h macro M_E
	#define M_E_temp M_E
	#undef M_E
#endif

using namespace std;

namespace TBTK{

map<
	string,
	pair<double, vector<pair<string, int>>>
> UnitHandler::constantsDefaultUnits;
map<string, double> UnitHandler::constantsBaseUnits;

/*double UnitHandler::hbar	= HBAR;
double UnitHandler::k_B		= K_B;
double UnitHandler::e		= E;
double UnitHandler::c		= C;
double UnitHandler::n_A		= N_A;
double UnitHandler::m_e		= M_E;
double UnitHandler::m_p		= M_P;
double UnitHandler::mu_B	= MU_B;
double UnitHandler::mu_n	= MU_N;
double UnitHandler::mu_0	= MU_0;
double UnitHandler::epsilon_0	= EPSILON_0;
double UnitHandler::a_0		= A_0;*/

double UnitHandler::J_per_eV;
double UnitHandler::eV_per_J;
double UnitHandler::kg_per_baseMass;
double UnitHandler::baseMass_per_kg;
double UnitHandler::u_per_baseMass;
double UnitHandler::baseMass_per_u;
double UnitHandler::T_per_baseMagneticField;
double UnitHandler::baseMagneticField_per_T;
double UnitHandler::V_per_baseVoltage;
double UnitHandler::baseVoltage_per_V;

UnitHandler::TemperatureUnit 	UnitHandler::temperatureUnit	= UnitHandler::TemperatureUnit::K;
UnitHandler::TimeUnit 		UnitHandler::timeUnit		= UnitHandler::TimeUnit::s;
UnitHandler::LengthUnit		UnitHandler::lengthUnit		= UnitHandler::LengthUnit::m;
UnitHandler::EnergyUnit		UnitHandler::energyUnit		= UnitHandler::EnergyUnit::eV;
UnitHandler::ChargeUnit		UnitHandler::chargeUnit		= UnitHandler::ChargeUnit::C;
UnitHandler::CountUnit		UnitHandler::countUnit		= UnitHandler::CountUnit::pcs;

double UnitHandler::temperatureScale	= 1.;
double UnitHandler::timeScale		= 1.;
double UnitHandler::lengthScale		= 1.;
double UnitHandler::energyScale		= 1.;
double UnitHandler::chargeScale		= 1.;
double UnitHandler::countScale		= 1.;

UnitHandler::StaticConstructor  UnitHandler::staticConstructor;

double UnitHandler::getConstantBaseUnits(const std::string &name){
	return constantsBaseUnits[name];
}

double UnitHandler::getConstantNaturalUnits(const std::string &name){
	double value = getConstantBaseUnits(name);

	const vector<pair<string, int>> &units = constantsDefaultUnits[name].second;
	for(unsigned int n = 0; n < units.size(); n++){
		const string &unit = units[n].first;
		int exponent = units[n].second;
		for(int c = 0; c < exponent; c++){
			if(unit.compare("K") == 0){
				value /= temperatureScale;
			}
			else if(unit.compare("s") == 0){
				value /= timeScale;
			}
			else if(unit.compare("m") == 0){
				value /= lengthScale;
			}
			else if(unit.compare("eV") == 0){
				value /= energyScale;
			}
			else if(unit.compare("C") == 0){
				value /= chargeScale;
			}
			else if(unit.compare("pcs") == 0){
				value /= countScale;
			}
			else{
				TBTKExit(
					"UnitHandler::getConstantNaturalUnits()",
					"Unknown default unit.",
					"This should never happen,"
					<< " contact the developer."
				);
			}
		}
		for(int c = 0; c < -exponent; c++){
			if(unit.compare("K") == 0){
				value *= temperatureScale;
			}
			else if(unit.compare("s") == 0){
				value *= timeScale;
			}
			else if(unit.compare("m") == 0){
				value *= lengthScale;
			}
			else if(unit.compare("eV") == 0){
				value *= energyScale;
			}
			else if(unit.compare("C") == 0){
				value *= chargeScale;
			}
			else if(unit.compare("pcs") == 0){
				value *= countScale;
			}
			else{
				TBTKExit(
					"UnitHandler::getConstantNaturalUnits()",
					"Unknown default unit.",
					"This should never happen,"
					<< " contact the developer."
				);
			}
		}
	}

	return value;
}

void UnitHandler::setTemperatureUnit(TemperatureUnit unit){
	double oldConversionFactor = getTemperatureConversionFactor();
	temperatureUnit = unit;
	double newConversionFactor = getTemperatureConversionFactor();
	temperatureScale *= newConversionFactor/oldConversionFactor;

//	updateK_B();
	updateConstants();
}

void UnitHandler::setTimeUnit(TimeUnit unit){
	double oldConversionFactor = getTimeConversionFactor();	
	timeUnit = unit;
	double newConversionFactor = getTimeConversionFactor();
	timeScale *= newConversionFactor/oldConversionFactor;

/*	updateHbar();
	updateC();
	updateM_e();
	updateM_p();
	updateMu_B();
	updateMu_n();
	updateMu_0();*/
	updateConstants();
}

void UnitHandler::setLengthUnit(LengthUnit unit){
	double oldConversionFactor = getLengthConversionFactor();
	lengthUnit = unit;
	double newConversionFactor = getLengthConversionFactor();
	lengthScale *= newConversionFactor/oldConversionFactor;

/*	updateC();
	updateM_e();
	updateM_p();
	updateMu_B();
	updateMu_n();
	updateMu_0();
	updateEpsilon_0();
	updateA_0();*/
	updateConstants();
}

void UnitHandler::setEnergyUnit(EnergyUnit unit){
	double oldConversionFactor = getEnergyConversionFactor();
	energyUnit = unit;
	double newConversionFactor = getEnergyConversionFactor();
	energyScale *= newConversionFactor/oldConversionFactor;

/*	updateHbar();
	updateK_B();
	updateM_e();
	updateM_p();
	updateMu_0();
	updateEpsilon_0();*/
	updateConstants();
}

void UnitHandler::setChargeUnit(ChargeUnit unit){
	double oldConversionFactor = getChargeConversionFactor();
	chargeUnit = unit;
	double newConversionFactor = getChargeConversionFactor();
	chargeScale *= newConversionFactor/oldConversionFactor;

/*	updateE();
	updateMu_B();
	updateMu_n();
	updateMu_0();
	updateEpsilon_0();*/
	updateConstants();
}

void UnitHandler::setCountUnit(CountUnit unit){
	double oldConversionFactor = getCountConversionFactor();
	countUnit = unit;
	double newConversionFactor = getCountConversionFactor();
	countScale *= newConversionFactor/oldConversionFactor;

//	updateN_A();
	updateConstants();
}

void UnitHandler::setTemperatureScale(double scale){
	temperatureScale = scale;
}

void UnitHandler::setTimeScale(double scale){
	timeScale = scale;
}

void UnitHandler::setLengthScale(double scale){
	lengthScale = scale;
}

void UnitHandler::setEnergyScale(double scale){
	energyScale = scale;
}

void UnitHandler::setChargeScale(double scale){
	chargeScale = scale;
}

void UnitHandler::setCountScale(double scale){
	countScale = scale;
}

void UnitHandler::setTemperatureScale(string scale){
	stringstream ss(scale);
	vector<string> components;
	string word;
	while(getline(ss, word, ' '))
		components.push_back(word);

	TBTKAssert(
		components.size() == 2,
		"UnitHandler::setTemperatureScale()",
		"Invalid temperature scale string '" << scale << "'.",
		"The string has to be on the format '[scale] [unit]', e.g."
		<< " '1 K'."
	);

	double s;
	try{
		s = stod(components[0]);
	}
	catch(const std::exception &e){
		TBTKExit(
			"UnitHandler::setTemperatureScale()",
			"Unable to parse '" << components[0] << "' as a"
			" double.",
			"The string has to be on the format '[scale] [unit]', e.g."
			<< " '1 K'."
		);
	}

	TemperatureUnit unit = getTemperatureUnit(components[1]);

	setTemperatureScale(s, unit);
}

void UnitHandler::setTimeScale(string scale){
	stringstream ss(scale);
	vector<string> components;
	string word;
	while(getline(ss, word, ' '))
		components.push_back(word);

	TBTKAssert(
		components.size() == 2,
		"UnitHandler::setTimeScale()",
		"Invalid time scale string '" << scale << "'.",
		"The string has to be on the format '[scale] [unit]', e.g."
		<< " '1 s'."
	);

	double s;
	try{
		s = stod(components[0]);
	}
	catch(const std::exception &e){
		TBTKExit(
			"UnitHandler::setTimeScale()",
			"Unable to parse '" << components[0] << "' as a"
			" double.",
			"The string has to be on the format '[scale] [unit]', e.g."
			<< " '1 s'."
		);
	}

	TimeUnit unit = getTimeUnit(components[1]);

	setTimeScale(s, unit);
}

void UnitHandler::setLengthScale(string scale){
	stringstream ss(scale);
	vector<string> components;
	string word;
	while(getline(ss, word, ' '))
		components.push_back(word);

	TBTKAssert(
		components.size() == 2,
		"UnitHandler::setLengthScale()",
		"Invalid length scale string '" << scale << "'.",
		"The string has to be on the format '[scale] [unit]', e.g."
		<< " '1 m'."
	);

	double s;
	try{
		s = stod(components[0]);
	}
	catch(const std::exception &e){
		TBTKExit(
			"UnitHandler::setLengthScale()",
			"Unable to parse '" << components[0] << "' as a"
			" double.",
			"The string has to be on the format '[scale] [unit]', e.g."
			<< " '1 m'."
		);
	}

	LengthUnit unit = getLengthUnit(components[1]);

	setLengthScale(s, unit);
}

void UnitHandler::setEnergyScale(string scale){
	stringstream ss(scale);
	vector<string> components;
	string word;
	while(getline(ss, word, ' '))
		components.push_back(word);

	TBTKAssert(
		components.size() == 2,
		"UnitHandler::setEnergyScale()",
		"Invalid energy scale string '" << scale << "'.",
		"The string has to be on the format '[scale] [unit]', e.g."
		<< " '1 eV'."
	);

	double s;
	try{
		s = stod(components[0]);
	}
	catch(const std::exception &e){
		TBTKExit(
			"UnitHandler::setEnergyScale()",
			"Unable to parse '" << components[0] << "' as a"
			" double.",
			"The string has to be on the format '[scale] [unit]', e.g."
			<< " '1 eV'."
		);
	}

	EnergyUnit unit = getEnergyUnit(components[1]);

	setEnergyScale(s, unit);
}

void UnitHandler::setChargeScale(string scale){
	stringstream ss(scale);
	vector<string> components;
	string word;
	while(getline(ss, word, ' '))
		components.push_back(word);

	TBTKAssert(
		components.size() == 2,
		"UnitHandler::setChargeScale()",
		"Invalid charge scale string '" << scale << "'.",
		"The string has to be on the format '[scale] [unit]', e.g."
		<< " '1 C'."
	);

	double s;
	try{
		s = stod(components[0]);
	}
	catch(const std::exception &e){
		TBTKExit(
			"UnitHandler::setChargeScale()",
			"Unable to parse '" << components[0] << "' as a"
			" double.",
			"The string has to be on the format '[scale] [unit]', e.g."
			<< " '1 C'."
		);
	}

	ChargeUnit unit = getChargeUnit(components[1]);

	setChargeScale(s, unit);
}

void UnitHandler::setCountScale(string scale){
	stringstream ss(scale);
	vector<string> components;
	string word;
	while(getline(ss, word, ' '))
		components.push_back(word);

	TBTKAssert(
		components.size() == 2,
		"UnitHandler::setCountScale()",
		"Invalid count scale string '" << scale << "'.",
		"The string has to be on the format '[scale] [unit]', e.g."
		<< " '1 pcs'."
	);

	double s;
	try{
		s = stod(components[0]);
	}
	catch(const std::exception &e){
		TBTKExit(
			"UnitHandler::setCountScale()",
			"Unable to parse '" << components[0] << "' as a"
			" double.",
			"The string has to be on the format '[scale] [unit]', e.g."
			<< " '1 pcs'."
		);
	}

	CountUnit unit = getCountUnit(components[1]);

	setCountScale(s, unit);
}

double UnitHandler::convertTemperatureAtB(
	double temperature,
	TemperatureUnit unit
){
	return temperature*getTemperatureConversionFactor(
		temperatureUnit
	)/getTemperatureConversionFactor(unit);
}

double UnitHandler::convertTemperatureBtA(
	double temperature,
	TemperatureUnit unit
){
	return temperature*getTemperatureConversionFactor(
		unit
	)/getTemperatureConversionFactor(temperatureUnit);
}

double UnitHandler::convertTemperatureAtN(
	double temperature,
	TemperatureUnit unit
){
	double temperatureInDefaultBaseUnits
		= temperature*getTemperatureConversionFactor(
			temperatureUnit
		)/getTemperatureConversionFactor(unit);

	return temperatureInDefaultBaseUnits/temperatureScale;
}

double UnitHandler::convertTemperatureNtA(
	double temperature,
	TemperatureUnit unit
){
	double temperatureInDefaultBaseUnits
		= temperature*temperatureScale/getTemperatureConversionFactor(
			temperatureUnit
		);
	return temperatureInDefaultBaseUnits*getTemperatureConversionFactor(unit);
}

double UnitHandler::convertTimeAtB(
	double time,
	TimeUnit unit
){
	return time*getTimeConversionFactor(timeUnit)/getTimeConversionFactor(unit);
}

double UnitHandler::convertTimeBtA(
	double time,
	TimeUnit unit
){
	return time*getTimeConversionFactor(unit)/getTimeConversionFactor(
		timeUnit
	);
}

double UnitHandler::convertTimeAtN(
	double time,
	TimeUnit unit
){
	double timeInDefaultBaseUnits
		= time*getTimeConversionFactor(
			timeUnit
		)/getTimeConversionFactor(unit);

	return timeInDefaultBaseUnits/timeScale;
}

double UnitHandler::convertTimeNtA(
	double time,
	TimeUnit unit
){
	double timeInDefaultBaseUnits
		= time*timeScale/getTimeConversionFactor(timeUnit);
	return timeInDefaultBaseUnits*getTimeConversionFactor(unit);
}

double UnitHandler::convertLengthAtB(
	double length,
	LengthUnit unit
){
	return length*getLengthConversionFactor(
		lengthUnit
	)/getLengthConversionFactor(unit);
}

double UnitHandler::convertLengthBtA(
	double length,
	LengthUnit unit
){
	return length*getLengthConversionFactor(
		unit
	)/getLengthConversionFactor(lengthUnit);
}

double UnitHandler::convertLengthAtN(
	double length,
	LengthUnit unit
){
	double lengthInDefaultBaseUnits
		= length*getLengthConversionFactor(
			lengthUnit
		)/getLengthConversionFactor(unit);

	return lengthInDefaultBaseUnits/lengthScale;
}

double UnitHandler::convertLengthNtA(
	double length,
	LengthUnit unit
){
	double lengthInDefaultBaseUnits
		= length*lengthScale/getLengthConversionFactor(lengthUnit);
	return lengthInDefaultBaseUnits*getLengthConversionFactor(unit);
}

double UnitHandler::convertEnergyAtB(
	double energy,
	EnergyUnit unit
){
	return energy*getEnergyConversionFactor(
		energyUnit
	)/getEnergyConversionFactor(unit);
}

double UnitHandler::convertEnergyBtA(
	double energy,
	EnergyUnit unit
){
	return energy*getEnergyConversionFactor(
		unit
	)/getEnergyConversionFactor(energyUnit);
}

double UnitHandler::convertEnergyAtN(
	double energy,
	EnergyUnit unit
){
	double energyInDefaultBaseUnits
		= energy*getEnergyConversionFactor(
			energyUnit
		)/getEnergyConversionFactor(unit);

	return energyInDefaultBaseUnits/energyScale;
}

double UnitHandler::convertEnergyNtA(
	double energy,
	EnergyUnit unit
){
	double energyInDefaultBaseUnits
		= energy*energyScale/getEnergyConversionFactor(energyUnit);
	return energyInDefaultBaseUnits*getEnergyConversionFactor(unit);
}

double UnitHandler::convertChargeAtB(
	double charge,
	ChargeUnit unit
){
	return charge*getChargeConversionFactor(
		chargeUnit
	)/getChargeConversionFactor(unit);
}

double UnitHandler::convertChargeBtA(
	double charge,
	ChargeUnit unit
){
	return charge*getChargeConversionFactor(
		unit
	)/getChargeConversionFactor(chargeUnit);
}

double UnitHandler::convertChargeAtN(
	double charge,
	ChargeUnit unit
){
	double chargeInDefaultBaseUnits
		= charge*getChargeConversionFactor(
			chargeUnit
		)/getChargeConversionFactor(unit);

	return chargeInDefaultBaseUnits/chargeScale;
}

double UnitHandler::convertChargeNtA(
	double charge,
	ChargeUnit unit
){
	double chargeInDefaultBaseUnits
		= charge*chargeScale/getChargeConversionFactor(chargeUnit);
	return chargeInDefaultBaseUnits*getChargeConversionFactor(unit);
}

double UnitHandler::convertCountAtB(
	double count,
	CountUnit unit
){
	return count*getCountConversionFactor(
		countUnit
	)/getCountConversionFactor(unit);
}

double UnitHandler::convertCountBtA(
	double count,
	CountUnit unit
){
	return count*getCountConversionFactor(unit)/getCountConversionFactor(
		countUnit
	);
}

double UnitHandler::convertCountAtN(
	double count,
	CountUnit unit
){
	double countInDefaultBaseUnits
		= count*getCountConversionFactor(
			countUnit
		)/getCountConversionFactor(unit);

	return countInDefaultBaseUnits/countScale;
}

double UnitHandler::convertCountNtA(
	double count,
	CountUnit unit
){
	double countInDefaultBaseUnits
		= count*countScale/getCountConversionFactor(countUnit);
	return countInDefaultBaseUnits*getCountConversionFactor(unit);
}

double UnitHandler::convertMassDtB(double mass, MassUnit unit){
	double massInDefaultBaseUnits = mass/getMassConversionFactor(unit);
	double cfE = getEnergyConversionFactor();
	double cfT = getTimeConversionFactor();
	double cfL = getLengthConversionFactor();
	return massInDefaultBaseUnits*cfE*cfT*cfT/(cfL*cfL);
}

double UnitHandler::convertMassBtD(double mass, MassUnit unit){
	double cfE = getEnergyConversionFactor();
	double cfT = getTimeConversionFactor();
	double cfL = getLengthConversionFactor();
	double massInDefaultBaseUnits = mass*cfL*cfL/(cfE*cfT*cfT);
	return massInDefaultBaseUnits*getMassConversionFactor(unit);
}

double UnitHandler::convertMassDtN(double mass, MassUnit unit){
	double massInDefaultBaseUnits = mass/getMassConversionFactor(unit);
	double cfE = getEnergyConversionFactor()/energyScale;
	double cfT = getTimeConversionFactor()/timeScale;
	double cfL = getLengthConversionFactor()/lengthScale;
	return massInDefaultBaseUnits*cfE*cfT*cfT/(cfL*cfL);
}

double UnitHandler::convertMassNtD(double mass, MassUnit unit){
	double cfE = getEnergyConversionFactor()/energyScale;
	double cfT = getTimeConversionFactor()/timeScale;
	double cfL = getLengthConversionFactor()/lengthScale;
	double massInDefaultBaseUnits = mass*cfL*cfL/(cfE*cfT*cfT);
	return massInDefaultBaseUnits*getMassConversionFactor(unit);
}

double UnitHandler::convertMagneticFieldDtB(
	double field,
	MagneticFieldUnit unit
){
	double magneticFieldInDefaultBaseUnits = field/getMagneticFieldConversionFactor(unit);
	double cfE = getEnergyConversionFactor();
	double cfT = getTimeConversionFactor();
	double cfC = getChargeConversionFactor();
	double cfL = getLengthConversionFactor();
	return magneticFieldInDefaultBaseUnits*cfE*cfT/(cfC*cfL*cfL);
}

double UnitHandler::convertMagneticFieldBtD(
	double field,
	MagneticFieldUnit unit
){
	double cfE = getEnergyConversionFactor();
	double cfT = getTimeConversionFactor();
	double cfC = getChargeConversionFactor();
	double cfL = getLengthConversionFactor();
	double magneticFieldInDefaultBaseUnits = field*cfC*cfL*cfL/(cfE*cfT);
	return magneticFieldInDefaultBaseUnits*getMagneticFieldConversionFactor(unit);
}

double UnitHandler::convertMagneticFieldDtN(
	double field,
	MagneticFieldUnit unit
){
	double magneticFieldInDefaultBaseUnits = field/getMagneticFieldConversionFactor(unit);
	double cfE = getEnergyConversionFactor()/energyScale;
	double cfT = getTimeConversionFactor()/timeScale;
	double cfC = getChargeConversionFactor()/chargeScale;
	double cfL = getLengthConversionFactor()/lengthScale;
	return magneticFieldInDefaultBaseUnits*cfE*cfT/(cfC*cfL*cfL);
}

double UnitHandler::convertMagneticFieldNtD(
	double field,
	MagneticFieldUnit unit
){
	double cfE = getEnergyConversionFactor()/energyScale;
	double cfT = getTimeConversionFactor()/timeScale;
	double cfC = getChargeConversionFactor()/chargeScale;
	double cfL = getLengthConversionFactor()/lengthScale;
	double magneticFieldInDefaultBaseUnits = field*cfC*cfL*cfL/(cfE*cfT);
	return magneticFieldInDefaultBaseUnits*getMagneticFieldConversionFactor(unit);
}

double UnitHandler::convertVoltageDtB(
	double voltage,
	VoltageUnit unit
){
	double voltageInDefaultBaseUnits = voltage/getVoltageConversionFactor(unit);
	double cfE = getEnergyConversionFactor();
	double cfC = getChargeConversionFactor();
	return voltageInDefaultBaseUnits*cfE/cfC;
}

double UnitHandler::convertVoltageBtD(
	double voltage,
	VoltageUnit unit
){
	double cfE = getEnergyConversionFactor();
	double cfC = getChargeConversionFactor();
	double voltageInDefaultBaseUnits = voltage*cfC/cfE;
	return voltageInDefaultBaseUnits*getVoltageConversionFactor(unit);
}

double UnitHandler::convertVoltageDtN(
	double voltage,
	VoltageUnit unit
){
	double voltageInDefaultBaseUnits = voltage/getVoltageConversionFactor(unit);
	double cfE = getEnergyConversionFactor()/energyScale;
	double cfC = getChargeConversionFactor()/chargeScale;
	return voltageInDefaultBaseUnits*cfE/cfC;
}

double UnitHandler::convertVoltageNtD(
	double voltage,
	VoltageUnit unit
){
	double cfE = getEnergyConversionFactor()/energyScale;
	double cfC = getChargeConversionFactor()/chargeScale;
	double voltageInDefaultBaseUnits = voltage*cfC/cfE;
	return voltageInDefaultBaseUnits*getVoltageConversionFactor(unit);
}

string UnitHandler::getTemperatureUnitString(){
	switch(temperatureUnit){
		case TemperatureUnit::kK:
			return "kK";
		case TemperatureUnit::K:
			return "K";
		case TemperatureUnit::mK:
			return "mK";
		case TemperatureUnit::uK:
			return "uK";
		case TemperatureUnit::nK:
			return "nK";
		default:
			return "Unknown unit";
	};
}

string UnitHandler::getTimeUnitString(){
	switch(timeUnit){
		case TimeUnit::s:
			return "s";
		case TimeUnit::ms:
			return "ms";
		case TimeUnit::us:
			return "us";
		case TimeUnit::ns:
			return "ns";
		case TimeUnit::ps:
			return "ps";
		case TimeUnit::fs:
			return "fs";
		case TimeUnit::as:
			return "as";
		default:
			return "Unknown unit";
	};
}

string UnitHandler::getLengthUnitString(){
	switch(lengthUnit){
		case LengthUnit::m:
			return "m";
		case LengthUnit::mm:
			return "mm";
		case LengthUnit::um:
			return "um";
		case LengthUnit::nm:
			return "nm";
		case LengthUnit::pm:
			return "pm";
		case LengthUnit::fm:
			return "fm";
		case LengthUnit::am:
			return "am";
		case LengthUnit::Ao:
			return "Ao";
		default:
			return "Unknown unit";
	};
}

string UnitHandler::getEnergyUnitString(){
	switch(energyUnit){
		case EnergyUnit::GeV:
			return "GeV";
		case EnergyUnit::MeV:
			return "MeV";
		case EnergyUnit::keV:
			return "keV";
		case EnergyUnit::eV:
			return "eV";
		case EnergyUnit::meV:
			return "meV";
		case EnergyUnit::ueV:
			return "ueV";
		case EnergyUnit::J:
			return "J";
		default:
			return "Unknown unit";
	};
}

string UnitHandler::getChargeUnitString(){
	switch(chargeUnit){
		case ChargeUnit::kC:
			return "kC";
		case ChargeUnit::C:
			return "C";
		case ChargeUnit::mC:
			return "mC";
		case ChargeUnit::uC:
			return "uC";
		case ChargeUnit::nC:
			return "nC";
		case ChargeUnit::pC:
			return "pC";
		case ChargeUnit::fC:
			return "fC";
		case ChargeUnit::aC:
			return "aC";
		case ChargeUnit::Te:
			return "Te";
		case ChargeUnit::Ge:
			return "Ge";
		case ChargeUnit::Me:
			return "Me";
		case ChargeUnit::ke:
			return "ke";
		case ChargeUnit::e:
			return "e";
		default:
			return "Unknown unit";
	}
}

string UnitHandler::getCountUnitString(){
	switch(countUnit){
		case CountUnit::pcs:
			return "pcs";
		case CountUnit::mol:
			return "mol";
		default:
			return "Unkown unit";
	}
}

string UnitHandler::getMassUnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << getTimeUnitString() << "^2" << "/" << getLengthUnitString() << "^2";

	return ss.str();
}

string UnitHandler::getMagneticFieldUnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << getTimeUnitString() << "/" << getChargeUnitString() << getLengthUnitString() << "^2";

	return ss.str();
}

string UnitHandler::getVoltageUnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << "/" << getChargeUnitString();

	return ss.str();
}

string UnitHandler::getHBARUnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << getTimeUnitString();

	return ss.str();
}

string UnitHandler::getK_BUnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << "/" << getTemperatureUnitString();

	return ss.str();
}

string UnitHandler::getEUnitString(){
	return getChargeUnitString();
}

string UnitHandler::getCUnitString(){
	stringstream ss;
	ss << getLengthUnitString() << "^2" << "/" << getTimeUnitString() << "^2";

	return ss.str();
}

string UnitHandler::getN_AUnitString(){
	return getCountUnitString();
}

string UnitHandler::getM_eUnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << getTimeUnitString() << "^2" << "/" << getLengthUnitString() << "^2";

	return ss.str();
}

string UnitHandler::getM_pUnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << getTimeUnitString() << "^2" << "/" << getLengthUnitString() << "^2";

	return ss.str();
}

string UnitHandler::getMu_BUnitString(){
	stringstream ss;
	ss << getChargeUnitString() << getLengthUnitString() << "^2" << "/" << getTimeUnitString();

	return ss.str();
}

string UnitHandler::getMu_nUnitString(){
	stringstream ss;
	ss << getChargeUnitString() << getLengthUnitString() << "^2" << "/" << getTimeUnitString();

	return ss.str();
}

string UnitHandler::getMu_0UnitString(){
	stringstream ss;
	ss << getEnergyUnitString() << getTimeUnitString() << "^2" << "/" << getChargeUnitString() << "^2" << getLengthUnitString();

	return ss.str();
}

string UnitHandler::getEpsilon_0UnitString(){
	stringstream ss;
	ss << getChargeUnitString() << "^2" << "/" << getEnergyUnitString() << getLengthUnitString();

	return ss.str();
}

string UnitHandler::getA_0UnitString(){
	return getLengthUnitString();
}

void UnitHandler::updateConstants(){
	constantsBaseUnits.clear();
	for(auto constant : constantsDefaultUnits){
		const string &name = constant.first;
		double value = constant.second.first;
		const vector<pair<string, int>> &units
			= constant.second.second;
		for(unsigned int n = 0; n < units.size(); n++){
			const string &unit = units[n].first;
			int exponent = units[n].second;
			for(int c = 0; c < exponent; c++){
				if(unit.compare("K") == 0){
					value *= getTemperatureConversionFactor();
				}
				else if(unit.compare("s") == 0){
					value *= getTimeConversionFactor();
				}
				else if(unit.compare("m") == 0){
					value *= getLengthConversionFactor();
				}
				else if(unit.compare("eV") == 0){
					value *= getEnergyConversionFactor();
				}
				else if(unit.compare("C") == 0){
					value *= getChargeConversionFactor();
				}
				else if(unit.compare("pcs") == 0){
					value *= getCountConversionFactor();
				}
				else{
					TBTKExit(
						"UnitHandler::updateConstants()",
						"Unknown default unit.",
						"This should never happen,"
						<< " contact the developer."
					);
				}
			}
			for(int c = 0; c < -exponent; c++){
				if(unit.compare("K") == 0){
					value /= getTemperatureConversionFactor();
				}
				else if(unit.compare("s") == 0){
					value /= getTimeConversionFactor();
				}
				else if(unit.compare("m") == 0){
					value /= getLengthConversionFactor();
				}
				else if(unit.compare("eV") == 0){
					value /= getEnergyConversionFactor();
				}
				else if(unit.compare("C") == 0){
					value /= getChargeConversionFactor();
				}
				else if(unit.compare("pcs") == 0){
					value /= getCountConversionFactor();
				}
				else{
					TBTKExit(
						"UnitHandler::updateConstants()",
						"Unknown default unit.",
						"This should never happen,"
						<< " contact the developer."
					);
				}
			}
		}
		constantsBaseUnits[name] = value;
	}
//	for(auto constant : constantsBaseUnits)
//		Streams::out << constant.first << "\t" << constant.second << "\n";
}

/*void UnitHandler::updateHbar(){
	hbar = HBAR;
	hbar *= getEnergyConversionFactor();
	hbar *= getTimeConversionFactor();
}

void UnitHandler::updateK_B(){
	k_B = K_B;
	k_B *= getEnergyConversionFactor();
	k_B /= getTemperatureConversionFactor();
}

void UnitHandler::updateE(){
	e = E;
	e *= getChargeConversionFactor();
}

void UnitHandler::updateC(){
	c = C;
	c *= getLengthConversionFactor();
	c /= getTimeConversionFactor();
}

void UnitHandler::updateN_A(){
	n_A = N_A;
	n_A *= getCountConversionFactor();
}

void UnitHandler::updateM_e(){
	m_e = M_E;
	m_e *= getEnergyConversionFactor();
	m_e *= getTimeConversionFactor()*getTimeConversionFactor();
	m_e /= getLengthConversionFactor()*getLengthConversionFactor();
}

void UnitHandler::updateM_p(){
	m_p = M_P;
	m_p *= getEnergyConversionFactor();
	m_p *= getTimeConversionFactor()*getTimeConversionFactor();
	m_p /= getLengthConversionFactor()*getLengthConversionFactor();
}

void UnitHandler::updateMu_B(){
	mu_B = MU_B;
	mu_B *= getChargeConversionFactor();
	mu_B *= getLengthConversionFactor()*getLengthConversionFactor();
	mu_B /= getTimeConversionFactor();
}

void UnitHandler::updateMu_n(){
	mu_n = MU_N;
	mu_n *= getChargeConversionFactor();
	mu_n *= getLengthConversionFactor()*getLengthConversionFactor();
	mu_n /= getTimeConversionFactor();
}

void UnitHandler::updateMu_0(){
	mu_0 = MU_0;
	mu_0 *= getEnergyConversionFactor();
	mu_0 *= getTimeConversionFactor()*getTimeConversionFactor();
	mu_0 /= getChargeConversionFactor()*getChargeConversionFactor();
	mu_0 /= getLengthConversionFactor();
}

void UnitHandler::updateEpsilon_0(){
	epsilon_0 = EPSILON_0;
	epsilon_0 *= getChargeConversionFactor()*getChargeConversionFactor();
	epsilon_0 /= getEnergyConversionFactor();
	epsilon_0 /= getLengthConversionFactor();
}

void UnitHandler::updateA_0(){
	a_0 = A_0;
	a_0 *= getLengthConversionFactor();
}*/

double UnitHandler::getTemperatureConversionFactor(){
	return getTemperatureConversionFactor(temperatureUnit);
}

double UnitHandler::getTemperatureConversionFactor(TemperatureUnit temperatureUnit){
	switch(temperatureUnit){
		case TemperatureUnit::kK:	//1e-3 kK per K
			return 1e-3;
		case TemperatureUnit::K:	//Reference scale
			return 1.;
		case TemperatureUnit::mK:
			return 1e3;
		case TemperatureUnit::uK:
			return 1e6;
		case TemperatureUnit::nK:
			return 1e9;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getTemperatureConversionUnit()",
				"Unknown unit - " << static_cast<int>(temperatureUnit) << ".",
				""
			);
	}
}

double UnitHandler::getTimeConversionFactor(){
	return getTimeConversionFactor(timeUnit);
}

double UnitHandler::getTimeConversionFactor(TimeUnit timeUnit){
	switch(timeUnit){
		case TimeUnit::s:	//Reference scale
			return 1.;
		case TimeUnit::ms:
			return 1e3;	//1e3 ms per second
		case TimeUnit::us:
			return 1e6;
		case TimeUnit::ns:
			return 1e9;
		case TimeUnit::ps:
			return 1e12;
		case TimeUnit::fs:
			return 1e15;
		case TimeUnit::as:
			return 1e18;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getTimeConversionFactor()",
				"Unknown unit - " << static_cast<int>(timeUnit) << ".",
				""
			);
	}
}

double UnitHandler::getLengthConversionFactor(){
	return getLengthConversionFactor(lengthUnit);
}

double UnitHandler::getLengthConversionFactor(LengthUnit lengthUnit){
	switch(lengthUnit){
		case LengthUnit::m:	//Reference scale
			return 1.;
		case LengthUnit::mm:	//1e3 mm per m
			return 1e3;
		case LengthUnit::um:
			return 1e6;
		case LengthUnit::nm:
			return 1e9;
		case LengthUnit::pm:
			return 1e12;
		case LengthUnit::fm:
			return 1e15;
		case LengthUnit::am:
			return 1e18;
		case LengthUnit::Ao:
			return 1e10;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getLengthConversionFactor()",
				"Unknown unit - " << static_cast<int>(lengthUnit) << ".",
				""
			);
	}
}

double UnitHandler::getEnergyConversionFactor(){
	return getEnergyConversionFactor(energyUnit);
}

double UnitHandler::getEnergyConversionFactor(EnergyUnit energyUnit){
	switch(energyUnit){
		case EnergyUnit::GeV:	//1e-9 GeV per eV
			return 1e-9;
		case EnergyUnit::MeV:
			return 1e-6;
		case EnergyUnit::keV:
			return 1e-3;
		case EnergyUnit::eV:	//Reference scale
			return 1.;
		case EnergyUnit::meV:
			return 1e3;
		case EnergyUnit::ueV:
			return 1e6;
		case EnergyUnit::J:
			return J_per_eV;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getEnergyConversionFactor()",
				"Unknown unit - " << static_cast<int>(energyUnit) << ".",
				""
			);
	}
}

double UnitHandler::getChargeConversionFactor(){
	return getChargeConversionFactor(chargeUnit);
}

double UnitHandler::getChargeConversionFactor(ChargeUnit chargeUnit){
	double E = constantsDefaultUnits["e"].first;
	switch(chargeUnit){
		case ChargeUnit::kC:	//1e-3 kC per C
			return 1e-3;
		case ChargeUnit::C:	//Reference scale
			return 1.;
		case ChargeUnit::mC:
			return 1e3;
		case ChargeUnit::uC:
			return 1e6;
		case ChargeUnit::nC:
			return 1e9;
		case ChargeUnit::pC:
			return 1e12;
		case ChargeUnit::fC:
			return 1e15;
		case ChargeUnit::aC:
			return 1e18;
		case ChargeUnit::Te:
			return 1e-12/E;
		case ChargeUnit::Ge:
			return 1e-9/E;
		case ChargeUnit::Me:
			return 13-6/E;
		case ChargeUnit::ke:
			return 1e-3/E;
		case ChargeUnit::e:
			return 1./E;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getChargeConversionFactor()",
				"Unknown unit - " << static_cast<int>(chargeUnit) << ".",
				""
			);
	}
}

double UnitHandler::getCountConversionFactor(){
	return getCountConversionFactor(countUnit);
}

double UnitHandler::getCountConversionFactor(CountUnit countUnit){
	double N_A = constantsDefaultUnits["N_A"].first;
	switch(countUnit){
		case CountUnit::pcs:
			return 1.;	//Reference scale
		case CountUnit::mol:	//1/N_A mol per pcs
			return 1./N_A;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getCountConversionFactor()",
				"Unknown unit - " << static_cast<int>(countUnit) << ".",
				""
			);
	}
}

double UnitHandler::getMassConversionFactor(MassUnit unit){
	switch(unit){
		case MassUnit::kg:
			return kg_per_baseMass;
		case MassUnit::g:
			return kg_per_baseMass*1e3;
		case MassUnit::mg:
			return kg_per_baseMass*1e6;
		case MassUnit::ug:
			return kg_per_baseMass*1e9;
		case MassUnit::ng:
			return kg_per_baseMass*1e12;
		case MassUnit::u:
			return u_per_baseMass;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getMassConversionFactor()",
				"Unknown unit - " << static_cast<int>(unit) << ".",
				""
			);
	}
}

double UnitHandler::getMagneticFieldConversionFactor(MagneticFieldUnit unit){
	switch(unit){
		case MagneticFieldUnit::MT:
			return T_per_baseMagneticField*1e-6;
		case MagneticFieldUnit::kT:
			return T_per_baseMagneticField*1e-3;
		case MagneticFieldUnit::T:
			return T_per_baseMagneticField;
		case MagneticFieldUnit::mT:
			return T_per_baseMagneticField*1e3;
		case MagneticFieldUnit::uT:
			return T_per_baseMagneticField*1e6;
		case MagneticFieldUnit::nT:
			return T_per_baseMagneticField*1e9;
		case MagneticFieldUnit::GG:
			return T_per_baseMagneticField*1e-5;
		case MagneticFieldUnit::MG:
			return T_per_baseMagneticField*1e-2;
		case MagneticFieldUnit::kG:
			return T_per_baseMagneticField*10.;
		case MagneticFieldUnit::G:
			return T_per_baseMagneticField*1e4;	//10^4G = 1T
		case MagneticFieldUnit::mG:
			return T_per_baseMagneticField*1e7;
		case MagneticFieldUnit::uG:
			return T_per_baseMagneticField*1e10;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getMagneticFieldConversionFactor()",
				"Unknown unit - " << static_cast<int>(unit) << ".",
				""
			);
	}
}

double UnitHandler::getVoltageConversionFactor(VoltageUnit unit){
	switch(unit){
		case VoltageUnit::GV:
			return V_per_baseVoltage*1e-9;
		case VoltageUnit::MV:
			return V_per_baseVoltage*1e-6;
		case VoltageUnit::kV:
			return V_per_baseVoltage*1e-3;
		case VoltageUnit::V:
			return V_per_baseVoltage;
		case VoltageUnit::mV:
			return V_per_baseVoltage*1e3;
		case VoltageUnit::uV:
			return V_per_baseVoltage*1e6;
		case VoltageUnit::nV:
			return V_per_baseVoltage*1e9;
		default:	//Should never happen, hard error generated for quick bug detection
			TBTKExit(
				"UnitHandler::getVoltageConversionFactor()",
				"Unknown unit - " << static_cast<int>(unit) << ".",
				""
			);
	}
}

UnitHandler::TemperatureUnit UnitHandler::getTemperatureUnit(string unit){
	if(unit.compare("kK") == 0){
		return TemperatureUnit::kK;
	}
	else if(unit.compare("K") == 0){
		return TemperatureUnit::K;
	}
	else if(unit.compare("mK") == 0){
		return TemperatureUnit::mK;
	}
	else if(unit.compare("uK") == 0){
		return TemperatureUnit::uK;
	}
	else if(unit.compare("nK") == 0){
		return TemperatureUnit::nK;
	}
	else{
		TBTKExit(
			"UnitHandler::getTemperatureUnit()",
			"Invalid temperature unit '" << unit << "'",
			""
		);
	}
}

UnitHandler::TimeUnit UnitHandler::getTimeUnit(string unit){
	if(unit.compare("s") == 0){
		return TimeUnit::s;
	}
	else if(unit.compare("ms") == 0){
		return TimeUnit::ms;
	}
	else if(unit.compare("us") == 0){
		return TimeUnit::us;
	}
	else if(unit.compare("ns") == 0){
		return TimeUnit::ns;
	}
	else if(unit.compare("ps") == 0){
		return TimeUnit::ps;
	}
	else if(unit.compare("fs") == 0){
		return TimeUnit::fs;
	}
	else if(unit.compare("as") == 0){
		return TimeUnit::as;
	}
	else{
		TBTKExit(
			"UnitHandler::getTimeUnit()",
			"Invalid time unit '" << unit << "'",
			""
		);
	}
}

UnitHandler::LengthUnit UnitHandler::getLengthUnit(string unit){
	if(unit.compare("m") == 0){
		return LengthUnit::m;
	}
	else if(unit.compare("mm") == 0){
		return LengthUnit::mm;
	}
	else if(unit.compare("um") == 0){
		return LengthUnit::um;
	}
	else if(unit.compare("nm") == 0){
		return LengthUnit::nm;
	}
	else if(unit.compare("pm") == 0){
		return LengthUnit::pm;
	}
	else if(unit.compare("fm") == 0){
		return LengthUnit::fm;
	}
	else if(unit.compare("am") == 0){
		return LengthUnit::am;
	}
	else if(unit.compare("Ao") == 0){
		return LengthUnit::Ao;
	}
	else{
		TBTKExit(
			"UnitHandler::getLengthUnit()",
			"Invalid length unit '" << unit << "'",
			""
		);
	}
}

UnitHandler::EnergyUnit UnitHandler::getEnergyUnit(string unit){
	if(unit.compare("GeV") == 0){
		return EnergyUnit::GeV;
	}
	else if(unit.compare("MeV") == 0){
		return EnergyUnit::MeV;
	}
	else if(unit.compare("keV") == 0){
		return EnergyUnit::keV;
	}
	else if(unit.compare("eV") == 0){
		return EnergyUnit::eV;
	}
	else if(unit.compare("meV") == 0){
		return EnergyUnit::meV;
	}
	else if(unit.compare("ueV") == 0){
		return EnergyUnit::ueV;
	}
	else if(unit.compare("J") == 0){
		return EnergyUnit::J;
	}
	else{
		TBTKExit(
			"UnitHandler::getEnergyUnit()",
			"Invalid energy unit '" << unit << "'",
			""
		);
	}
}

UnitHandler::ChargeUnit UnitHandler::getChargeUnit(string unit){
	if(unit.compare("kC") == 0){
		return ChargeUnit::kC;
	}
	else if(unit.compare("C") == 0){
		return ChargeUnit::C;
	}
	else if(unit.compare("mC") == 0){
		return ChargeUnit::mC;
	}
	else if(unit.compare("uC") == 0){
		return ChargeUnit::uC;
	}
	else if(unit.compare("nC") == 0){
		return ChargeUnit::nC;
	}
	else if(unit.compare("pC") == 0){
		return ChargeUnit::pC;
	}
	else if(unit.compare("fC") == 0){
		return ChargeUnit::fC;
	}
	else if(unit.compare("aC") == 0){
		return ChargeUnit::aC;
	}
	else if(unit.compare("Te") == 0){
		return ChargeUnit::Te;
	}
	else if(unit.compare("Ge") == 0){
		return ChargeUnit::Ge;
	}
	else if(unit.compare("Me") == 0){
		return ChargeUnit::Me;
	}
	else if(unit.compare("ke") == 0){
		return ChargeUnit::ke;
	}
	else if(unit.compare("e") == 0){
		return ChargeUnit::e;
	}
	else{
		TBTKExit(
			"UnitHandler::getChargeUnit()",
			"Invalid charge unit '" << unit << "'",
			""
		);
	}
}

UnitHandler::CountUnit UnitHandler::getCountUnit(string unit){
	if(unit.compare("pcs") == 0){
		return CountUnit::pcs;
	}
	else if(unit.compare("mol") == 0){
		return CountUnit::mol;
	}
	else{
		TBTKExit(
			"UnitHandler::getCountUnit()",
			"Invalid count unit '" << unit << "'",
			""
		);
	}
}

UnitHandler::StaticConstructor::StaticConstructor(){
	constantsDefaultUnits = {
		{"e",		{1.602176634e-19,			{{"C", 1}}}},
		{"c",		{2.99792458e8,				{{"m", 1}, {"s", -1}}}},
		{"N_A",		{6.02214076e23,				{{"pcs", 1}}}},
		{"a_0",		{5.29177210903*1e-11, 			{{"m", 1}}}}
	};
	double e = constantsDefaultUnits["e"].first;
	//Source "The International System of Units (SI) 9th Edition. Bureau
	//International des Poids et Mesures. 2019."
	constantsDefaultUnits["h"] =		{6.62607015e-34/e,		{{"eV", 1}, {"s", 1}}};
	constantsDefaultUnits["k_B"] =		{1.380649e-23/e,		{{"eV", 1}, {"K", -1}}};

	double c = constantsDefaultUnits["c"].first;
	//Source "The NIST reference on Constants, Units, and Uncertainty."
	//https://physics.nist.gov/cuu/Constants/index.html
	constantsDefaultUnits["m_e"] =		{9.1093837015e-31/e,		{{"eV", 1}, {"s", 2}, {"m", -2}}};
	constantsDefaultUnits["m_p"] =		{1.67262192369e-27/e,		{{"eV", 1}, {"s", 2}, {"m", -2}}};
	constantsDefaultUnits["mu_0"] =		{1.25663706212e-6/e,		{{"eV", 1}, {"s", 2}, {"C", -2}, {"m", -1}}};
	constantsDefaultUnits["epsilon_0"] =	{8.8541878128e-12*e,		{{"C", 2}, {"eV", -1}, {"m", -1}}};

	double h = constantsDefaultUnits["h"].first;
	constantsDefaultUnits["hbar"] =		{h/(2*M_PI),			{{"eV", 1}, {"s", 1}}};
	double hbar = constantsDefaultUnits["hbar"].first;
	double m_e = constantsDefaultUnits["m_e"].first;
	double m_p = constantsDefaultUnits["m_p"].first;
	constantsDefaultUnits["mu_B"] =		{e*hbar/(2*m_e),		{{"C", 1}, {"m", 2}, {"s", -1}}};
	constantsDefaultUnits["mu_N"] =		{e*hbar/(2*m_p),		{{"C", 1}, {"m", 2}, {"s", -1}}};

	J_per_eV			= e;
	eV_per_J			= 1./J_per_eV;
	kg_per_baseMass			= e;
	baseMass_per_kg			= 1./kg_per_baseMass;
	u_per_baseMass			= (c*c)/9.31494095e8;
	baseMass_per_u			= 1./u_per_baseMass;
	T_per_baseMagneticField		= e;
	baseMagneticField_per_T		= 1./T_per_baseMagneticField;
	V_per_baseVoltage		= e;
	baseVoltage_per_V		= 1./V_per_baseVoltage;

	updateConstants();
}

};

#ifdef M_E_temp
	#define M_E M_E_temp
	#undef M_E_temp
#endif
