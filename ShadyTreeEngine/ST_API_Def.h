#ifdef SHADYTREE_EXPORTS
    #define ST_API __declspec(dllexport) 
#else
    #define ST_API __declspec(dllimport) 
#endif
