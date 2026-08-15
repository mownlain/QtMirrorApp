cmake_minimum_required(VERSION 3.16)

project(QtMirrorApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network)

qt_standard_project_setup(REQUIRES 6.5)

# qt_add_executable တွင် MACOSX_BUNDLE ပါဝင်ရန် လိုအပ်ပါသည်
qt_add_executable(appQtMirrorApp MACOSX_BUNDLE
    main.cpp
    screenstreamer.h
    screenstreamer.cpp
    imageprovider.h
)

# macOS App Properties များ သတ်မှတ်ခြင်း
set_target_properties(appQtMirrorApp PROPERTIES
    MACOSX_BUNDLE_BUNDLE_NAME "QtMirrorApp"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.yourdomain.qtmirrorapp"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
)

# --- Windows CMD Window မပေါ်စေရန် ဒီနေရာတွင် ဖြည့်ပေးပါ ---
if(WIN32)
    set_target_properties(appQtMirrorApp PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
# --------------------------------------------------

# QML Module မှန်ကန်စွာ သတ်မှတ်ခြင်း
qt_add_qml_module(appQtMirrorApp
    URI QtMirrorApp
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(appQtMirrorApp PRIVATE
    Qt6::Quick
    Qt6::Network
)
# macOS Target ဖြစ်ပါက AGL Error မတက်စေရန် OpenGL / Cocoa Framework ကို သီးသန့် ချိတ်ပေးခြင်း
if(APPLE)
    target_link_libraries(appQtMirrorApp PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()