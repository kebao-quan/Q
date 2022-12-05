# CMakeList.txt : CMake project for QMessage, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0079 NEW)

set(OQS_CPP_VERSION_NUM 0.7.2)
set(OQS_CPP_VERSION_STR "${OQS_CPP_VERSION_NUM}")

# liboqs-cpp version number
add_definitions(-DOQS_CPP_VERSION="${OQS_CPP_VERSION_STR}")


# Add source to this project's executable.
add_executable (QMessage "QMessage.cpp" "QMessage.h"  "imgui/ma.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET QMessage PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
add_subdirectory ("liboqs-cpp")


target_include_directories(${PROJECT_NAME} 
	PUBLIC liboqs-cpp/include
)


target_include_directories(${PROJECT_NAME} 
	PUBLIC liboqs-cpp/liboqsDLL/include
)

target_link_directories(${PROJECT_NAME}
	PUBLIC liboqs-cpp/liboqsDLL/lib
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE oqs
)

