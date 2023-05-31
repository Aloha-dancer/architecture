cmake_minimum_required(VERSION 3.2)

project(hl_server C CXX)

SET(USER_SERVICE "user_service")
SET(SERVING_SERVICE "serving_service")
SET(ORDER_SERVICE "order_service")
set (writer "writer")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL Data)
find_package(CppKafka REQUIRED)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories("/usr/include/mysql")
include_directories("/usr/local/include/mysql")
link_directories("/usr/local/lib")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${USER_SERVICE}  user_main.cpp
                                config/config.cpp
                                database/cache.cpp
                                database/database.cpp
                                entities/user/user.cpp)

add_executable(loader cache_testing/test.cpp)

add_executable(${writer} writer.cpp
        database/cache.cpp
        config/config.cpp
        database/database.cpp
        entities/service/service.cpp
        entities/order/order.cpp
        entities/user/user.cpp)

target_include_directories(${USER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${USER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${USER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${USER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${USER_SERVICE} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoData"
                             "PocoDataMySQL"
                             "mysqlclient"
                             ZLIB::ZLIB
                             "rdkafka++"
                             CppKafka::cppkafka)

target_include_directories(loader PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(loader PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(loader PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(loader PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(${writer} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${writer} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${writer} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${writer} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${writer} PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        ${Poco_LIBRARIES}
        "PocoData"
        "PocoDataMySQL"
        "mysqlclient"
        ZLIB::ZLIB
        "rdkafka++"
        CppKafka::cppkafka)


target_link_libraries(loader PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        ${Poco_LIBRARIES}
        "PocoData"
        "PocoDataMySQL"
        "mysqlclient"
        ZLIB::ZLIB)

add_executable(${SERVING_SERVICE} service_main.cpp
                                  config/config.cpp
                                  database/cache.cpp
                                  database/database.cpp
                                  entities/service/service.cpp)


target_include_directories(${SERVING_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${SERVING_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${SERVING_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${SERVING_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${SERVING_SERVICE} PRIVATE 
                          ${CMAKE_THREAD_LIBS_INIT} 
                          ${Poco_LIBRARIES}
                          "PocoData"
                          "PocoDataMySQL"
                          "mysqlclient"
                          ZLIB::ZLIB)

add_executable(${ORDER_SERVICE} order_main.cpp
                                config/config.cpp
                                database/cache.cpp
                                database/database.cpp
                                entities/order/order.cpp)


target_include_directories(${ORDER_SERVICE} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${ORDER_SERVICE} PRIVATE -Wall -Wextra -pedantic -Werror )
set_target_properties(${ORDER_SERVICE} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${ORDER_SERVICE} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${ORDER_SERVICE} PRIVATE 
                       ${CMAKE_THREAD_LIBS_INIT} 
                       ${Poco_LIBRARIES}
                       "PocoData"
                       "PocoDataMySQL"
                       "mysqlclient"
                       ZLIB::ZLIB)