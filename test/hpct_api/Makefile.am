check_PROGRAMS = # Append below
check_PROGRAMS += C_input_read_variables C_input_nofile_available \
		  C_input_read_missing_variable C_input_register_variables \
		  C_input_register_read_missing_variable C_timer_sum \
		  C_math_isnan C_math_isinf C_unique_dir C_scratch_dir \
		  C_check_file_path
check_PROGRAMS += F_unique_dir F_scratch_dir F_check_file_path F_math

INCLUDES      = -I$(top_srcdir)/src -I$(top_builddir)/src
LIBS          = -L$(top_builddir)/src -lgrvy
AM_CFLAGS     = $(INCLUDES)
AM_CPPFLAGS   = $(INCLUDES)
AM_F77FLAGS   = $(INCLUDES)
AM_FCFLAGS    = $(INCLUDES)
AM_LDFLAGS    = $(LIBS)

# C Tests

C_input_read_variables_SOURCES                 = C_input.c C_examplefile.c examplefile.h
C_input_nofile_available_SOURCES               = C_input-nofile.c
C_input_read_missing_variable_SOURCES          = C_input-missing-var.c C_examplefile.c examplefile.h
C_input_register_variables_SOURCES             = C_input-register.c C_examplefile.c examplefile.h
C_input_register_read_missing_variable_SOURCES = C_input-reg-missing-var.c
C_timer_sum_SOURCES                            = C_timer.c
C_unique_dir_SOURCES                           = C_unique_dir.c
C_scratch_dir_SOURCES                          = C_scratch_dir.c
C_math_isnan_SOURCES                           = C_math-isnan.c
C_math_isinf_SOURCES                           = C_math-isinf.c
C_check_file_path_SOURCES                      = C_check_file_path.c

# Fortran Tests

F_unique_dir_SOURCES      = F_unique_dir.f90
F_scratch_dir_SOURCES     = F_scratch_dir.f90
F_math_SOURCES            = F_math.f90
F_check_file_path_SOURCES = F_check_file_path.f90

TESTS_ENVIRONMENT = HPCT_INPUT_EXAMPLE_DIR=$(srcdir)
TESTS             = init.sh \
		    C_input_read_variables \
		    C_input_nofile_available \
		    C_input_read_missing_variable \
		    C_input_register_variables \
		    C_input_register_read_missing_variable \
		    C_math_isnan \
		    C_math_isinf \
		    C_check_file_path \
		    C_unique_dir \
		    wrapper_C_scratch_dir.sh \
		    wrapper_F_check_file_path.sh \
		    wrapper_F_unique_dir.sh \
		    wrapper_F_scratch_dir.sh \
		    F_math \
		    finalize.sh

#		    C_timer_sum 

EXTRA_DIST = init.sh finalize.sh input-example.txt \
	     wrapper_C_scratch_dir.sh \
	     wrapper_F_unique_dir.sh \
	     wrapper_F_scratch_dir.sh \
	     wrapper_F_check_file_path.sh

