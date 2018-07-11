
/* Non-throwing internal API */

SEXP ps__exe(DWORD pid);
SEXP ps__name(DWORD pid);
SEXP ps__ppid(DWORD pid);
SEXP ps__proc_name(DWORD pid);
SEXP ps__get_cmdline(DWORD pid);
SEXP ps__get_cwd(DWORD pid);
SEXP ps__get_environ(DWORD pid);
SEXP ps__proc_num_threads(DWORD pid);
SEXP ps__proc_cpu_times(DWORD pid);
SEXP ps__proc_info(DWORD pid);
SEXP ps__proc_username(DWORD pid);
SEXP ps__proc_suspend(DWORD pid);
SEXP ps__proc_resume(DWORD pid);
SEXP ps__proc_kill(DWORD pid);
