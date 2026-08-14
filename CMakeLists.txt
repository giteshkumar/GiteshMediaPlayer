cmake_minimum_required(VERSION 3.5)

project(GiteshMediaPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# =====================================================
# FIND QT
# =====================================================

find_package(QT NAMES Qt6 Qt5 COMPONENTS
    Widgets
    Multimedia
    MultimediaWidgets
    LinguistTools
    REQUIRED
)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
    Widgets
    Multimedia
    MultimediaWidgets
    LinguistTools
    REQUIRED
)


# =====================================================
# SOURCE FILES
# =====================================================

set(TS_FILES
    GiteshMediaPlayer_en_AS.ts
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ${TS_FILES}
)


# =====================================================
# EXECUTABLE
# =====================================================

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

    qt_add_executable(GiteshMediaPlayer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )

else()

    if(ANDROID)

        add_library(GiteshMediaPlayer SHARED
            ${PROJECT_SOURCES}
        )

    else()

        add_executable(GiteshMediaPlayer
            ${PROJECT_SOURCES}
        )

    endif()

endif()


# =====================================================
# LINK QT LIBRARIES
# =====================================================

target_link_libraries(GiteshMediaPlayer PRIVATE

    Qt${QT_VERSION_MAJOR}::Widgets

    Qt${QT_VERSION_MAJOR}::Multimedia

    Qt${QT_VERSION_MAJOR}::MultimediaWidgets

)


# =====================================================
# QT 6 FINALIZATION
# =====================================================

if(QT_VERSION_MAJOR EQUAL 6)

    qt_finalize_executable(GiteshMediaPlayer)

endif()
