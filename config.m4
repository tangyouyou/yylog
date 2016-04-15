dnl $Id$
dnl config.m4 for extension yylog

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(yylog, for yylog support,
dnl Make sure that the comment is aligned:
dnl [  --with-yylog             Include yylog support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(yylog, whether to enable yylog support,
Make sure that the comment is aligned:
[  --enable-yylog           Enable yylog support])

if test "$PHP_YYLOG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-yylog -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/yylog.h"  # you most likely want to change this
  dnl if test -r $PHP_YYLOG/$SEARCH_FOR; then # path given as parameter
  dnl   YYLOG_DIR=$PHP_YYLOG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for yylog files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       YYLOG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$YYLOG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the yylog distribution])
  dnl fi

  dnl # --with-yylog -> add include path
  dnl PHP_ADD_INCLUDE($YYLOG_DIR/include)

  dnl # --with-yylog -> check for lib and symbol presence
  dnl LIBNAME=yylog # you may want to change this
  dnl LIBSYMBOL=yylog # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YYLOG_DIR/$PHP_LIBDIR, YYLOG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_YYLOGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong yylog lib version or lib not found])
  dnl ],[
  dnl   -L$YYLOG_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  PHP_SUBST(YYLOG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(yylog, yylog.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
