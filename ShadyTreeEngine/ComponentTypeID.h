#pragma once 

//why this has ST_API: make variable defined in cpp (so it can be set to 0), 
//but visible outside dll so that getID can be instantiated when generated at compile time.
ST_API extern int typeIDGen; 

/// <summary>
/// Retrieves a unique int identifier for a type. 
/// </summary>
/// <returns></returns>
template<class T>
int getID()
{
    static int idKey = typeIDGen++;
    return idKey;
}