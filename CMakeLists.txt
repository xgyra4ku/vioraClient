cmake_minimum_required(VERSION 3.16)

project(voicebet VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти необходимые компоненты Qt
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# Указываем файлы переводов
set(TS_FILES voicebet_ru_RU.ts)

# Указываем исходные файлы проекта
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        network.h network.cpp
        structure.h
        core.h core.cpp
        ${TS_FILES}
)

# Создание исполнимого файла для Qt6
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(voicebet
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        network.h network.cpp
        structure.h
        core.h core.cpp
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    # Создание исполнимого файла для Qt5 или других платформ
    if(ANDROID)
        add_library(voicebet SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(voicebet
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# Линкуем Qt библиотеку
target_link_libraries(voicebet PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Линковка с библиотекой Winsock для работы с сокетами на Windows
if(WIN32)
    target_link_libraries(voicebet PRIVATE ws2_32)
endif()

# Для iOS/macOS устанавливаем свойства бандла (если необходимо)
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.voicebet)
endif()

# Устанавливаем свойства для таргета
set_target_properties(voicebet PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Устанавливаем таргет
include(GNUInstallDirs)
install(TARGETS voicebet
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Финализация исполнимого файла для Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(voicebet)
endif()
