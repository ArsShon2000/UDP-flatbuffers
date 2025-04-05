cmake_minimum_required(VERSION 3.5)

project(figures VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(FLATBUFFERS_PATH "C:/Users/User/Downloads/flatbuffers-24.3.25")

set(PROJECT_SOURCES
    InfoWidget/infowidget.cpp
    DrawWidget/drawwidget.cpp
    UdpReceiver/udpreceiver.cpp
    main.cpp
    mainwindow.cpp
    mainwindow.ui
    icons/src.qrc
)

set(PROJECT_HEADERS
    InfoWidget/infowidget.h
    DrawWidget/drawwidget.h
    UdpReceiver/udpreceiver.h
    mainwindow.h
    api_generated.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(figures
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(figures SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(figures
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(figures PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

target_include_directories(figures PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${FLATBUFFERS_PATH}/include
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_directories(figures PRIVATE ${FLATBUFFERS_PATH}/build/release)
else()
    target_link_directories(figures PRIVATE ${FLATBUFFERS_PATH}/build/debug)
endif()

target_link_libraries(figures PRIVATE flatbuffers)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.figures)
endif()
set_target_properties(figures PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS figures
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(figures)
endif()
