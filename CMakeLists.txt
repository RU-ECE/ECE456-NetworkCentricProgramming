cmake_minimum_required(VERSION 3.28)
project(Rutgers_ECE_451_NetworkCentricProgramming)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

add_executable(S01_In_Class_Primes sessions/01/inclass/primes.cpp)
add_executable(S01_In_Class_SaveWebP sessions/01/inclass/savewebp.cpp)
add_executable(S01_WriteImg sessions/01/writeimg.cpp)

add_executable(S02_Lifetime_Scope sessions/02/01lifetime_scope.c)
add_executable(S02_Primitives sessions/02/02primitives.c)
add_executable(S02_Functions_And_Types sessions/02/03functions_and_types.cpp)
add_executable(S02_High_Speed_IO sessions/02/04high_speed_io.cpp)
