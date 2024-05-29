/*
 * BinReloc - a library for creating relocatable executables
 * Written by: Mike Hearn <mike@theoretic.com>
 *             Hongli Lai <h.lai@chello.nl>
 * http://autopackage.org/
 *
 * This source code is public domain. You can relicense this code
 * under whatever license you want.
 *
 * See http://autopackage.org/docs/binreloc/ for
 * more information and how to use this.
 *
 * NOTE: if you're using C++ and are getting "undefined reference
 * to br_*", try renaming prefix.c to prefix.cpp
 */

#ifndef _OB_PREFIX_H_
#define _OB_PREFIX_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern char* ob_symbol_unique; 

	/* These are convience macros that replace the ones usually used
	   in Autoconf/Automake projects */
	#undef SELFPATH
	#undef PREFIX
	#undef PREFIXDIR
	#undef BINDIR
	#undef SBINDIR
	#undef DATADIR
	#undef LIBDIR
	#undef LIBEXECDIR
	#undef ETCDIR
	#undef SYSCONFDIR
	#undef CONFDIR
	#undef LOCALEDIR

	#define SELFPATH	(ob_br_thread_local_store (ob_br_locate (ob_symbol_unique)))
	#define PREFIX		(ob_br_thread_local_store (ob_br_locate_prefix (ob_symbol_unique)))
	#define PREFIXDIR	(ob_br_thread_local_store (ob_br_locate_prefix (ob_symbol_unique)))
	#define BINDIR		(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/bin")))
	#define SBINDIR		(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/sbin")))
	#define DATADIR		(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/share")))
	#define LIBDIR		(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/lib")))
	#define LIBEXECDIR	(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/libexec")))
	#define ETCDIR		(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/etc")))
	#define SYSCONFDIR	(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/etc")))
	#define CONFDIR		(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/etc")))
	#define LOCALEDIR	(ob_br_thread_local_store (ob_br_prepend_prefix (ob_symbol_unique, "/share/locale")))


/* The following functions are used internally by BinReloc
   and shouldn't be used directly in applications. */

char *ob_br_locate		(void *symbol);
char *ob_br_locate_prefix	(void *symbol);
char *ob_br_prepend_prefix	(void *symbol, const char *path);

const char *ob_br_thread_local_store (char *str);


/* These macros and functions are not guarded by the ENABLE_BINRELOC
 * macro because they are portable. You can use these functions.
 */

	#define BR_SELFPATH(suffix)	(ob_br_thread_local_store (ob_br_strcat (SELFPATH, suffix)))
	#define BR_PREFIX(suffix)	(ob_br_thread_local_store (ob_br_strcat (PREFIX, suffix)))
	#define BR_PREFIXDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (BR_PREFIX, suffix)))
	#define BR_BINDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (BINDIR, suffix)))
	#define BR_SBINDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (SBINDIR, suffix)))
	#define BR_DATADIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (DATADIR, suffix)))
	#define BR_LIBDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (LIBDIR, suffix)))
	#define BR_LIBEXECDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (LIBEXECDIR, suffix)))
	#define BR_ETCDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (ETCDIR, suffix)))
	#define BR_SYSCONFDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (SYSCONFDIR, suffix)))
	#define BR_CONFDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (CONFDIR, suffix)))
	#define BR_LOCALEDIR(suffix)	(ob_br_thread_local_store (ob_br_strcat (LOCALEDIR, suffix)))        

char *ob_br_strcat	(const char *str1, const char *str2);
char *ob_br_extract_dir	(const char *path);
char *ob_br_extract_prefix(const char *path);
typedef char *(*ob_br_locate_fallback_func) (void *symbol, void *data);
void ob_br_set_locate_fallback_func (ob_br_locate_fallback_func func, void *data);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PREFIX_H_ */
