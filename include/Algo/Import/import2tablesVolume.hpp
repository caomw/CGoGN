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

namespace Import
{

template <typename PFP>
ImportVolumique::ImportType MeshTablesVolume<PFP>::getFileType(const std::string& filename)
{
	if ((filename.rfind(".tet")!=std::string::npos) || (filename.rfind(".TET")!=std::string::npos))
		return ImportVolumique::TET;
	if ((filename.rfind(".trianbgz")!=std::string::npos) || (filename.rfind(".TRIANBGZ")!=std::string::npos))
		return ImportVolumique::TRIANBGZ;
	if ((filename.rfind(".ply")!=std::string::npos) || (filename.rfind(".PLY")!=std::string::npos))
		return ImportVolumique::PLY;

	return ImportVolumique::UNKNOWNVOLUME;
}

template <typename PFP>
bool MeshTablesVolume<PFP>::importMesh(const std::string& filename, ImportVolumique::ImportType kind,float scaleFactor=1.0f)
{
	if (kind == ImportVolumique::UNKNOWNVOLUME)
		kind = getFileType(filename);

	switch (kind)
	{
	case ImportVolumique::PLY:
		return importPly(filename);
		break;
	case ImportVolumique::TET:
		return importTet(filename,scaleFactor);
		break;
	case ImportVolumique::TRIANBGZ:
		return importTrianBinGz(filename);
		break;
	default:
		std::cerr << "Not yet supported" << std::endl;
		break;
	}
	return false;
}

template <typename PFP>
bool MeshTablesVolume<PFP>::importTet(const std::string& filename,float scaleFactor=1.0f)
{
	// open file
	std::ifstream fp(filename.c_str(), std::ios::in);
	if (!fp.good())
	{
		std::cerr << "Unable to open file " << filename<< std::endl;
		return false;
	}


    std::string ligne;
    int nbv,nbt;
    // lecture des nombres de sommets/tetra
	std::getline (fp, ligne);
	std::stringstream oss(ligne);
	oss >> nbv;

	//std::cout << "nbV = " << nbv << std::endl;

	std::getline (fp, ligne);
	std::stringstream oss2(ligne);
	oss2 >> nbt;

	//std::cout << "nbT = " << nbt << std::endl;

	//lecture sommets
	std::vector<unsigned int> verticesID;
	verticesID.reserve(nbv);
	for(int i=0; i<nbv;++i)
	{
    	do
    	{
    		std::getline (fp, ligne);
    	} while (ligne.size()==0);

		std::stringstream oss(ligne);

		float x,y,z;
		oss >> x;
		oss >> y;
		oss >> z;
		// on peut ajouter ici la lecture de couleur si elle existe
		VEC3 pos(x*scaleFactor,y*scaleFactor,z*scaleFactor);

		//std::cout << "VEC3 = " << pos << std::endl;

		unsigned int id = m_container.insertLine();
		m_positions[id] = pos;

		verticesID.push_back(id);
	}
	m_nbVertices = verticesID.size();

	//std::cout << "nbVertices = " << m_nbVertices << std::endl;
	m_nbVolumes = nbt;
	//std::cout << "nbVolumes = " << m_nbVolumes << std::endl;

	// lecture tetra
	// normalement m_nbVolumes*12 (car on ne charge que des tetra)

	m_nbFaces=nbt*4;
	m_emb.reserve(nbt*12);

	for (unsigned i=0; i<m_nbVolumes ;++i)
	{
    	do
    	{
    		std::getline (fp, ligne);
    	} while (ligne.size()==0);

		std::stringstream oss(ligne);
		int n;
		oss >> n; // nb de faces d'un volume ?
//		m_nbVerticesPerFace.push_back(3);
//		m_nbVerticesPerFace.push_back(3);
//		m_nbVerticesPerFace.push_back(3);
//		m_nbVerticesPerFace.push_back(3);

//		m_nbFacesPerVolume.push_back(4);
		m_nbEdges.push_back(3);
		m_nbEdges.push_back(3);
		m_nbEdges.push_back(3);
		m_nbEdges.push_back(3);

		int s0,s1,s2,s3;

		oss >> s0;
		oss >> s1;
		oss >> s2;
		oss >> s3;

		m_emb.push_back(verticesID[s0]);
		m_emb.push_back(verticesID[s1]);
		m_emb.push_back(verticesID[s2]);

		m_emb.push_back(verticesID[s1]);
		m_emb.push_back(verticesID[s0]);
		m_emb.push_back(verticesID[s3]);

		m_emb.push_back(verticesID[s2]);
		m_emb.push_back(verticesID[s1]);
		m_emb.push_back(verticesID[s3]);

		m_emb.push_back(verticesID[s0]);
		m_emb.push_back(verticesID[s2]);
		m_emb.push_back(verticesID[s3]);
	}

	fp.close();
	return true;
}

template <typename PFP>
bool MeshTablesVolume<PFP>::importTrianBinGz(const std::string& filename)
{
//	// open file
//	igzstream fs(filename.c_str(), std::ios::in|std::ios::binary);
//
//	if (!fs.good())
//	{
//		std::cerr << "Unable to open file " << filename << std::endl;
//		return false;
//	}
//	// read nb of points
//	int nbp;
//	fs.read(reinterpret_cast<char*>(&nbp), sizeof(int));
//
//	// read points
//	std::vector<EMB*> vertices;
//	{// juste pour limiter la portee des variables
//		vertices.reserve(nbp);
//		int labEmb=0;
//		float* buffer = new float[nbp*3];
//		fs.read(reinterpret_cast<char*>(buffer),3*nbp*sizeof(float));
//		float *ptr = buffer;
//		for (int i=0; i<nbp; ++i)
//		{
//			gmtl::Vec3f pos;
//			pos[0]= *ptr++;
//			pos[1]= *ptr++;
//			pos[2]= *ptr++;
//			EMB* em = EMB::create(pos);
//			em->setLabel(labEmb++);
//			vertices.push_back(em);
//		}
//		delete[] buffer;
//	}
//	m_nbVertices = vertices.size();
//
//	// read nb of faces
//	fs.read(reinterpret_cast<char*>(&m_nbFaces), sizeof(int));
//	m_nbVolumes=1;
//	m_nbFacesPerVolume.push_back(m_nbFaces);
//	m_nbVerticesPerFace.reserve(m_nbFaces);
//	m_emb.reserve(3*m_nbFaces);
//
//	// read indices of faces
//	{// juste pour limiter la portee des variables
//		int* buffer = new int[m_nbFaces*6];
//		fs.read(reinterpret_cast<char*>(buffer),6*m_nbFaces*sizeof(float));
//		int *ptr = buffer;
//
//		for (unsigned i=0; i<m_nbFaces; i++)
//		{
//			m_nbVerticesPerFace.push_back(3);
//			m_emb.push_back(vertices[*ptr++]);
//			m_emb.push_back(vertices[*ptr++]);
//			m_emb.push_back(vertices[*ptr++]);
//		}
//	}
//
//	fs.close();
	return true;
}

template<typename PFP>
bool MeshTablesVolume<PFP>::importPly(const std::string& filename)
{
//	PlyImportData pid;
//
//	if (! pid.read_file(filename) )
//	{
//		std::cerr << "Unable to open file " << filename<< std::endl;
//		return false;
//	}
//
//    // lecture des nombres de sommets/faces/ar�tes
//	m_nbVertices = pid.nbVertices();
//	m_nbFaces = pid.nbFaces();
//	m_nbVolumes=1;
//
//
//	//lecture sommets
//	std::vector<EMB*> vertices;
//	vertices.reserve(m_nbVertices);
//	for (unsigned i=0; i<m_nbVertices;++i)
//	{
//		EMB* em =EMB::create(pid.vertexPosition(i));
//		em->setLabel(i);
//		vertices.push_back(em);
//	}
//
//	m_nbVerticesPerFace.reserve(m_nbFaces);
//	m_emb.reserve(m_nbVertices*8);
//	m_nbFacesPerVolume.push_back(m_nbFaces);
//
//	for (unsigned i=0; i<m_nbFaces;++i)
//	{
//		int n = pid.getFaceValence(i);
//		m_nbVerticesPerFace.push_back(n);
//		int* indices = pid.getFaceIndices(i);
//		for (int j=0;j<n; ++j)
//		{
//			m_emb.push_back(vertices[ indices[j] ]);
//		}
//	}

	return true;
}

} // namespace Import

} // namespace Algo

} // namespace CGoGN