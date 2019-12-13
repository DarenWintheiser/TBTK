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

/** @package TBTKcalc
 *  @file Energy.h
 *  @brief Energy.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_QUANTITY_ENERGY
#define COM_DAFER45_TBTK_QUANTITY_ENERGY

#include "TBTK/Real.h"
#include "TBTK/TBTKMacros.h"

#include <map>
#include <string>

namespace TBTK{
namespace Quantity{

/** @brief Energy.
 *
 *  A Energy is a Real value which implicitly is assumed to have units of
 *  energy. */
class Energy : public Real{
public:
	/** Default constructor. */
	Energy(){};

	/** Constructs a Quantity from a double. */
	Energy(double value) : Real(value){};

	/** Energy units (base unit):
	 *  - GeV - gigaelectron Volt
	 *  - MeV - megaelectron Volt
	 *  - keV - kiloelectron Volt
	 *  - eV - electron Volt
	 *  - meV - millielectron Volt
	 *  - ueV - microelectron Volt
	 *  - J - Joule */
	enum class Unit {GeV, MeV, keV, eV, meV, ueV, J};

	/** Get unit string for the given Unit. */
	static std::string getUnitString(Unit unit);

	/** Convert a string to a Unit. */
	static Unit getUnit(const std::string &str);

	/** Get the conversion factor for converting from the reference unit to
	 *  the given unit. */
	static double getConversionFactor(Unit unit);
private:
	static std::map<Unit, std::string> unitToString;
	static std::map<std::string, Unit> stringToUnit;
};

}; //End of namesapce Energy
}; //End of namesapce TBTK

#endif
