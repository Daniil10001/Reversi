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

add_library(BOARD SHARED board.cpp)
target_compile_definitions(BOARD PUBLIC BOARD)
target_link_libraries(BOARD PUBLIC ${LIBS} -lpanel)

add_library(MESSAGE SHARED message.cpp)
target_compile_definitions(MESSAGE PUBLIC MESSAGE)
target_link_libraries(MESSAGE PUBLIC ${LIBS} -lpanel)

add_library(ENGINE SHARED game_eg.cpp)
target_compile_definitions(ENGINE PUBLIC ENGINE)
target_link_libraries(ENGINE PUBLIC BOARD MESSAGE ${LIBS} -lpanel)


add_executable( Reversi main.cpp )

target_link_libraries( Reversi PUBLIC ${LIBS} -lpanel  BOARD ENGINE MESSAGE)
 
