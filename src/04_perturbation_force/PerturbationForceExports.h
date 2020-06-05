/**
 * @file PerturbationForceExports.h
 *
 * \brief Definitions for dll exports on Windows.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#ifdef WIN32
#   ifdef PerturbationForce_EXPORTS
#       define PerturbationForce_API __declspec(dllexport)
#   else
#       define PerturbationForce_API  __declspec(dllimport)
#   endif
#else
#   define PerturbationForce_API
#endif // WIN32
