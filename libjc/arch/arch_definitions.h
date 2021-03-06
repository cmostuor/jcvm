
/*
 * @COPYRIGHT@
 *
 * $Id: arch_definitions.h,v 1.9 2005/05/14 22:20:17 archiecobbs Exp $
 */

#ifndef _ARCH_DEFINITIONS_H_
#define _ARCH_DEFINITIONS_H_

/************************************************************************
 *				Overview				*
 ************************************************************************/

/*

This purpose of this file is to define operating system and/or architecture
specific values. After defining some generic, overridable values, this file
includes the appropriate architecture-specific definitions header file.

Here is a summary of things that need to be defined by this file or
an included file:

String constants
----------------

    _JC_PATH_SEPARATOR	String that separates paths in a search path
    _JC_FILE_SEPARATOR	String that separates directories in a pathname
    _JC_LINE_SEPARATOR	String that marks the end of line in a text file

    _JC_LIBRARY_FMT	printf(3) format string converting a simple native
			library name into a system library filename
    _JC_TEMP_DIR	Operating system directory for temporary files

    _JC_STACK_MINIMUM	Default minimum thread stack size (zero for none)
    _JC_STACK_MAXIMUM	Default maximum thread stack size (zero for none)
    _JC_STACK_DEFAULT	Default thread stack size (should not be zero)

    _JC_DEFAULT_HEAP_SIZE
			Default heap size
    _JC_DEFAULT_LOADER_SIZE
			Default class loader memory size
    _JC_DEFAULT_HEAP_GRANULARITY
			Default heap block size granularity factor: a number
			from 0 to 99, where higher is more granular. For a
			value X, each block size is at least (100-x)% bigger
			than the next bigger block size.

    The last five strings must contain simple numbers (not expressions)
    that can be converted into numerical values via stroull().

Numerical constants
-------------------

    _JC_PAGE_SHIFT	log_2 of the size of a VM page (as returned by
			getpagesize(3)). E.g., 4096 byte pages -> 12.

    _JC_ELF_CLASS	ELF object class
    _JC_ELF_DATA	ELF object data format
    _JC_ELF_MACHINE	ELF object machine type

    _JC_STACK_ALIGN	Runtime stack alignment

    _JC_BIG_ENDIAN	1 if big endian, 0 if little endian

Other definitions
-----------------

    _JC_REGISTER_OFFS	Initializer for an array of integers representing
    			the offsets into a mcontext_t structure where the
			registers live. Used for garbage collection.

*/

/************************************************************************
 *	Generic O/S dependent but architecture independent stuff	*
 ************************************************************************/

	/********	Generic (override as necessary)		*********/

#define _JC_PATH_SEPARATOR	":"
#define _JC_FILE_SEPARATOR	"/"
#define _JC_LINE_SEPARATOR	"\n"
#define _JC_TEMP_DIR		"/var/tmp"
#define _JC_LIBRARY_FMT		"lib%s.so"

#define _JC_STACK_MINIMUM	"0"
#define _JC_STACK_MAXIMUM	"0"
#define _JC_STACK_DEFAULT	"262144"		/* 256K */

#define _JC_DEFAULT_HEAP_SIZE		"134217728"	/* 128M */
#define _JC_DEFAULT_LOADER_SIZE		"33554432"	/* 32M */
#define _JC_DEFAULT_HEAP_GRANULARITY	"85"

/************************************************************************
 *			Architecture-specific definitions		*
 ************************************************************************/

/*
 * The following arch-specific definitions remain:
 *
 * _JC_PAGE_SHIFT
 * _JC_ELF_CLASS
 * _JC_ELF_DATA
 * _JC_ELF_MACHINE
 */

#if defined(__i386__)
#include "i386/i386_definitions.h"
#elif defined(__sparc__)
#include "sparc/sparc_definitions.h"
#elif defined(__alpha__)
#include "alpha/alpha_definitions.h"
#elif defined(__ia64__)
#include "ia64/ia64_definitions.h"
#elif defined(__powerpc__)
#include "powerpc/powerpc_definitions.h"
#else
#error "Unsupported architecture for architecture-specific definitions"
#endif

#endif	/* _ARCH_DEFINITIONS_H_ */

