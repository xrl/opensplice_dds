/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2010 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */

#include "idl_scope.h"
#include "idl_keyDef.h"

#include "os_heap.h"
#include "os_stdlib.h"

/* This module registers the key list defined for
   structures and unions as well as the scope and
   type name to support the generation of query
   helper functions.
*/


/* Contains the list of key definitions */
static idl_keyDef idl_keyDefinitions;

/* Create a new key map specified by scope,
   type name and keylist
*/
static idl_keyMap
idl_keyMapNew (
    const c_metaObject scope,
    const char *typeName,
    const char *keyList)
{
    idl_keyMap keyMap = os_malloc(C_SIZEOF(idl_keyMap));

    keyMap->keyList = os_strdup(keyList);
    keyMap->typeName = os_strdup(typeName);
    keyMap->scope = scope;

    return keyMap;
}

/* Free a key map, without freeing the scope */
static void
idl_keyMapFree (
    const idl_keyMap keyMap)
{
    os_free (keyMap->typeName);
    os_free (keyMap->keyList);
    os_free (keyMap);
}

/* Create a new key definition list */
idl_keyDef
idl_keyDefNew (
    void)
{
    idl_keyDef keyDef = os_malloc(C_SIZEOF(idl_keyDef));

    keyDef->keyList = c_iterNew (0);
    return keyDef;
}

/* Free a key definition list, freeing all list elements */
void
idl_keyDefFree (
    idl_keyDef keyDef)
{
    idl_keyMap keyMap;

    while ((keyMap = c_iterTakeFirst (keyDef->keyList))) {
	idl_keyMapFree (keyMap);
    }
    os_free (keyDef);
}

/* Add a key definition to the specified key definition list */
void
idl_keyDefAdd (
    idl_keyDef keyDef,
    c_metaObject scope,
    const char *typeName,
    const char *keyList)
{
    c_iterInsert (keyDef->keyList, idl_keyMapNew (scope, typeName, keyList));
}

/* Find the key list related to the specified typename in
   the specified scope
*/
c_char *
idl_keyResolve (
    idl_keyDef keyDef,
    idl_scope scope,
    const char *typeName)
{
    c_long li;
    c_long si;
    idl_keyMap keyMap;
    c_metaObject typeScope;

    li = 0;
    /* check all key definition list elements */
    while (li < c_iterLength (keyDef->keyList)) {
	keyMap = c_iterObject (keyDef->keyList, li);
	if (strcmp(typeName, keyMap->typeName) == 0) {
	    /* if the typename equals, check if the scope compares */
	    if ((idl_scopeStackSize(scope) == 0) && (keyMap->scope->definedIn == NULL)) {
		/* Global scope */
		return keyMap->keyList;
	    }
	    si = idl_scopeStackSize (scope)-1;
	    typeScope = keyMap->scope;
	    while (si >= 0) {
		/* for each scope element */
		if (idl_scopeElementType(idl_scopeIndexed (scope, si)) == idl_tModule &&
		    strcmp (typeScope->name, idl_scopeElementName(idl_scopeIndexed (scope, si))) == 0) {
		    /* the scope is a module and the scope name compares */
		    si--;
		    if (typeScope) {
			typeScope = typeScope->definedIn;
		    }
		    if (si == -1) {
			/* bottom of the stack is reached */
		        if (typeScope == NULL || typeScope->name == NULL) {
			    /* the typeScope has reached the bottom too,
			       thus the scopes are equal
			    */
			    return keyMap->keyList;
			}
		    }
		} else {
		    si = -1;
		}
	    }
	}
	li++;
    }
    return NULL;
}

/* Set the default key definition list */
void
idl_keyDefDefSet (
    idl_keyDef keyDef)
{
    idl_keyDefinitions = keyDef;    
}

/* Get the default key definition list */
idl_keyDef
idl_keyDefDefGet (
    void)
{
    return idl_keyDefinitions;
}
