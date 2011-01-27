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

#include "Topology/generic/attribmap.h"

namespace CGoGN
{

AttribMap::AttribMap() : GenericMap()
{
	AttribContainer& dartCont = m_attribs[DART_ORBIT] ;
	unsigned int mark_index = dartCont.addAttribute<Mark>("Mark") ;
	AttribMultiVect<Mark>& amv = dartCont.getDataVector<Mark>(mark_index) ;
	m_markerTables[DART_ORBIT] = &amv ;
}

/****************************************
 *   EMBEDDING ATTRIBUTES MANAGEMENT    *
 ****************************************/

void AttribMap::addEmbedding(unsigned int orbit)
{
	std::ostringstream oss;
	oss << "EMB_" << orbit;

	AttribContainer& dartCont = m_attribs[DART_ORBIT] ;
	unsigned int index = dartCont.addAttribute<unsigned int>(oss.str()) ;
	AttribMultiVect<unsigned int>& amv = dartCont.getDataVector<unsigned int>(index) ;
	m_embeddings[orbit] = &amv ;

	// set new embedding to EMBNULL for all the darts of the map
	for(unsigned int i = dartCont.begin(); i < dartCont.end(); dartCont.next(i))
		amv[i] = EMBNULL ;

	AttribContainer& cellCont = m_attribs[orbit] ;
	unsigned int mark_index = cellCont.addAttribute<Mark>("Mark") ;
	AttribMultiVect<Mark>& amvMark = cellCont.getDataVector<Mark>(mark_index) ;
	m_markerTables[orbit] = &amvMark ;
}

/****************************************
 *               UTILITIES              *
 ****************************************/

unsigned int AttribMap::computeIndexCells(AttributeHandler<unsigned int>& idx)
{
	AttribContainer& cont = m_attribs[AttribContainer::orbitAttr(idx.id())] ;
	unsigned int cpt = 0 ;
	for (unsigned int i = cont.begin(); i != cont.end(); cont.next(i))
	{
		idx[i] = cpt++ ;
	}
	return cpt ;
}

} // namespace CGoGN