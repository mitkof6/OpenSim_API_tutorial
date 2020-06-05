#ifdef WIN32
#   ifdef FixationController_EXPORTS
#       define FixationController_API __declspec(dllexport)
#   else
#       define FixationController_API  __declspec(dllimport)
#   endif
#else
#   define FixationController_API
#endif // WIN32