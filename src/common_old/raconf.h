// Copyright (c) rAthena Project (www.rathena.org) - Licensed under GNU GPL
// For more information, see LICENCE in the main folder

#ifndef _rA_CONF_H_
#define _rA_CONF_H_

#include "cbasetypes.h"

#ifdef  __cplusplus
extern "C" {
#endif

// rAthena generic configuration file parser
//
//  Config file Syntax is athena style
//	extended with ini style support (including sections)
//
//	Comments are started with // or ; (ini style)
//

typedef struct raconf   * praconf;


/**
 * Parses a rAthna Configuration file
 *
 * @param file_name path to the file to parse
 *
 * @returns not NULL incase of success
 */
praconf raconf_parse(const char* file_name);


/**
 * Frees a Handle received from raconf_parse
 *
 * @param rc - the handle to free
 */
void    raconf_destroy(praconf rc);


/**
 * Gets the value for Section / Key pair, if key not exists returns _default!
 *
 */
bool            raconf_getbool(praconf rc, const char* section, const char* key, bool _default);
float           raconf_getfloat(praconf rc, const char* section, const char* key, float _default);
int64           raconf_getint(praconf rc, const char* section, const char* key, int64 _default);
const char* raconf_getstr(praconf rc, const char* section, const char* key, const char* _default);

/**
 * Gets the value for Section / Key pair, but has fallback section option if not found in section,
 * if not found in both - default gets returned.
 *
 */
bool        raconf_getboolEx(praconf rc, const char* section, const char* fallback_section, const char* key, bool _default);
float       raconf_getfloatEx(praconf rc, const char* section, const char* fallback_section, const char* key, float _default);
int64       raconf_getintEx(praconf rc, const char* section, const char* fallback_section, const char* key, int64 _default);
const char* raconf_getstrEx(praconf rc, const char* section, const char* fallback_section, const char* key, const char* _default);

#ifdef  __cplusplus
}
#endif

#endif
