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

namespace CGoGN
{

/// INLINE FUNCTIONS

inline Map2::Map2() : Map1()
{
	m_phi2 = addRelation("phi2") ;
}

inline std::string Map2::mapTypeName()
{
	return "Map2" ;
}

inline unsigned int Map2::dimension()
{
	return 2 ;
}

/*! @name Basic Topological Operators
 * Access and Modification
 *************************************************************************/

inline Dart Map2::newDart()
{
	Dart d = Map1::newDart() ;
	(*m_phi2)[d.index] = d ;
	return d ;
}

inline Dart Map2::phi2(Dart d)
{
	return (*m_phi2)[d.index] ;
}

template <int N>
inline Dart Map2::phi(Dart d)
{
	assert( (N > 0) || !"negative parameters not allowed in template multi-phi");
	if (N < 10)
	{
		switch(N)
		{
			case 1 : return phi1(d) ;
			case 2 : return phi2(d) ;
			default : assert(!"Wrong multi-phi relation value") ; return d ;
		}
	}
	switch(N%10)
	{
		case 1 : return phi1(phi<N/10>(d)) ;
		case 2 : return phi2(phi<N/10>(d)) ;
		default : assert(!"Wrong multi-phi relation value") ; return d ;
	}
}

inline Dart Map2::alpha0(Dart d)
{
	return phi2(d) ;
}

#ifdef NO_BOUND_MAP
inline Dart Map2::alpha1(Dart d)
{
	return phi2(phi_1(d)) ;
}
#else
// alpha1 avec bord
inline Dart Map2::alpha1(Dart d)
{
	Dart e = phi_1(d);
	Dart f = phi2(e);

	if (f != e)
		return f;

	f = d;
	e = phi2(f);
	while (e != f)
	{
		f = phi1(e);
		e = phi2(f);
	}
	return f;
}
#endif

#ifdef NO_BOUND_MAP
inline Dart Map2::alpha_1(Dart d)
{
	return phi1(phi2(d)) ;
}

#else
// alpha_1 avec bord
inline Dart Map2::alpha_1(Dart d)
{
	Dart e = phi2(d);

	if (e != d)
		return phi1(e);

	e = d;
	Dart f = phi_1(d);
	while (phi2(f) != f)
	{
		e = phi2(f);
		f = phi_1(e);
	}
	return e;
}
#endif

inline void Map2::phi2sew(Dart d, Dart e)
{
	assert((*m_phi2)[d.index] == d) ;
	assert((*m_phi2)[e.index] == e) ;
	(*m_phi2)[d.index] = e ;
	(*m_phi2)[e.index] = d ;
}

inline void Map2::phi2unsew(Dart d)
{
	Dart e = (*m_phi2)[d.index] ;
	(*m_phi2)[d.index] = d ;
	(*m_phi2)[e.index] = e ;
}

/*! @name Generator and Deletor
 *  To generate or delete faces in a 2-map
 *************************************************************************/

inline void Map2::deleteFace(Dart d)
{
	deleteOrientedFace(d) ;
}

/*! @name Topological Queries
 *  Return or set various topological information
 *************************************************************************/

inline bool Map2::sameVertex(Dart d, Dart e)
{
	return sameOrientedVertex(d, e) ;
}

/*! @name Cell Functors
 *  Apply functors to all darts of a cell
 *************************************************************************/

inline bool Map2::foreach_dart_of_volume(Dart d, FunctorType& f)
{
	return foreach_dart_of_oriented_volume(d, f);
}

inline bool Map2::foreach_dart_of_cc(Dart d, FunctorType& f)
{
	return foreach_dart_of_oriented_volume(d, f);
}

} // namespace CGoGN