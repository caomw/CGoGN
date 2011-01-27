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

#include "Algo/Geometry/localFrame.h"

namespace CGoGN
{

namespace Algo
{

namespace PMesh
{

template <typename PFP>
ProgressiveMesh<PFP>::ProgressiveMesh(
		MAP& map, DartMarker& inactive,
		Algo::Decimation::SelectorType s, Algo::Decimation::ApproximatorType a,
		typename PFP::TVEC3& position
	) :
	m_map(map), positionsTable(position), inactiveMarker(inactive), dartSelect(inactiveMarker)
{
	std::cout << "  creating approximator and predictor.." << std::flush ;
	switch(a)
	{
		case Algo::Decimation::A_QEM : {
			m_approximators.push_back(new Algo::Decimation::Approximator_QEM<PFP>(m_map, positionsTable)) ;
			break ; }
		case Algo::Decimation::A_MidEdge : {
			m_approximators.push_back(new Algo::Decimation::Approximator_MidEdge<PFP>(m_map, positionsTable)) ;
			break ; }
		case Algo::Decimation::A_HalfCollapse : {
			Algo::Decimation::Predictor_HalfCollapse<PFP>* pred = new Algo::Decimation::Predictor_HalfCollapse<PFP>(m_map, positionsTable) ;
			m_predictors.push_back(pred) ;
			m_approximators.push_back(new Algo::Decimation::Approximator_HalfCollapse<PFP>(m_map, positionsTable, pred)) ;
			break ; }
		case Algo::Decimation::A_CornerCutting : {
			Algo::Decimation::Predictor_CornerCutting<PFP>* pred = new Algo::Decimation::Predictor_CornerCutting<PFP>(m_map, positionsTable) ;
			m_predictors.push_back(pred) ;
			m_approximators.push_back(new Algo::Decimation::Approximator_CornerCutting<PFP>(m_map, positionsTable, pred)) ;
			break ; }
		case Algo::Decimation::A_TangentPredict1 : {
			Algo::Decimation::Predictor_TangentPredict1<PFP>* pred = new Algo::Decimation::Predictor_TangentPredict1<PFP>(m_map, positionsTable) ;
			m_predictors.push_back(pred) ;
			m_approximators.push_back(new Algo::Decimation::Approximator_MidEdge<PFP>(m_map, positionsTable, pred)) ;
			break ; }
		case Algo::Decimation::A_TangentPredict2 : {
			Algo::Decimation::Predictor_TangentPredict2<PFP>* pred = new Algo::Decimation::Predictor_TangentPredict2<PFP>(m_map, positionsTable) ;
			m_predictors.push_back(pred) ;
			m_approximators.push_back(new Algo::Decimation::Approximator_MidEdge<PFP>(m_map, positionsTable, pred)) ;
			break ; }
		case Algo::Decimation::A_LightfieldFull :	{
			m_approximators.push_back(new Algo::Decimation::Approximator_QEM<PFP>(m_map, positionsTable)) ;
			AttributeHandler<Geom::Matrix<3,3,typename PFP::REAL> > frame = m_map.template getAttribute<Geom::Matrix<3,3,typename PFP::REAL> >(VERTEX_ORBIT, "frame") ;
			AttributeHandler<Geom::Matrix<3,6,typename PFP::REAL> > RGBfunctions = m_map.template getAttribute<Geom::Matrix<3,6,typename PFP::REAL> >(VERTEX_ORBIT, "RGBfunctions") ;
			m_approximators.push_back(new Algo::Decimation::Approximator_Frame<PFP>(m_map, frame)) ;
			m_approximators.push_back(new Algo::Decimation::Approximator_RGBfunctions<PFP>(m_map, RGBfunctions)) ;
			break ; }
	}
	std::cout << "..done" << std::endl ;

	std::cout << "  creating selector.." << std::flush ;
	switch(s)
	{
		case Algo::Decimation::S_MapOrder : {
			m_selector = new Algo::Decimation::EdgeSelector_MapOrder<PFP>(m_map, positionsTable, m_approximators, dartSelect) ;
			break ; }
		case Algo::Decimation::S_Random : {
			m_selector = new Algo::Decimation::EdgeSelector_Random<PFP>(m_map, positionsTable, m_approximators, dartSelect) ;
			break ; }
		case Algo::Decimation::S_EdgeLength : {
			m_selector = new Algo::Decimation::EdgeSelector_Length<PFP>(m_map, positionsTable, m_approximators, dartSelect) ;
			break ; }
		case Algo::Decimation::S_QEM : {
			m_selector = new Algo::Decimation::EdgeSelector_QEM<PFP>(m_map, positionsTable, m_approximators, dartSelect) ;
			break ; }
		case Algo::Decimation::S_MinDetail : {
			m_selector = new Algo::Decimation::EdgeSelector_MinDetail<PFP>(m_map, positionsTable, m_approximators, dartSelect) ;
			break ; }
		case Algo::Decimation::S_Curvature : {
			m_selector = new Algo::Decimation::EdgeSelector_Curvature<PFP>(m_map, positionsTable, m_approximators, dartSelect) ;
			break ; }
		case Algo::Decimation::S_Lightfield : {
			m_selector = new Algo::Decimation::EdgeSelector_Lightfield<PFP>(map, positionsTable, m_approximators, dartSelect) ;
			break ; }
	}
	std::cout << "..done" << std::endl ;

	m_initOk = true ;

	std::cout << "  initializing approximators.." << std::flush ;
	for(typename std::vector<Algo::Decimation::ApproximatorGen<PFP>*>::iterator it = m_approximators.begin(); it != m_approximators.end(); ++it)
	{
		if(! (*it)->init())
			m_initOk = false ;
		if((*it)->getApproximatedAttributeName() == "position")
			m_positionApproximator = reinterpret_cast<Algo::Decimation::Approximator<PFP, VEC3>*>(*it) ;
	}
	std::cout << "..done" << std::endl ;

	std::cout << "  initializing predictors.." << std::flush ;
	for(typename std::vector<Algo::Decimation::PredictorGen<PFP>*>::iterator it = m_predictors.begin(); it != m_predictors.end(); ++it)
		if(! (*it)->init())
			m_initOk = false ;
	std::cout << "..done" << std::endl ;

	std::cout << "  initializing selector.." << std::flush ;
	m_initOk = m_selector->init() ;
	std::cout << "..done" << std::endl ;

	m_detailAmount = REAL(1) ;
	m_localFrameDetailVectors = false ;
	quantizationInitialized = false ;
	quantizationApplied = false ;
}

template <typename PFP>
ProgressiveMesh<PFP>::~ProgressiveMesh()
{
	for(unsigned int i = 0; i < m_splits.size(); ++i)
		delete m_splits[i] ;
	if(m_selector)
		delete m_selector ;
	for(typename std::vector<Algo::Decimation::ApproximatorGen<PFP>*>::iterator it = m_approximators.begin(); it != m_approximators.end(); ++it)
		delete (*it) ;
	for(typename std::vector<Algo::Decimation::PredictorGen<PFP>*>::iterator it = m_predictors.begin(); it != m_predictors.end(); ++it)
		delete (*it) ;
	if(quantizationInitialized)
		delete q ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::createPM(unsigned int percentWantedVertices)
{
	unsigned int nbVertices = m_map.getNbOrbits(VERTEX_ORBIT) ;
	unsigned int nbWantedVertices = nbVertices * percentWantedVertices / 100 ;
	std::cout << "  creating PM (" << nbVertices << " vertices).." << std::flush ;

	bool finished = false ;
	Dart d ;
	while(!finished)
	{
		if(!m_selector->nextEdge(d))
			break ;

		--nbVertices ;
		Dart d2 = m_map.phi2(m_map.phi_1(d)) ;
		Dart dd2 = m_map.phi2(m_map.phi_1(m_map.phi2(d))) ;

		VSplit<PFP>* vs = new VSplit<PFP>(m_map, d, dd2, d2) ;	// create new VSplit node
		m_splits.push_back(vs) ;								// and store it

		for(typename std::vector<Algo::Decimation::ApproximatorGen<PFP>*>::iterator it = m_approximators.begin(); it != m_approximators.end(); ++it)
		{
			(*it)->approximate(d) ;					// compute approximated attributes with its associated detail
			(*it)->saveApprox(d) ;
		}

		m_selector->updateBeforeCollapse(d) ;		// update selector

		edgeCollapse(vs) ;							// collapse edge

		unsigned int newV = m_map.embedNewCell(VERTEX_CELL, d2) ;
		unsigned int newE1 = m_map.embedNewCell(EDGE_CELL, d2) ;
		unsigned int newE2 = m_map.embedNewCell(EDGE_CELL, dd2) ;
		vs->setApproxV(newV) ;
		vs->setApproxE1(newE1) ;
		vs->setApproxE2(newE2) ;

		for(typename std::vector<Algo::Decimation::ApproximatorGen<PFP>*>::iterator it = m_approximators.begin(); it != m_approximators.end(); ++it)
			(*it)->affectApprox(d2);				// affect data to the resulting vertex

		m_selector->updateAfterCollapse(d2, dd2) ;	// update selector

		if(nbVertices <= nbWantedVertices)
			finished = true ;
	}
	delete m_selector ;
	m_selector = NULL ;

	m_cur = m_splits.size() ;
	std::cout << "..done (" << nbVertices << " vertices)" << std::endl ;

	initQuantization() ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::edgeCollapse(VSplit<PFP>* vs)
{
	Dart d = vs->getEdge() ;
	Dart dd = m_map.phi2(d) ;

	inactiveMarker.markOrbit(FACE_ORBIT, d) ;
	inactiveMarker.markOrbit(FACE_ORBIT, dd) ;

	m_map.extractTrianglePair(d) ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::vertexSplit(VSplit<PFP>* vs)
{
	Dart d = vs->getEdge() ;
	Dart dd = m_map.phi2(d) ;
	Dart d2 = vs->getLeftEdge() ;
	Dart dd2 = vs->getRightEdge() ;

	m_map.insertTrianglePair(d, d2, dd2) ;

	inactiveMarker.unmarkOrbit(FACE_ORBIT, d) ;
	inactiveMarker.unmarkOrbit(FACE_ORBIT, dd) ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::coarsen()
{
	if(m_cur == m_splits.size())
		return ;

	VSplit<PFP>* vs = m_splits[m_cur] ; // get the split node
	++m_cur ;

	Dart d = vs->getEdge() ;
	Dart dd = m_map.phi2(d) ;		// get some darts
	Dart d2 = vs->getLeftEdge() ;
	Dart dd2 = vs->getRightEdge() ;

	edgeCollapse(vs) ;	// collapse edge

	m_map.embedOrbit(VERTEX_CELL, d2, vs->getApproxV()) ;
	m_map.embedOrbit(EDGE_CELL, d2, vs->getApproxE1()) ;
	m_map.embedOrbit(EDGE_CELL, dd2, vs->getApproxE2()) ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::refine()
{
	if(m_cur == 0)
		return ;

	--m_cur ;
	VSplit<PFP>* vs = m_splits[m_cur] ; // get the split node

	Dart d = vs->getEdge() ;
	Dart dd = m_map.phi2(d) ; 		// get some darts
	Dart dd2 = vs->getRightEdge() ;
	Dart d2 = vs->getLeftEdge() ;
	Dart d1 = m_map.phi2(d2) ;
	Dart dd1 = m_map.phi2(dd2) ;

	unsigned int v1 = m_map.getDartEmbedding(VERTEX_ORBIT, d) ;				// get the embedding
	unsigned int v2 = m_map.getDartEmbedding(VERTEX_ORBIT, dd) ;			// of the new vertices
	unsigned int e1 = m_map.getDartEmbedding(EDGE_ORBIT, m_map.phi1(d)) ;
	unsigned int e2 = m_map.getDartEmbedding(EDGE_ORBIT, m_map.phi_1(d)) ;	// and new edges
	unsigned int e3 = m_map.getDartEmbedding(EDGE_ORBIT, m_map.phi1(dd)) ;
	unsigned int e4 = m_map.getDartEmbedding(EDGE_ORBIT, m_map.phi_1(dd)) ;

	if(!m_predictors.empty())
	{
		for(typename std::vector<Algo::Decimation::PredictorGen<PFP>*>::iterator pit = m_predictors.begin();
			pit != m_predictors.end();
			++pit)
		{
			(*pit)->predict(d2, dd2) ;
		}
	}

	typename PFP::MATRIX33 invLocalFrame ;
	if(m_localFrameDetailVectors)
	{
		typename PFP::MATRIX33 localFrame = Algo::Geometry::vertexLocalFrame<PFP>(m_map, dd2, positionsTable) ;
		localFrame.invert(invLocalFrame) ;
	}

	vertexSplit(vs) ; // split vertex

	m_map.embedOrbit(VERTEX_ORBIT, d, v1) ;		// embed the
	m_map.embedOrbit(VERTEX_ORBIT, dd, v2) ;	// new vertices
	m_map.embedOrbit(EDGE_ORBIT, d1, e1) ;
	m_map.embedOrbit(EDGE_ORBIT, d2, e2) ;		// and new edges
	m_map.embedOrbit(EDGE_ORBIT, dd1, e3) ;
	m_map.embedOrbit(EDGE_ORBIT, dd2, e4) ;

	if(!m_predictors.empty())
	{
		typename std::vector<Algo::Decimation::PredictorGen<PFP>*>::iterator pit ;
		typename std::vector<Algo::Decimation::ApproximatorGen<PFP>*>::iterator ait ;
		for(pit = m_predictors.begin(), ait = m_approximators.begin();
			pit != m_predictors.end();
			++pit, ++ait)
		{
			typename PFP::MATRIX33* detailTransform = NULL ;
			if(m_localFrameDetailVectors)
				detailTransform = &invLocalFrame ;

			(*pit)->affectPredict(d) ;
			if((*ait)->getType() == Algo::Decimation::A_HalfCollapse)
			{
				(*ait)->addDetail(dd, m_detailAmount, true, detailTransform) ;
			}
			else
			{
				(*ait)->addDetail(d, m_detailAmount, true, detailTransform) ;
				(*ait)->addDetail(dd, m_detailAmount, false, detailTransform) ;
			}
		}
	}
}

template <typename PFP>
void ProgressiveMesh<PFP>::gotoLevel(unsigned int l)
{
	if(l == m_cur || l > m_splits.size() || l < 0)
		return ;

	if(l > m_cur)
		while(m_cur != l)
			coarsen() ;
	else
		while(m_cur != l)
			refine() ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::recomputeApproxAndDetails()
{
//	if(!m_predictors.empty())
//	{
//		gotoLevel(0) ;
//		while(m_cur < nbSplits())
//		{
//			VSplit<PFP>* vs = m_splits[m_cur] ;
//			++m_cur ;
//			unsigned int e = vs->getApprox() ;
//			m_approximator->approximate(vs, e) ;
//			edgeCollapse(vs, e) ;
//		}
//	}
}

template <typename PFP>
void ProgressiveMesh<PFP>::setDetailAmount(double a)
{
	m_detailAmount = a ;
	unsigned int c = m_cur ;
	gotoLevel(nbSplits()) ;
	gotoLevel(c) ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::localizeDetailVectors()
{
	if(m_positionApproximator->getPredictor() && !m_localFrameDetailVectors)
	{
		bool quantizationWasApplied = quantizationApplied ;
		unsigned int nbCodeVectors = 0 ;
		if(quantizationWasApplied)
		{
			nbCodeVectors = q->getNbCodeVectors() ;
			resetDetailVectors() ;
		}
		m_localFrameDetailVectors = true ;
		gotoLevel(nbSplits()) ;
		while(m_cur > 0)
		{
			Dart d = m_splits[m_cur-1]->getEdge() ;
			Dart dd2 = m_splits[m_cur-1]->getRightEdge() ;
			typename PFP::MATRIX33 localFrame = Algo::Geometry::vertexLocalFrame<PFP>(m_map, dd2, positionsTable) ;
			VEC3 det = m_positionApproximator->getDetail(d) ;
			det = localFrame * det ;
			m_positionApproximator->setDetail(d, det) ;
			refine() ;
		}
		quantizationInitialized = false ;
		initQuantization() ;
		if(quantizationWasApplied)
			quantizeDetailVectors(nbCodeVectors) ;
	}
}

template <typename PFP>
void ProgressiveMesh<PFP>::globalizeDetailVectors()
{
	if(!m_predictors.empty() && m_localFrameDetailVectors)
	{
		bool quantizationWasApplied = quantizationApplied ;
		unsigned int nbCodeVectors = 0 ;
		if(quantizationWasApplied)
		{
			nbCodeVectors = q->getNbCodeVectors() ;
			resetDetailVectors() ;
		}
		m_localFrameDetailVectors = false ;
		gotoLevel(nbSplits()) ;
		while(m_cur > 0)
		{
			Dart d = m_splits[m_cur-1]->getEdge() ;
			Dart dd2 = m_splits[m_cur-1]->getRightEdge() ;
			typename PFP::MATRIX33 localFrame = Algo::Geometry::vertexLocalFrame<PFP>(m_map, dd2, positionsTable) ;
			typename PFP::MATRIX33 invLocalFrame ;
			localFrame.invert(invLocalFrame) ;
			VEC3 det = m_positionApproximator->getDetail(d) ;
			det = invLocalFrame * det ;
			m_positionApproximator->setDetail(d, det) ;
			refine() ;
		}
		quantizationInitialized = false ;
		initQuantization() ;
		if(quantizationWasApplied)
			quantizeDetailVectors(nbCodeVectors) ;
	}
}

template <typename PFP>
void ProgressiveMesh<PFP>::initQuantization()
{
	if(m_positionApproximator->getPredictor() && !quantizationInitialized)
	{
		gotoLevel(nbSplits()) ;
		originalDetailVectors.resize(m_splits.size()) ;
		for(unsigned int i = 0; i < m_splits.size(); ++i)
			originalDetailVectors[i] = m_positionApproximator->getDetail(m_splits[i]->getEdge()) ;
		q = new Quantization<VEC3>(originalDetailVectors) ;
		quantizationInitialized = true ;
		std::cout << "  Differential Entropy -> " << q->getDifferentialEntropy() << std::endl ;
	}
}

template <typename PFP>
void ProgressiveMesh<PFP>::quantizeDetailVectors(unsigned int nbClasses)
{
	initQuantization() ;
	if(quantizationInitialized)
	{
		gotoLevel(nbSplits()) ;
		std::vector<VEC3> resultat;
		q->vectorQuantizationNbRegions(nbClasses, resultat) ;
		for(unsigned int i = 0; i < m_splits.size(); ++i)
			m_positionApproximator->setDetail(m_splits[i]->getEdge(), resultat[i]) ;
		quantizationApplied = true ;
		gotoLevel(0) ;
		std::cout << "Discrete Entropy -> " << q->getDiscreteEntropy() << " (codebook size : " << q->getNbCodeVectors() << ")" << std::endl ;
/*
		Point p;
		p.x = q->getEntropieDiscrete() ;
		p.y = computeDistance2() ;
		p.nbClasses = q->getNbClasses() ;
		courbe.push_back(p) ;
*/
	}
}

template <typename PFP>
void ProgressiveMesh<PFP>::quantizeDetailVectors(float distortion)
{
	initQuantization() ;
	if(quantizationInitialized)
	{
		gotoLevel(nbSplits()) ;
		std::vector<typename PFP::VEC3> resultat;
		q->vectorQuantizationDistortion(distortion, resultat) ;
		for(unsigned int i = 0; i < m_splits.size(); ++i)
			m_positionApproximator->setDetail(m_splits[i]->getEdge(), resultat[i]) ;
		quantizationApplied = true ;
		gotoLevel(0) ;
		std::cout << "Discrete Entropy -> " << q->getDiscreteEntropy() << " (codebook size : " << q->getNbCodeVectors() << ")" << std::endl ;
	}
}

template <typename PFP>
void ProgressiveMesh<PFP>::resetDetailVectors()
{
	if(quantizationInitialized)
	{
		gotoLevel(nbSplits()) ;
		for(unsigned int i = 0; i < m_splits.size(); ++i)
			m_positionApproximator->setDetail(m_splits[i]->getEdge(), originalDetailVectors[i]) ;
		delete q ;
		quantizationInitialized = false ;
		quantizationApplied = false ;
		gotoLevel(0) ;
	}
}

/*
template <typename PFP>
float ProgressiveMesh<PFP>::computeDistance2()
{
	float distance = 0; // sum of 2-distance between original vertices and new vertices

	gotoLevel(0) ; // mesh reconstruction from detail vectors
	DartMarker mUpdate(m_map) ;
	for(Dart d = m_map.begin(); d != m_map.end(); m_map.next(d)) // vertices loop
	{
		if(!mUpdate.isMarked(d))
		{
			mUpdate.markOrbit(VERTEX_ORBIT, d) ;
			EMB* dEmb = reinterpret_cast<EMB*>(m_map.getVertexEmb(d)) ;
			// computes the 2-distance between original vertex and new vertex
			dEmb->updateDistance2() ;
			distance += dEmb->getDistance2() ;
		}
	}

	return distance ;
}

template <typename PFP>
void ProgressiveMesh<PFP>::calculCourbeDebitDistortion()
{
	Dart d;
	EMB* dEmb;
	std::vector<Vector3f> source;
	std::vector<Vector3f> resultat;
	float distance;
	Point p;

	std::cout << "calcul de la courbe débit distortion " << std::flush;

	// get original detail vectors
	for(unsigned int i = 0; i < m_splits.size(); ++i)
	{
		source.push_back(Vector3f(*(m_splits.at(i)->getDetailDown())));
		source.push_back(Vector3f(*(m_splits.at(i)->getDetailUp())));
	}

	// vector quantization initialisation
	Quantization<Vector3f> q (source);
	q.vectorQuantizationInit();
	entropieDifferentielle = q.getEntropieDifferentielle();
	determinantSigma = q.getDeterminantSigma();
	traceSigma = q.getTraceSigma();


	// several quantizations of the same detail vectors to compute the curve
	for(unsigned int i = 8 ; i < m_splits.size() ; i *= 2)
	{
		q.vectorQuantization(i, resultat);

		// insert new vectors into the model to compute the distance
		for(unsigned int j = 0; j < m_splits.size(); ++j)
		{
			gmtl::Vec3f* v = resultat.at(j*2).getGmtl();
			m_splits.at(j)->setDetailDown(v);
			v = resultat.at(j*2+1).getGmtl();
			m_splits.at(j)->setDetailUp(v);
		}
		distance = computeDistance2();

		p.x = q.getEntropieDiscrete();
		p.y = distance;
		p.nbClasses = q.getNbClasses();
		courbe.push_back(p);
		// returns to coarse mesh
		gotoLevel(nbSplits());
		std::cout << "..." << std::flush;
	}
	q.erase();
}
*/

} //namespace PMesh

} //namespace Algo

} //namespace CGoGN