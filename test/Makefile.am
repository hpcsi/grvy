SUBDIRS         = 

BUILT_SOURCES   = .license.stamp

check_PROGRAMS  = # Append below
check_PROGRAMS += C_input_read_variables C_input_nofile_available \
		  C_input_read_missing_variable C_input_register_variables \
		  C_input_register_read_missing_variable C_timer_sum C_timer_stats \
		  C_math_isnan C_math_isinf C_unique_dir C_scratch_dir \
		  C_check_file_path C_input_fdump C_log C_log_printf \
		  C_version C_version_stdout 

check_PROGRAMS += CXX_input CXX_input_dump CXX_input_register_variables

check_PROGRAMS += F_unique_dir F_scratch_dir F_check_file_path F_math F_log \
		  F_input_read_variables F_input_nofile_available F_input_read_missing_variable \
		  F_input_register_variables F_input_read_bigstring F_timer F_input_dump F_version

INCLUDES        = -I$(top_srcdir)/src -I$(top_builddir)/src
LIBS            = -L$(top_builddir)/src -lgrvy
AM_CFLAGS       = $(INCLUDES)
AM_CPPFLAGS     = $(INCLUDES) #$(BOOST_CPPFLAGS)
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
C_log_SOURCES				       = C_log.c
C_log_printf_SOURCES			       = C_log_printf.c
C_version_SOURCES			       = C_version.c
C_version_stdout_SOURCES		       = C_version_stdout.c

#-----------------
# C++ Test Sources
#-----------------

CXX_input_SOURCES                              = C++_input.cpp C_examplefile.c examplefile.h
CXX_input_register_variables_SOURCES           = C++_input-register.cpp C_examplefile.c examplefile.h
CXX_input_dump_SOURCES                         = C++_input_dump.cpp C_examplefile.c examplefile.h

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
F_input_dump_SOURCES                           = F_input_dump.F90
F_timer_SOURCES                                = F_timer.f90
F_version_SOURCES                              = F_version.F90

#---------------------------------
# Embedded license header support
#---------------------------------

test_csources =  \
     $(C_input_read_variables_SOURCES) \
     $(C_input_nofile_available_SOURCES) \
     $(C_input_read_missing_variable_SOURCES) \
     $(C_input_register_variables_SOURCES) \
     $(C_input_register_read_missing_variable_SOURCES) \
     $(C_timer_sum_SOURCES) \
     $(C_unique_dir_SOURCES) \
     $(C_scratch_dir_SOURCES) \
     $(C_math_isnan_SOURCES) \
     $(C_math_isinf_SOURCES) \
     $(C_check_file_path_SOURCES) \
     $(C_input_fdump_SOURCES) \
     $(C_log_SOURCES) \
     $(C_log_printf_SOURCES) \
     $(C_version_SOURCES) \
     $(C_version_stdout_SOURCES) \
     $(CXX_input_SOURCES) \
     $(CXX_input_register_variables_SOURCES) \
     $(CXX_input_dump_SOURCES)

test_fsources =  \
     $(F_unique_dir_SOURCES) \
     $(F_scratch_dir_SOURCES) \
     $(F_math_SOURCES) \
     $(F_check_file_path_SOURCES) \
     $(F_log_SOURCES) \
     $(F_input_read_variables_SOURCES) \
     $(F_input_nofile_available_SOURCES) \
     $(F_input_read_missing_variable_SOURCES) \
     $(F_input_register_variables_SOURCES) \
     $(F_input_read_bigstring_SOURCES) \
     $(F_input_dump_SOURCES) \
     $(F_timer_SOURCES)


.license.stamp: $(top_srcdir)/LICENSE
	$(top_srcdir)/src/common/lic_utils/update_license.pl $(top_srcdir)/LICENSE $(test_csources)
	$(top_srcdir)/src/common/lic_utils/update_license.pl --c2f_comment $(top_srcdir)/LICENSE $(test_fsources)
	echo 'updated source license headers' >$@


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
		    CXX_input \
		    CXX_input_register_variables \
		    CXX_input_dump \
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
		    F_input_dump \
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

CLEANFILES        = .license.stamp *.gcno *.gcda
