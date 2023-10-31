# Try to force the compiler
SET(CMAKE_C_COMPILER_WORKS TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_WORKS TRUE CACHE INTERNAL "")
SET(CMAKE_C_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_FORCED TRUE CACHE INTERNAL "")
SET(CMAKE_C_COMPILER_ID_RUN TRUE CACHE INTERNAL "")
SET(CMAKE_CXX_COMPILER_ID_RUN TRUE CACHE INTERNAL "")

set(CMAKE_CXX_COMPILER_ID "GHS")
set(CMAKE_CXX_COMPILER_VERSION "")
set(CMAKE_CXX_COMPILER_WRAPPER "")
set(CMAKE_CXX_COMPILE_FEATURES "cxx_std_14")

set(CMAKE_C_COMPILER_ID_RUN "GHS")
set(CMAKE_C_COMPILER_VERSION "")
set(CMAKE_C_COMPILER_WRAPPER "")
set(CMAKE_C_COMPILE_FEATURES "c_std_99")

set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "ppc")

# set pathes for all used tools to ghs, since it is a MULTI Compiler
set(CMAKE_C_COMPILER ccppc CACHE FILEPATH "")
set(CMAKE_C_LINKER cxppc CACHE FILEPATH "")
set(CMAKE_CXX_COMPILER cxppc CACHE FILEPATH "")
set(CMAKE_AR cxppc CACHE FILEPATH "" )

# clean all host includes since it is a cross compiling run
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# needed to get correct dependency-files
set(CMAKE_C_DEPFILE_EXTENSION_REPLACE 1)
set(CMAKE_CXX_DEPFILE_EXTENSION_REPLACE 1)

# disable
set(GHS_FLAGS_C " -noSPE " #disable SPE
                " -fhard " #use hardware floating point unit
                " --short_enum " #make enums as short as possible
                " -floatsingle "  #use same size for float and double
                " -MD"      # make dependency files
                " -time64"
                )

set(GHS_FLAGS_CPP " -language=cxx " #enable cpp
                  " --stdl" # use std lib without exceptions
                  " --no_exceptions"  #disable exceptions
                  " --new_style_casts" #enable static_cast<>
                  " --c++14"  # use cpp 14    
                  " --pending_instantiations=0" #allow template recursion            
                  )

set(GHS_FLAGS_DEBUGING " -Omaxdebug " # max debug information
                       " -dwarf2" # enable debugging with DWARF
                       " -g" # enable source level debugging in debug mode only
                       )

set(GHS_FLAGS_RELEASING " -Ospeed " # optimize for size 
                        " -dwarf2" # enable debugging with DWARF
                        )


set(GHS_FLAGS_LINKING " -map" # generate default map file
                      " -Mx" # enables Map File Cross-Referencing
                      " -vle" # enable variable length encoding
                      " -hex=${PROJECT_NAME}.hex" # enable hex files
                      " -sda"
                      " -farcallpatch"
                      " -paddr_offset=0"
                      )

set(GHS_FLAGS_CONSOL_OUTPUT " --no_wrap_diagnostics" # no line break after 80 chars
                            " --brief_diagnostics" # only print short diagnostics
                            #" -full_debug_info" # print infos on defined and never used symbols
                            )

set(PPC_CPU "ppc5746mz420")
set(GHS_FLAGS_BSP " -bsp generic " # no specific bsl
                  " -cpu=${PPC_CPU} " # our cpu
                  )

# since we defined all flags as lists of stings to be able to comment them easily, we need to join them together
# again in one large string that we can pass to the compiler
string(REPLACE ";" " " GHS_FLAGS_C "${GHS_FLAGS_C}")
string(REPLACE ";" " " GHS_FLAGS_CPP "${GHS_FLAGS_CPP}")
string(REPLACE ";" " " GHS_FLAGS_DEBUGING "${GHS_FLAGS_DEBUGING}")
string(REPLACE ";" " " GHS_FLAGS_RELEASING "${GHS_FLAGS_RELEASING}")
string(REPLACE ";" " " GHS_FLAGS_LINKING "${GHS_FLAGS_LINKING}")
string(REPLACE ";" " " GHS_FLAGS_CONSOL_OUTPUT "${GHS_FLAGS_CONSOL_OUTPUT}")
string(REPLACE ";" " " GHS_FLAGS_BSP "${GHS_FLAGS_BSP}")

set(GHS_FLAGS_DEBUG "${GHS_FLAGS_C} ${GHS_FLAGS_CPP} ${GHS_FLAGS_DEBUGING} ${GHS_FLAGS_LINKING} ${GHS_FLAGS_CONSOL_OUTPUT} ${GHS_FLAGS_BSP} ${} -gsize ")
set(GHS_FLAGS_RELEASE "${GHS_FLAGS_C} ${GHS_FLAGS_CPP} ${GHS_FLAGS_RELEASING} ${GHS_FLAGS_LINKING} ${GHS_FLAGS_CONSOL_OUTPUT} ${GHS_FLAGS_BSP} ${} -gsize ")

unset(CMAKE_C_FLAGS CACHE)
unset(CMAKE_CXX_FLAGS CACHE)
unset(CMAKE_EXE_LINKER_FLAGS CACHE)
unset(CMAKE_C_COMPILE_OBJECT CACHE)
unset(CMAKE_C_CREATE_STATIC_LIBRARY CACHE)
unset(CMAKE_CXX_CREATE_STATIC_LIBRARY CACHE)
unset(CMAKE_CXX_LINK_EXECUTABLE CACHE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${GHS_FLAGS_DEBUG}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS "${GHS_FLAGS_DEBUG} --diag_suppress=111" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS "${GHS_FLAGS_DEBUG}" CACHE STRING "" FORCE)
else() #not a debug build
    set(CMAKE_C_FLAGS "${GHS_FLAGS_RELEASE}" CACHE STRING "" FORCE)
    set(CMAKE_CXX_FLAGS "${GHS_FLAGS_RELEASE} --diag_suppress=111" CACHE STRING "" FORCE)
    set(CMAKE_EXE_LINKER_FLAGS "${GHS_FLAGS_RELEASE}" CACHE STRING "" FORCE)
endif()

set(CMAKE_C_COMPILE_OBJECT "<CMAKE_C_COMPILER>  <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> -archive <LINK_FLAGS> <OBJECTS> -o <TARGET> ")
set(CMAKE_CXX_CREATE_STATIC_LIBRARY "<CMAKE_AR> -archive <LINK_FLAGS> <OBJECTS> -o <TARGET> ")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
#Added for systemcommon
set(ETL_PROFILE "PROFILE_CPP14_GENERIC" CACHE STRING "Defines what profile header to include.  See https://www.etlcpp.com/setup.html" FORCE)
add_definitions(-DPB_CONVERT_DOUBLE_FLOAT)
set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-sys_include_directory ")

set(DEVICE_UNDER_TEST_ARCH "PPC")
set(PROJECT_PLATFORM "ppc5746mz420") 
