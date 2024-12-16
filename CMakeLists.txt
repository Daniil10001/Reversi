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

add_library(BOARD OBJECT board.cpp)
target_compile_definitions(BOARD PUBLIC BOARD)


add_library(ENGINE OBJECT game_eg.cpp)
target_compile_definitions(ENGINE PUBLIC ENGINE)
target_link_libraries(ENGINE PUBLIC BOARD)

add_library(MESSAGE OBJECT message.cpp)
target_compile_definitions(MESSAGE PUBLIC MESSAGE)

add_executable( Reversi main.cpp )

set(LIBSG "BOARD ENGINE MESSAGE")
cmake_print_variables(LIBSG)
#add_dependencies(Reversi BOARD ENGINE MESSAGE)
target_link_libraries( Reversi PUBLIC ${LIBS} -lpanel  BOARD ENGINE MESSAGE)
 
