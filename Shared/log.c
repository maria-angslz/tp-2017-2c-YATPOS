#include <commons/log.h>
#include <mstring.h>
#include <process.h>
#include <stdarg.h>
#include <stdlib.h>
#include <system.h>

#define LOG_ENABLED true
#define LOG_MAX 256

static t_log *silent = NULL;
static t_log *printer = NULL;

static void template(bool error, bool print, const char *format, va_list args);

// ========== Funciones públicas ==========

void log_init() {
	if(silent != NULL) return;
	char *pname = (char*) process_name(process_current());
	char *logfile = mstring_create("%s/logs/%s.log", system_userdir(), pname);
	pname = mstring_create("%s%s", pname, process_node());
	silent = log_create(logfile, pname, false, LOG_LEVEL_TRACE);
	printer = log_create(logfile, pname, true, LOG_LEVEL_TRACE);
	free(logfile);
	free(pname);
}

void log_inform(const char *format, ...) {
	va_list args;
	va_start(args, format);
	template(false, false, format, args);
	va_end(args);
}

void log_print(const char *format, ...) {
	va_list args;
	va_start(args, format);
	template(false, true, format, args);
	va_end(args);
}

void log_report(const char *format, ...) {
	va_list args;
	va_start(args, format);
	template(true, true, format, args);
	va_end(args);
}

void log_term() {
	log_destroy(silent);
	log_destroy(printer);
	silent = NULL;
	printer = NULL;
}

// ========== Funciones privadas ==========

static void template(bool error, bool print, const char *format, va_list args) {
	if(!LOG_ENABLED || process_current() == PROC_UNDEFINED) return;

	char log[LOG_MAX];
	vsnprintf(log, LOG_MAX, format, args);

	t_log *logger = print ? printer : silent;
	if(error) log_error(logger, log);
	else log_debug(logger, log);
}
