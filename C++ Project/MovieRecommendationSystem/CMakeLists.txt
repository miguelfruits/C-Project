# CMakeLists.txt — Movie Recommendation System
#
# How to build:
#   mkdir build && cd build && cmake .. && cmake --build .
#
# On Windows, make sure MySQL Server is installed at the default path, or
# adjust MYSQL_INCLUDE_DIR and MYSQL_LIB below to match your installation.
#
# On Linux, install the MySQL C connector dev package first:
#   sudo apt-get install libmysqlclient-dev   (Debian/Ubuntu)
#   sudo dnf install mysql-devel              (Fedora/RHEL)
# Then cmake will locate it automatically via find_package or pkg-config.

cmake_minimum_required(VERSION 3.14)
project(MovieRecommendationSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Source files
# ---------------------------------------------------------------------------
set(SOURCES
    main.cpp
    DataStore.cpp
    AuthManager.cpp
    MovieManager.cpp
    RatingManager.cpp
    FavouritesManager.cpp
    RecommendationEngine.cpp
    WatchHistoryManager.cpp
    MenuController.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

# ---------------------------------------------------------------------------
# MySQL Connector/C++ (JDBC API) — platform detection
# ---------------------------------------------------------------------------
if(WIN32)
    # --- Windows: MySQL Connector/C++ 9.x installation path ---
    # Adjust CONNECTOR_ROOT if the connector is installed elsewhere.
    set(CONNECTOR_ROOT
        "C:/mysql-connector-c++-9.7.0-winx64/mysql-connector-c++-9.7.0-winx64"
        CACHE PATH "Root directory of MySQL Connector/C++ installation")

    set(CONNECTOR_INCLUDE_DIR "${CONNECTOR_ROOT}/include")
    set(CONNECTOR_LIB         "${CONNECTOR_ROOT}/lib64/vs14/mysqlcppconn.lib")

    if(NOT EXISTS "${CONNECTOR_INCLUDE_DIR}")
        message(WARNING
            "MySQL Connector/C++ include directory not found at '${CONNECTOR_INCLUDE_DIR}'. "
            "Set CONNECTOR_ROOT to the correct installation path, e.g.:\n"
            "  cmake .. -DCONNECTOR_ROOT=\"C:/path/to/mysql-connector-c++\"")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE "${CONNECTOR_INCLUDE_DIR}")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${CONNECTOR_LIB}")

    # Copy the runtime DLL next to the executable so it can be found at launch.
    set(CONNECTOR_DLL "${CONNECTOR_ROOT}/lib64/mysqlcppconn-10-vs14.dll")
    if(EXISTS "${CONNECTOR_DLL}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CONNECTOR_DLL}"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMENT "Copying mysqlcppconn-10-vs14.dll to output directory")
    endif()

else()
    # --- Linux / macOS: connector is typically on the default search path ---
    # Install via: sudo apt-get install libmysqlcppconn-dev  (Debian/Ubuntu)
    #              sudo dnf install mysql-connector-c++-devel (Fedora/RHEL)
    find_library(CONNECTOR_LIB NAMES mysqlcppconn)
    if(CONNECTOR_LIB)
        target_link_libraries(${PROJECT_NAME} PRIVATE "${CONNECTOR_LIB}")
    else()
        message(WARNING
            "Could not locate mysqlcppconn. "
            "Ensure the MySQL Connector/C++ development package is installed.")
        target_link_libraries(${PROJECT_NAME} PRIVATE mysqlcppconn)
    endif()
endif()
