cmake_minimum_required (VERSION 3.8)
project(client)

include_directories(${PROJECT_SOURCE_DIR}/QMessage/include)



add_executable(client "UDPClient.cpp")




target_include_directories(${PROJECT_NAME} 
	PUBLIC C:/Users/12415/source/repos/QMessage/QMessage/liboqs-cpp-0.7.2/include
	PUBLIC C:/Users/12415/source/repos/QMessage/QMessage/liboqs-cpp-0.7.2/liboqsDLL/include
	PUBLIC include
	PUBLIC C:/Users/12415/source/repos/QMessage/QMessage/cryptopp/include/cryptopp
)


target_link_directories(${PROJECT_NAME}
	PUBLIC C:/Users/12415/source/repos/QMessage/QMessage/liboqs-cpp-0.7.2/liboqsDLL/lib
	PUBLIC C:/Users/12415/source/repos/QMessage/QMessage/cryptopp/lib
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