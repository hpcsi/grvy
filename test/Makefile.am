SUBDIRS           =
INCLUDES          = -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CFLAGS         = $(INCLUDES)
AM_CPPFLAGS       = $(INCLUDES) 
AM_F77FLAGS       = $(INCLUDES)
AM_FCFLAGS        = $(INCLUDES)
AM_LDFLAGS        = ../src/libgrvy.la
BUILT_SOURCES     = .license.stamp

EXTRA_DIST        = init.sh finalize.sh diff_dir.sh version hostinfo \
	            gadd.sh gadd_unknown_option.sh gadd_with_options.sh \
	            gdump.sh gdump_unknown_options.sh gdump_with_options.sh  \
		    C_timer_expanded_width.sh C_wrapper_log_printf.sh  C_wrapper_scratch_dir.sh \
                    C_timer_std_width.sh C_wrapper_log.sh C_timer_no_finalize.sh F_input_dump.sh \
		    F_get_command_arguments.sh  F_timer.sh F_wrapper_check_file_path.sh F_wrapper_scratch_dir.sh \
		    F_timer_expanded_width.sh  F_timer_std_width.sh F_wrapper_log.sh F_wrapper_unique_dir.sh \
		    CXX_timer_expanded_width.sh CXX_timer_std_width.sh C_ocore.sh F_ocore.sh \
		    input-example.txt ref_files .license.stamp

CLEANFILES        = .license.stamp *.gcno *.gcda .hostinfo
TESTS             = # Append below
TESTS_ENVIRONMENT = GRVY_INPUT_EXAMPLE_DIR=$(srcdir)
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

TESTS                                          += C_input_read_variables
check_PROGRAMS                                 += C_input_read_variables
C_input_read_variables_SOURCES                  = C_input.c C_examplefile.c examplefile.h

TESTS                                          += C_input_nofile_available
check_PROGRAMS                                 += C_input_nofile_available
C_input_nofile_available_SOURCES                = C_input-nofile.c

TESTS                                          += C_input_read_missing_variable
check_PROGRAMS                                 += C_input_read_missing_variable
C_input_read_missing_variable_SOURCES           = C_input-missing-var.c C_examplefile.c examplefile.h

TESTS                                          += C_input_register_variables
check_PROGRAMS                                 += C_input_register_variables
C_input_register_variables_SOURCES              = C_input-register.c C_examplefile.c examplefile.h

TESTS                                          += C_input_register_read_missing_variable
check_PROGRAMS                                 += C_input_register_read_missing_variable
C_input_register_read_missing_variable_SOURCES  = C_input-reg-missing-var.c

TESTS                                          += C_input_dos_newlines
check_PROGRAMS                                 += C_input_dos_newlines
C_input_dos_newlines_SOURCES                    = C_input-dos-newlines.c

TESTS                                          += C_timer_sum
check_PROGRAMS                                 += C_timer_sum
C_timer_sum_SOURCES                             = C_timer.c

TESTS                                          += C_timer_std_width.sh
check_PROGRAMS                                 += C_timer_std_width
C_timer_std_width_SOURCES                       = C_timer_std_width.c

TESTS                                          += C_timer_expanded_width.sh
check_PROGRAMS                                 += C_timer_expanded_width
C_timer_expanded_width_SOURCES                  = C_timer_expanded_width.c

TESTS                                          += C_timer_stats
check_PROGRAMS                                 += C_timer_stats
C_timer_stats_SOURCES                           = C_timer_stats.c

TESTS                                          += C_timer_baseline
check_PROGRAMS                                 += C_timer_baseline
C_timer_baseline_SOURCES                       = C_timer_baseline.c

TESTS                                          += C_timer_no_init
check_PROGRAMS                                 += C_timer_no_init
C_timer_no_init_SOURCES                         = C_timer_no_init.c

TESTS                                          += C_timer_with_reset
check_PROGRAMS                                 += C_timer_with_reset
C_timer_with_reset_SOURCES                      = C_timer_with_reset.c

TESTS                                          += C_timer_no_finalize.sh
check_PROGRAMS                                 += C_timer_no_finalize
C_timer_no_finalize_SOURCES                     = C_timer_no_finalize.c

TESTS                                          += C_unique_dir
check_PROGRAMS                                 += C_unique_dir
C_unique_dir_SOURCES                            = C_unique_dir.c

TESTS                                          += C_wrapper_scratch_dir.sh
check_PROGRAMS                                 += C_scratch_dir
C_scratch_dir_SOURCES                           = C_scratch_dir.c

TESTS                                          += C_math_isnan
check_PROGRAMS                                 += C_math_isnan
C_math_isnan_SOURCES                            = C_math-isnan.c

TESTS                                          += C_math_isinf
check_PROGRAMS                                 += C_math_isinf
C_math_isinf_SOURCES                            = C_math-isinf.c

TESTS                                          += C_check_file_path
check_PROGRAMS                                 += C_check_file_path
C_check_file_path_SOURCES                       = C_check_file_path.c



TESTS                                          += C_input_fdump
check_PROGRAMS                                 += C_input_fdump
C_input_fdump_SOURCES                           = C_input_dump.c C_examplefile.c examplefile.h

TESTS                                          += C_wrapper_log.sh
check_PROGRAMS                                 += C_log
C_log_SOURCES                                   = C_log.c

TESTS                                          += C_wrapper_log_printf.sh
check_PROGRAMS                                 += C_log_printf
C_log_printf_SOURCES                            = C_log_printf.c

TESTS                                          += C_version
check_PROGRAMS                                 += C_version
C_version_SOURCES                               = C_version.c

TESTS                                          += C_version_stdout
check_PROGRAMS                                 += C_version_stdout
C_version_stdout_SOURCES                        = C_version_stdout.c

# Following tests rely on various Boost library linkage

if LINK_BOOST_LIBS

  TESTS                                        += C_check_copy_dir
  check_PROGRAMS                               += C_check_copy_dir
  C_check_copy_dir_SOURCES                      = C_check_copy_dir.c

endif

if HDF5_ENABLED

  TESTS                                        += C_hdf_hist_timing
  check_PROGRAMS                               += C_hdf_hist_timing
  C_hdf_hist_timing_SOURCES                     = C_hdf_hist_timing.c

  TESTS                                        += F_hdf_hist_timing
  check_PROGRAMS                               += F_hdf_hist_timing
  F_hdf_hist_timing_SOURCES                     = F_hdf_hist_timing.F90
  F_hdf_hist_timing_LDADD                       = ../src/libgrvyf.la

endif

if MPI_ENABLED

  TESTS                                        += C_ocore.sh
  check_PROGRAMS                               += C_ocore
  C_ocore_SOURCES                               = C_ocore.c

endif

#-----------------
# C++ Tests
#-----------------

TESTS                                          += CXX_input
check_PROGRAMS                                 += CXX_input
CXX_input_SOURCES                               = C++_input.cpp C_examplefile.c examplefile.h

TESTS                                          += CXX_input_register_variables
check_PROGRAMS                                 += CXX_input_register_variables
CXX_input_register_variables_SOURCES            = C++_input-register.cpp C_examplefile.c examplefile.h

TESTS                                          += CXX_input_dump
check_PROGRAMS                                 += CXX_input_dump
CXX_input_dump_SOURCES                          = C++_input_dump.cpp C_examplefile.c examplefile.h

TESTS                                          += CXX_timer_std_width.sh
check_PROGRAMS                                 += CXX_timer_std_width
CXX_timer_std_width_SOURCES                     = C++_timer_std_width.cpp

TESTS                                          += CXX_timer_expanded_width.sh
check_PROGRAMS                                 += CXX_timer_expanded_width
CXX_timer_expanded_width_SOURCES                = C++_timer_expanded_width.cpp

TESTS                                          += CXX_timer_no_init
check_PROGRAMS                                 += CXX_timer_no_init
CXX_timer_no_init_SOURCES                       = C++_timer_no_init.cpp

if HDF5_ENABLED
TESTS                                          += CXX_hdf_hist_timing
check_PROGRAMS                                 += CXX_hdf_hist_timing
CXX_hdf_hist_timing_SOURCES                     = C++_hdf_hist_timing.cpp 
endif

if MPI_ENABLED
TESTS                                          +=  CXX_ocore.sh
check_PROGRAMS                                 +=  CXX_ocore
CXX_ocore_SOURCES                               =  C++_ocore.cpp
endif

#-----------------
# F90 Tests
#-----------------

TESTS                                           += F_wrapper_unique_dir.sh
check_PROGRAMS                                  += F_unique_dir
F_unique_dir_SOURCES                             = F_unique_dir.f90
F_unique_dir_LDADD                               = ../src/libgrvyf.la

TESTS                                            += F_wrapper_scratch_dir.sh
check_PROGRAMS                                   += F_scratch_dir
F_scratch_dir_SOURCES                             = F_scratch_dir.f90
F_scratch_dir_LDADD                               = ../src/libgrvyf.la

TESTS                                            += F_math
check_PROGRAMS                                   += F_math
F_math_SOURCES                                    = F_math.f90
F_math_LDADD                                      = ../src/libgrvyf.la

TESTS                                            += F_wrapper_check_file_path.sh
check_PROGRAMS                                   += F_check_file_path
F_check_file_path_SOURCES                         = F_check_file_path.f90
F_check_file_path_LDADD                           = ../src/libgrvyf.la

TESTS                                            += F_wrapper_log.sh
check_PROGRAMS                                   += F_log
F_log_SOURCES                                     = F_log.f90
F_log_LDADD                                       = ../src/libgrvyf.la

TESTS                                            += F_input_read_variables
check_PROGRAMS                                   += F_input_read_variables
F_input_read_variables_SOURCES                    = F_input_read_variables.f90
F_input_read_variables_LDADD                      = ../src/libgrvyf.la 

TESTS                                            += F_input_nofile_available
check_PROGRAMS                                   += F_input_nofile_available
F_input_nofile_available_SOURCES                  = F_input-nofile.f90
F_input_nofile_available_LDADD                    = ../src/libgrvyf.la

TESTS                                            += F_input_read_missing_variable
check_PROGRAMS                                   += F_input_read_missing_variable
F_input_read_missing_variable_SOURCES             = F_input-missing-var.f90
F_input_read_missing_variable_LDADD               = ../src/libgrvyf.la

TESTS                                            += F_input_register_variables
check_PROGRAMS                                   += F_input_register_variables
F_input_register_variables_SOURCES                = F_input-register.f90
F_input_register_variables_LDADD                  = ../src/libgrvyf.la

TESTS                                            += F_input_read_bigstring
check_PROGRAMS                                   += F_input_read_bigstring
F_input_read_bigstring_SOURCES                    = F_input_read_bigstring.f90
F_input_read_bigstring_LDADD                      = ../src/libgrvyf.la

TESTS                                            += F_input_dump.sh
check_PROGRAMS                                   += F_input_dump
F_input_dump_SOURCES                              = F_input_dump.F90
F_input_dump_LDADD                                = ../src/libgrvyf.la

TESTS                                            += F_timer.sh
check_PROGRAMS                                   += F_timer
F_timer_SOURCES                                   = F_timer.f90
F_timer_LDADD                                     = ../src/libgrvyf.la

TESTS                                            += F_timer_std_width.sh
check_PROGRAMS                                   += F_timer_std_width
F_timer_std_width_SOURCES                         = F_timer_std_width.f90
F_timer_std_width_LDADD                           = ../src/libgrvyf.la

TESTS                                            += F_timer_expanded_width.sh
check_PROGRAMS                                   += F_timer_expanded_width
F_timer_expanded_width_SOURCES                    = F_timer_expanded_width.f90
F_timer_expanded_width_LDADD                      = ../src/libgrvyf.la

TESTS                                            += F_version
check_PROGRAMS                                   += F_version
F_version_SOURCES                                 = F_version.F90
F_version_LDADD                                   = ../src/libgrvyf.la

TESTS                                            += F_get_command_arguments.sh
check_PROGRAMS                                   += F_get_command_arguments
F_get_command_arguments_SOURCES                   = F_get_command_arguments.f90
F_get_command_arguments_LDADD                     = ../src/libgrvyf.la

# Following tests rely on various Boost library linkage

if LINK_BOOST_LIBS

  TESTS                                          += F_check_copy_dir
  check_PROGRAMS                                 += F_check_copy_dir
  F_check_copy_dir_SOURCES                        = F_check_copy_dir.F90
  F_check_copy_dir_LDADD                          = ../src/libgrvyf.la

endif

if MPI_ENABLED

  TESTS                                          += F_ocore.sh
  check_PROGRAMS                                 += F_ocore
  F_ocore_SOURCES                                 = F_ocore.f90
  F_ocore_LDADD                                   = ../src/libgrvyf.la

endif

#--------------------
# Standalone Binaries
#--------------------

TESTS                                           += version hostinfo

if HDF5_ENABLED
TESTS                                           += gadd.sh
TESTS                                           += gadd_unknown_option.sh
TESTS                                           += gadd_with_options.sh
TESTS                                           += gdump.sh
TESTS                                           += gdump_unknown_options.sh
TESTS                                           += gdump_with_options.sh
endif

#------------------
# Test Final Banner
#------------------

TESTS += finalize.sh

#---------------------------------
# Embedded license header support
#---------------------------------

.license.stamp: $(top_srcdir)/LICENSE
	for file in $(srcdir)/*.h $(srcdir)/*.c ; do \
		$(top_srcdir)/src/common/lic_utils/update_license.pl \
			$(top_srcdir)/LICENSE $$file | tee -a $@ ;\
	done
	for file in $(srcdir)/*.f90 $(srcdir)/*.F90 ; do \
		$(top_srcdir)/src/common/lic_utils/update_license.pl \
			--c2f_comment $(top_srcdir)/LICENSE $$file | tee -a $@ ;\
	done

	for file in $(srcdir)/*.sh ; do \
		$(top_srcdir)/src/common/lic_utils/update_license.pl \
			--c2sh_comment $(top_srcdir)/LICENSE $$file | tee -a $@ ;\
	done
	touch .license.stamp
