/*
 * BinReloc - a library for creating relocatable executables
 * Written by: Mike Hearn <mike@theoretic.com>
 *             Hongli Lai <h.lai@chello.nl>
 * http://autopackage.org/
 * 
 * This source code is public domain. You can relicense this code
 * under whatever license you want.
 *
 * NOTE: if you're using C++ and are getting "undefined reference
 * to ob_br_*", try renaming prefix.c to prefix.cpp
 */

/* WARNING, BEFORE YOU MODIFY PREFIX.C:
 *
 * If you make changes to any of the functions in prefix.c, you MUST
 * change the BR_NAMESPACE macro (in prefix.h).
 * This way you can avoid symbol table conflicts with other libraries
 * that also happen to use BinReloc.
 *
 * Example:
 * #define BR_NAMESPACE(funcName) foobar_ ## funcName
 * --> expands ob_br_locate to foobar_ob_br_locate
 */

#ifndef _PREFIX_C_
#define _PREFIX_C_

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "prefix_unix.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char* ob_symbol_unique = "dsjdkjskadhslkfhds";

/* kulhanek */
const char* obprefix(const char* suffix)
{
 return( BR_PREFIX(suffix) );
}

#undef NULL
#define NULL ((void *) 0)

#ifdef __GNUC__
	#define ob_br_return_val_if_fail(expr,val) if (!(expr)) {fprintf (stderr, "** BinReloc (%s): assertion %s failed\n", __PRETTY_FUNCTION__, #expr); return val;}
#else
	#define ob_br_return_val_if_fail(expr,val) if (!(expr)) return val
#endif /* __GNUC__ */


static ob_br_locate_fallback_func fallback_func = (ob_br_locate_fallback_func) NULL;
static void *fallback_data = NULL;


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>


/**
 * ob_br_locate:
 * symbol: A symbol that belongs to the app/library you want to locate.
 * Returns: A newly allocated string containing the full path of the
 *	    app/library that func belongs to, or NULL on error. This
 *	    string should be freed when not when no longer needed.
 *
 * Finds out to which application or library symbol belongs, then locate
 * the full path of that application or library.
 * Note that symbol cannot be a pointer to a function. That will not work.
 *
 * Example:
 * --> main.c
 * #include "prefix.h"
 * #include "libfoo.h"
 *
 * int main (int argc, char *argv[]) {
 *	printf ("Full path of this app: %s\n", ob_br_locate (&argc));
 *	libfoo_start ();
 *	return 0;
 * }
 *
 * --> libfoo.c starts here
 * #include "prefix.h"
 *
 * void libfoo_start () {
 *	--> "" is a symbol that belongs to libfoo (because it's called
 *	--> from libfoo_start()); that's why this works.
 *	printf ("libfoo is located in: %s\n", ob_br_locate (""));
 * }
 */
char *
ob_br_locate (void *symbol)
{
    printf("here");

	char line[5000];
	FILE *f;
	char *path;

	ob_br_return_val_if_fail (symbol != NULL, NULL);

	f = fopen ("/proc/self/maps", "r");
	if (!f) {
		if (fallback_func)
			return fallback_func(symbol, fallback_data);
		else
			return NULL;
	}

	while (!feof (f))
	{
		unsigned long start, end;

		if (!fgets (line, sizeof (line), f))
			continue;

        printf("%s\n",line);

		if (! (strstr (line, " r-xp ") || strstr (line, " r--p ") ) || !strchr (line, '/'))
			continue;

        printf("%s\n",line);
		sscanf (line, "%lx-%lx ", &start, &end);

        printf("---> %lx-%lx: %lx\n",start,end,symbol);


		if (symbol >= (void *) start && symbol < (void *) end)
		{
            printf("found");
			char *tmp;
			size_t len;

			/* Extract the filename; it is always an absolute path */
			path = strchr (line, '/');

			/* Get rid of the newline */
			tmp = strrchr (path, '\n');
			if (tmp) *tmp = 0;

			/* Get rid of "(deleted)" */
			len = strlen (path);
			if (len > 10 && strcmp (path + len - 10, " (deleted)") == 0)
			{
				tmp = path + len - 10;
				*tmp = 0;
			}

			fclose(f);
			return strdup (path);
		}
	}

	fclose (f);
	return NULL;
}


/**
 * ob_br_locate_prefix:
 * symbol: A symbol that belongs to the app/library you want to locate.
 * Returns: A prefix. This string should be freed when no longer needed.
 *
 * Locates the full path of the app/library that symbol belongs to, and return
 * the prefix of that path, or NULL on error.
 * Note that symbol cannot be a pointer to a function. That will not work.
 *
 * Example:
 * --> This application is located in /usr/bin/foo
 * ob_br_locate_prefix (&argc);   --> returns: "/usr"
 */
char *
ob_br_locate_prefix (void *symbol)
{
	char *path, *prefix;

	ob_br_return_val_if_fail (symbol != NULL, NULL);

	path = ob_br_locate (symbol);
	if (!path) return NULL;

	prefix = ob_br_extract_prefix (path);
	free (path);
	return prefix;
}


/**
 * ob_br_prepend_prefix:
 * symbol: A symbol that belongs to the app/library you want to locate.
 * path: The path that you want to prepend the prefix to.
 * Returns: The new path, or NULL on error. This string should be freed when no
 *	    longer needed.
 *
 * Gets the prefix of the app/library that symbol belongs to. Prepend that prefix to path.
 * Note that symbol cannot be a pointer to a function. That will not work.
 *
 * Example:
 * --> The application is /usr/bin/foo
 * ob_br_prepend_prefix (&argc, "/share/foo/data.png");   --> Returns "/usr/share/foo/data.png"
 */
char *
ob_br_prepend_prefix (void *symbol, const char *path)
{
	char *tmp, *newpath;

	ob_br_return_val_if_fail (symbol != NULL, NULL);
	ob_br_return_val_if_fail (path != NULL, NULL);

	tmp = ob_br_locate_prefix (symbol);
	if (!tmp) return NULL;

	if (strcmp (tmp, "/") == 0)
		newpath = strdup (path);
	else
		newpath = ob_br_strcat (tmp, path);

	/* Get rid of compiler warning ("ob_br_prepend_prefix never used") */
	if (0) ob_br_prepend_prefix (NULL, NULL);

	free (tmp);
	return newpath;
}

static char *ob_br_last_value = (char *) NULL;

static void
ob_br_free_last_value ()
{
    if (ob_br_last_value)
        free (ob_br_last_value);
}

/**
 * ob_br_thread_local_store:
 * str: A dynamically allocated string.
 * Returns: str. This return value must not be freed.
 *
 * Store str in a thread-local variable and return str. The next
 * you run this function, that variable is freed too.
 * This function is created so you don't have to worry about freeing
 * strings. Just be careful about doing this sort of thing:
 *
 * some_function( BR_DATADIR("/one.png"), BR_DATADIR("/two.png") )
 *
 * Examples:
 * char *foo;
 * foo = ob_br_thread_local_store (strdup ("hello")); --> foo == "hello"
 * foo = ob_br_thread_local_store (strdup ("world")); --> foo == "world"; "hello" is now freed.
 */
const char *
ob_br_thread_local_store (char *str)
{
	#if BR_PTHREADS
		char *specific;

		pthread_once (&ob_br_thread_key_once, ob_br_thread_local_store_init);

		specific = (char *) pthread_getspecific (ob_br_thread_key);
		ob_br_str_free (specific);
		pthread_setspecific (ob_br_thread_key, str);

	#else /* BR_PTHREADS */
		static int initialized = 0;

		if (!initialized)
		{
			atexit (ob_br_free_last_value);
			initialized = 1;
		}

		if (ob_br_last_value)
			free (ob_br_last_value);
		ob_br_last_value = str;
	#endif /* BR_PTHREADS */

	return (const char *) str;
}


/**
 * ob_br_strcat:
 * str1: A string.
 * str2: Another string.
 * Returns: A newly-allocated string. This string should be freed when no longer needed.
 *
 * Concatenate str1 and str2 to a newly allocated string.
 */
char *
ob_br_strcat (const char *str1, const char *str2)
{
    printf("%s-%s\n",str1,str2);

	char *result;
	size_t len1, len2;

	if (!str1) str1 = "";
	if (!str2) str2 = "";

	len1 = strlen (str1);
	len2 = strlen (str2);

	result = (char *) malloc (len1 + len2 + 1);
	memcpy (result, str1, len1);
	memcpy (result + len1, str2, len2);
	result[len1 + len2] = '\0';

	return result;
}


/* Emulates glibc's strndup() */
static char *
ob_br_strndup (char *str, size_t size)
{
	char *result = (char *) NULL;
	size_t len;

	ob_br_return_val_if_fail (str != (char *) NULL, (char *) NULL);

	len = strlen (str);
	if (!len) return strdup ("");
	if (size > len) size = len;

	result = (char *) calloc (sizeof (char), len + 1);
	memcpy (result, str, size);
	return result;
}


/**
 * ob_br_extract_dir:
 * path: A path.
 * Returns: A directory name. This string should be freed when no longer needed.
 *
 * Extracts the directory component of path. Similar to g_dirname() or the dirname
 * commandline application.
 *
 * Example:
 * ob_br_extract_dir ("/usr/local/foobar");  --> Returns: "/usr/local"
 */
char *
ob_br_extract_dir (const char *path)
{
	char *end, *result;

	ob_br_return_val_if_fail (path != (char *) NULL, (char *) NULL);

	end = strrchr (path, '/');
	if (!end) return strdup (".");

	while (end > path && *end == '/')
		end--;
	result = ob_br_strndup ((char *) path, end - path + 1);
	if (!*result)
	{
		free (result);
		return strdup ("/");
	} else
		return result;
}


/**
 * ob_br_extract_prefix:
 * path: The full path of an executable or library.
 * Returns: The prefix, or NULL on error. This string should be freed when no longer needed.
 *
 * Extracts the prefix from path. This function assumes that your executable
 * or library is installed in an LSB-compatible directory structure.
 *
 * Example:
 * ob_br_extract_prefix ("/usr/bin/gnome-panel");       --> Returns "/usr"
 * ob_br_extract_prefix ("/usr/local/lib/libfoo.so");   --> Returns "/usr/local"
 * ob_br_extract_prefix ("/usr/local/libfoo.so");       --> Returns "/usr"
 */
char *
ob_br_extract_prefix (const char *path)
{
	char *end, *tmp, *result;

	ob_br_return_val_if_fail (path != (char *) NULL, (char *) NULL);

	if (!*path) return strdup ("/");
	end = strrchr (path, '/');
	if (!end) return strdup (path);

	tmp = ob_br_strndup ((char *) path, end - path);
	if (!*tmp)
	{
		free (tmp);
		return strdup ("/");
	}
	end = strrchr (tmp, '/');
	if (!end) return tmp;

	result = ob_br_strndup (tmp, end - tmp);
	free (tmp);

	if (!*result)
	{
		free (result);
		result = strdup ("/");
	}

	return result;
}


/**
 * ob_br_set_fallback_function:
 * func: A function to call to find the binary.
 * data: User data to pass to func.
 *
 * Sets a function to call to find the path to the binary, in
 * case "/proc/self/maps" can't be opened. The function set should
 * return a string that is safe to free with free().
 */
void
ob_br_set_locate_fallback_func (ob_br_locate_fallback_func func, void *data)
{
	fallback_func = func;
	fallback_data = data;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PREFIX_C */
