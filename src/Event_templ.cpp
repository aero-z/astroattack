/*----------------------------------------------------------\
|                     Event_templ.cpp                       |
|                     ---------------                       |
|               Quelldatei von Astro Attack                 |
|                  Christian Zommerfelds                    |
|                          2011                             |
\----------------------------------------------------------*/

#include <boost/bind.hpp>

// for 1 argument
#define EVENT1 Event1<ArgType>

template <typename ArgType>
EventConnection EVENT1::RegisterListener(Function func)
{
    EventConnection evCon;
    typename FunctionList::iterator pos = m_listenerFuncs.insert( m_listenerFuncs.end(), std::make_pair(func,evCon) );
    return evCon;
}

template <typename ArgType>
void EVENT1::Fire(ArgType arg)
{
    for (typename FunctionList::iterator it = m_listenerFuncs.begin(); it != m_listenerFuncs.end();)
    {
        typename FunctionList::iterator cur_pos = it++;
        if (!cur_pos->second.IsValid())
            m_listenerFuncs.erase(cur_pos);
        else
            (cur_pos->first)(arg); // call the function
    }
}

template <typename ArgType>
void EVENT1::UnregisterListener(typename FunctionList::iterator pos)
{
    m_listenerFuncs.erase(pos);
}

// same thing for 2 arguments
#define EVENT2 Event2<ArgType1, ArgType2>

template <typename ArgType1, typename ArgType2>
EventConnection EVENT2::RegisterListener(Function func)
{
    EventConnection evCon;
    typename FunctionList::iterator pos = m_listenerFuncs.insert( m_listenerFuncs.end(), std::make_pair(func,evCon) );
    return evCon;
}

template <typename ArgType1, typename ArgType2>
void EVENT2::Fire(ArgType1 arg1,ArgType2 arg2)
{
    for (typename FunctionList::iterator it = m_listenerFuncs.begin(); it != m_listenerFuncs.end();)
    {
        typename FunctionList::iterator cur_pos = it++;
        if (!cur_pos->second.IsValid())
            m_listenerFuncs.erase(cur_pos);
        else
            (cur_pos->first)(arg1,arg2); // call the function
    }
}

template <typename ArgType1, typename ArgType2>
void EVENT2::UnregisterListener(typename FunctionList::iterator pos)
{
    m_listenerFuncs.erase(pos);
}

// Astro Attack - Christian Zommerfelds - 2011
