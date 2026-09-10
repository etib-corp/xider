# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

if(EXISTS "/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitclone-lastrun.txt" AND EXISTS "/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitinfo.txt" AND
  "/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitinfo.txt")
  message(VERBOSE
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

# Even at VERBOSE level, we don't want to see the commands executed, but
# enabling them to be shown for DEBUG may be useful to help diagnose problems.
cmake_language(GET_MESSAGE_LOG_LEVEL active_log_level)
if(active_log_level MATCHES "DEBUG|TRACE")
  set(maybe_show_command COMMAND_ECHO STDOUT)
else()
  set(maybe_show_command "")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/julithein/Projects/xider/xider/build/_deps/openal-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/julithein/Projects/xider/xider/build/_deps/openal-src'")
endif()

# try the clone 1 + N times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
math(EXPR max_tries "1 + 2")
while(error_code AND number_of_tries LESS ${max_tries})
  if(number_of_tries GREATER 0 AND 0 GREATER 0)
    message(STATUS "Retry #${number_of_tries}, waiting 0 seconds before next attempt...")
    execute_process(COMMAND ${CMAKE_COMMAND} -E sleep 0)
  endif()
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --progress --config "advice.detachedHead=false" "https://github.com/kcat/openal-soft.git" "openal-src"
    WORKING_DIRECTORY "/Users/julithein/Projects/xider/xider/build/_deps"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(NOTICE "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository:\n  'https://github.com/kcat/openal-soft.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "3f94a50884e2ae4963092fead7d299127e97e5d5" --
  WORKING_DIRECTORY "/Users/julithein/Projects/xider/xider/build/_deps/openal-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '3f94a50884e2ae4963092fead7d299127e97e5d5'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/julithein/Projects/xider/xider/build/_deps/openal-src"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/julithein/Projects/xider/xider/build/_deps/openal-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitinfo.txt" "/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/julithein/Documents/Preskater/Projects.nosync/xider/xider/build/_deps/openal-subbuild/openal-populate-prefix/src/openal-populate-stamp/openal-populate-gitclone-lastrun.txt'")
endif()
