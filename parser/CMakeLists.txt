# Copyright(c) 2023 Intel Corporation

# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files(the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and / or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

cmake_minimum_required(VERSION 3.5)
project(IntelOCLEtw)
set(DLL_NAME IntelOCLEtw)


set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(VulkanETW_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# D3DETWsetLinkerFlags: apply linker flags for given configuration
# linkerFlags: linker specific options
# linkerTarget: optional parameter - apply linker flags for specfied target
macro (VulkanEtwSetLinkerFlags linkerFlags linkerTarget)
    foreach (opt ${linkerFlags})
        if ("${linkerTarget}" STREQUAL "")
            set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${opt}")
            set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${opt}")
        else()
            set (CMAKE_SHARED_LINKER_FLAGS_${linkerTarget} "${CMAKE_SHARED_LINKER_FLAGS_${linkerTarget}} ${opt}")
            set (CMAKE_EXE_LINKER_FLAGS_${linkerTarget} "${CMAKE_SHARED_LINKER_FLAGS_${linkerTarget}} ${opt}")
        endif()
    endforeach()
endmacro()

# common defines
macro (VulkanEtwAddCommonTargetDefines target)
    if (TARGET ${target})
        set_property(TARGET ${target} APPEND PROPERTY COMPILE_DEFINITIONS
            $<$<CONFIG:Release>: _RELEASE NDEBUG>
            $<$<CONFIG:ReleaseInternal>: _RELEASE_INTERNAL NDEBUG>
            $<$<CONFIG:Debug>: _DEBUG>
            D3DETW_EXPORTS
        )
    endif()
endmacro()

#Todo: integrate with .h
# macro (GenerateMediaEtwManifest target)
#     if(TARGET ${target})
#         add_custom_command(
#             TARGET ${target} PRE_BUILD
#             COMMAND ${CMAKE_COMMAND} -E copy ${MEDIA_DRIVER_PROPRIETARY}/windows/common/IntelMediaEtw.man ${PROJECT_BINARY_DIR}/IntelMediaEtw.man
#             COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/patchManifest.py ${PROJECT_BINARY_DIR}/IntelMediaEtw.man ${MEDIA_DRIVER_AGNOSTIC}/common/os/mos_os_trace_event.h
#             )
#     endif()
# endmacro()

if (MSVC)
    set( CMAKE_CONFIGURATION_TYPES
        "Debug"
        "Release"
        "ReleaseInternal")

    set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we need"
        FORCE)
endif()

MESSAGE("platform: ${CMAKE_HOST_SYSTEM_NAME}")
MESSAGE("source_dir: ${VulkanETW_SOURCE_DIR}")
MESSAGE("arch: ${ARCH}")
MESSAGE("build type: ${CMAKE_BUILD_TYPE}")


set(SOURCES_
    ${VulkanETW_SOURCE_DIR}/VkEtw.man
    )

source_group("Resource Files" FILES
    ${VulkanETW_SOURCE_DIR}/VkEtw.man
    )


#clear all default-cmake settings
SET (CMAKE_SHARED_LINKER_FLAGS_RELEASEINTERNAL "")
SET (CMAKE_SHARED_LINKER_FLAGS_RELEASE "")
SET (CMAKE_SHARED_LINKER_FLAGS_DEBUG "")
SET (CMAKE_SHARED_LINKER_FLAGS "")

SET (CMAKE_EXE_LINKER_FLAGS "")
SET (CMAKE_EXE_LINKER_FLAGS_DEBUG "")
SET (CMAKE_EXE_LINKER_FLAGS_RELEASEINTERNAL "")
SET (CMAKE_EXE_LINKER_FLAGS_RELEASE "")

SET (CMAKE_STATIC_LINKER_FLAGS "")
SET (CMAKE_LOCAL_LINKER_FLAGS "")

SET (CMAKE_CXX_STANDARD_LIBRARIES "")
SET (CMAKE_C_STANDARD_LIBRARIES "")

SET (CMAKE_CXX_FLAGS_RELEASEINTERNAL "")
SET (CMAKE_CXX_FLAGS_DEBUG  "")
SET (CMAKE_CXX_FLAGS_RELEASE "")

# set compiler options
include(Windows.cmake)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_UNICODE /DUNICODE")

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_library( ${DLL_NAME} SHARED ${SOURCES_} ${PROJECT_BINARY_DIR}/VkEtw.rc )

set_target_properties(${DLL_NAME} PROPERTIES OUTPUT_NAME IntelOCLEtw)

VulkanETWAddCommonTargetDefines(${DLL_NAME})

add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/VkEtw.rc
                   COMMENT "Running mc.exe to compile Vulkan ETW manifest"
                   COMMAND mc -h ${PROJECT_BINARY_DIR} -r ${PROJECT_BINARY_DIR} -z VkEtw -um ${VulkanETW_SOURCE_DIR}/VkEtw.man
                  )
