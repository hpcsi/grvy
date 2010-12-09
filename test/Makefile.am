SUBDIRS           =
INCLUDES          = -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CFLAGS         = $(INCLUDES)
AM_CPPFLAGS       = $(INCLUDES) #$(BOOST_CPPFLAGS)
AM_F77FLAGS       = $(INCLUDES)
AM_FCFLAGS        = $(INCLUDES)
AM_LDFLAGS        =
BUILT_SOURCES     = .license.stamp
EXTRA_DIST        = init.sh finalize.sh C_*.sh F_*.sh
EXTRA_DIST       += input-example.txt ref_files
CLEANFILES        = .license.stamp *.gcno *.gcda
TESTS             = # Append below
TESTS_ENVIRONMENT = HPCT_INPUT_EXAMPLE_DIR=$(srcdir)
check_PROGRAMS    = # Append below

# C and Fortran require libgrvy.la and libgrvyf.la, respectively
# A single LIBS variable will not handle this, so LDADD ad nauseum.

#-----------------
# Test Init Banner
#-----------------

TESTS += init.sh

#---------------
# C Tests
#---------------

TESTS                          += C_input_read_variables
check_PROGRAMS                 += C_input_read_variables
C_input_read_variables_SOURCES  = C_input.c C_examplefile.c examplefile.h
C_input_read_variables_LDADD    = ../src/libgrvy.la

TESTS                            += C_input_nofile_available
check_PROGRAMS                   += C_input_nofile_available
C_input_nofile_available_SOURCES  = C_input-nofile.c
C_input_nofile_available_LDADD    = ../src/libgrvy.la

TESTS                                 += C_input_read_missing_variable
check_PROGRAMS                        += C_input_read_missing_variable
C_input_read_missing_variable_SOURCES  = C_input-missing-var.c C_examplefile.c examplefile.h
C_input_read_missing_variable_LDADD    = ../src/libgrvy.la

TESTS                              += C_input_register_variables
check_PROGRAMS                     += C_input_register_variables
C_input_register_variables_SOURCES  = C_input-register.c C_examplefile.c examplefile.h
C_input_register_variables_LDADD    = ../src/libgrvy.la

TESTS                                          += C_input_register_read_missing_variable
check_PROGRAMS                                 += C_input_register_read_missing_variable
C_input_register_read_missing_variable_SOURCES  = C_input-reg-missing-var.c
C_input_register_read_missing_variable_LDADD    = ../src/libgrvy.la

TESTS               += C_timer_sum
check_PROGRAMS      += C_timer_sum
C_timer_sum_SOURCES  = C_timer.c
C_timer_sum_LDADD    = ../src/libgrvy.la

TESTS                 += C_timer_stats
check_PROGRAMS        += C_timer_stats
C_timer_stats_SOURCES  = C_timer_stats.c
C_timer_stats_LDADD    = ../src/libgrvy.la

TESTS                 += C_unique_dir
check_PROGRAMS        += C_unique_dir
C_unique_dir_SOURCES   = C_unique_dir.c
C_unique_dir_LDADD     = ../src/libgrvy.la

TESTS                 += C_wrapper_scratch_dir.sh
check_PROGRAMS        += C_scratch_dir
C_scratch_dir_SOURCES  = C_scratch_dir.c
C_scratch_dir_LDADD    = ../src/libgrvy.la

TESTS                += C_math_isnan
check_PROGRAMS       += C_math_isnan
C_math_isnan_SOURCES  = C_math-isnan.c
C_math_isnan_LDADD    = ../src/libgrvy.la

TESTS                += C_math_isinf
check_PROGRAMS       += C_math_isinf
C_math_isinf_SOURCES  = C_math-isinf.c
C_math_isinf_LDADD    = ../src/libgrvy.la

TESTS                     += C_check_file_path
check_PROGRAMS            += C_check_file_path
C_check_file_path_SOURCES  = C_check_file_path.c
C_check_file_path_LDADD    = ../src/libgrvy.la

TESTS                 += C_input_fdump
check_PROGRAMS        += C_input_fdump
C_input_fdump_SOURCES  = C_input_dump.c C_examplefile.c examplefile.h
C_input_fdump_LDADD    = ../src/libgrvy.la

TESTS          += C_wrapper_log.sh
check_PROGRAMS += C_log
C_log_SOURCES   = C_log.c
C_log_LDADD     = ../src/libgrvy.la

TESTS                += C_wrapper_log_printf.sh
check_PROGRAMS       += C_log_printf
C_log_printf_SOURCES  = C_log_printf.c
C_log_printf_LDADD    = ../src/libgrvy.la

TESTS             += C_version
check_PROGRAMS    += C_version
C_version_SOURCES  = C_version.c
C_version_LDADD    = ../src/libgrvy.la

TESTS                    += C_version_stdout
check_PROGRAMS           += C_version_stdout
C_version_stdout_SOURCES  = C_version_stdout.c
C_version_stdout_LDADD    = ../src/libgrvy.la

#-----------------
# C++ Tests
#-----------------

TESTS              += CXX_input
check_PROGRAMS     += CXX_input
CXX_input_SOURCES   = C++_input.cpp C_examplefile.c examplefile.h
CXX_input_LDADD     = ../src/libgrvy.la

TESTS                                += CXX_input_register_variables
check_PROGRAMS                       += CXX_input_register_variables
CXX_input_register_variables_SOURCES  = C++_input-register.cpp C_examplefile.c examplefile.h
CXX_input_register_variables_LDADD    = ../src/libgrvy.la

TESTS                  += CXX_input_dump
check_PROGRAMS         += CXX_input_dump
CXX_input_dump_SOURCES  = C++_input_dump.cpp C_examplefile.c examplefile.h
CXX_input_dump_LDADD    = ../src/libgrvy.la

#-----------------
# F90 Tests
#-----------------

TESTS                += F_wrapper_unique_dir.sh
check_PROGRAMS       += F_unique_dir
F_unique_dir_SOURCES  = F_unique_dir.f90
F_unique_dir_LDADD    = ../src/libgrvyf.la

TESTS                 += F_wrapper_scratch_dir.sh
check_PROGRAMS        += F_scratch_dir
F_scratch_dir_SOURCES  = F_scratch_dir.f90
F_scratch_dir_LDADD    = ../src/libgrvyf.la

TESTS          += F_math
check_PROGRAMS += F_math
F_math_SOURCES  = F_math.f90
F_math_LDADD    = ../src/libgrvyf.la

TESTS                     += F_wrapper_check_file_path.sh
check_PROGRAMS            += F_check_file_path
F_check_file_path_SOURCES  = F_check_file_path.f90
F_check_file_path_LDADD    = ../src/libgrvyf.la

TESTS          += F_wrapper_log.sh
check_PROGRAMS += F_log
F_log_SOURCES   = F_log.f90
F_log_LDADD     = ../src/libgrvyf.la

TESTS                          += F_input_read_variables
check_PROGRAMS                 += F_input_read_variables
F_input_read_variables_SOURCES  = F_input_read_variables.f90
F_input_read_variables_LDADD    =  ../src/libgrvy.la ../src/libgrvyf.la 

TESTS                            += F_input_nofile_available
check_PROGRAMS                   += F_input_nofile_available
F_input_nofile_available_SOURCES  = F_input-nofile.f90
F_input_nofile_available_LDADD    = ../src/libgrvyf.la

TESTS                                 += F_input_read_missing_variable
check_PROGRAMS                        += F_input_read_missing_variable
F_input_read_missing_variable_SOURCES  = F_input-missing-var.f90
F_input_read_missing_variable_LDADD    = ../src/libgrvyf.la

TESTS                              += F_input_register_variables
check_PROGRAMS                     += F_input_register_variables
F_input_register_variables_SOURCES  = F_input-register.f90
F_input_register_variables_LDADD    = ../src/libgrvyf.la

TESTS                          += F_input_read_bigstring
check_PROGRAMS                 += F_input_read_bigstring
F_input_read_bigstring_SOURCES  = F_input_read_bigstring.f90
F_input_read_bigstring_LDADD    = ../src/libgrvyf.la

TESTS                += F_input_dump
check_PROGRAMS       += F_input_dump
F_input_dump_SOURCES  = F_input_dump.F90
F_input_dump_LDADD    = ../src/libgrvyf.la

TESTS           += F_timer.sh
check_PROGRAMS  += F_timer
F_timer_SOURCES  = F_timer.f90
F_timer_LDADD    = ../src/libgrvyf.la

TESTS             += F_version
check_PROGRAMS    += F_version
F_version_SOURCES  = F_version.F90
F_version_LDADD    = ../src/libgrvyf.la

TESTS                           += F_get_command_arguments.sh
check_PROGRAMS                  += F_get_command_arguments
F_get_command_arguments_SOURCES  = F_get_command_arguments.f90
F_get_command_arguments_LDADD    = ../src/libgrvyf.la

#------------------
# Test Final Banner
#------------------

TESTS += finalize.sh

#---------------------------------
# Embedded license header support
#---------------------------------

.license.stamp: $(top_builddir)/LICENSE
	for file in $(srcdir)/*.h $(srcdir)/*.c ; do \
		$(top_srcdir)/src/common/lic_utils/update_license.pl $(top_builddir)/LICENSE $$file | tee -a $@ ;\
	done
	for file in $(srcdir)/*.f90 $(srcdir)/*.F90 ; do \
		$(top_srcdir)/src/common/lic_utils/update_license.pl --c2f_comment $(top_builddir)/LICENSE $$file | tee -a $@ ;\
	done
	touch .license.stamp
