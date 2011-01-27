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

namespace Algo
{

namespace Decimation
{

template <typename PFP>
void decimate(
	typename PFP::MAP& map, SelectorType s, ApproximatorType a,
	typename PFP::TVEC3& position, unsigned int nbWantedVertices, const FunctorSelect& selected
)
{
	std::vector<ApproximatorGen<PFP>*> approximators ;
	EdgeSelector<PFP>* selector = NULL ;

	switch(a)
	{
		case A_QEM :
			approximators.push_back(new Approximator_QEM<PFP>(map, position)) ;
			break ;
		case A_MidEdge :
			approximators.push_back(new Approximator_MidEdge<PFP>(map, position)) ;
			break ;
		case A_CornerCutting :
			approximators.push_back(new Approximator_CornerCutting<PFP>(map, position)) ;
			break ;
		case A_TangentPredict1 :
			approximators.push_back(new Approximator_MidEdge<PFP>(map, position)) ;
			break ;
		case A_TangentPredict2 :
			approximators.push_back(new Approximator_MidEdge<PFP>(map, position)) ;
			break ;
		case A_HalfCollapse :
			approximators.push_back(new Approximator_HalfCollapse<PFP>(map, position)) ;
			break ;
		case A_LightfieldFull :
		{
			approximators.push_back(new Approximator_QEM<PFP>(map, position)) ;
			AttributeHandler<Geom::Matrix<3,3,typename PFP::REAL> > frame = map.template getAttribute<Geom::Matrix<3,3,typename PFP::REAL> >(VERTEX_ORBIT, "frame") ;
			AttributeHandler<Geom::Matrix<3,6,typename PFP::REAL> > RGBfunctions = map.template getAttribute<Geom::Matrix<3,6,typename PFP::REAL> >(VERTEX_ORBIT, "RGBfunctions") ;
			approximators.push_back(new Approximator_Frame<PFP>(map, frame)) ;
			approximators.push_back(new Approximator_RGBfunctions<PFP>(map, RGBfunctions)) ;
			break ;
		}
	}

	switch(s)
	{
		case S_MapOrder :
			selector = new EdgeSelector_MapOrder<PFP>(map, position, approximators) ;
			break ;
		case S_Random :
			selector = new EdgeSelector_Random<PFP>(map, position, approximators) ;
			break ;
		case S_EdgeLength :
			selector = new EdgeSelector_Length<PFP>(map, position, approximators) ;
			break ;
		case S_QEM :
			selector = new EdgeSelector_QEM<PFP>(map, position, approximators) ;
			break ;
		case S_Lightfield :
			selector = new EdgeSelector_Lightfield<PFP>(map, position, approximators) ;
			break ;
		case S_Curvature :
			selector = new EdgeSelector_Curvature<PFP>(map, position, approximators) ;
			break ;
		case S_MinDetail :
			selector = new EdgeSelector_Random<PFP>(map, position, approximators) ;
			break ;
	}

	for(typename std::vector<ApproximatorGen<PFP>*>::iterator it = approximators.begin(); it != approximators.end(); ++it)
		(*it)->init() ;

	if(!selector->init())
		return ;

	unsigned int nbVertices = map.getNbOrbits(VERTEX_ORBIT) ;
	bool finished = false ;
	Dart d ;
	while(!finished)
	{
		if(!selector->nextEdge(d))
			break ;

		--nbVertices ;

		Dart d2 = map.phi2(map.phi_1(d)) ;
		Dart dd2 = map.phi2(map.phi_1(map.phi2(d))) ;

		for(typename std::vector<ApproximatorGen<PFP>*>::iterator it = approximators.begin(); it != approximators.end(); ++it)
		{
			(*it)->approximate(d) ;				// compute approximated attributes
			(*it)->saveApprox(d) ;
		}

		selector->updateBeforeCollapse(d) ;		// update selector

		map.collapseEdge(d) ;					// collapse edge

		for(typename std::vector<ApproximatorGen<PFP>*>::iterator it = approximators.begin(); it != approximators.end(); ++it)
			(*it)->affectApprox(d2);			// affect data to the resulting vertex

		selector->updateAfterCollapse(d2, dd2) ;// update selector

		if(nbVertices <= nbWantedVertices)
			finished = true ;
	}

	delete selector ;
	for(typename std::vector<ApproximatorGen<PFP>*>::iterator it = approximators.begin(); it != approximators.end(); ++it)
		delete (*it) ;
}

} //namespace Decimation

} //namespace Algo

} //namespace CGoGN