cmake_minimum_required(VERSION 2.6.4)
project(CFS_GENERIC_REACTION_WHEEL C)

include(../../../ComponentSettings.cmake)

include_directories(public_inc)
include_directories(mission_inc)
include_directories(platform_inc)
include_directories(../shared)
include_directories(src)

aux_source_directory(src APP_SRC_FILES)
include_directories(${hwlib_MISSION_DIR}/fsw/public_inc)

# Create the app module
add_cfe_app(generic_rw ${APP_SRC_FILES}
            ../shared/generic_reaction_wheel_device.c)

# If UT is enabled, then add the tests from the subdirectory
if (ENABLE_UNIT_TESTS)
  add_subdirectory(unit-test)
endif (ENABLE_UNIT_TESTS)

