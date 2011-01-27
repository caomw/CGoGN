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
#ifndef __SQUELETTE3CARTE__
#define __SQUELETTE3CARTE__

#include <iostream>

#include "Utils/glutwin_atb.h"

#include "Topology/map/map3.h"
#include "Topology/generic/cellmarker.h"

#include "Geometry/matrix.h"
#include "Geometry/vector_gen.h"

#include "Algo/Import/import.h"
#include "Algo/Geometry/boundingbox.h"
//#include "Algo/Render/map_glRender.h"
#include "Algo/Render/vbo_MapRender.h"
#include "Algo/Render/topo3_vboRender.h"
#include "Algo/Modelisation/tetrahedron.h"
#include "Algo/Modelisation/primitives3d.h"


using namespace CGoGN ;

struct PFP {
	// definition de la carte
	typedef Map3 MAP;

	// definition du type de reel utilise
	typedef float REAL;
	// definition du type de vecteur (point) utilise
	typedef Geom::Vector<3,REAL> VEC3;
	// definition du type de matrice 3x3 utilise
	typedef Geom::Matrix<3,3,REAL> MATRIX33;
	// definition du type de matrice 4x4 utilise
	typedef Geom::Matrix<4,4,REAL> MATRIX44;

	// definition du type du AttributeHandler de vecteur 3D
	typedef AttributeHandler<VEC3> TVEC3;

	// definition du type du AttributeHandler de reels
	typedef AttributeHandler<REAL> TREAL;
};

// qq initialisation cachees (car penible syntaxiquement)
INIT_STATICS_MAP();


PFP::MAP myMap;
SelectorTrue allDarts;
PFP::TVEC3 position ;
Dart dglobal;

unsigned int idNorm;
unsigned int idCol;

class MyGlutWin: public Utils::GlutWin_ATB
{
public:

	/*
	 * Enumeration of possible styles of rendering
	 */
	enum renderObjStyleType {O_ALL, O_FACES, O_LINE, O_EXPLOD };
	enum renderTopoStyleType {T_ALL, T_EXPLOD };


	/*
	 * Background color
	 */
	Geom::Vec4f backCol;

	/*
	 * Object position
	 */
	Geom::Vec3f gPosObj;

	/*
	 * Object width
	 */
	float gWidthObj;

	/*
	 * Object Render Style
	 */
	int renderObjStyle;

	/*
	 * Topo Render Style
	 */
	int renderTopoStyle;

	/*
	 * Draw Object ?
	 */
	bool render_obj;

	/*
	 * Draw Topo ?
	 */
    bool render_topo;

	/*
	 * Draw help ?
	 */
	 bool render_help;

	 /*
	  * Coeff of Object exploding
	  */
	 Geom::Vec3f coefObjExplod;

	 /*
	  * Coeff of Topo exploding
	  */
	 Geom::Vec3f coefTopoExplod;

	 /*
	  * Menu Bar
	  */
	 TwBar* viewer_bar ;

	 /*
	  * VBO Object rendering
	  */
	 Algo::Render::VBO::MapRender_VBO<PFP>* m_render;

	 /*
	  * VBO Topo rendering
	  */
	 //Algo::Render::VBO::topo3_VBORenderMap<PFP::MAP>* m_render_topo;
	 Algo::Render::VBO::topo3_VBORenderMapD* m_render_topo;

	 /*
	  * ALGO VARIABLES
	  */


	 MyGlutWin(	int* argc, char **argv, int winX, int winY);

	 /*
	  * Redraw callback
	  */
	 void myRedraw();

	 /*
	  * Keyboard callback
	  */
	 void myKeyboard(unsigned char keycode, int x, int y);

	 /*
	  * Initialise the variables
	  */
	 void init();

	 /*
	  *
	  */
	 void initRender();

	 /*
	  * Initialise the ATB environnement
	  */
	 void initGUI();
};

#endif