cmake_minimum_required(VERSION 3.16)

project(QRCodeScanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Multimedia Concurrent)

add_subdirectory(lib/zxing-cpp/core)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appQRCodeScanner
    main.cpp
)

qt_add_qml_module(appQRCodeScanner
    URI QRCodeScanner
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES qrcodescanner.h qrcodescanner.cpp
    RESOURCES .gitignore
    RESOURCES README.md

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQRCodeScanner PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQRCodeScanner
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/android
)

target_link_libraries(appQRCodeScanner
    PRIVATE Qt6::Quick Qt6::Multimedia Qt6::Concurrent ZXing
)

include(GNUInstallDirs)

install(TARGETS appQRCodeScanner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
