
/*
 * @COPYRIGHT@
 *
 * $Id: structures.h,v 1.37 2005/11/09 18:14:22 archiecobbs Exp $
 */

#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

/************************************************************************
 *				Definitions				*
 ************************************************************************/

/*
 * Forward structure declarations and typedef's
 */
typedef struct _jc_boot _jc_boot;
typedef struct _jc_boot_array _jc_boot_array;
typedef struct _jc_boot_fields _jc_boot_fields;
typedef struct _jc_boot_methods _jc_boot_methods;
typedef struct _jc_boot_objects _jc_boot_objects;
typedef struct _jc_boot_types _jc_boot_types;
typedef struct _jc_class_node _jc_class_node;
typedef struct _jc_class_ref _jc_class_ref;
typedef struct _jc_class_save _jc_class_save;
typedef struct _jc_classbytes _jc_classbytes;
typedef struct _jc_cpath_entry _jc_cpath_entry;
typedef struct _jc_dwarf2_line_hdr _jc_dwarf2_line_hdr;
typedef struct _jc_elf_debug_lines _jc_elf_debug_lines;
typedef struct _jc_elf_info _jc_elf_info;
typedef struct _jc_elf_loadable _jc_elf_loadable;
typedef struct _jc_ex_info _jc_ex_info;
typedef struct _jc_exec_stack _jc_exec_stack;
typedef struct _jc_fat_lock _jc_fat_lock;
typedef struct _jc_fat_locks _jc_fat_locks;
typedef struct _jc_heap _jc_heap;
typedef struct _jc_heap_size _jc_heap_size;
typedef struct _jc_heap_sweep _jc_heap_sweep;
typedef struct _jc_initialization _jc_initialization;
typedef struct _jc_interp_stack _jc_interp_stack;
typedef struct _jc_java_stack _jc_java_stack;
typedef struct _jc_jvm _jc_jvm;
typedef struct _jc_map_state _jc_map_state;
typedef struct _jc_method_node _jc_method_node;
typedef struct _jc_native_frame _jc_native_frame;
typedef struct _jc_native_frame_list _jc_native_frame_list;
typedef struct _jc_native_lib _jc_native_lib;
typedef struct _jc_objpath_entry _jc_objpath_entry;
typedef struct _jc_properties _jc_properties;
typedef struct _jc_property _jc_property;
typedef struct _jc_resolve_info _jc_resolve_info;
typedef struct _jc_saved_frame _jc_saved_frame;
typedef struct _jc_scan_frame _jc_scan_frame;
typedef struct _jc_scan_list _jc_scan_list;
typedef struct _jc_splay_tree _jc_splay_tree;
typedef struct _jc_stab _jc_stab;
typedef struct _jc_stack_crawl _jc_stack_crawl;
typedef struct _jc_threads _jc_threads;
typedef struct _jc_trace_info _jc_trace_info;
typedef struct _jc_type_node _jc_type_node;
typedef struct _jc_uni_mem _jc_uni_mem;
typedef struct _jc_uni_page_list _jc_uni_page_list;
typedef struct _jc_uni_pages _jc_uni_pages;

TAILQ_HEAD(_jc_uni_page_list, _jc_uni_pages);
SLIST_HEAD(_jc_native_frame_list, _jc_native_frame);

#include "cf_parse.h"
#include "zip.h"

/************************************************************************
 *			    Splay trees					*
 ************************************************************************/

/* One splay tree */
struct _jc_splay_tree {
	size_t		offset;		/* offset of node in an item */
	int		size;		/* number of nodes in tree */
	_jc_splay_node	*root;		/* root of the tree */
	_jc_splay_cmp_t	*compare;	/* item comparision function */
};

/************************************************************************
 *			ELF file structures				*
 ************************************************************************/

/* This describes a loadable section within an ELF object */
struct _jc_elf_loadable {
	const char		*name;		/* name of section */
	const Elf_Shdr		*shdr;		/* section header */
	const Elf_Shdr		*rel;		/* normal relocations section */
	const Elf_Shdr		*rela;		/* addend relocations section */
	const char		*bytes;		/* start of section in file */
	char			*vaddr;		/* start of loaded section */
	Elf_Off			offset;		/* offset of loaded section */
};

/* This describes the ELF debug section containing line number info */
struct _jc_elf_debug_lines {
	u_char			type;		/* _JC_LINE_DEBUG_* */
	const char		*strings;	/* associated string section */
	_jc_elf_loadable	loadable;	/* loading info */
};

/*
 * Linking and symbol information associated with an ELF object.
 * This information is only needed for loading and linking.
 */
struct _jc_elf_info {
	char			*map_base;	/* base of mmap() region */
	size_t			map_size;	/* size of mmap() region */
	const Elf_Ehdr		*ehdr;		/* ELF header (=map_base) */
	const Elf_Shdr		*shdrs;		/* section headers */
	int			num_symbols;	/* number of symbols */
	const Elf_Sym		*symbols;	/* symbols */
	int			num_loadables;	/* number loadable sections */
	_jc_elf_loadable	*loadables;	/* loadable sections */
	_jc_elf_loadable	**shdr2section;	/* maps shdr index -> section */
	const char		*strings;	/* strings in string table */
	_jc_env			*resolver;	/* currently resolving thread */
	_jc_elf_debug_lines	debug_lines;	/* ELF debug line info */
};

/*
 * Information associated with a loaded ELF object.
 */
struct _jc_elf {
	_jc_elf_info		*info;		/* transient linking info */
	_jc_class_loader	*loader;	/* loader who loaded me */
	char			*vaddr;		/* load address of object */
	Elf_Off			vsize;		/* size of loaded object */
	_jc_splay_tree		types;		/* types defined in file */
	volatile _jc_word	refs;		/* number of references */
	char			pathname[0];	/* original file pathname */
};

/* One entry in the object file search path */
struct _jc_objpath_entry {
	int			type;		/* _JC_OBJPATH_* */
	char			*pathname;
};

/* Information about a method function during ELF file processing */
struct _jc_method_node {
	const char		*cname;		/* encoded class name */
	int			clen;		/* length of 'cname' */
	const char		*mname;		/* encoded method name */
	int			mlen;		/* length of 'mname' */
	size_t			size;		/* size of function body */
	_jc_method		*method;	/* associated _jc_method */
	_jc_splay_node		node;		/* node in temporary tree */
};

/* Information maintained when resolving classes */
struct _jc_resolve_info {
	_jc_type		*type;
	_jc_class_loader	*loader;
	_jc_object		**implicit_refs;
	int			num_implicit_refs;
	int			num_implicit_alloc;
};

/* Superclass and superinterfaces for an unresolved ELF type */
struct _jc_super_info {
	_jc_type		*superclass;
	_jc_type		*interfaces[0];
};

/************************************************************************
 *			Line Number Information				*
 ************************************************************************/

/* One entry in a method's [PC -> Java line number table] map */
struct _jc_pc_map {
	const void	*pc;
	uint16_t	jline;
};

/* Transient state used when building a PC map */
struct _jc_map_state {
	int			map_alloc;
	int			last_linenum;
	int			last_map;
	_jc_linenum_info	linenum;
	_jc_pc_map_info		pc_map;
};

/* ELF stabs entry format */
struct _jc_stab {
	uint32_t	sindex;
	u_char		type;
	u_char		misc;
	uint16_t	desc;
	Elf_Addr	value;
};

/* DWARF line program header */
struct _jc_dwarf2_line_hdr {
	unsigned char	minimum_instruction_length;
	unsigned char	default_is_stmt;
	signed char	line_base;
	unsigned char	line_range;
	unsigned char	opcode_base;
	unsigned char	standard_opcode_lengths[0];
};

/* Class bytes: in-memory copy of a class file byte[] array */
struct _jc_classbytes {
	u_char			*bytes;
	size_t			length;
	jlong			hash;
	volatile _jc_word	refs;
	void			(*freer)(_jc_classbytes *);
};

/*
 * VM meta-information about a class file, stored in the vm->classfiles tree.
 *
 * The 'bytes' field is non-NULL only if (a) the class was first loaded by
 * a non-bootstrap class loader, and (b) object file generation is enabled.
 *
 * Otherwise, we don't need it (not (b)) or we can get it by searching the
 * boot loader classpath (not (a)).
 *
 * Each loaded type holds one reference for its own class and one for
 * each class listed in its dependency list.
 */
struct _jc_class_node {
	const char	*name;			/* name of class */
	jint		refs;			/* number of references */
	jlong		hash;			/* hash of class file bytes */
	_jc_classbytes	*bytes;			/* saved bytes for code gen */
	_jc_splay_node	node;			/* in vm->classfiles */
};

/* A reference to one class from another class */
struct _jc_class_ref {
	const char	*name;			/* NOTE: not nul-terminated */
	size_t		len;
};

/************************************************************************
 *			Uni-allocator memory				*
 ************************************************************************/

/*
 * This structure lives at the beginning of each string of pages
 * allocated by a uni-allocator.
 */
struct _jc_uni_pages {
	TAILQ_ENTRY(_jc_uni_pages)	link;
	int				num_pages;
	size_t				offset;
};

/*
 * This describes one uni-allocator.
 */
struct _jc_uni_mem {
	_jc_uni_page_list		pages;
	int				min_pages;
	volatile _jc_word		*avail_pages;
};

/************************************************************************
 *			Thread related structures			*
 ************************************************************************/

/*
 * Native reference frame, containing explicit references to objects that
 * are held by e.g. JNI code. These must be explicitly accounted for
 * because we are not able to reliably scan a native call frame for
 * object references directly. Same applies to libjc internal code.
 */
struct _jc_native_frame {
	SLIST_ENTRY(_jc_native_frame)	link;
	_jc_word			flags;
	_jc_object			*refs[_JC_NATIVE_REFS_PER_FRAME];
};

/*
 * Common information that lives at the beginning of
 * _jc_exec_stack and _jc_interp_stack.
 */
struct _jc_java_stack {
	jboolean			interp;
	_jc_java_stack			*next;
};

/*
 * Structure representing a contiguous portion of the C call stack
 * (some functions within which may be Java method C functions).
 * The "frame" and "pc" fields define the top of a C stack fragment
 * (or, in the case of the current thread, they may be NULL to indicate
 * that the top of the stack is the top of the real C stack and has not
 * yet been "marked"). The bottom of the fragment is always a stack
 * frame for _jc_invoke_jcni_a(). The next deeper contiguous stack frame
 * fragment is pointed to by the "next" field.
 *
 * The "next" field is needed is so we can "skip over" stack frames
 * associated with native code, signal frames, etc. This is because
 * we can't reliabily follow the chain of frame pointers through
 * those frames (especially signal frames).
 */
struct _jc_exec_stack {
	_jc_java_stack			jstack;
	_jc_stack_frame			frame;
	const void			*pc;
	mcontext_t			regs;
};

/*
 * Represents one interpreted Java method stack frame.
 */
struct _jc_interp_stack {
	_jc_java_stack			jstack;
	_jc_method			*method;
	_jc_word			*locals;
	const int			*pcp;
	jboolean			clipped;
};

/*
 * Structure used to save an exception Java stack trace. When the
 * VMThrowable.fillInStackTrace() is called, what gets saved in the
 * "vmdata" field of the VMThrowable object is an array of these.
 *
 * If/when Throwable.getStackTrace() is called later, this
 * information gets converted into a StackTraceElement[] array.
 *
 * For exec methods, 'pc' is the actual CPU program counter.
 * For interp methods, 'pc' points to the (int) instruction index.
 */
struct _jc_saved_frame {
	_jc_method			*method;
	union {
		const void			*pc;	/* executable */
		int				ipc;	/* interpreted */
	}				u;
};

/*
 * Structure used when crawling the stack looking for Java methods.
 * See _jc_stack_crawl_first() and _jc_stack_crawl_next().
 */
struct _jc_stack_crawl {
	_jc_method			*method;	/* current method */
	_jc_java_stack			*stack;		/* current chunk */
	_jc_stack_frame			frame;		/* exec only */
	const void			*pc;		/* exec only */
};

/*
 * Thread-private structure used when trying to find class files.
 * See _jc_get_class_node().
 */
struct _jc_class_save {
	const char	*name;			/* name of class */
	_jc_classbytes	*bytes;			/* class file bytes */
	_jc_class_save	*next;			/* next in list */
};

/*
 * Exception info stored for future posting.
 */
struct _jc_ex_info {
	jshort		num;
	char		msg[200];
};

/*
 * The JC internal structure that corresponds to a Java thread.
 */
struct _jc_env {
	_jc_env_head			head;		/* must be first */
	_jc_jvm				*vm;		/* vm that owns me */

	/* Interpreter trampoline info */
	_jc_method			*interp;	/* interpret method */

	/* Stack info */
	void				*stack;		/* thread's stack */
	size_t				stack_size;	/* size of stack */
	char				*stack_limit;	/* for stack overflow */
	_jc_java_stack			*java_stack;	/* java stack info */
	_jc_method			*jni_method;	/* innermost JNI meth */
	_jc_value			retval;		/* invoke rtn value */

	/* Thread info */
	volatile _jc_word		status;		/* JC_THRDSTAT_* */
	jint				thread_id;	/* unique thread id */
	_jc_object			*instance;	/* java.lang.Thread */
	char				text_status[_JC_MAX_TEXT_STATUS];

	/* Lock info */
	_jc_word			thinlock_id;
	struct {

	    /* Information about other threads waiting on this one */
	    struct {
		pthread_mutex_t		mutex;		/* protects struct */
#ifndef NDEBUG
		_jc_env			*mutex_owner;	/* current owner */
#endif
		volatile _jc_word	contention;	/* contention flag */
		SLIST_HEAD(, _jc_env)	waiters;	/* waiting on me */
	    } owner;

	    /* Information about the other thread this one is waiting on */
	    struct {
		_jc_object		*object;	/* contended object */
		pthread_cond_t		cond;		/* try again signal */
		SLIST_ENTRY(_jc_env)	link;		/* link in 'waiters' */
	    } waiter;
	}				lock;

	/* Local native reference frames */
	_jc_native_frame_list		native_locals;

	/* JNI native interface */
	const struct JNINativeInterface	*jni_interface;

	/* Recursive exception detector */
	jlong				in_vmex;	/* JC_VMEX_* bits */

	/* Exception information filled in by some functions on error */
	_jc_ex_info			ex;

#if !HAVE_GETCONTEXT
	/* For our poor man's getcontext() using signals */
	mcontext_t			*ctx;
#endif

	/* Thread flags */
	const char			*generating;
	jboolean			out_of_memory;
	jboolean			handling_signal;

	/*
	 * Support for Thread.interrupt() and suspend/resume.
	 * Note the VM global mutex protects the suspend/resume fields.
	 */
	jboolean			suspended;
	jboolean			resumption_initialized;
	pthread_cond_t			resumption;
	volatile _jc_word		interrupt_status;
	_jc_fat_lock			*interruptible_lock;

	/* Used by _jc_get_class_node() */
	_jc_class_save			*class_save;

	/* Support for threads throwing exceptions in other threads */
	_jc_object			*volatile cross_exception;

	/* Link in VM list */
	LIST_ENTRY(_jc_env)		link;		/* entry in vm list */
};

/************************************************************************
 *			Class loader information			*
 ************************************************************************/

/*
 * An entry in a class loader's initialized or partially derived types tree.
 *
 * In the case of the partially derived tree, the structure pointed to by
 * the "type" field is not complete; only type->name can be used.
 */
struct _jc_type_node {
	_jc_type		*type;		/* corresponding type */
	_jc_splay_node		node;		/* initiat*, deriving trees */
	_jc_env			*thread;	/* thread loading type */
};

/*
 * Information about a native library.
 */
struct _jc_native_lib {
	char				*name;
	void				*handle;
	STAILQ_ENTRY(_jc_native_lib)	link;
};

/*
 * Information associated with a class loader.
 *
 * All fields are protected by the mutex.
 */
struct _jc_class_loader {
	_jc_object			*instance;
	pthread_mutex_t			mutex;
#ifndef NDEBUG
	_jc_env				*mutex_owner;
#endif
	pthread_cond_t			cond;
	_jc_uni_mem			uni;
	_jc_splay_tree			initiated_types;/* _jc_type_node's */
	_jc_splay_tree			deriving_types;	/* _jc_type_node's */
	_jc_splay_tree			defined_types;	/* _jc_type's */
	int				num_implicit_refs;
	_jc_object			**implicit_refs;
	jboolean			*objects_loaded;
	jboolean			gc_mark;
	jboolean			waiters;
	STAILQ_HEAD(, _jc_native_lib)	native_libs;
	LIST_ENTRY(_jc_class_loader)	link;
};

/************************************************************************
 *		Bootstrap class loader information			*
 ************************************************************************/

/*
 * Classes that are specially resolved at bootstrap time.
 */
struct _jc_boot_types {

	/* Normal class types */
	_jc_type	*AccessibleObject;
	_jc_type	*Buffer;
	_jc_type	*Class;
	_jc_type	*ClassLoader;
	_jc_type	*Cloneable;
	_jc_type	*Constructor;
	_jc_type	*ReadWrite;
	_jc_type	*Error;
	_jc_type	*Field;
	_jc_type	*Generate;
	_jc_type	*Method;
	_jc_type	*Object;
	_jc_type	*PhantomReference;
	_jc_type	*Pointer;
	_jc_type	*Reference;
	_jc_type	*Serializable;
	_jc_type	*SoftReference;
	_jc_type	*StackTraceElement;
	_jc_type	*String;
	_jc_type	*System;
	_jc_type	*Thread;
	_jc_type	*ThreadGroup;
	_jc_type	*Throwable;
	_jc_type	*VMStackWalker;
	_jc_type	*VMThread;
	_jc_type	*VMThrowable;
	_jc_type	*WeakReference;

	/* Primitive types */
	_jc_type	*prim[_JC_TYPE_MAX];

	/* Primitive array types */
	_jc_type	*prim_array[_JC_TYPE_MAX];

	/* Primitive wrapper types */
	_jc_type	*prim_wrapper[_JC_TYPE_MAX];

	/* Exception and Error classes */
	_jc_type	*vmex[_JC_VMEXCEPTION_MAX];

	/* Object array types */
	_jc_type	*Class_array;
	_jc_type	*Constructor_array;
	_jc_type	*Field_array;
	_jc_type	*Method_array;
	_jc_type	*StackTraceElement_array;
};

/*
 * Methods that are specially resolved at bootstrap time.
 */
struct _jc_boot_methods {
	struct {
		_jc_method	*isAccessible;
	}			AccessibleObject;
	struct {
		_jc_method	*loadClass;
		_jc_method	*getSystemClassLoader;
	}			ClassLoader;
	struct {
		_jc_method	*init;
	}			Constructor;
	struct {
		_jc_method	*init;
	}			ReadWrite;
	struct {
		_jc_method	*init;
	}			Field;
	struct {
		_jc_method	*v;
		_jc_method	*generateObject;
	}			Generate;
	struct {
		_jc_method	*init;
		_jc_method	*invoke;
	}			Method;
	struct {
		_jc_method	*finalize;
		_jc_method	*notifyAll;
		_jc_method	*toString;
		_jc_method	*wait;
	}			Object;
	struct {
		_jc_method	*enqueue;
	}			Reference;
	struct {
		_jc_method	*init;
	}			StackTraceElement;
	struct {
		_jc_method	*init;
		_jc_method	*intern;
	}			String;
	struct {
		_jc_method	*init;
		_jc_method	*stop;
	}			Thread;
	struct {
		_jc_method	*addThread;
		_jc_method	*init;
		_jc_method	*uncaughtException;
	}			ThreadGroup;
	struct {
		_jc_method	*init;
		_jc_method	*run;
	}			VMThread;
	struct {
		_jc_method	*value;
		_jc_method	*init;
	}			prim_wrapper[_JC_TYPE_MAX];
	struct {
		_jc_method	*init;
	}			vmex[_JC_VMEXCEPTION_MAX];
};

/*
 * Fields that are specially resolved at bootstrap time.
 */
struct _jc_boot_fields {
	struct {
		_jc_field	*cap;
		_jc_field	*address;
	}			Buffer;
	struct {
		_jc_field	*pd;
		_jc_field	*vmdata;
	}			Class;
	struct {
		_jc_field	*parent;
		_jc_field	*vmdata;
	}			ClassLoader;
	struct {
		_jc_field	*clazz;
		_jc_field	*slot;
	}			Constructor;
	struct {
		_jc_field	*declaringClass;
		_jc_field	*slot;
	}			Field;
	struct {
		_jc_field	*declaringClass;
		_jc_field	*slot;
	}			Method;
	struct {
		_jc_field	*data;
	}			Pointer;
	struct {
		_jc_field	*queue;
		_jc_field	*referent;
	}			Reference;
	struct {
		_jc_field	*value;
		_jc_field	*offset;
		_jc_field	*count;
	}			String;
	struct {
		_jc_field	*in;
		_jc_field	*out;
		_jc_field	*err;
	}			System;
	struct {
		_jc_field	*daemon;
		_jc_field	*group;
		_jc_field	*name;
		_jc_field	*priority;
		_jc_field	*vmThread;
	}			Thread;
	struct {
		_jc_field	*root;
	}			ThreadGroup;
	struct {
		_jc_field	*cause;
		_jc_field	*detailMessage;
		_jc_field	*vmState;
	}			Throwable;
	struct {
		_jc_field	*thread;
		_jc_field	*vmdata;
	}			VMThread;
	struct {
		_jc_field	*vmdata;
	}			VMThrowable;
};

/*
 * Objects that are specially instantiated at bootstrap time.
 */
struct _jc_boot_objects {
	_jc_object		*vmex[_JC_VMEXCEPTION_MAX];
	_jc_object		*systemThreadGroup;
};

/*
 * Bootstrap class loader info for deriving array types.
 */
struct _jc_boot_array {
	int			num_interfaces;
	_jc_type		**interfaces;
	_jc_method		***imethod_hash_table;
	const void		**imethod_quick_table;
};

/* One entry in a classpath style search path */
struct _jc_cpath_entry {
	int			type;			/* _JC_CPATH_* */
	char			*pathname;
	_jc_zip			*zip;
};

/*
 * Boot class loader info. All fields are read only after VM creation
 * except *loader and "loading_types", which are protected by loader->mutex.
 */
struct _jc_boot {
	int			class_path_len;
	_jc_cpath_entry		*class_path;
	_jc_boot_array		array;
	_jc_boot_types		types;
	_jc_boot_methods	methods;
	_jc_boot_fields		fields;
	_jc_boot_objects	objects;
	_jc_class_loader	*loader;
	_jc_splay_tree		loading_types;		/* _jc_type_node's */
};

/************************************************************************
 *			Java heap and garbage collection		*
 ************************************************************************/

/*
 * Heap info.
 *
 * All info in this structure is either read-only or accessed using
 * atomic operations (except during garbage collection when the world
 * is stopped).
 */
struct _jc_heap {
	int			num_pages;
	void			*volatile pages;
	int			num_sizes;
	_jc_heap_size		*sizes;
	volatile int		next_page;
	void			*mem;
	size_t			size;
	int			granularity;
	int			max_pages;
};

/*
 * Descriptor for each heap block size.
 */
struct _jc_heap_size {
	_jc_word	*volatile pages;	/* list of pages this size */
	_jc_word	*volatile hint;		/* next free (?) block to try */
	int		size;			/* size of each block */
	int		num_blocks;		/* number blocks per page */
};

/*
 * State maintained when visiting all objects in the heap.
 */
struct _jc_heap_sweep {
	char		*page;
	char		*end;
	char		*block;
	_jc_heap_size	*size;
	int		bsi;
	int		block_size;
	int		blocks_left;
	int		blocks_live;
	int		npages;
	_jc_heap	*heap;
};

/*
 * GC trace reference list structure. Points to a list of object
 * references to scan during garbage collection.
 */
struct _jc_scan_list {
	_jc_object	**start;
	_jc_object	**end;
};

/*
 * One frame in the stack of reference pointers we keep while tracing
 * objects during GC. Each frame holds a fixed number of _jc_scan_list's.
 * We aggregate them into larger frames to decrease the number of
 * allocation operations required.
 */
struct _jc_scan_frame {
	int		posn;		/* current position in this frame */
	_jc_scan_frame	*prev;		/* previous reference stack frame */
	_jc_scan_frame	*next;		/* next reference stack frame */
	_jc_scan_list	lists[64];	/* reference lists */
};

/*
 * Garbage collection state and statistics.
 */
struct _jc_trace_info {
	_jc_heap	*heap;
	jboolean	follow_soft;
	jboolean	wakeup_finalizer;
	_jc_word	mark_bits;
	_jc_word	gc_stack_visited;
	_jc_scan_frame	bottom_frame;
	int		num_finalizable;
	int		num_refs_cleared;
	int		num_recycled_pages;
	int		num_large_pages;
	int		num_small_pages;
	int		num_recycled_objects;
	int		num_fat_locks_recycled;
	int		num_large_objects;
	int		num_small_objects[0];
};

/************************************************************************
 *			Java virtual machine information		*
 ************************************************************************/

/* 
 * Structure used only during initialization.
 */
struct _jc_initialization {
	jboolean		may_execute;	/* ok to run java code */
	jboolean		create_class;	/* ok to create Class objects */
	_jc_ex_info		ex;		/* exception info */
	_jc_saved_frame		*frames;	/* exception stack frames */
	int			num_frames;
};

/*
 * One system property.
 */
struct _jc_property {
	const char	*name;		/* propery name */
	const char	*value;		/* propery value */
};

/*
 * System properties, stored as a sorted array.
 */
struct _jc_properties {
	int		allocated;	/* number of allocated properties */
	int		length;		/* actual number of properties */
	_jc_property	*elems;		/* properties */
};

/*
 * An expanded, or 'fat lock', object monitor.
 */
struct _jc_fat_lock {
	_jc_word			id;
	jint				recursion_count;
	jchar				notify_count;
	jbyte				notify_wakeup;
	pthread_mutex_t			mutex;
#ifndef NDEBUG
	_jc_env				*mutex_owner;
#endif
	pthread_cond_t			cond;
	pthread_cond_t			notify;
	union {
	    _jc_env			*owner;
	    SLIST_ENTRY(_jc_fat_lock)	link;
	}			u;
};

/*
 * Fat locks.
 */
struct _jc_fat_locks {
	_jc_fat_lock			**by_id;	/* indexed by id */
	jint				next_id;	/* next fat lock id */
	SLIST_HEAD(, _jc_fat_lock)	free_list;
};

/*
 * Information about threads associated with a VM instance.
 * All of this structure is protected by the VM mutex.
 *
 * Note: the free entries in the 'by_id' array form a linked list of
 * free thread IDs, starting with the slot indexed by 'free_id_list'.
 */
struct _jc_threads {
	_jc_env			**by_id;	/* threads indexed by id */
	jint			next_free_id;	/* free thread id list */
	LIST_HEAD(, _jc_env)	alive_list;	/* list of alive threads */
	LIST_HEAD(, _jc_env)	free_list;	/* free thread structures */
	int			num_free;	/* length of free list */

	/* Stack size parameters */
	size_t			stack_minimum;
	size_t			stack_maximum;
	size_t			stack_default;

	/* Priority parameters */
	int			prio_min;	/* min scheduling priority */
	int			prio_max;	/* max scheduling priority */
	jint			java_prio_min;	/* Thread.MIN_PRIORITY */
	jint			java_prio_max;	/* Thread.MAX_PRIORITY */
	jint			java_prio_norm;	/* Thread.NORM_PRIORITY */
};

/*
 * Virtual machine info.
 *
 * All information in this structure are protected by vm->mutex, with a
 * few exceptions. Access to fields which are read-only after VM creation,
 * or which are accessed with atomic operations, does not require the
 * mutex of course.
 */
struct _jc_jvm {
	_jc_fat_locks			fat_locks;
	LIST_ENTRY(_jc_jvm)		link;
	pthread_mutex_t			mutex;
#ifndef NDEBUG
	_jc_env				*mutex_owner;
#endif
	_jc_threads			threads;
	_jc_native_frame_list		native_globals;
	_jc_heap			heap;

	/*
	 * Properties and variables derived from them.
	 */
	_jc_properties			system_properties;
	jboolean			generation_enabled;
	jboolean			line_numbers;
	jboolean			resolve_native_directly;
	jboolean			without_classfiles;
	jboolean			loader_enabled;
	jboolean			ignore_resolution_failures;
	jboolean			compiler_disabled;
	char				**source_path;
	_jc_objpath_entry		*object_path;
	int				object_path_len;
	int				max_loader_pages;

	/*
	 * This is non-NULL during bootstrap only.
	 */
	_jc_initialization		*initialization;

	/*
	 * JNI invoke interface.
	 */
	const struct JNIInvokeInterface	*jni_interface;

	/*
	 * Function pointers received from JNI.
	 */
	int				(*vfprintf)(FILE *,
					    const char *, va_list);
	void				(*exit)(int)
					    __attribute__ ((noreturn));
	void				(*abort)(void)
					    __attribute__ ((noreturn));

	/*
	 * Class loading info.
	 */
	_jc_boot			boot;
	LIST_HEAD(, _jc_class_loader)	class_loaders;

	/*
	 * Tree of known class files and their hash values.
	 */
	_jc_splay_tree			classfiles;	/* _jc_cfile_node's */

	/*
	 * Tree mapping stack PC values to Java methods (or C functions).
	 */
	_jc_splay_tree			method_tree;	/* _jc_method's */
	_jc_method			invoke_method;	/* _jc_invoke_jcni_a()*/

	/*
	 * Used for 'stop the world' operations, suspend/resume, shutdown
	 */
	int				pending_halt_count;
	jboolean			world_stopped;
	jboolean			world_ending;
	pthread_cond_t			all_halted;
	pthread_cond_t			world_restarted;
	pthread_cond_t			vm_destruction;
	char				*check_address;

	/*
	 * VM internal threads
	 */
	jobject				debug_thread;
	jobject				finalizer_thread;

	/*
	 * Misc global parameters.
	 */
	jint				verbose_flags;
	jint				gc_cycles;
	volatile _jc_word		avail_loader_pages;
	_jc_word			gc_stack_visited;
	FILE				*object_list;
};

#endif	/* _STRUCTURES_H_ */
