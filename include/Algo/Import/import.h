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

#ifndef __IMPORT_H__
#define __IMPORT_H__

#include "Algo/Import/import2tables.h"

namespace CGoGN
{

namespace Algo
{

namespace Import 
{

/**
* import a mesh
* @param map the map in which the function imports the mesh
* @param filename (*.{trian,trianbgz,off,obj,ply})
* @param positions table of vertices positions attribute
* @param m a marker that will be set by the function. If closeObject=false the phi2 that have fixed point are marked, else the created darts of the boundary are marked.
* @param kind what kind of mesh is the file (if none (-1) determined by filename extension) (cf enum in Mesh2Tables for other kind values)
* @param closeObject a boolean indicating if the imported mesh should be closed
* @return a boolean indicating if import was successfull
*/
template <typename PFP>
bool importMesh(typename PFP::MAP& map, const std::string& filename, typename PFP::TVEC3& positions, ImportSurfacique::ImportType kind = ImportSurfacique::UNKNOWNSURFACE);

template <typename PFP>
bool importPLYPTM(typename PFP::MAP& map, const std::string& filename, typename PFP::TVEC3& positions, ImportSurfacique::ImportType kind,
		typename PFP::TFRAME& frame, typename PFP::TRGBFUNCTIONS& funcs);

/**
 * import a volumic mesh
 */
template <typename PFP>
bool importMesh(typename PFP::MAP& map, const std::string& filename, typename PFP::TVEC3& positions, ImportVolumique::ImportType kind= ImportVolumique::UNKNOWNVOLUME);

//template <typename PFP>
//bool importObjWithTex(typename PFP::MAP& map, const std::string& filename);
//


/*
 * TODO a transformer en utilisant un MeshTableVolume.
 */
template <typename PFP>
bool importOFFWithELERegions(typename PFP::MAP& the_map, typename PFP::TVEC3& m_position, char* filenameOFF,char * filenameELE);

template <typename PFP>
bool importTet(typename PFP::MAP& the_map, typename PFP::TVEC3& m_position, char* filename, float scaleFactor=1.0f);

template <typename PFP>
bool importTs(typename PFP::MAP& the_map, typename PFP::TVEC3& m_position, char* filename, float scaleFactor=1.0f);

template <typename PFP>
bool importInESS(typename PFP::MAP& the_map, typename PFP::TVEC3& m_position, char* filename);

}

}

}

#include "Algo/Import/importMesh.hpp"
//#include "Algo/Import/importObjTex.hpp"
#include "Algo/Import/importObjEle.hpp"
#include "Algo/Import/importTet.hpp"
#include "Algo/Import/importTs.hpp"
#include "Algo/Import/importInESS.hpp"

#endif