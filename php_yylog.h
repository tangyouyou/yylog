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
  | Author:  tangyouyou www.youyoudon.com                                |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_YYLOG_H
#define PHP_YYLOG_H

extern zend_module_entry yylog_module_entry;
#define phpext_yylog_ptr &yylog_module_entry

#define YYLOG_VERSION   "0.1.1" /* Replace with version number for your extension */
#define YYLOG_EXT_NAME  "yylog"
#define YYLOG_AUTHOR    "tangyouyou"


#ifdef PHP_WIN32
#	define PHP_YYLOG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_YYLOG_API __attribute__ ((visibility("default")))
#else
#	define PHP_YYLOG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif


#define YYLOG_BUFFER_NAME     "yylog_buffer"
#define YYLOG_BUFFER_SIZE     "yylog_buffer_size"

#define YYLOG_ALL           "all"
#define YYLOG_ERROR         "error"




ZEND_BEGIN_MODULE_GLOBALS(yylog)
	   zend_string   *default_path;
     zend_string   *base_path;
     zend_string   *logger;
     zend_string   *last_logger;
     zend_bool     use_buffer;
     int           buffer_size;
ZEND_END_MODULE_GLOBALS(yylog)

extern ZEND_DECLARE_MODULE_GLOBALS(yylog);

#define YYLOG_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(yylog, v)

#if defined(ZTS) && defined(COMPILE_DL_YYLOG)
ZEND_TSRMLS_CACHE_EXTERN();
#endif

#endif	/* PHP_YYLOG_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
