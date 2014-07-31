#ifndef CONST_H
#define CONST_H

#include <string>

/**
  * Define trackers.
  **/

const int NR_TRACKERS = 2;
const std::string TRACKERS_TYPES[NR_TRACKERS] =
{
    "KLT",
    "LOAD"
};
const std::string TRACKERS_DEFAULT_PARAMS[NR_TRACKERS] =
{
    "700,26,10",
    ""
};

/**
 * Define apps.
 **/

const int NR_APPS = 1;
const std::string APPS_TYPES[NR_APPS] =
{
    "Coherent Filter"
};
const std::string APPS_DEFAULT_PARAMS[NR_APPS] =
{
    "10,15,0.6"
};

#endif // CONST_H
