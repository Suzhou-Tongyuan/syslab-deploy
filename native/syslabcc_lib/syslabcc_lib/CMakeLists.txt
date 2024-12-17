cmake_minimum_required(VERSION 3.5)

#===========settings from scc============
set(OUTPUT_EXE "")
set(OUTPUT_DL "out")
set(OUTPUT_SL "")
set(EXTRA_ARGS "-DSYSLABCC_THREADPOOLSIZE=4")
set(TARGET_OS "windows")
set(TARGET_ARCH "x86_64")
set(LIBC "glibc")
set(HARDFLOAT "false")
set(GLIBC_VERSION "2.17")
set(ENDIANNESS "little")
set(ARMCPU "")
set(STATIC_MINGW "false")

#============lower variables=============
string(TOLOWER ${TARGET_OS} TARGET_OS)
string(TOLOWER ${TARGET_ARCH} TARGET_ARCH)
string(TOLOWER ${ENDIANNESS} ENDIANNESS)
string(TOLOWER ${LIBC} LIBC)
string(TOLOWER ${HARDFLOAT} HARDFLOAT)
string(TOLOWER ${STATIC_MINGW} STATIC_MINGW)

#============compiler config=============

include(${CMAKE_SOURCE_DIR}/cmakeconfig/toolconfig.cmake)

#==============project part==============
project("CMakeProject")
## BDWGC
file(GLOB srcs_bdwgc "bdwgc/*.c")
add_library(bdwgc_o OBJECT ${srcs_bdwgc})
target_compile_options(bdwgc_o PRIVATE "-fPIC" "${ARMCPUOPTION}")
target_include_directories(bdwgc_o PRIVATE "bdwgc/include" "atomic_ops/src")

# define bdwgc macros
set(bdwgc_definitions
        ENABLE_DISCLAIM
        GC_ATOMIC_UNCOLLECTABLE
        GC_GCJ_SUPPORT
        JAVA_FINALIZATION
        NO_EXECUTE_PERMISSION
        USE_MMAP
        USE_MUNMAP
        GC_THREADS
        PARALLEL_MARK
        THREAD_LOCAL_ALLOC
        GC_ALWAYS_MULTITHREADED
)
if("${TARGET_OS}" STREQUAL "windows")
    list(APPEND bdwgc_definitions WIN32_THREADS USE_SPIN_LOCK)
endif()
target_compile_definitions(bdwgc_o PRIVATE ${bdwgc_definitions})


if(OUTPUT_SL STREQUAL "")
    # bdwgc dynamic library
    add_library(bdwgc SHARED $<TARGET_OBJECTS:bdwgc_o> "win32-implib/bdwgc.c")
    target_include_directories(bdwgc PRIVATE "bdwgc/include" "atomic_ops/src")
    target_compile_options(bdwgc PUBLIC "-fPIC" "${ARMCPUOPTION}")
    set_target_properties(bdwgc PROPERTIES PREFIX "")
    set_target_properties(bdwgc PROPERTIES OUTPUT_NAME "bdwgc")

    # move to bin
    add_custom_command(TARGET bdwgc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:bdwgc>
        "${CMAKE_SOURCE_DIR}/bin/$<TARGET_FILE_NAME:bdwgc>"
    )
endif()


# syslabcrt-io
file(GLOB_RECURSE srcs_syslabcc_io "syslabcrt-io/src/*.c")
add_library(syslabcrt_io_o OBJECT ${srcs_syslabcc_io})
target_compile_options(syslabcrt_io_o PRIVATE "-fPIC" "${ARMCPUOPTION}")
target_include_directories(syslabcrt_io_o PRIVATE "syslabcrt-io/include")


if(OUTPUT_SL STREQUAL "")
    add_library(syslabcrt_io STATIC $<TARGET_OBJECTS:syslabcrt_io_o>)
    target_include_directories(syslabcrt_io PUBLIC "syslabcrt-io/include")
    target_compile_options(syslabcrt_io PRIVATE "-fPIC")
endif()


# syslabcrt-dylib
file(GLOB srcs_syslabcc_dylib "syslabcrt-dylib/syslabcrt_dylib.c")
add_library(syslabcrt_dylib_o OBJECT ${srcs_syslabcc_dylib})
target_compile_options(syslabcrt_dylib_o PRIVATE "-fPIC" "${ARMCPUOPTION}")


if(OUTPUT_SL STREQUAL "")
    add_library(syslabcrt_dylib STATIC $<TARGET_OBJECTS:syslabcrt_dylib_o>)
    target_compile_options(syslabcrt_dylib PRIVATE "-fPIC")
endif()


# executable, static or shared library
if (NOT "${OUTPUT_EXE}" STREQUAL "")
    set(OUTPUT_TARGET ${OUTPUT_EXE})
    add_executable(${OUTPUT_EXE} "${OUTPUT_EXE}.cpp")
    target_compile_options(${OUTPUT_EXE} PRIVATE ${ZIG_EXTRA_EXE})
elseif(NOT "${OUTPUT_DL}" STREQUAL "")
    set(OUTPUT_TARGET ${OUTPUT_DL})
    add_library(${OUTPUT_DL} SHARED "${OUTPUT_DL}.cpp")
    target_compile_options(${OUTPUT_DL} PRIVATE "-fPIC")
else()
    set(OUTPUT_TARGET ${OUTPUT_SL})
    add_library(${OUTPUT_SL} STATIC "${OUTPUT_SL}.cpp" "win32-implib/bdwgc.c" $<TARGET_OBJECTS:bdwgc_o> $<TARGET_OBJECTS:syslabcrt_io_o> $<TARGET_OBJECTS:syslabcrt_dylib_o>)
endif()

target_include_directories(${OUTPUT_TARGET} PRIVATE "syslabcrt-intrinsics" "ghc-filesystem")
target_compile_definitions(${OUTPUT_TARGET} PRIVATE GHC_WITH_EXCEPTIONS)
target_compile_options(${OUTPUT_TARGET} PRIVATE ${FINAL_ARGS} ${EXTRA_ARGS})



if("${OUTPUT_SL}" STREQUAL "")
    target_link_libraries(${OUTPUT_TARGET} syslabcrt_io syslabcrt_dylib)

    # Avoiding duplicate symbols in Windows, bdwgc.dll and libbdwgc.dll.a must coexist in build directory and then use target_link_directories and target_link_options instruction to link the library
    if("${TARGET_OS}" STREQUAL "windows" AND "${C_TOOLCHAIN}" STREQUAL "zig")
        target_link_directories(${OUTPUT_TARGET} PRIVATE ${CMAKE_BINARY_DIR})
        # Assure that build directory has bdwgc.dll and libbdwgc.dll.a in order to multithreaded synchronization.
        add_dependencies(${OUTPUT_TARGET} bdwgc)
        if (DEFINED USE_MSVC)
            target_link_libraries(${OUTPUT_TARGET} bdwgc)
        else()
            # only for zig
            target_link_options(${OUTPUT_TARGET} PRIVATE -lbdwgc)
        endif()
    else()
        target_link_libraries(${OUTPUT_TARGET} bdwgc)
        target_link_options(${OUTPUT_TARGET} PRIVATE "${ARMCPUOPTION}")
    endif()

    set_target_properties(${OUTPUT_TARGET} PROPERTIES BUILD_RPATH "\$ORIGIN:\$ORIGIN:\$ORIGIN/lib:.")

else()
    target_include_directories(${OUTPUT_TARGET} PRIVATE "syslabcrt-io/include")
    target_compile_options(${OUTPUT_TARGET} PRIVATE "${ARMCPUOPTION}")
endif()

set_target_properties(${OUTPUT_TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set_target_properties(${OUTPUT_TARGET} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set_target_properties(${OUTPUT_TARGET} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# bundle dependencies
if(EXISTS "${CMAKE_SOURCE_DIR}/lib")
    file(COPY "${CMAKE_SOURCE_DIR}/lib/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/lib/")
endif()