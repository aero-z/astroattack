/*
 * CompTrigger_Conditions.h
 * This file is part of Astro Attack
 * Copyright 2011 Christian Zommerfelds
 */

// Enzelne Bedingungen für CompTrigger Komponente

#include "../GNU_config.h" // GNU Compiler-Konfiguration einbeziehen (für Linux Systeme)
#include "CompTrigger.h"

#include <map>
#include <string>

// ========== CompareVariable =========
enum CompareOperator
{
    GreaterThan,
    GreaterThanOrEqualTo,
    LessThan,
    LessThanOrEqualTo,
    EqualTo,
    NotEqualTo
};

class ConditionCompareVariable : public Condition
{
public:
    ConditionCompareVariable( std::map<const std::string, int>::iterator itVariable, CompareOperator comp, int numToCompareWith );
    bool ConditionIsTrue();
    ConditionIdType ID() const { return "CompareVariable"; }
    CompareOperator GetCompareType() const { return m_compareType; }
    int GetNum() const { return m_numToCompareWith; }
    std::string GetVariable() const { return m_itVariable->first; }
    
private:
    std::map<const std::string, int>::iterator m_itVariable;

    CompareOperator m_compareType;
    int m_numToCompareWith;
};

// ========== EntityTouchedThis =========
class ConditionEntityTouchedThis : public Condition
{
public:
    ConditionEntityTouchedThis( std::string entityName );
    bool ConditionIsTrue();
    ConditionIdType ID() const { return "EntityTouchedThis"; }
    std::string GetEntityName() const { return m_entityName; }

private:
    std::string m_entityName;
};
