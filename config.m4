dnl $Id$
dnl config.m4 for extension yymodel

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(yymodel, for yymodel support,
dnl Make sure that the comment is aligned:
dnl [  --with-yymodel             Include yymodel support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(yymodel, whether to enable yymodel support,
Make sure that the comment is aligned:
[  --enable-yymodel           Enable yymodel support])

if test "$PHP_YYMODEL" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-yymodel -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/yymodel.h"  # you most likely want to change this
  dnl if test -r $PHP_YYMODEL/$SEARCH_FOR; then # path given as parameter
  dnl   YYMODEL_DIR=$PHP_YYMODEL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for yymodel files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       YYMODEL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$YYMODEL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the yymodel distribution])
  dnl fi

  dnl # --with-yymodel -> add include path
  dnl PHP_ADD_INCLUDE($YYMODEL_DIR/include)

  dnl # --with-yymodel -> check for lib and symbol presence
  dnl LIBNAME=yymodel # you may want to change this
  dnl LIBSYMBOL=yymodel # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $YYMODEL_DIR/$PHP_LIBDIR, YYMODEL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_YYMODELLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong yymodel lib version or lib not found])
  dnl ],[
  dnl   -L$YYMODEL_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(YYMODEL_SHARED_LIBADD)

  PHP_NEW_EXTENSION(yymodel, yymodel.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
