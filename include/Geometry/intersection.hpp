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

namespace Geom
{

template <typename VEC3>
Intersection intersectionRayTriangle(const VEC3& P, const VEC3& Dir, const VEC3& Ta,  const VEC3& Tb, const VEC3& Tc, VEC3& Inter)
{
	typedef typename VEC3::DATA_TYPE T ;

	VEC3 u = Ta - P ;
	VEC3 v = Tb - P ;
	VEC3 w = Tc - P ;
	T x = tripleProduct(Dir, u, v) ;
	T y = tripleProduct(Dir, v, w) ;
	T z = tripleProduct(Dir, w, u) ;
	if((x < T(0) && y < T(0) && z < T(0)) || (x > T(0) && y > T(0) && z > T(0)))
	{
		T sum = x + y + z ;
		T alpha = y / sum ;
		T beta = z / sum ;
		T gamma = T(1) - alpha - beta ;
		Inter = Ta * alpha + Tb * beta + Tc * gamma ;
		return FACE_INTERSECTION ;
	}
	if(
		( x == T(0) && y > T(0) && z > T(0) ) || ( x == T(0) && y < T(0) && z < T(0) ) || // intersection on [Ta,Tb]
		( x > T(0) && y == T(0) && z > T(0) ) || ( x < T(0) && y == T(0) && z < T(0) ) || // intersection on [Tb,Tc]
		( x > T(0) && y > T(0) && z == T(0) ) || ( x < T(0) && y < T(0) && z == T(0) )	  // intersection on [Tc,Ta]
	)
		return EDGE_INTERSECTION ;
	if(
		( x == T(0) && y == T(0) && z != T(0) ) || // intersection on Tb
		( x == T(0) && y != T(0) && z == T(0) ) || // intersection on Ta
		( x != T(0) && y == T(0) && z == T(0) )	   // intersection on Tc
	)
		return VERTEX_INTERSECTION ;
	return NO_INTERSECTION ;
}

template <typename VEC3>
Intersection intersectionLineTriangle(const VEC3& P, const VEC3& Dir, const VEC3& Ta,  const VEC3& Tb, const VEC3& Tc, VEC3& Inter)
{
	typedef typename VEC3::DATA_TYPE T ;

	VEC3 u = Tb - Ta ;
	VEC3 v = Tc - Ta ;
	VEC3 n = u ^ v ;

	VEC3 w0 = P - Ta ;
    T a = -(n * w0) ;
    T b = (n * Dir) ;

#define PRECISION 1e-20
    if(fabs(b) < PRECISION)			//ray parallel to triangle
			return NO_INTERSECTION ;
#undef PRECISION

	T r = a / b ;
	Inter = P + r * Dir ;			// intersect point of ray and plane

    // is I inside T?
	T uu = u.norm2() ;
	T uv = u * v ;
	T vv = v.norm2() ;
	VEC3 w = Inter - Ta ;
	T wu = w * u ;
	T wv = w * v ;
	T D = (uv * uv) - (uu * vv) ;

    // get and test parametric coords
	T s = ((uv * wv) - (vv * wu)) / D ;
	if(s < T(0) || s > T(1))
		return NO_INTERSECTION ;
	T t = ((uv * wu) - (uu * wv)) / D ;
	if(t < T(0) || (s + t) > T(1))
        return NO_INTERSECTION ;

	if((s == T(0) || s == T(1)))
		if(t == T(0) || t == T(1))
			return VERTEX_INTERSECTION ;
		else
			return EDGE_INTERSECTION ;
	else if(t == T(0) || t == T(1))
			return EDGE_INTERSECTION ;

    return FACE_INTERSECTION ;
}

template <typename VEC3>
Intersection intersectionLineTriangle2D(const VEC3& P, const VEC3& Dir, const VEC3& Ta,  const VEC3& Tb, const VEC3& Tc, VEC3& Inter)
{
	Inclusion inc = isPointInTriangle(P,Ta,Tb,Tc) ;
	VEC3 N = Ta ^ Tb ;

	switch(inc)
	{
		case FACE_INCLUSION :
			Inter = P ;
			return FACE_INTERSECTION ;
		case EDGE_INCLUSION :
			Inter = P ;
			return EDGE_INTERSECTION ;
		case VERTEX_INCLUSION :
			Inter = P ;
			return VERTEX_INTERSECTION ;
		default : //NO_INCLUSION : test if ray enters the triangle
			VEC3 P2 = P + Dir ;
			Orientation2D oA = testOrientation2D(Ta, P, P2, N) ;
			Orientation2D oB = testOrientation2D(Tb, P, P2, N) ;
			Orientation2D oC = testOrientation2D(Tc, P, P2, N) ;

			if(oA == oB && oB == oC)
				return NO_INTERSECTION ;

			Orientation2D oPBC = testOrientation2D(P,Tb,Tc,N) ;
			if(oPBC == LEFT)  // same side of A, test edge AC and AB
			{
				if(oA == LEFT)
				{
					if(oB == ALIGNED)
					{
						Inter = Tb ;
						return VERTEX_INTERSECTION ;
					}
					//inter with AB
					std::cout << __FILE__ << " TODO compute edge coplanar intersection AB" << std::endl ;
					return EDGE_INTERSECTION ;
				}
				if(oA == ALIGNED)
				{
					Inter = Ta ;
					return VERTEX_INTERSECTION ;
				}
				if(oC == ALIGNED)
				{
					Inter = Tc ;
					return VERTEX_INTERSECTION ;
				}
				//inter with AC
				std::cout << __FILE__ << " TODO compute edge coplanar intersection AC" << std::endl ;
				return EDGE_INTERSECTION ;
			}
			if(oPBC == RIGHT) // same side of BC, test this edge
			{
				if(oB == ALIGNED)
				{
					Inter = Tb ;
					return VERTEX_INTERSECTION ;
				}
				if(oC == ALIGNED)
				{
					Inter = Tc ;
					return VERTEX_INTERSECTION ;
				}
				//inter with BC
				std::cout << __FILE__ << " TODO compute edge coplanar intersection BC" << std::endl ;
				return EDGE_INTERSECTION ;
			}

			//aligned with BC
			//possibly colliding with edge AB or AC
			Orientation2D oPAB = testOrientation2D(P,Ta,Tb,N) ;
			if(oPAB == RIGHT) //possibly colliding with edge AB
			{
				if(oA == ALIGNED)
				{
					Inter = Ta ;
					return VERTEX_INTERSECTION ;
				}
				//inter with AB
				std::cout << __FILE__ << " TODO compute edge coplanar intersection AB" << std::endl ;
				return EDGE_INTERSECTION ;
			}
			if(oPAB == ALIGNED)
			{
				Inter = Tb ;
				return VERTEX_INTERSECTION ;
			}
			//possibly colliding with edge AC
			else if(oC == ALIGNED)
			{
				Inter = Tc ;
				return VERTEX_INTERSECTION ;
			}
			else if(oA == ALIGNED)
			{
				Inter = Ta ;
				return VERTEX_INTERSECTION ;
			}
			//inter with AC
			std::cout << __FILE__ << " TODO compute edge coplanar intersection AC" << std::endl ;
			return EDGE_INTERSECTION ;
	}
}

template <typename VEC3>
Intersection intersectionSegmentTriangle(const VEC3& PA, const VEC3& PB, const VEC3& Ta, const VEC3& Tb, const VEC3& Tc, VEC3& Inter)
{
	typedef typename VEC3::DATA_TYPE T ;
	const T precision = std::numeric_limits<T>::min();

	VEC3 u = Tb - Ta ;
	VEC3 v = Tc - Ta ;
	VEC3 Dir = PB - PA ;

	VEC3 n = u ^ v ;

	VEC3 w0 = PA - Ta ;
    float a = -(n * w0) ;
    float b = (n * Dir) ;

    if(fabs(b) < precision)			//ray parallel to triangle
			return NO_INTERSECTION ;

	//compute intersection
	T r = a / b ;

	if((r < -precision) || (r > (T(1) + precision)))
		return NO_INTERSECTION;

	Inter = PA + r * Dir;			// intersect point of ray and plane

    // is I inside T?
	T uu = u.norm2() ;
	T uv = u * v ;
	T vv = v.norm2() ;
	VEC3 w = Inter - Ta ;
	T wu = w * u ;
	T wv = w * v ;
	T D = (uv * uv) - (uu * vv) ;

    // get and test parametric coords
	T s = ((uv * wv) - (vv * wu)) / D ;
	if(s < T(0) || s > T(1))
		return NO_INTERSECTION ;
	T t = ((uv * wu) - (uu * wv)) / D ;
	if(t < T(0) || (s + t) > T(1))
        return NO_INTERSECTION ;

	if((s == T(0) || s == T(1)))
		if(t == T(0) || t == T(1))
			return VERTEX_INTERSECTION ;
		else
			return EDGE_INTERSECTION ;
	else if(t == T(0) || t == T(1))
			return EDGE_INTERSECTION ;

    return FACE_INTERSECTION ;
}

// template <typename VEC3>
// Intersection intersectionSegmentSegment2D(const VEC3& PA, const VEC3& PB, const VEC3& PC,  const VEC3& PD, VEC3& Inter) 
// {
// 	std::cout << __FILE__ << " " << __LINE__ << " to write intersectionSegmentSegment2D" << std::endl;
// 	return NO_INTERSECTION;
// }

template <typename VEC3, typename PLANE3D>
Intersection intersectPlaneRay(const PLANE3D& pl,const VEC3& p1,const VEC3& dir, VEC3& Inter)
{
	typename VEC3::DATA_TYPE isect;
	isect = (pl.normal()*(pl.normal()*pl.d()-p1)) / (pl.normal()*dir);

	if(0.0f <= isect)
	{
		Inter = p1 + dir * isect;
		return FACE_INTERSECTION;
	}

	return NO_INTERSECTION;
}

template <typename VEC3>
Intersection intersection2DSegmentSegment(const VEC3& PA, const VEC3& PB, const VEC3& QA,  const VEC3& QB, VEC3& Inter)
{
	typedef typename VEC3::DATA_TYPE T ;

	VEC3 vp1p2(PB);
	vp1p2 -= PA;
	VEC3 vq1q2(QB);
	vq1q2 -= QA;
	VEC3 vp1q1(QA);
	vp1q1 -= PA;
	T delta = vp1p2[0]*vq1q2[1]- vp1p2[1]*vq1q2[0];
	T coeff = vp1q1[0]*vq1q2[1]- vp1q1[1]*vq1q2[0];
	Inter = VEC3((PA[0]*delta+vp1p2[0]*coeff)/delta,(PA[1]*delta+vp1p2[1]*coeff)/delta,(PA[2]*delta+vp1p2[2]*coeff)/delta);

	//TODO : add error cases
	return EDGE_INTERSECTION;
}

}

}