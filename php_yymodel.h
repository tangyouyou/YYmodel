/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_YYMODEL_H
#define PHP_YYMODEL_H

extern zend_module_entry yymodel_module_entry;
#define phpext_yymodel_ptr &yymodel_module_entry


#ifdef PHP_WIN32
#	define PHP_YYMODEL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_YYMODEL_API __attribute__ ((visibility("default")))
#else
#	define PHP_YYMODEL_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#define YYMODEL_EXT_NAME   "yymodel"
#define YYMODEL_AUTHOR     "tangyouyou"
#define YYMODEL_VERSION    "1.1.2"

#define YYMODEL_FIELD       "field"
#define YYMODEL_WHERE       "where"
#define YYMODEL_ORDER       "order"
#define YYMODEL_LIMIT       "limit"
#define YYMODEL_GROUP       "group"

#define YYMODEL_JOIN_TABLE  "join_table"
#define YYMODEL_JOIN_MESS   "join_meesage"
#define YYMODEL_JOIN_TYPE   "join_type"

#define YYMODEL_INSERT      "insert"
#define YYMODEL_SELECT      "select"
#define YYMODEL_UPDATE      "update"
#define YYMODEL_DELETE      "delete"
#define YYMODEL_FIND        "find"

#if PHP_VERSION_ID >= 70000
#define   YYMODEL_RETURN_STRINGL(k,l) RETURN_STRINGL(k,l)
#else
#define   YYMODEL_RETURN_STRINGL(k,l) RETURN_STRINGL(k,l,0)
#endif


ZEND_BEGIN_MODULE_GLOBALS(yymodel)
#if PHP_VERSION_ID >= 70000
     zend_string *table_name;
     zend_string *field;
     zend_string *where;
     zend_string *limit;
     zend_string *order;
     zend_string *group;
     zend_string *having;
     zend_string *sql;
     zend_string *prefix_name;
     zend_string *database_name;
#else
     char *table_name;
     char *field;
     char *where;
     char *limit;
     char *order;
     char *group;
     char *having;
     char *sql;
     char *prefix_name;
     char *database_name;
#endif
     
     zend_bool is_where;
     zend_bool is_order;
     zend_bool is_limit;
     zend_bool is_group;
     zend_bool is_having;
     zend_bool is_join;
ZEND_END_MODULE_GLOBALS(yymodel)


extern ZEND_DECLARE_MODULE_GLOBALS(yymodel);

/* Always refer to the globals in your function as YYMODEL_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
//#define YYMODEL_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(yymodel, v)

#ifdef ZTS
#define YYMODEL_G(v) TSRMG(yymodel_globals_id, zend_yymodel_globals *, v)
#else
#define YYMODEL_G(v) (yymodel_globals.v)
#endif


#if defined(ZTS) && defined(COMPILE_DL_YYMODEL)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif	/* PHP_YYMODEL_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
