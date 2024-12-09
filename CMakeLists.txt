cmake_minimum_required(VERSION 3.8)
project( Reversy )
set(CMAKE_CXX_STANDARD 14)
include(CMakePrintHelpers)

execute_process(COMMAND pkg-config --cflags ncurses OUTPUT_VARIABLE FLAGS)
execute_process(COMMAND pkg-config --libs ncurses OUTPUT_VARIABLE LIBS)

string(REGEX REPLACE "\n$" "" FLAGS "${FLAGS}")
string(REGEX REPLACE "\n$" "" LIBS "${LIBS}")

cmake_print_variables(FLAGS LIBS)
add_compile_options(${FLAGS})
add_compile_options(-O2 -Wall)

add_executable( Reversi main.cpp board.cpp game_eg.cpp message.cpp)

target_link_libraries( Reversi ${LIBS} -lpanel)
 
