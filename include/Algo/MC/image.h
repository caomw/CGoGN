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

#ifndef IMAGE_H
#define IMAGE_H


#include <gmtl/Vec.h>
#include "Algo/MC/type.h"

#include "img3D_IO.h"

#include "Zinrimage.h"

namespace CGoGN
{

namespace Algo
{

namespace MC
{

/**
 * voxel image class
 *
 * The class Image manage 3D voxel image
 * of any type
 * @param DataType the type of voxel
 */
template< typename  DataType >
class Image
{
protected:
	/**
	 * pointer to data
	 */
	DataType *m_Data;

	/**
	 * X size
	 */
	int m_WX;

	/**
	 * Y size
	 */
	int m_WY;

	/**
	 * Z size
	 */
	int m_WZ;

	/**
	 * slice size
	 */
	int32 m_WXY;


	/**
	 * X origin
	 */
	int32 m_OX;

	/**
	 * Y origin
	 */
	int32 m_OY;

	/**
	 * Z origin
	 */
	int32 m_OZ;


	/**
	 * Voxel X size
	 */
	float m_SX;

	/**
	 * Voxel Y size
	 */
	float m_SY;

	/**
	 * Voxel Z size
	 */
	float m_SZ;

	/**
	 * memory allocated ?
	 */
	bool m_Alloc;

	/**
	* Test if a point is in the image
	*
	* @param  _V the coordinates to test
	*/
	//bool correct(const gmtl::Vec3i& _V);

	/**
	 * internal inrimage prt
	 */
	PTRINRIMAGE mImage;


public:

	/**
	* default constructor
	*/
	Image();

	/**
	* destructor
	*/
	~Image();


	/**
	* Load a file (png) in an empty image
	* @param _cName file to open
	*/
	bool loadPNG3D(const char *_cName);


	/**
	* Load an ipb-format file in an empty image
	* @param _cName file to open
	*/
	bool loadIPB(const char* _cName);

	/**
	* Load an inr.gz format file in an empty image
	* @param filname file to open
	*/
	bool loadInrgz(const char* filename);

	/**
	* Constructor
	* @param data pointer on voxel
	* @param wx width in X
	* @param wy width in Y
	* @param wz width in Z
	* @param sx voxel size in X
	* @param sy voxel size in Y
	* @param sz voxel size in Z
	* @param copy	sets to true if data must be copying, false if data are used directly (and release by the destructor).
	*/
	Image(DataType *data, int32 wx, int32 wy, int32 wz, float sx, float sy, float sz, bool copy = false );


	/**
	* Load a raw image
	*/
	void loadRaw(char *filename);

	/**
	 * Load a vox file
	 */
	void loadVox(char *filename);

	/**
	* save current image into file
	* @param _cName file to save
	*/
    void saveInrMask(const char* _cName);

    /**
	* get the width along X axis
	*/
	int32 getWidthX() const { return m_WX;}

	/**
	* get the width along Y axis
	*/
	int32 getWidthY() const { return m_WY;}

	/**
	* get the width along Z axis
	*/
	int32 getWidthZ() const { return m_WZ;}

	/**
	* get widthX*widthY (size of a slice)
	*/
	int32 getWidthXY() const { return m_WXY;}

	/** set the real size of voxel of image
	 * @param vx x size
	 * @param vy y size
	 * @param vz z size
	 */
	void setVoxelSize(float vx, float vy, float vz) { m_SX = vx; m_SY = vy; m_SZ = vz;}

	/** set origin (equivalent of frame size)
	 * @param ox x size
	 * @param oy y size
	 * @param oz z size
	 */
	void setOrigin(int32 ox, int32 oy, int32 oz) {m_OX = ox; m_OY = oy; m_OZ = oz;}

	/**
	 * get the origin
	 * @return a vector with origin
	 */
	gmtl::Vec3i getOrigin() const { return gmtl::Vec3i(m_OX, m_OY, m_OZ);}

	/**
	* get the data const version
	*/
	const DataType* getData() const {return m_Data;}

	/**
	* get the data non const version
	*/
	DataType* getData() {return m_Data;}

	/**
	* get the subsampling width in X of current slot
	*/
	float getVoxSizeX() const { return m_SX;}

	/**
	* get the subsampling width in Y of current slot
	*/
	float getVoxSizeY() const { return m_SY;}

	/**
	* get the subsampling width in Z of current slot
	*/
	float getVoxSizeZ() const { return m_SZ;}


	/**
	* get the voxel value
	*
	* @param  _lX,_lY,_lZ position
	* @return the value of the voxel
	*/
	DataType getVoxel(int32 _lX,int32 _lY, int32 _lZ);

	/**
	* get the voxel address (const ptr)
	*
	* @param _lX,_lY,_lZ position
	* @return the address of the voxel
	*/
	const DataType* getVoxelPtr(int32 _lX,int32 _lY, int32 _lZ) const;

	/**
	* get the voxel address
	*
	* @param  _lX,_lY,_lZ position
	* @return the address of the voxel
	*/
	DataType* getVoxelPtr(int32 _lX,int32 _lY, int32 _lZ);


	/**
	* get the voxel value
	*
	* @param  _Vec vector of voxel position
	* @return the value of the voxel
	*/
	DataType getVoxel( const gmtl::Vec3i &_Vec);


	/**
	*  Blur the image with pseudo gaussian filter
	* @param   _lWidth width of filtering
	* @return the new image
	*/
	Image<DataType>* filtering(int32 _lWidth);

	/**
	*  add Frame of zero around the image
	* @param  _lWidth the width of frame to add
	* @return the new image
	*/
	Image<DataType>* addFrame(int32 _lWidth);

	/**
	 * Get the lower corner of bounding AABB
	 */
//	gmtl::Vec3f boundMin() const { return gmtl::Vec3f(0.0f, 0.0f, 0.0f);}
	gmtl::Vec3f boundMin() const { return gmtl::Vec3f(m_SX*m_OX, m_SY*m_OY, m_SZ*m_OZ);}

	/**
	 * Get the upper corner of bounding AABB
	 */
//	gmtl::Vec3f boundMax() const { return gmtl::Vec3f(m_SX*(m_WX-2*m_OX), m_SY*(m_WY-2*m_OY), m_SZ*(m_WZ-2*m_OZ));}
	gmtl::Vec3f boundMax() const { return gmtl::Vec3f(m_SX*(m_WX-m_OX), m_SY*(m_WY-m_OY), m_SZ*(m_WZ-m_OZ));}
	/**
	 * Compute the volume in cm3
	 * @param wind the windowing function
	 */
	template< typename Windowing >
	float computeVolume(const Windowing& wind) const;

	void Blur3();


	void createMaskOffsetSphere(std::vector<int>& table, int _i32radius);

	float computeCurvatureCount(const DataType *ptrVox, const std::vector<int>& sphere, DataType val);

};


} // end namespace
} // end namespace
} // end namespace

#include "image.hpp"



#endif