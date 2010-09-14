SUBDIRS         = hpct_api

check_PROGRAMS  = # Append below
check_PROGRAMS += C_input_read_variables C_input_nofile_available \
		  C_input_read_missing_variable C_input_register_variables \
		  C_input_register_read_missing_variable C_timer_sum C_timer_stats \
		  C_math_isnan C_math_isinf C_unique_dir C_scratch_dir \
		  C_check_file_path C_input_fdump C_log C_log_printf \
		  C_version C_version_stdout

check_PROGRAMS += F_unique_dir F_scratch_dir F_check_file_path F_math F_log \
		  F_input_read_variables F_input_nofile_available F_input_read_missing_variable \
		  F_input_register_variables F_input_read_bigstring F_timer

INCLUDES        = -I$(top_srcdir)/src -I$(top_builddir)/src
LIBS            = -L$(top_builddir)/src -lgrvy
AM_CFLAGS       = $(INCLUDES)
AM_CPPFLAGS     = $(INCLUDES)
AM_F77FLAGS     = $(INCLUDES)
AM_FCFLAGS      = $(INCLUDES)
AM_LDFLAGS      = $(LIBS)

#---------------
# C Test Sources
#---------------

C_input_read_variables_SOURCES                 = C_input.c C_examplefile.c examplefile.h
C_input_nofile_available_SOURCES               = C_input-nofile.c
C_input_read_missing_variable_SOURCES          = C_input-missing-var.c C_examplefile.c examplefile.h
C_input_register_variables_SOURCES             = C_input-register.c C_examplefile.c examplefile.h
C_input_register_read_missing_variable_SOURCES = C_input-reg-missing-var.c
C_timer_sum_SOURCES                            = C_timer.c
C_timer_stats_SOURCE                           = C_timer_stats.c
C_unique_dir_SOURCES                           = C_unique_dir.c
C_scratch_dir_SOURCES                          = C_scratch_dir.c
C_math_isnan_SOURCES                           = C_math-isnan.c
C_math_isinf_SOURCES                           = C_math-isinf.c
C_check_file_path_SOURCES                      = C_check_file_path.c
C_input_fdump_SOURCES                          = C_input_dump.c C_examplefile.c examplefile.h
C_log					       = C_log.c
C_log_printf				       = C_log_printf.c
C_version				       = C_version.c
C_version_stdout			       = C_version_stdout.c

#-----------------
# F90 Test Sources
#-----------------

F_unique_dir_SOURCES                           = F_unique_dir.f90
F_scratch_dir_SOURCES                          = F_scratch_dir.f90
F_math_SOURCES                                 = F_math.f90
F_check_file_path_SOURCES                      = F_check_file_path.f90
F_log_SOURCES                                  = F_log.f90
F_input_read_variables_SOURCES                 = F_input_read_variables.f90
F_input_nofile_available_SOURCES               = F_input-nofile.f90
F_input_read_missing_variable_SOURCES          = F_input-missing-var.f90
F_input_register_variables_SOURCES             = F_input-register.f90
F_input_read_bigstring_SOURCES                 = F_input_read_bigstring.f90
F_timer_SOURCES                                = F_timer.f90

##F_input_register_read_missing_variables_SOURCES = F_input-reg-missing-var.f90

TESTS_ENVIRONMENT = HPCT_INPUT_EXAMPLE_DIR=$(srcdir)
TESTS             = init.sh \
		    C_input_read_variables \
		    C_input_nofile_available \
		    C_input_read_missing_variable \
		    C_input_register_variables \
		    C_input_register_read_missing_variable \
		    C_timer_sum \
		    C_timer_stats \
		    C_math_isnan \
		    C_math_isinf \
		    C_check_file_path \
		    C_unique_dir \
		    C_wrapper_scratch_dir.sh \
		    C_input_fdump \
		    C_wrapper_log.sh \
		    C_wrapper_log_printf.sh \
		    C_version \
		    C_version_stdout \
		    F_wrapper_check_file_path.sh \
		    F_wrapper_unique_dir.sh \
		    F_wrapper_scratch_dir.sh \
		    F_wrapper_log.sh \
		    F_timer.sh \
		    F_math \
		    F_input_read_variables \
		    F_input_nofile_available \
		    F_input_read_missing_variable \
                    F_input_register_variables \
		    F_input_read_bigstring \
		    finalize.sh

EXTRA_DIST        = init.sh finalize.sh input-example.txt \
	            C_wrapper_scratch_dir.sh \
	            F_wrapper_unique_dir.sh \
	            F_wrapper_scratch_dir.sh \
	            F_wrapper_check_file_path.sh \
                    C_wrapper_log.sh \
		    C_wrapper_log_printf.sh \
                    F_wrapper_log.sh \
                    F_timer.sh \
                    ref_files
