#include <config.h>
#include <log.h>
#include <mstring.h>
#include <path.h>
#include <process.h>
#include <system.h>
#include <thread.h>
#include <stdlib.h>

static t_process current = PROC_UNDEFINED;
static char *node = NULL;

static char *names[] = {"(Undefined)", "YAMA", "FileSystem", "Master", "Worker", "DataNode"};

// ========== Funciones públicas ==========

void process_init() {
	putenv("LC_ALL=C");
	current = process_type(path_name(system_proc()));
	node = (char*) process_node("");
	system_init();
	log_init();
	config_init();
	thread_init();
}

t_process process_current() {
	return current;
}

const char *process_name(t_process process) {
	return names[process];
}

t_process process_type(const char *name) {
	int l = sizeof(names) / sizeof(*names);
	for(int i = 0; i < l; i++) {
		if(mstring_equal(name, names[i])) return i;
	}
	return PROC_UNDEFINED;
}

const char *_process_node(const char *name) {
	if(name != NULL && node == NULL) {
		node = mstring_duplicate(name);
	}
	return node;
}

void process_term() {
	thread_term();
	log_term();
	config_term();
	free(node);
}
