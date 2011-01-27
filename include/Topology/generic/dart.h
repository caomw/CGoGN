/*******************************************************************************
* CGoGN: Combinatorial and Geometric modeling with Generic N-dimensional Maps  *
* version 0.1                                                                  *
* Copyright (C) 2009, IGG Team, LSIIT, University of Strasbourg                *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Web site: https://iggservis.u-strasbg.fr/CGoGN/                              *
* Contact information: cgogn@unistra.fr                                        *
*                                                                              *
*******************************************************************************/

#ifndef DART_H_
#define DART_H_

#include <iostream>

namespace CGoGN
{

struct Dart
{
	unsigned int index;

	Dart(): index(0xffffffff) {}

	static Dart nil() { Dart d; d.index = 0xffffffff; return d; }

	static Dart create(unsigned int i) { Dart d; d.index = i; return d; }

	Dart(unsigned int v): index(v) {}

	bool isNil() { return index == 0xffffffff ; }

	/**
	 * affectation operator
	 * @param d the dart to store in this
	 */
	Dart operator=(Dart d) { index = d.index; return *this; }

	/**
	 * equality operator
	 * @param d the dart to compare with
	 */
	bool operator==(Dart d) { return d.index == index; }

	/**
	 * different operator
	 * @param d the dart to compare with
	 */
	bool operator!=(Dart d) { return d.index != index; }

	/**
	 * less operator, can be used for sorting
	 * @param d the dart to compare with
	 */
	bool operator<(Dart d) { return d.index < index; }

	friend std::ostream& operator<<( std::ostream &out, const Dart& fa ) { return out << fa.index; }
	friend std::istream& operator>>( std::istream &in, Dart& fa ) { in >> fa.index; return in; }

	void operator += (const Dart& fa) {}
	void operator -= (const Dart& fa) {}
	void operator *= (double v) {}
	void operator /= (double v) {}

	/**
	 * CGoGN name
	 */
	static std::string CGoGNnameOfType() { return "Dart"; }

	/**
	 * label is the index (cleaner that use d.index outside of maps
	 */
	unsigned int label() { return index; }
};

const Dart NIL = Dart::nil();
const Dart DART_END = Dart::create(NIL.index - 1);

}

#endif /* DART_H_ */