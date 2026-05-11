cmake_minimum_required(VERSION 3.28)
project(Rutgers_ECE_451_NetworkCentricProgramming LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

function(add_example target source)
    add_executable(${target} ${source})
endfunction()

function(add_threaded_example target source)
    add_example(${target} ${source})
    target_link_libraries(${target} PRIVATE Threads::Threads)
endfunction()

# Explicit target lists keep vendored third-party source trees under sessions/04
# out of the top-level course build.

# Session 01
add_example(S01_In_Class_Primes sessions/01/in_class/primes.cpp)
add_example(S01_In_Class_SaveWebP sessions/01/in_class/savewebp.cpp)
add_example(S01_WriteImg sessions/01/writeimg.cpp)

# Session 02
add_example(S02_Lifetime_Scope sessions/02/01lifetime_scope.c)
add_example(S02_Primitives sessions/02/02primitives.c)
add_example(S02_Functions_And_Types sessions/02/03functions_and_types.cpp)
add_example(S02_High_Speed_IO sessions/02/04high_speed_io.cpp)
add_example(S02_Bench sessions/02/05bench.cpp)

# Session 03
add_example(S03_Pointers sessions/03/01_pointers.cpp)
add_example(S03_Bounds_Error sessions/03/02bounds_error.cpp)
add_example(S03_Const_Pointer sessions/03/03_constpointer.cpp)
add_example(S03_BadInline sessions/03/badinline.cpp)
add_example(S03_Inline_Example sessions/03/inline_example.cpp)

# Session 04
add_executable(S04_A sessions/04/a.cpp sessions/04/b.cpp)
add_executable(S04_A2 sessions/04/a2.cpp sessions/04/b2.cpp)
add_executable(S04_TestMathLib sessions/04/testmathlib.cpp sessions/04/mathlib.cpp)
add_executable(S04_BadTestMathLib sessions/04/badtestmathlib.cpp sessions/04/mathlib.cpp)

# Session 05
add_example(S05_Points sessions/05/points.cpp)
add_threaded_example(S05_Threads sessions/05/threads.cpp)

# Session 06
add_example(S06_06_Attack1 sessions/06/06attack1.cpp)

# Session 09
add_example(S09_Simple_Crypt sessions/09/01_simple_crypt.cpp)
add_example(S09_Test sessions/09/test.cpp)

# Session 12
add_example(S12_TestJson sessions/12/testjson.cpp)

# Session 13
add_example(S13_00_HelloWorld sessions/13/00_helloword.cpp)
add_example(S13_01_Cpp_Macros_Are_Awful sessions/13/01_cpp_macros_are_awful.cpp)
add_example(S13_02_MemoryLeak sessions/13/02_memoryleak.cpp)
add_example(S13_04_Double_Free sessions/13/04_double_free.cpp)
add_threaded_example(S13_05_Threadsafe sessions/13/05_threadsafe.cpp)
add_example(S13_06_Pointer sessions/13/06_pointer.cpp)

find_package(OpenSSL QUIET COMPONENTS SSL Crypto)
if(OpenSSL_CRYPTO_FOUND)
    add_example(S09_AES256Crypto sessions/09/02_aes256crypto.cpp)
    target_link_libraries(S09_AES256Crypto PRIVATE OpenSSL::Crypto)
else()
    message(STATUS "Skipping S09_AES256Crypto because OpenSSL::Crypto was not found.")
endif()

if(UNIX)
    # Session 03
    add_example(S03_Calling_Kernel sessions/03/04_calling_kernel.cpp)

    # Session 04
    add_example(S04_WriteFile sessions/04/writefile.cpp)

    # Session 05
    add_example(S05_Client sessions/05/client.cpp)
    add_example(S05_Server sessions/05/server.cpp)

    # Session 06
    add_threaded_example(S06_01_Threads sessions/06/01threads.cpp)
    add_threaded_example(S06_02_Race_Conditions sessions/06/02race_conditions.cpp)
    add_threaded_example(S06_03_Mutex sessions/06/03mutex.cpp)
    add_threaded_example(S06_04_Deadlock sessions/06/04deadlock.cpp)
    add_example(S06_05_Fork sessions/06/05fork.cpp)
    add_example(S06_08_Client sessions/06/08client.cpp)
    add_example(S06_08_Server sessions/06/08server.cpp)
    add_threaded_example(S06_Collaborative_Server sessions/06/server.cpp)
    add_threaded_example(S06_Thread_Vs_Fork sessions/06/thread_vs_fork.cpp)

    find_package(Curses QUIET)
    if(CURSES_FOUND)
        add_threaded_example(S06_Collaborative_Client sessions/06/client.cpp)
        target_include_directories(S06_Collaborative_Client PRIVATE ${CURSES_INCLUDE_DIRS})
        target_link_libraries(S06_Collaborative_Client PRIVATE ${CURSES_LIBRARIES})

        add_example(S06_Editor sessions/06/Editor.cpp)
        target_include_directories(S06_Editor PRIVATE ${CURSES_INCLUDE_DIRS})
        target_link_libraries(S06_Editor PRIVATE ${CURSES_LIBRARIES})
    else()
        message(STATUS "Skipping ncurses-based session 06 examples because Curses was not found.")
    endif()

    # Session 07
    add_example(S07_01_Hello sessions/07/01hello.cpp)
    add_example(S07_02_Hello sessions/07/02hello.cpp)
    add_example(S07_Logger sessions/07/Logger.cpp)
    add_example(S07_TestLog sessions/07/TestLog.cpp)
    add_example(S07_Midterm_Kernel1 sessions/07/midtermprep/code/kernel1.cpp)
    add_example(S07_Midterm_Kernel2 sessions/07/midtermprep/code/kernel2.cpp)
    add_example(S07_Midterm_Kernel3 sessions/07/midtermprep/code/kernel3.cpp)
    add_example(S07_Midterm_Optim2 sessions/07/midtermprep/code/optim2.cpp)
    add_example(S07_Midterm_Optim3 sessions/07/midtermprep/code/optim3.cpp)
    add_example(S07_Midterm_Optim4 sessions/07/midtermprep/code/optim4.cpp)
    add_example(S07_Midterm_TestSignal sessions/07/midtermprep/code/testsignal.cpp)
    add_threaded_example(S07_Midterm_Thread1 sessions/07/midtermprep/code/thread1.cpp)
    add_threaded_example(S07_Midterm_Thread_Order sessions/07/midtermprep/thread_order.cpp)
    add_threaded_example(S07_Midterm_Thread_Race sessions/07/midtermprep/thread_race.cpp)
    add_threaded_example(S07_Midterm_Thread_Race2 sessions/07/midtermprep/thread_race2.cpp)
    add_threaded_example(S07_Midterm_Threads sessions/07/midtermprep/threads.cpp)

    # Session 10
    add_example(S10_Sendfile_Udp sessions/10/sendfile_udp.cpp)

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_example(S10_RawLink sessions/10/rawlink.cpp)
    else()
        message(STATUS "Skipping S10_RawLink because it requires Linux AF_PACKET headers.")
    endif()

    find_path(MSQUIC_INCLUDE_DIR NAMES msquic.h)
    find_library(MSQUIC_LIBRARY NAMES msquic)
    if(MSQUIC_INCLUDE_DIR AND MSQUIC_LIBRARY)
        add_example(S10_FileQuic sessions/10/filequic.cpp)
        target_include_directories(S10_FileQuic PRIVATE ${MSQUIC_INCLUDE_DIR})
        target_link_libraries(S10_FileQuic PRIVATE ${MSQUIC_LIBRARY})
    else()
        message(STATUS "Skipping S10_FileQuic because MsQuic was not found.")
    endif()

    # Session 11
    add_example(S11_Minimal_Http_Server sessions/11/minimal_http_server.cpp)

    if(OpenSSL_SSL_FOUND)
        add_example(S11_Https_Server sessions/11/https_server.cpp)
        target_link_libraries(S11_Https_Server PRIVATE OpenSSL::SSL OpenSSL::Crypto)
    else()
        message(STATUS "Skipping S11_Https_Server because OpenSSL::SSL was not found.")
    endif()
endif()

# Not added:
# - sessions/03/declarations.cpp: helper/demo code without a main function.
# - sessions/03/multipleinclude.cpp: depends on a missing C.hpp example header.
# - sessions/04/b.cpp and sessions/04/b2.cpp: support files linked into S04_A and S04_A2.
# - sessions/04/mathlib.cpp: support file linked into the mathlib test executables.
# - sessions/06/07denial_of_service.cpp: currently incomplete and does not compile.
# - sessions/07/midtermprep/pointers1.cpp: intentionally contains compile errors.
# - sessions/12/skeleton_sign.cpp: exercise scaffold with unimplemented functions.
# - sessions/13/02b_memoryleakfix.cpp: currently incomplete and does not compile.
# - sessions/13/06_openfile.cpp: currently incomplete and does not compile.
