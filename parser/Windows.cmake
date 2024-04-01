# INTEL CONFIDENTIAL
# Copyright 2023 Intel Corporation All Rights Reserved.
#
# The source code contained or described herein and all documents related
# to the source code ("Material") are owned by Intel Corporation or its
# suppliers or licensors. Title to the Material remains with Intel Corporation
# or its suppliers and licensors. The Material contains trade secrets and
# proprietary and confidential information of Intel or its suppliers and
# licensors. The Material is protected by worldwide copyright and trade secret
# laws and treaty provisions. No part of the Material may be used, copied,
# reproduced, modified, published, uploaded, posted, transmitted, distributed,
# or disclosed in any way without Intel's prior express written permission.

# No license under any patent, copyright, trade secret or other intellectual
# property right is granted to or conferred upon you by disclosure or delivery
# of the Materials, either expressly, by implication, inducement, estoppel or
# otherwise. Any license under such intellectual property rights must be express
# and approved by Intel in writing.

#this file should contain only compiler and linker flags


SET (VULKANETW_COMPILER_FLAGS_COMMON
    "/MP"                           #MultiProcessorCompilation true
    "/W3"                           #WarningLevel Level3
    "/WX"                           #TreatWarningAsError true
    "/Gz"
    "/Zi"                           #DebugInformationFormat ProgramDatabase
    "/EHsc"                         #ExceptionHandling Sync
    "/GS"                           #BufferSecurityCheck
)

#Release
SET( VULKANETW_COMPILER_FLAGS_RELEASE
      "/Gy"
      "/Ox"  # Full Optimization
      "/Ob2" # Inline Function Expansion: Any Suitable
      "/MD"  # Runtime Library: Multi-threaded
)

#Release Internal
SET( VULKANETW_COMPILER_FLAGS_RELEASEINTERNAL  ${MEDIAETW_COMPILER_FLAGS_RELEASE})

#Debug
SET( VULKANETW_COMPILER_FLAGS_DEBUG
      "/RTC1"
      "/Od"  #OptimizationDisabled
      "/MDd" #RtMultiThreadedStaticDebug
)

SET (VULKANETW_LINKER_FLAGS_COMMON
    "/DYNAMICBASE"
    "/SUBSYSTEM:CONSOLE"
    "/NOENTRY"
)

SET (VULKANETW_LINKER_FLAGS_RELEASE
    "/OPT:ICF"        #EnableCOMDATFolding
    "/OPT:REF"        #OptimizeReferences
    "/DEBUG"           #GenerateDebugInfo
    "/INCREMENTAL:NO"
    "/MANIFESTUAC:NO"
)

SET( VULKANETW_LINKER_FLAGS_RELEASEINTERNAL  ${MEDIAETW_LINKER_FLAGS_RELEASE})

SET (VULKANETW_LINKER_FLAGS_DEBUG
    "/INCREMENTAL"
    "/DEBUG" #GenerateDebugInfo
)
    
#set predefined compiler flags set
add_compile_options("${VULKANETW_COMPILER_FLAGS_COMMON}")
add_compile_options("$<$<CONFIG:Debug>:${VULKANETW_COMPILER_FLAGS_DEBUG}>")
add_compile_options("$<$<CONFIG:Release>:${VULKANETW_COMPILER_FLAGS_RELEASE}>")
add_compile_options("$<$<CONFIG:ReleaseInternal>:${VULKANETW_COMPILER_FLAGS_RELEASEINTERNAL}>")

VulkanEtwSetLinkerFlags("${VULKANETW_LINKER_FLAGS_COMMON}" "")
VulkanEtwSetLinkerFlags("${VULKANETW_LINKER_FLAGS_DEBUG}" "DEBUG")
VulkanEtwSetLinkerFlags("${VULKANETW_LINKER_FLAGS_RELEASE}" "RELEASE" )
VulkanEtwSetLinkerFlags("${VULKANETW_LINKER_FLAGS_RELEASEINTERNAL}" "RELEASEINTERNAL")
