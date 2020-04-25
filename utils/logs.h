
#include <fstream>

#ifndef _LOGS_INCLUDED
#define _LOGS_INCLUDED

inline std::ofstream g_logger("log.txt");

template<typename T>
inline static void LOG(T msg) { g_logger << msg; g_logger.flush(); }

#endif // !_LOGS_INCLUDED
