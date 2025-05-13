##################################################################
#
# Coverage Unit Test build recipe
#
# This CMake file contains the recipe for building the generic_reaction_wheel unit tests.
# It is invoked from the parent directory when unit tests are enabled.
#
##################################################################

#
#
# NOTE on the subdirectory structures here:
#
# - "inc" provides local header files shared between the coveragetest,
#    wrappers, and overrides source code units
# - "coveragetest" contains source code for the actual unit test cases
#    The primary objective is to get line/path coverage on the FSW 
#    code units.
#

add_cfe_coverage_stubs(generic_rw-internal stubs/generic_reaction_wheel_device_stubs.c stubs/libuart_stubs.c) 
target_link_libraries(coverage-generic_rw-internal-stubs ut_core_api_stubs ut_assert)
target_include_directories(coverage-generic_rw-internal-stubs PUBLIC $<TARGET_PROPERTY:generic_rw,INCLUDE_DIRECTORIES>)

 
# Use the UT assert public API, and allow direct
# inclusion of source files that are normally private
include_directories(${PROJECT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${hwlib_MISSION_DIR}/fsw/public_inc)
include_directories(${PROJECT_SOURCE_DIR}/../../../../fsw/apps/hwlib/fsw/public_inc)
include_directories(${PROJECT_SOURCE_DIR}/../../../../fsw/osal/ut_assert/inc)



# Add a coverage test executable called "generic_rw-ALL" that 
# covers all of the functions in generic_reaction_wheel_app.  
#
# Also note in a more complex app/lib the coverage test can also
# be broken down into smaller units (in which case one should use
# a unique suffix other than "ALL" for each unit).  For example,
# OSAL implements a separate coverage test per source unit.
add_cfe_coverage_test(generic_rw ALL 
    "coveragetest/coveragetest_generic_reaction_wheel_app.c"
    "../src/generic_reaction_wheel_app.c"
)

# Check if the device test file exists
add_cfe_coverage_test(generic_rw DEVICE 
    "coveragetest/coveragetest_generic_reaction_wheel_device.c"
    "../../shared/generic_reaction_wheel_device.c"
    "stubs/libuart_stubs.c"
)

add_cfe_coverage_dependency(generic_rw ALL generic_rw-internal)


# The generic_reaction_wheel uses library functions provided by generic_reaction_wheel_lib so must be linked
# with the generic_reaction_wheel_lib stub library (this is mainly just an example of how this 
# can be done).
#add_cfe_coverage_dependency(generic_rw ALL generic_reaction_wheel_lib)

