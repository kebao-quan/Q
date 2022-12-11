# CMakeList.txt : CMake project for QMessage, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
cmake_policy(SET CMP0079 NEW)

set(OQS_CPP_VERSION_NUM 0.7.2)
set(OQS_CPP_VERSION_STR "${OQS_CPP_VERSION_NUM}")


# liboqs-cpp version number
add_definitions(-DOQS_CPP_VERSION="${OQS_CPP_VERSION_STR}")

add_subdirectory ("liboqs-cpp-0.7.2")
# add_subdirectory ("src")

# Add source to this project's executable.
add_executable (QMessage 
	"src/QMessage.cpp"
	"include/QMessage.h"
	"src/util.cpp"
	"src/ServerSocket.cpp"
	"src/message.cpp"
	"src/encrypt.cpp"
	"src/UseImgui.cpp"
	"include/showimgui.h"
	"src/UDPServer.cpp"
	"include/UDPClient.h"
	"src/UDPClient.cpp")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET QMessage PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.



target_include_directories(${PROJECT_NAME} 
	PUBLIC liboqs-cpp-0.7.2/include
	PUBLIC liboqs-cpp-0.7.2/liboqsDLL/include
	PUBLIC cryptopp/include/cryptopp
	PUBLIC include
)


target_link_directories(${PROJECT_NAME}
	PUBLIC liboqs-cpp-0.7.2/liboqsDLL/lib
	PUBLIC cryptopp/lib
)

target_link_libraries(${PROJECT_NAME}
	PRIVATE oqs 
	PRIVATE cryptopp
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} 
	PUBLIC wsock32
	PUBLIC ws2_32)
endif()


 find_package(glad CONFIG REQUIRED)
 target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)


 find_package(glfw3 CONFIG REQUIRED)
 target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

 find_package(imgui CONFIG REQUIRED)
 target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)