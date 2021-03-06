#include "surface_renderVector.h"

#include "mapHandler.h"

namespace CGoGN
{

namespace SCHNApps
{

bool Surface_RenderVector_Plugin::enable()
{
	//	magic line that init static variables of GenericMap in the plugins
	GenericMap::copyAllStatics(m_schnapps->getStaticPointers());

	m_dockTab = new Surface_RenderVector_DockTab(m_schnapps, this);
	m_schnapps->addPluginDockTab(this, m_dockTab, "Surface_RenderVector");

	m_vectorShader = new Utils::ShaderVectorPerVertex();
	m_vectorShader->setColor(Geom::Vec4f(1.0f, 0.0f, 0.0f, 1.0f));

	registerShader(m_vectorShader);

	connect(m_schnapps, SIGNAL(selectedViewChanged(View*, View*)), this, SLOT(selectedViewChanged(View*, View*)));
	connect(m_schnapps, SIGNAL(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)), this, SLOT(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)));
	connect(m_schnapps, SIGNAL(mapAdded(MapHandlerGen*)), this, SLOT(mapAdded(MapHandlerGen*)));
	connect(m_schnapps, SIGNAL(mapRemoved(MapHandlerGen*)), this, SLOT(mapRemoved(MapHandlerGen*)));

	foreach(MapHandlerGen* map, m_schnapps->getMapSet().values())
		mapAdded(map);

	m_dockTab->updateMapParameters();

	return true;
}

void Surface_RenderVector_Plugin::disable()
{
	delete m_vectorShader;

	disconnect(m_schnapps, SIGNAL(selectedViewChanged(View*, View*)), this, SLOT(selectedViewChanged(View*, View*)));
	disconnect(m_schnapps, SIGNAL(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)), this, SLOT(selectedMapChanged(MapHandlerGen*, MapHandlerGen*)));
	disconnect(m_schnapps, SIGNAL(mapAdded(MapHandlerGen*)), this, SLOT(mapAdded(MapHandlerGen*)));
	disconnect(m_schnapps, SIGNAL(mapRemoved(MapHandlerGen*)), this, SLOT(mapRemoved(MapHandlerGen*)));

	foreach(MapHandlerGen* map, m_schnapps->getMapSet().values())
		mapRemoved(map);
}

void Surface_RenderVector_Plugin::drawMap(View* view, MapHandlerGen* map)
{
	const MapParameters& p = h_viewParameterSet[view][map];

	if (p.positionVBO)
	{
		m_vectorShader->setAttributePosition(p.positionVBO);
		int nb = p.vectorVBOs.size();
		for (int i = 0; i < nb; ++i)
		{
			m_vectorShader->setAttributeVector(p.vectorVBOs[i]);
			m_vectorShader->setScale(map->getBBdiagSize() / 100.0f * p.scaleFactors[i]);
			const QColor& col = p.colors[i];
			m_vectorShader->setColor(Geom::Vec4f(col.redF(), col.greenF(), col.blueF(), 0.0f));
			glLineWidth(1.0f);
			map->draw(m_vectorShader, Algo::Render::GL2::POINTS);
		}
	}
}





void Surface_RenderVector_Plugin::selectedViewChanged(View *prev, View *cur)
{
	m_dockTab->updateMapParameters();
}

void Surface_RenderVector_Plugin::selectedMapChanged(MapHandlerGen *prev, MapHandlerGen *cur)
{
	m_dockTab->updateMapParameters();
}

void Surface_RenderVector_Plugin::mapAdded(MapHandlerGen* map)
{
	connect(map, SIGNAL(vboAdded(Utils::VBO*)), this, SLOT(vboAdded(Utils::VBO*)));
	connect(map, SIGNAL(vboRemoved(Utils::VBO*)), this, SLOT(vboRemoved(Utils::VBO*)));
}

void Surface_RenderVector_Plugin::mapRemoved(MapHandlerGen* map)
{
	disconnect(map, SIGNAL(vboAdded(Utils::VBO*)), this, SLOT(vboAdded(Utils::VBO*)));
	disconnect(map, SIGNAL(vboRemoved(Utils::VBO*)), this, SLOT(vboRemoved(Utils::VBO*)));
}





void Surface_RenderVector_Plugin::vboAdded(Utils::VBO *vbo)
{
	MapHandlerGen* map = static_cast<MapHandlerGen*>(QObject::sender());

	if(map == m_schnapps->getSelectedMap())
	{
		if(vbo->dataSize() == 3)
		{
			m_dockTab->addPositionVBO(QString::fromStdString(vbo->name()));
			m_dockTab->addVectorVBO(QString::fromStdString(vbo->name()));
		}
	}
}

void Surface_RenderVector_Plugin::vboRemoved(Utils::VBO *vbo)
{
	MapHandlerGen* map = static_cast<MapHandlerGen*>(QObject::sender());

	if(map == m_schnapps->getSelectedMap())
	{
		if(vbo->dataSize() == 3)
		{
			m_dockTab->removePositionVBO(QString::fromStdString(vbo->name()));
			m_dockTab->removeVectorVBO(QString::fromStdString(vbo->name()));
		}
	}

	QSet<View*> viewsToUpdate;

	QHash<View*, QHash<MapHandlerGen*, MapParameters> >::iterator i;
	for (i = h_viewParameterSet.begin(); i != h_viewParameterSet.end(); ++i)
	{
		View* view = i.key();
		QHash<MapHandlerGen*, MapParameters>& viewParamSet = i.value();
		MapParameters& mapParam = viewParamSet[map];
		if(mapParam.positionVBO == vbo)
		{
			mapParam.positionVBO = NULL;
			if(view->isLinkedToMap(map)) viewsToUpdate.insert(view);
		}
		if(mapParam.vectorVBOs.contains(vbo))
		{
			mapParam.vectorVBOs.removeOne(vbo);
			if(view->isLinkedToMap(map)) viewsToUpdate.insert(view);
		}
	}

	foreach(View* v, viewsToUpdate)
		v->updateGL();
}





void Surface_RenderVector_Plugin::changePositionVBO(const QString& view, const QString& map, const QString& vbo)
{
	View* v = m_schnapps->getView(view);
	MapHandlerGen* m = m_schnapps->getMap(map);
	if(v && m)
	{
		Utils::VBO* vbuf = m->getVBO(vbo);
		h_viewParameterSet[v][m].positionVBO = vbuf;
		if(v->isSelectedView())
		{
			if (v->isLinkedToMap(m))	v->updateGL();
			if (m->isSelectedMap()) m_dockTab->updateMapParameters();
		}
	}
}

void Surface_RenderVector_Plugin::addVectorVBO(const QString& view, const QString& map, const QString& vbo)
{
	View* v = m_schnapps->getView(view);
	MapHandlerGen* m = m_schnapps->getMap(map);
	if(v && m)
	{
		Utils::VBO* vbuf = m->getVBO(vbo);
		h_viewParameterSet[v][m].vectorVBOs.append(vbuf);
		h_viewParameterSet[v][m].colors.append(QColor("red"));
		h_viewParameterSet[v][m].scaleFactors.append(1.0f);
		if(v->isSelectedView())
		{
			if(v->isLinkedToMap(m))	v->updateGL();
			if(m->isSelectedMap()) m_dockTab->updateMapParameters();
		}
	}
}

void Surface_RenderVector_Plugin::removeVectorVBO(const QString& view, const QString& map, const QString& vbo)
{
	View* v = m_schnapps->getView(view);
	MapHandlerGen* m = m_schnapps->getMap(map);
	if(v && m)
	{
		Utils::VBO* vbuf = m->getVBO(vbo);
		int idx = h_viewParameterSet[v][m].vectorVBOs.indexOf(vbuf);
		h_viewParameterSet[v][m].vectorVBOs.removeAt(idx);
		h_viewParameterSet[v][m].colors.removeAt(idx);
		h_viewParameterSet[v][m].scaleFactors.removeAt(idx);
		if(v->isSelectedView())
		{
			if(v->isLinkedToMap(m))	v->updateGL();
			if(m->isSelectedMap()) m_dockTab->updateMapParameters();
		}
	}
}

void Surface_RenderVector_Plugin::changeVectorScaleFactor(const QString& view, const QString& map, const QString& vbo, float f)
{
	View* v = m_schnapps->getView(view);
	MapHandlerGen* m = m_schnapps->getMap(map);
	if(v && m)
	{
		Utils::VBO* vboPtr = m->getVBO(vbo);
		
		int idx = h_viewParameterSet[v][m].vectorVBOs.indexOf(vboPtr);
		h_viewParameterSet[v][m].scaleFactors[idx] = f;
		if(v->isSelectedView())
		{
			if(v->isLinkedToMap(m))	v->updateGL();
			if (m->isSelectedMap()) m_dockTab->updateMapParameters();
		}
	}
}

void Surface_RenderVector_Plugin::changeVectorColor(const QString& view, const QString& map, const QString& vbo, const QString& col)
{
	View* v = m_schnapps->getView(view);
	MapHandlerGen* m = m_schnapps->getMap(map);
	if (v && m)
	{
		Utils::VBO* vboPtr = m->getVBO(vbo);

		int idx = h_viewParameterSet[v][m].vectorVBOs.indexOf(vboPtr);
		h_viewParameterSet[v][m].colors[idx] = QColor(col);
		if (v->isSelectedView())
		{
			if (v->isLinkedToMap(m))	v->updateGL();
			if (m->isSelectedMap()) m_dockTab->updateMapParameters();
		}
	}
}

#if CGOGN_QT_DESIRED_VERSION == 5
	Q_PLUGIN_METADATA(IID "CGoGN.SCHNapps.Plugin")
#else
	Q_EXPORT_PLUGIN2(Surface_RenderVector_Plugin, Surface_RenderVector_Plugin)
#endif


} // namespace SCHNApps

} // namespace CGoGN
