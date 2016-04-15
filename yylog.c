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
#include "ext/standard/file.h" // for php_stream
  
#include "php_yylog.h"


ZEND_DECLARE_MODULE_GLOBALS(yylog);

zend_class_entry *yylog_ce;



PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("yylog.default_path",  "/log", PHP_INI_ALL, OnUpdateString, default_path, zend_yylog_globals, yylog_globals)
    STD_PHP_INI_ENTRY("yylog.last_logger", "default", PHP_INI_ALL, OnUpdateString, last_logger, zend_yylog_globals, yylog_globals)
    STD_PHP_INI_ENTRY("yylog.use_buffer", "0", PHP_INI_ALL, OnUpdateBool, use_buffer, zend_yylog_globals, yylog_globals)
    STD_PHP_INI_ENTRY("yylog.buffer_size", "0", PHP_INI_ALL, OnUpdateLongGEZero, buffer_size, zend_yylog_globals, yylog_globals)
PHP_INI_END()


void yylog_init_logger(TSRMLS_D)
{
	YYLOG_G(base_path) = estrdup(YYLOG_G(default_path));
	YYLOG_G(logger)	   = estrdup(YYLOG_G(last_logger));
}

int check_dir_exist(char *log_path TSRMLS_DC)
{
	if (access(log_path, 0) == 0) {
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

int mk_log_dir(TSRMLS_D)
{
	char *base_path;
	char *logger;
	char *dir_path;
	base_path = YYLOG_G(base_path);
	logger = YYLOG_G(logger);

	spprintf(&dir_path, 0, "%s/%s", base_path, logger);

	int check_dir = check_dir_exist(dir_path TSRMLS_CC);
	if (check_dir == FAILURE) {
		zval *zcontext = NULL;
		long mode = 0777;
		zend_bool resursive = 1;
		php_stream_context *context;
		umask(1);
		
		context = php_stream_context_from_zval(zcontext, 0);
		if (php_stream_mkdir(dir_path, mode, (resursive ? PHP_STREAM_MKDIR_RECURSIVE : 0) | REPORT_ERRORS, context)) {
			return FAILURE;
		} else {
			return SUCCESS;
		}
	} else {
		return FAILURE;
	}
}

static int yylog_save_log_by_type(INTERNAL_FUNCTION_PARAMETERS, char *type)
{
	mk_log_dir(TSRMLS_C);
	int argc = ZEND_NUM_ARGS();
#if PHP_VERSION_ID >= 70000
	zend_string *message;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &message) == FAILURE) {
		return;
	}
	//php_printf("%s\n",ZSTR_VAL(message));
#else
	zval *message;
	if (zend_parse_parameters(argc TSRMLS_CC, "z", &message) == FAILURE) {
		return;
	}
	if (Z_TYPE_P(message) == IS_STRING || Z_STRLEN_P(message) > 0) {
		
	}
#endif
}

PHP_FUNCTION(yylog_version)
{
	char *str;
	int len;
	len = spprintf(&str, 0, "%s", YYLOG_VERSION);
	RETURN_STRINGL(str, len);
}

PHP_FUNCTION(yylog_author)
{
	char *str;
	int len;
	len = spprintf(&str, 0, "%s", YYLOG_AUTHOR);
	RETURN_STRINGL(str, len);
}

PHP_METHOD(YYLOG_EXT_NAME, __construct)
{
	yylog_init_logger(TSRMLS_C);
}

PHP_METHOD(YYLOG_EXT_NAME, setBasePath)
{
	int argc = ZEND_NUM_ARGS();
#if PHP_VERSION_ID >= 70000
	zend_string *path;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &path) == FAILURE) {
		return;
	}
	YYLOG_G(base_path) = estrdup(ZSTR_VAL(path));
	RETURN_TRUE;
#else
	zval *path;
	if (zend_parse_parameters(argc TSRMLS_CC, "z", &path) == FAILURE) {
		return;
	}
	if ( argc>0 && (Z_TYPE_P(path) == IS_STRING || Z_STRLEN_P(path) >0) ) {
		if (strcmp(YYLOG_G(base_path), YYLOG_G(default_path)) == 0) {
			efree(YYLOG_G(base_path));
		} else {
			YYLOG_G(base_path) = estrdup(Z_STRVAL_P(path));
		}

#if PHP_VERSION_ID >= 70000
		zval_ptr_dtor(path);
#else
		zval_ptr((void *)&path);
#endif
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
#endif
}

PHP_METHOD(YYLOG_EXT_NAME, getBasePath)
{
	char *path;
	int len;
	len = spprintf(&path, 0 , "%s", YYLOG_G(base_path));
	RETURN_STRINGL(path, len);
}

PHP_METHOD(YYLOG_EXT_NAME, setLogger)
{	
	int argc = ZEND_NUM_ARGS();
#if PHP_VERSION_ID >= 70000
	zend_string *logger;
	if (zend_parse_parameters(argc TSRMLS_CC, "S", &logger) == FAILURE) {
		return;
	}
	YYLOG_G(logger) = estrdup(ZSTR_VAL(logger));
	RETURN_TRUE;
#else
	zval *logger;
	if (zend_parse_parameters(argc TSRMLS_CC, "z", &logger) == FAILURE) {
		return;
	}
	if ( argc>0 && (Z_TYPE_P(logger) == IS_STRING || Z_STRLEN_P(logger) >0) ) {
		if (strcmp(YYLOG_G(logger), YYLOG_G(last_logger)) == 0) {
			efree(YYLOG_G(logger));
		} else {
			YYLOG_G(logger) = estrdup(Z_STRVAL_P(logger));
		}
#if PHP_VERSION_ID >= 70000
		zval_ptr_dtor(logger);
#else
		zval_ptr((void *)&logger);
#endif
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
	
#endif
}

PHP_METHOD(YYLOG_EXT_NAME, getLogger)
{
	char *path;
	int len;
	len = spprintf(&path, 0 , "%s", YYLOG_G(logger));
	RETURN_STRINGL(path, len);
}


/* {{{ YYlog::error($message)
 */
PHP_METHOD(YYLOG_EXT_NAME, error)
{
	yylog_save_log_by_type(INTERNAL_FUNCTION_PARAM_PASSTHRU,YYLOG_ERROR);
}

const zend_function_entry yylog_methods[] = {
	PHP_ME(YYLOG_EXT_NAME, __construct, NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
	PHP_ME(YYLOG_EXT_NAME, setBasePath, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYLOG_EXT_NAME, getBasePath, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYLOG_EXT_NAME, setLogger, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYLOG_EXT_NAME, getLogger, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(YYLOG_EXT_NAME, error, NULL, ZEND_ACC_PUBLIC)
	{ NULL, NULL, NULL}
};

/* {{{ yylog_functions[]
 *
 * Every user visible function must have an entry in yylog_functions[].
 */
const zend_function_entry yylog_functions[] = {
	PHP_FE(yylog_author, NULL)
	PHP_FE(yylog_version,NULL)
	{ NULL, NULL, NULL}
};
/* }}} */



/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(yylog)
{
	zend_class_entry ce;
	INIT_CLASS_ENTRY(ce, "YYlog", yylog_methods);
#if PHP_VERSION_ID >= 70000
	yylog_ce = zend_register_internal_class_ex(&ce, NULL);
#else
	yylog_ce = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC);
#endif
	yylog_ce->ce_flags |= ZEND_ACC_IMPLICIT_PUBLIC;

	zend_declare_property_null(yylog_ce, ZEND_STRL(YYLOG_BUFFER_NAME), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(yylog_ce, ZEND_STRL(YYLOG_BUFFER_SIZE), ZEND_ACC_PUBLIC TSRMLS_CC);

	REGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(yylog)
{
	
	UNREGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(yylog)
{
#if defined(COMPILE_DL_YYLOG) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(yylog)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(yylog)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "yylog support", "enabled");
	php_info_print_table_row(2, "yylog version", YYLOG_VERSION);
	php_info_print_table_row(2, "yylog author", YYLOG_AUTHOR);
	php_info_print_table_end();

	
	DISPLAY_INI_ENTRIES();
	
}
/* }}} */



/* {{{ yylog_module_entry
 */
zend_module_entry yylog_module_entry = {
	STANDARD_MODULE_HEADER,
	YYLOG_EXT_NAME,
	yylog_functions,
	PHP_MINIT(yylog),
	PHP_MSHUTDOWN(yylog),
	PHP_RINIT(yylog),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(yylog),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(yylog),
	YYLOG_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_YYLOG
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE();
#endif
ZEND_GET_MODULE(yylog)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
