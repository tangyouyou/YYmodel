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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/standard/php_array.h"

#if PHP_VERSION_ID >= 70000
#include "zend_smart_str.h"
#else
#include "ext/standard/php_smart_str.h"
#endif

#include "php_yymodel.h"


ZEND_DECLARE_MODULE_GLOBALS(yymodel)

zend_class_entry *yymodel_ce;

char *sql;
int sql_len;

/* True global resources - no need for thread safety here */


PHP_INI_BEGIN()
   STD_PHP_INI_ENTRY("yymodel.table_name", "test", PHP_INI_ALL, OnUpdateString, table_name, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_ENTRY("yymodel.field", "*", PHP_INI_ALL, OnUpdateString, field, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_ENTRY("yymodel.where", "", PHP_INI_ALL, OnUpdateString, where, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_ENTRY("yymodel.limit", "", PHP_INI_ALL, OnUpdateString, limit, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_ENTRY("yymodel.order", "", PHP_INI_ALL, OnUpdateString, order, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_ENTRY("yymodel.group", "", PHP_INI_ALL, OnUpdateString, group, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_ENTRY("yymodel.having", "", PHP_INI_ALL, OnUpdateString, having, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_ENTRY("yymodel.sql", "", PHP_INI_ALL, OnUpdateString, sql, zend_yymodel_globals, yymodel_globals)

   STD_PHP_INI_BOOLEAN("yymodel.is_where", "0", PHP_INI_ALL, OnUpdateBool, is_where, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_BOOLEAN("yymodel.is_limit", "0", PHP_INI_ALL, OnUpdateBool, is_limit, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_BOOLEAN("yymodel.is_group", "0", PHP_INI_ALL, OnUpdateBool, is_group, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_BOOLEAN("yymodel.is_order", "0", PHP_INI_ALL, OnUpdateBool, is_order, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_BOOLEAN("yymodel.is_having", "0", PHP_INI_ALL, OnUpdateBool, is_having, zend_yymodel_globals, yymodel_globals)
   STD_PHP_INI_BOOLEAN("yymodel.is_join", "0", PHP_INI_ALL, OnUpdateBool, is_join, zend_yymodel_globals, yymodel_globals)
PHP_INI_END()

// char *get_array_keys(HashTable *ht, int numbers TSRMLS_DC)
// {
// 	zend_ulong num_key;
// 	zend_string *str_key;
// 	zval *entry;
// 	zval new_val;
// 	char *delim_char = ",";
// 	char *key_str;
// 	zend_string *key;
// 	array_init_size(return_value, numbers);
//             if (!numbers) {
//                     return;
//             }
//             zend_hash_real_init(Z_ARRVAL_P(return_value), 1);
//             ZEND_HASH_FILL_PACKED(Z_ARRVAL_P(return_value)) {
//                     /* Go through input array and add keys to the return array */
//                     ZEND_HASH_FOREACH_KEY_VAL_IND(ht, num_key, str_key, entry) {
//                             if (str_key) {
//                                     ZVAL_STR_COPY(&new_val, str_key);
//                             } else {
//                                     ZVAL_LONG(&new_val, num_key);
//                             }
//                             ZEND_HASH_FILL_ADD(&new_val);
//                     } ZEND_HASH_FOREACH_END();
//             } ZEND_HASH_FILL_END();
// 	// 这里需将delim_char的长度强制为1
// 	zend_string *delim = zend_string_init(delim_char, 1, 0);
// 	php_implode(delim, return_value, return_value);
// 	key = Z_STRVAL_P(return_value);
// 	spprintf(&key_str, 0 , "(%s)",key);
// 	return key_str;
// }

PHP_METHOD(YYMODEL_EXT_NAME, __construct)
{
	int argc = ZEND_NUM_ARGS();

	zval *table;
	if (zend_parse_parameters(argc TSRMLS_CC, "z", &table) == FAILURE) {
		return;
	}
	if (Z_TYPE_P(table) != IS_STRING) {
		php_error_docref(NULL, E_ERROR, "the table name must be a string");
	}
	YYMODEL_G(table_name) = estrdup(Z_STRVAL_P(table));
}

PHP_METHOD(YYMODEL_EXT_NAME, field)
{
	char *str;
	int str_len;
	int argc = ZEND_NUM_ARGS();

	zval *field;
	zend_bool type;
	if (zend_parse_parameters(argc TSRMLS_CC, "z|b", &field, &type) == FAILURE) {
		return;
	}
	if (Z_TYPE_P(field) == IS_STRING) {
		YYMODEL_G(field) = estrdup(Z_STRVAL_P(field));
	}

	if (Z_TYPE_P(field) == IS_ARRAY) {
	HashTable *ht;
	ht = Z_ARRVAL_P(field);

	char *delim_char = ",";

	// 这里需将delim_char的长度强制为1
	zend_string *delim = zend_string_init(delim_char, 1, 0);
	php_implode(delim, field, return_value);
	YYMODEL_G(field) = Z_STRVAL_P(return_value);

	zend_string_release(delim);

	}
	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(YYMODEL_EXT_NAME, where)
{
	int argc = ZEND_NUM_ARGS();
	YYMODEL_G(is_where) = 1;

#if PHP_VERSION_ID >=70000 
	zend_string *where;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &where) == FAILURE) {
		return;
	}
	YYMODEL_G(where) = estrdup(ZSTR_VAL(where));
#endif
	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(YYMODEL_EXT_NAME, limit)
{
	int argc = ZEND_NUM_ARGS();
	YYMODEL_G(is_limit) = 1;

#if PHP_VERSION_ID >=70000 
	zend_string *limit;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &limit) == FAILURE) {
		return;
	}
	YYMODEL_G(limit) = estrdup(ZSTR_VAL(limit));
#endif
	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(YYMODEL_EXT_NAME, group)
{
	int argc = ZEND_NUM_ARGS();
	YYMODEL_G(is_group) = 1;

#if PHP_VERSION_ID >=70000 
	zend_string *group;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &group) == FAILURE) {
		return;
	}
	YYMODEL_G(group) = estrdup(ZSTR_VAL(group));
#endif
	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(YYMODEL_EXT_NAME, order)
{
	int argc = ZEND_NUM_ARGS();
	YYMODEL_G(is_order) = 1;

#if PHP_VERSION_ID >=70000 
	zend_string *order;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &order) == FAILURE) {
		return;
	}
	YYMODEL_G(order) = estrdup(ZSTR_VAL(order));
#else
	zval *order;
	if (zend_parse_parameters(argc TSRMLS_CC, "z", &order) == FAILURE) {
		return;
	}
	if (Z_TYPE_P(order) != IS_STRING) {
		php_error_docref(NULL, E_WARNING, "the parameters must be a string");
	} else {
		YYMODEL_G(order) = estrdup(Z_STRVAL_P(order));
	}

#endif
	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(YYMODEL_EXT_NAME, having)
{
	int argc = ZEND_NUM_ARGS();
	YYMODEL_G(is_having) = 1;

#if PHP_VERSION_ID >=70000 
	zend_string *having;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &having) == FAILURE) {
		return;
	}
	YYMODEL_G(having) = estrdup(ZSTR_VAL(having));
#endif
	RETURN_ZVAL(getThis(), 1, 0);
}

PHP_METHOD(YYMODEL_EXT_NAME, select)
{
	if (YYMODEL_G(is_join) == 1) {
		zval *join_table_name;
		zval *join_on_message;
		zval *join_type;
		zval *self = getThis();
		join_table_name = zend_read_property(yymodel_ce, self, ZEND_STRL(YYMODEL_JOIN_TABLE), 1 ,NULL TSRMLS_CC);
		join_on_message = zend_read_property(yymodel_ce, self, ZEND_STRL(YYMODEL_JOIN_MESS), 1, NULL TSRMLS_CC);
		join_type = zend_read_property(yymodel_ce, self, ZEND_STRL(YYMODEL_JOIN_TYPE), 1, NULL TSRMLS_CC);

		sql_len = spprintf(&sql, 0, "select %s from %s %s join %s on %s", YYMODEL_G(field), YYMODEL_G(table_name),
			Z_STRVAL_P(join_type), Z_STRVAL_P(join_table_name), Z_STRVAL_P(join_on_message));
	} else {
		sql_len = spprintf(&sql, 0, "select %s from %s", YYMODEL_G(field), YYMODEL_G(table_name));
	}
	if (YYMODEL_G(is_where) == 1) {
		sql_len = spprintf(&sql, 0, "%s where %s ", sql, YYMODEL_G(where));
	}
	if (YYMODEL_G(is_order) == 1) {
		sql_len = spprintf(&sql, 0, "%s order by %s ", sql, YYMODEL_G(order));
	}
	if (YYMODEL_G(is_limit) == 1) {
		sql_len = spprintf(&sql, 0, "%s limit %s ", sql, YYMODEL_G(limit));
	}
	if (YYMODEL_G(is_group) == 1) {
		sql_len = spprintf(&sql, 0, "%s group by %s ", sql, YYMODEL_G(group));
	}

	YYMODEL_G(sql) = sql;

	RETURN_STRINGL(sql, sql_len);
}

PHP_METHOD(YYMODEL_EXT_NAME, find)
{
	spprintf(&sql, 0, "select %s from %s limit 1", YYMODEL_G(field), YYMODEL_G(table_name));

	if (YYMODEL_G(is_where) == 1) {
		sql_len = spprintf(&sql, 0, "%s where %s ", sql, YYMODEL_G(where));
	}
	if (YYMODEL_G(is_order) == 1) {
		sql_len = spprintf(&sql, 0, "%s order by %s ", sql, YYMODEL_G(order));
	}
	if (YYMODEL_G(is_limit) == 1) {
		sql_len = spprintf(&sql, 0, "%s limit %s ", sql, YYMODEL_G(limit));
	}
	if (YYMODEL_G(is_group) == 1) {
		sql_len = spprintf(&sql, 0, "%s group by %s ", sql, YYMODEL_G(group));
	}
	YYMODEL_G(sql) = sql;

	RETURN_STRINGL(sql, sql_len);
}

PHP_METHOD(YYMODEL_EXT_NAME, count)
{
	spprintf(&sql, 0, "select count(*) from %s", YYMODEL_G(table_name));

	if (YYMODEL_G(is_where) == 1) {
		sql_len = spprintf(&sql, 0, "%s where %s ", sql, YYMODEL_G(where));
	}
	if (YYMODEL_G(is_order) == 1) {
		sql_len = spprintf(&sql, 0, "%s order by %s ", sql, YYMODEL_G(order));
	}
	if (YYMODEL_G(is_limit) == 1) {
		sql_len = spprintf(&sql, 0, "%s limit %s ", sql, YYMODEL_G(limit));
	}
	if (YYMODEL_G(is_group) == 1) {
		sql_len = spprintf(&sql, 0, "%s group by %s ", sql, YYMODEL_G(group));
	}
	YYMODEL_G(sql) = sql;

	RETURN_STRINGL(sql, sql_len);
}

PHP_METHOD(YYMODEL_EXT_NAME, insert)
{
	int argc = ZEND_NUM_ARGS();
	zval *data;
	if (zend_parse_parameters(argc TSRMLS_CC, "z", &data) == FAILURE) {
		return;
	}
	// INSERT INTO `yy_admin` (`uid`, `username`, `password`) VALUES ('2', 'username', 'password');
	if (Z_TYPE_P(data) == IS_STRING) {
		sql_len = spprintf(&sql, 0, "insert into `%s` values (%s)", YYMODEL_G(table_name), Z_STRVAL_P(data));
	}
	if (Z_TYPE_P(data) == IS_ARRAY) {
		HashTable *ht;
		ht = Z_ARRVAL_P(data);

#if PHP_VERSION_ID >= 70000
		zend_ulong num_key;
		zend_string *str_key;
		zval *entry;
		zval new_val;
		zend_string *key;
		zend_string *value;
		char *key_str;
		char *value_str;

		//int numelems = zend_hash_num_elements();

		//key_str = get_array_keys(ht, numelems TSRMLS_CC);

		array_init_size(return_value, zend_hash_num_elements(ht));
            if (!zend_hash_num_elements(ht)) {
                    return;
            }
            zend_hash_real_init(Z_ARRVAL_P(return_value), 1);
            ZEND_HASH_FILL_PACKED(Z_ARRVAL_P(return_value)) {
                    /* Go through input array and add keys to the return array */
                    ZEND_HASH_FOREACH_KEY_VAL_IND(ht, num_key, str_key, entry) {
                            if (str_key) {
                                    ZVAL_STR_COPY(&new_val, str_key);
                            } else {
                                    ZVAL_LONG(&new_val, num_key);
                            }
                            ZEND_HASH_FILL_ADD(&new_val);
                    } ZEND_HASH_FOREACH_END();
            } ZEND_HASH_FILL_END();

        char *delim_char = ",";

		// 这里需将delim_char的长度强制为1
		zend_string *delim = zend_string_init(delim_char, 1, 0);
		php_implode(delim, return_value, return_value);

		key = Z_STRVAL_P(return_value);

		spprintf(&key_str, 0 , "(%s)",key);

		array_init_size(return_value, zend_hash_num_elements(ht));

        if (!zend_hash_num_elements(ht)) {
                return;
        }

        zend_hash_real_init(Z_ARRVAL_P(return_value), 1);

        /* Go through input array and add values to the return array */
        ZEND_HASH_FILL_PACKED(Z_ARRVAL_P(return_value)) {
                ZEND_HASH_FOREACH_VAL(ht, entry) {
                        if (UNEXPECTED(Z_ISREF_P(entry) && Z_REFCOUNT_P(entry) == 1)) {
                                entry = Z_REFVAL_P(entry);
                        }
                        Z_TRY_ADDREF_P(entry);
                        ZEND_HASH_FILL_ADD(entry);
                } ZEND_HASH_FOREACH_END();
        } ZEND_HASH_FILL_END();


        char *delim_char_char = "','";

		// 这里需将delim_char的长度强制为1
		zend_string *delim_char_value = zend_string_init(delim_char_char, 3, 0);

		// 这里需将delim_char的长度强制为1
		php_implode(delim_char_value, return_value, return_value);

		value = Z_STRVAL_P(return_value);

		spprintf(&value_str, 0 , "('%s')",value);
		sql_len = spprintf(&sql, 0, "insert into `%s` %s values %s", YYMODEL_G(table_name),
			key_str, value_str);
#endif
		YYMODEL_G(sql) = estrdup(sql);
	}
	RETURN_STRINGL(sql, sql_len);
}

PHP_METHOD(YYMODEL_EXT_NAME, delete) {
	sql_len = spprintf(&sql, 0, "delete from %s", YYMODEL_G(table_name));
	if (YYMODEL_G(is_where) == 1) {
		sql_len = spprintf(&sql, 0, "%s where %s", sql, YYMODEL_G(where));
	}
	YYMODEL_G(sql) = estrdup(sql);
	RETURN_STRINGL(sql, sql_len);
}

PHP_METHOD(YYMODEL_EXT_NAME, update) {
	// UPDATE `yy_admin` SET `uid`='5', `username`='username', `password`='password' WHERE (`uid`='5');
	int argc = ZEND_NUM_ARGS();
	zval *data;
	if (zend_parse_parameters(argc TSRMLS_CC, "z", &data) == FAILURE) {
		return;
	}
	if (Z_TYPE_P(data) == IS_STRING) {
		sql_len = spprintf(&sql, 0, "update `%s` set %s", YYMODEL_G(table_name), Z_STRVAL_P(data));
	}
	if (Z_TYPE_P(data) == IS_ARRAY) {
		HashTable *ht;
		ht = Z_ARRVAL_P(data);
		int number;
#if PHP_VERSION_ID >= 70000
		zend_ulong num_key;
		zend_string *str_key;
		zval *entry;
		//char update_str[1024];
		//size_t update_len;
		number = zend_hash_num_elements(ht);
		smart_str buf = {0};
		//smart_str buf1 = {0};

		ZEND_HASH_FOREACH_KEY_VAL(ht, num_key, str_key, entry) {
			zend_string *s = zval_get_string(entry);

			sql_len = spprintf(&sql, 0, "`%s`='%s',",
				ZSTR_VAL(str_key), ZSTR_VAL(s));

			smart_str_appendl(&buf,sql,sql_len);
			smart_str_0(&buf);
			
		} ZEND_HASH_FOREACH_END();

		sql_len = spprintf(&sql, ZSTR_LEN(buf.s) + 2, "update `%s` set %s", YYMODEL_G(table_name),
				ZSTR_VAL(buf.s));
		smart_str_free(&buf);
#else
		zval **ppzval;
		number = zend_hash_num_elements(ht);

		zend_hash_internal_pointer_reset(ht);

		while(zend_hash_get_current_data(ht, (void**)&ppzval) == SUCCESS) {
			char *key;
			ulong idx=0;

			zend_hash_get_current_key(ht, &key, &idx, 0);

			zend_hash_move_forward(ht);
		}

#endif
	}
	if (YYMODEL_G(is_where) == 1) {
		sql_len = spprintf(&sql, 0, "%s where %s", sql, YYMODEL_G(where));
	}

	YYMODEL_G(sql) = estrdup(sql);
	RETURN_STRINGL(sql, sql_len);
}

PHP_METHOD(YYMODEL_EXT_NAME, join)
{
	YYMODEL_G(is_join) = 1;
	int argc = ZEND_NUM_ARGS();
	zval *self;
	self = getThis();
#if PHP_VERSION_ID >= 70000
	zend_string *join_table_name;
	zend_string *join_on_message;
	zend_string *join_type;
	if (zend_parse_parameters(argc TSRMLS_CC, "SS|S", &join_table_name,
		&join_on_message, &join_type) == FAILURE) {
		return;
	}
	if (argc == 3) {
		if (strcmp(ZSTR_VAL(join_type),"inner") == 0 || strcmp(ZSTR_VAL(join_type), "left") == 0 
		 || strcmp(ZSTR_VAL(join_type),"right") == 0) {
			zend_update_property_string(yymodel_ce, getThis(), ZEND_STRL(YYMODEL_JOIN_TYPE), ZSTR_VAL(join_type) TSRMLS_CC);
		} else {
			return;
		}
	}
	zend_update_property_string(yymodel_ce, getThis(), ZEND_STRL(YYMODEL_JOIN_TABLE), ZSTR_VAL(join_table_name) TSRMLS_CC);
	zend_update_property_string(yymodel_ce, getThis(), ZEND_STRL(YYMODEL_JOIN_MESS), ZSTR_VAL(join_on_message) TSRMLS_CC);
#else
	zval *join_table_name;
	zval *join_on_message;
	zval *join_type;

	if (zend_parse_parameters(argc TSRMLS_CC, "zz|z", &join_table_name,
		&join_on_message, &join_type) == FAILURE) {
		return;
	}
	if ((Z_TYPE_P(join_table_name) != IS_STRING) || 
		(Z_TYPE_P(join_on_message) != IS_STRING)) {
		php_error_docref(NULL, E_WARNING, "the parameters must be a string");
		RETURN_FAILRE;
	}
	if (argc == 3) {
		if (Z_TYPE_P(join_type) != IS_STRING) {
			php_error_docref(NULL, E_WARNING, "the parameters must be a string");
			RETURN_FAILRE;
		} else {
			if (strcmp(Z_STRVAL_P(join_type),"inner") == 0 || strcmp(Z_STRVAL_P(join_type), "left") == 0 
		 		|| strcmp(Z_STRVAL_P(join_type),"right") == 0) {
				zend_update_property_string(yymodel_ce, getThis(), ZEND_STRL(YYMODEL_JOIN_TYPE), Z_STRVAL_P(join_type) TSRMLS_CC);
			} else {
				return;
			}
		}
	}
	zend_update_property_string(yymodel_ce, getThis(), ZEND_STRL(YYMODEL_JOIN_TABLE), Z_STRVAL_P(join_table_name) TSRMLS_CC);
	zend_update_property_string(yymodel_ce, getThis(), ZEND_STRL(YYMODEL_JOIN_MESS), Z_STRVAL_P(join_on_message) TSRMLS_CC);

#endif
	RETURN_ZVAL(self, 1, 0);
}

PHP_METHOD(YYMODEL_EXT_NAME, getLastSql)
{
	RETURN_STRINGL(YYMODEL_G(sql), strlen(YYMODEL_G(sql)));
}

PHP_METHOD(YYMODEL_EXT_NAME, __destruct)
{
	YYMODEL_G(is_where) = 0;
}

static zend_function_entry yymodel_methods[] = {
	PHP_ME(YYMODEL_EXT_NAME, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(YYMODEL_EXT_NAME, __destruct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
	PHP_ME(YYMODEL_EXT_NAME, field, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, where, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, limit, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, order, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, group, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, having, NULL, ZEND_ACC_PUBLIC)

	PHP_ME(YYMODEL_EXT_NAME, select, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, find, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, insert, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, update, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, delete, NULL, ZEND_ACC_PUBLIC)

	PHP_ME(YYMODEL_EXT_NAME, join, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYMODEL_EXT_NAME, getLastSql, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL}
};

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(yymodel)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "YYmodel", yymodel_methods);
#if PHP_VERSION_ID >= 70000
	yymodel_ce = zend_register_internal_class_ex(&ce, NULL);
#else
	yymodel_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
#endif

	/*declare */
	zend_declare_property_string(yymodel_ce, ZEND_STRL(YYMODEL_FIELD), "*", ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(yymodel_ce, ZEND_STRL(YYMODEL_JOIN_TABLE), "", ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(yymodel_ce, ZEND_STRL(YYMODEL_JOIN_MESS), "", ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_string(yymodel_ce, ZEND_STRL(YYMODEL_JOIN_TYPE), "inner", ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_WHERE), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_ORDER), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_LIMIT), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_GROUP), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_INSERT), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_DELETE), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_UPDATE), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_SELECT), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yymodel_ce, ZEND_STRL(YYMODEL_FIND), ZEND_ACC_PUBLIC TSRMLS_CC);


	REGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(yymodel)
{
	UNREGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(yymodel)
{
#if defined(COMPILE_DL_YYMODEL) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(yymodel)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(yymodel)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yymodel support", "enabled");
	php_info_print_table_row(2, "yymodel_author",  YYMODEL_AUTHOR);
	php_info_print_table_row(2, "yymodel_version", YYMODEL_VERSION);
	php_info_print_table_end();

	
	DISPLAY_INI_ENTRIES();
	
}
/* }}} */

/* {{{ yymodel_functions[]
 *
 * Every user visible function must have an entry in yymodel_functions[].
 */
const zend_function_entry yymodel_functions[] = {
	PHP_FE_END	/* Must be the last line in yymodel_functions[] */
};
/* }}} */

/* {{{ yymodel_module_entry
 */
zend_module_entry yymodel_module_entry = {
	STANDARD_MODULE_HEADER,
	YYMODEL_EXT_NAME,
	yymodel_functions,
	PHP_MINIT(yymodel),
	PHP_MSHUTDOWN(yymodel),
	PHP_RINIT(yymodel),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(yymodel),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(yymodel),
	YYMODEL_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_YYMODEL
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(yymodel)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
