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

#include "Algo/Geometry/centroid.h"

namespace CGoGN
{

namespace Algo
{

namespace Geometry
{

template <typename PFP>
typename PFP::REAL tetrahedronSignedVolume(typename PFP::MAP& map, Dart d, const typename PFP::TVEC3& position)
{
	typename PFP::VEC3 p1 = position[d] ;
	typename PFP::VEC3 p2 = position[map.phi1(d)] ;
	typename PFP::VEC3 p3 = position[map.phi_1(d)] ;
	typename PFP::VEC3 p4 = position[map.phi_1(map.phi2(d))] ;

	return Geom::tetraSignedVolume(p1, p2, p3, p4) ;
}

template <typename PFP>
typename PFP::REAL tetrahedronVolume(typename PFP::MAP& map, Dart d, const typename PFP::TVEC3& position)
{
	typename PFP::VEC3 p1 = position[d] ;
	typename PFP::VEC3 p2 = position[map.phi1(d)] ;
	typename PFP::VEC3 p3 = position[map.phi_1(d)] ;
	typename PFP::VEC3 p4 = position[map.phi_1(map.phi2(d))] ;

	return Geom::tetraVolume(p1, p2, p3, p4) ;
}

template <typename PFP>
typename PFP::REAL convexPolyhedronVolume(typename PFP::MAP& map, Dart d, const typename PFP::TVEC3& position)
{
	typedef typename PFP::VEC3 VEC3;

	if(map.isVolumeTetrahedron(d))
		return tetrahedronVolume<PFP>(map,d,position) ;
	else
	{
		typename PFP::REAL vol = 0 ;
		VEC3 vCentroid = Algo::Geometry::volumeCentroid<PFP>(map, d, position) ;

		DartMarkerStore mark(map);		// Lock a marker

		std::vector<Dart> visitedFaces ;
		visitedFaces.reserve(100) ;
		visitedFaces.push_back(d) ;
		mark.markOrbit(FACE_ORBIT, d) ;

		for(typename std::vector<Dart>::iterator face = visitedFaces.begin(); face != visitedFaces.end(); ++face)
		{
			Dart e = *face ;
			if(map.isFaceTriangle(e))
			{
				VEC3 p1 = position[e] ;
				VEC3 p2 = position[map.phi1(e)] ;
				VEC3 p3 = position[map.phi_1(e)] ;
				vol += Geom::tetraVolume(p1, p2, p3, vCentroid) ;
			}
			else
			{
				VEC3 fCentroid = Algo::Geometry::faceCentroid<PFP>(map, e, position) ;
				Dart f = e ;
				do
				{
					VEC3 p1 = position[f] ;
					VEC3 p2 = position[map.phi1(f)] ;
					vol += Geom::tetraVolume(p1, p2, fCentroid, vCentroid) ;
					f = map.phi1(f) ;
				} while(f != e) ;
			}
			do	// add all face neighbours to the table
			{
				Dart ee = map.phi2(e) ;
				if(!mark.isMarked(ee)) // not already marked
				{
					visitedFaces.push_back(ee) ;
					mark.markOrbit(FACE_ORBIT, e) ;
				}
				e = map.phi1(e) ;
			} while(e != *face) ;
		}

		return vol ;
	}
}

template <typename PFP>
float totalVolume(typename PFP::MAP& map, const typename PFP::TVEC3& position, const FunctorSelect& select)
{
	typename PFP::REAL vol = 0 ;
	DartMarker mark(map) ;
	for(Dart d = map.begin(); d != map.end(); map.next(d))
	{
		if(select(d) && !mark.isMarked(d))
		{
			mark.markOrbit(VOLUME_ORBIT, d) ;
			vol += convexPolyhedronVolume<PFP>(map, d, position) ;
		}
	}
	return vol ;
}

} // namespace Geometry

} // namespace Algo

} // namespace CGoGN