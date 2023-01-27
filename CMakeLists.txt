cmake_minimum_required(VERSION 3.22.2)
project(Motion VERSION 0.1.0 LANGUAGES CXX)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost ${USE_BOOST_VERSION} REQUIRED COMPONENTS coroutine)
find_package(OpenCV REQUIRED COMPONENTS
    opencv_videoio
    opencv_core
    opencv_imgproc
    opencv_highgui
)

include(cmake/external-projects.cmake)
include_directories(${CMAKE_SOURCE_DIR}/include)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    set(RESTC_CPP_LIB restc-cppD)
else()
    set(RESTC_CPP_LIB restc-cpp)
endif()

add_executable(${PROJECT_NAME} src/main.cpp src/motion.cpp src/rest.cpp)
add_dependencies(${PROJECT_NAME} externalRestcCpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
target_link_libraries(${PROJECT_NAME}
    ${RESTC_CPP_LIB}
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${OpenCV_LIBRARIES}
    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
