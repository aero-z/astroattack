/*
 * CompTrigger_Conditions.cpp
 * This file is part of Astro Attack
 * Copyright 2011 Christian Zommerfelds
 */

#include "CompTrigger_Conditions.h"
#include "CompPhysics.h"
#include "../Logger.h"

// ========== CompareVariable =========
ConditionCompareVariable::ConditionCompareVariable( std::map<const std::string, int>::iterator itVariable, CompareOperator comp, int numToCompareWith )
: m_itVariable ( itVariable ),
  m_compareType ( comp ),
  m_numToCompareWith ( numToCompareWith )
{}

bool ConditionCompareVariable::isConditionTrue()
{
    switch ( m_compareType )
    {
    case GreaterThan:
        return m_itVariable->second > m_numToCompareWith;
    case GreaterThanOrEqualTo:
        return m_itVariable->second >= m_numToCompareWith;
    case LessThan:
        return m_itVariable->second < m_numToCompareWith;
    case LessThanOrEqualTo:
        return m_itVariable->second <= m_numToCompareWith;
    case EqualTo:
        return m_itVariable->second == m_numToCompareWith;
    case NotEqualTo:
        return m_itVariable->second != m_numToCompareWith;
    default:
        return false;
    }
}

// ========== EntityTouchedThis =========
ConditionEntityTouchedThis::ConditionEntityTouchedThis( std::string entityName )
: m_entityName ( entityName )
{}

bool ConditionEntityTouchedThis::isConditionTrue()
{
    // could use events instead of polling every time

    // TODO: all physics components
    CompPhysics* thisCompPhysics = m_pCompTrigger->getSiblingComponent<CompPhysics>();
    if ( thisCompPhysics == NULL )
    {
        gAaLog.write("WARNING entity '%s': testing if 'EntityTouchedThis' condition is true but there is no CompPhysics", m_pCompTrigger->getEntityId().c_str());
        return false;
    }

    ContactVector contacts = thisCompPhysics->getContacts(true);

    for (size_t i=0; i<contacts.size(); i++)
        if (contacts[i]->comp.getEntityId() == m_entityName)
            return true;

    return false;
}
