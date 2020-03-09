project(blurcur)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED)

find_package(OpenCV REQUIRED
COMPONENTS core highgui imgproc
)

include_directories(${Boost_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})


target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBRARIES})
