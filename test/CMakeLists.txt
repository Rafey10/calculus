cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

pkg_check_modules(CHECK REQUIRED check)

include_directories(${CMAKE_SOURCE_DIR}/src ${CHECK_INCLUDE_DIRS})
link_directories(${CHECK_LIBRARY_DIRS})

add_executable(check-service tests.c)

target_link_libraries(check-service b64decode pofix-stack pofix-calc infix-calc
	${CHECK_LIBRARIES}
	${CMAKE_THREAD_LIBS_INIT})