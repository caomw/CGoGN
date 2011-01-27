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

#ifndef __ALGO_GEOMETRY_LAPLACIAN_H__
#define __ALGO_GEOMETRY_LAPLACIAN_H__

#include "Geometry/basic.h"

#include "OpenNL/linear_solver.h"
#include "OpenNL/sparse_matrix.h"
#include "OpenNL/full_vector.h"

namespace CGoGN
{

namespace Algo
{

namespace Geometry
{

enum LaplacianType
{
	TOPOLOGICAL
};

template <typename PFP>
void computeLaplacianVertices(
	typename PFP::MAP& map,
	LaplacianType type,
	const typename PFP::TVEC3& position,
	typename PFP::TVEC3& laplacian,
	const FunctorSelect& select = SelectorTrue()) ;

template <typename PFP>
void computeLaplacianVertex_Topo(
	typename PFP::MAP& map,
	Dart d,
	const typename PFP::TVEC3& position,
	typename PFP::TVEC3& laplacian) ;

} // namespace Geoemtry

} // namespace Algo

} // namespace CGoGN

#include "Algo/Geometry/laplacian.hpp"

#endif