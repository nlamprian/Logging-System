cmake_minimum_required ( VERSION 2.8.7 )
project ( loggingSystem )

file ( GLOB LOGS ${PROJECT_SOURCE_DIR}/data/*.txt )
file ( COPY ${LOGS} DESTINATION ${PROJECT_BINARY_DIR} )

include_directories ( ${PROJECT_SOURCE_DIR}/include )

file ( GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.c )

add_executable ( loggingSystem ${SRCS} )
