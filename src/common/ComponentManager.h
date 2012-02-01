/*
 * ComponentManager.h
 * This file is part of Astro Attack
 * Copyright 2012 Christian Zommerfelds
 */

#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H

#include "Logger.h"
#include "IdTypes.h"

#include <map>
#include <list>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

class Component;
struct GameEvents;
class Logger;

typedef std::list< boost::shared_ptr<Component> > ComponentList;
typedef std::multimap< ComponentTypeId, boost::shared_ptr<Component> > ComponentMap;
typedef std::map< EntityIdType, ComponentMap > EntityMap;

struct Entity
{
    EntityIdType id;
    ComponentList components;
};

class ComponentManager
{
public:
    ComponentManager(GameEvents& events);
    ~ComponentManager();

    void addEntity(const EntityIdType& id, ComponentList& components);
    void removeEntity(const EntityIdType& id);
    const EntityMap& getAllEntities() const;

    // === GetComponent ===
    // There are 2 version of this method:
    // One that returns the component as its specific type using templates
    //   e.g. CompX x = GetComponent<CompX>();
    // And one that returns the component as a base Component type
    //   e.g. Component x = GetComponent("CompX");
    // Prefer using the first one (no need to cast)
    // Use the second version if you can only get the component ID at runtime (as a string)
    template <typename CompType>       CompType* getComponent(const EntityIdType& entId, const ComponentIdType& compId="");
    template <typename CompType> const CompType* getComponent(const EntityIdType& entId, const ComponentIdType& compId="") const;
    //      Component* getComponent(const EntityIdType& entId, const ComponentTypeId& compType);
    //const Component* getComponent(const EntityIdType& entId, const ComponentTypeId& compType) const;
    //      Component* getComponentById(const EntityIdType& entId, const ComponentIdType& compId);
    //const Component* getComponentById(const EntityIdType& entId, const ComponentIdType& compId) const;

    // === GetComponents ===
    // Returns a vector of all matching components in an entity
    // As GetComponent, this method has multiple versions (see above)
    template <typename CompType> std::vector<      CompType*> getComponents(const EntityIdType& id);
    template <typename CompType> std::vector<const CompType*> getComponents(const EntityIdType& id) const;
    //std::vector<      Component*> getComponents(const EntityIdType& id, const ComponentTypeId& compType);
    //std::vector<const Component*> getComponents(const EntityIdType& id, const ComponentTypeId& compType) const;
    // Returns all components (regardless of entity)
    const ComponentMap* getComponents(const EntityIdType& id);

    void writeEntitiesToLogger(Logger& logger, LogLevel level);

private:
    GameEvents& m_gameEvents;

    EntityMap m_entities;

    Component* getComponent(const EntityIdType& entId, const ComponentTypeId& compType, const ComponentIdType& compId) const;
};

// needs to be implemented here because of templates
template <typename CompType>
CompType* ComponentManager::getComponent(const EntityIdType& entId, const ComponentIdType& compId)
{
   return static_cast<CompType*>(getComponent(entId, CompType::getTypeIdStatic(), compId));
}

template <typename CompType>
const CompType* ComponentManager::getComponent(const EntityIdType& entId, const ComponentIdType& compId) const
{
   return static_cast<const CompType*>(getComponent(entId, CompType::getTypeIdStatic(), compId));
}

template <typename CompType>
std::vector<CompType*> ComponentManager::getComponents(const EntityIdType& id)
{
    std::vector<CompType*> ret;

    EntityMap::iterator eit = m_entities.find(id);
    if (eit == m_entities.end())
        return ret;

    ComponentMap::iterator begin;
    ComponentMap::iterator end;
    if (CompType::getTypeIdStatic() == "Component")
    {
        begin = eit->second.begin();
        end = eit->second.end();
    }
    else
    {
        std::pair<ComponentMap::iterator, ComponentMap::iterator> equalRange = eit->second.equal_range(CompType::getTypeIdStatic());
        begin = equalRange.first;
        end = equalRange.second;
    }

    for (ComponentMap::iterator it = begin; it != end; ++it )
        ret.push_back( static_cast<CompType*>(it->second.get()) );
    return ret;
}

template <typename CompType>
std::vector<const CompType*> ComponentManager::getComponents(const EntityIdType& id) const
{
    std::vector<const CompType*> ret;

    EntityMap::const_iterator eit = m_entities.find(id);
    if (eit == m_entities.end())
        return ret;

    ComponentMap::iterator begin;
    ComponentMap::iterator end;
    if (CompType::getTypeIdStatic() == "Component")
    {
        begin = eit->second.begin();
        end = eit->second.end();
    }
    else
    {
        std::pair<ComponentMap::iterator, ComponentMap::iterator> equalRange = eit->second.equal_range(CompType::getTypeIdStatic());
        begin = equalRange.first;
        end = equalRange.second;
    }

    for (ComponentMap::const_iterator it = begin; it != end; ++it )
        ret.push_back( static_cast<CompType*>(it->second.get()) );
    return ret;
}

#endif /* COMPONENTMANAGER_H */
