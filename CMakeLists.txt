cmake_minimum_required(VERSION 3.5)

project(IOT_QT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 添加头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}/windows
    ${PROJECT_SOURCE_DIR}/customControl
)

# 子目录头文件
file(GLOB HEADFILE
    ${PROJECT_SOURCE_DIR}/windows/*.h
    ${PROJECT_SOURCE_DIR}/customControl/*.h
)

# 子目录源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/windows SRC_WINDOWS)
aux_source_directory(${PROJECT_SOURCE_DIR}/customControl SRC_CUSTOMCONTROL)

# 添加资源文件
qt_add_resources(RES res.qrc)

# 项目所有文件
set(PROJECT_SOURCES
    main.cpp
    ${HEADFILE}
    ${SRC_WINDOWS}
    ${SRC_CUSTOMCONTROL}
    ${RES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(IOT_QT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET IOT_QT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(IOT_QT SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(IOT_QT
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(IOT_QT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    # 自定义动态库
    # windows
    # customControl
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.IOT_QT)
endif()
set_target_properties(IOT_QT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS IOT_QT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(IOT_QT)
endif()
