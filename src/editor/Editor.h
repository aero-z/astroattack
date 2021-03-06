/*
 * Editor.h
 * This file is part of Astro Attack
 * Copyright (c) 2014 Christian Zommerfelds
 */

#ifndef EDITOR_H
#define EDITOR_H

#include "common/Vector2D.h"
#include "common/IdTypes.h"
//#include "common/Event.h"

#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <boost/scoped_ptr.hpp>

class GameEvents;
class ComponentManager;
class PhysicsSystem;
class Component;

typedef boost::optional<std::pair<EntityId, std::vector<const Component*> > > OptEntity;

struct EditorGuiData
{
    EditorGuiData() : indexCurVertex (0), currentTexture ("") {}

    Vector2D createdVertices[8];
    int indexCurVertex;
    std::string currentTexture;
    OptEntity selectedEntity;
};

class Editor
{
public:
    Editor(GameEvents&, PhysicsSystem&);
	~Editor();
    void setTextureList(const std::vector<std::string>& textureList);
    void clearLevel();
    void loadLevel(const std::string& fileName);
    void saveLevel(const std::string& fileName);

    void cmdAddVertex(const Vector2D& worldPos);
    void cmdCreateBlock();
    void cmdCancelBlock();
    void cmdCancelVertex();
    void cmdNextTexture();
    void cmdPrevTexture();
    void cmdSelect(const Vector2D& pos);
    bool cmdSetNewEntityId(const EntityId& id);
    void cmdSetNewCompId(const ComponentId& id, const Component* comp);

    const EditorGuiData& getGuiData();

private:

    //void onNewEntity(const EntityIdType& id);
    //void onRemoveEntity(const EntityIdType& id);

    GameEvents& m_events;
    PhysicsSystem& m_physics;
    boost::scoped_ptr<ComponentManager> m_compMgr;

    std::vector<std::string> m_textureList;
    std::vector<std::string>::iterator m_currentTextureIt;

    EditorGuiData m_guiData;

    //EventConnection m_eventConnection1;
    //EventConnection m_eventConnection2;
};

#endif /* EDITOR_H */
