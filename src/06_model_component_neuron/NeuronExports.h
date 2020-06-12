/**
 * @file NeuronExports.h
 *
 * \brief Definitions for dll exports on Windows.
 *
 * @author Dimitar Stanev <jimstanev@gmail.com>
 */
#ifdef WIN32
#   ifdef Neuron_EXPORTS
#       define Neuron_API __declspec(dllexport)
#   else
#       define Neuron_API  __declspec(dllimport)
#   endif
#else
#   define Neuron_API
#endif // WIN32
