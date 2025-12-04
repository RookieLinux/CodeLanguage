
cmake_minimum_required(VERSION 3.16)

project(RealtimeCustomPlotDemo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 组件
find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml Widgets PrintSupport)

# 启用 Qt 自动化工具 (这是关键!)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 或者使用 Qt6 的标准项目设置(推荐,包含上面三个设置)
qt_standard_project_setup()


function(collect_source_files DIRECTORY OUT_VAR)
    file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS "${DIRECTORY}/*.cpp")
    file(GLOB_RECURSE H_FILES CONFIGURE_DEPENDS "${DIRECTORY}/*.h")
    file(GLOB_RECURSE HPP_FILES CONFIGURE_DEPENDS "${DIRECTORY}/*.hpp")
    set(${OUT_VAR} ${CPP_FILES} ${H_FILES} ${HPP_FILES} PARENT_SCOPE)
endfunction()

set(ALL_FILES)
collect_source_files(${CMAKE_CURRENT_SOURCE_DIR} ALL_FILES)


# 添加可执行文件
#message(WARNING "All File In ${CMAKE_CURRENT_SOURCE_DIR}" ${ALL_FILES})
qt_add_executable(RealtimeCustomPlotDemo
#    ${ALL_FILES}
        "3rdparty/qcustomplot/baseplot.cpp"
        "3rdparty/qcustomplot/baseplot.h"
        "3rdparty/qcustomplot/RealTimePlot.cpp"
        "3rdparty/qcustomplot/RealTimePlot.h"
        "3rdparty/qcustomplot/qcustomplot.cpp"
        "3rdparty/qcustomplot/qcustomplot.h"
        "3rdparty/qcustomplot/axis.cpp"
        "3rdparty/qcustomplot/axis.h"
        "3rdparty/qcustomplot/graph.cpp"
        "3rdparty/qcustomplot/graph.h"
        "3rdparty/qcustomplot/grid.cpp"
        "3rdparty/qcustomplot/grid.h"
        "3rdparty/qcustomplot/ticker.cpp"
        "3rdparty/qcustomplot/ticker.h"
        "3rdparty/qcustomplot/macros.h"
        "datastreamer.cpp"
        "datastreamer.h"
        "main.cpp"
        "../qml.qrc"
)

set_target_properties(RealtimeCustomPlotDemo PROPERTIES OUTPUT_NAME "RealtimeCustomPlotApp")

target_include_directories(RealtimeCustomPlotDemo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/qcustomplot
)

# 添加 QML 模块(如果使用 qmlRegisterType)
qt_add_qml_module(RealtimeCustomPlotDemo
    URI RealtimeCustomPlotDemo
    VERSION 1.0
    QML_FILES ../qml/main.qml
    RESOURCE_PREFIX /
)

# 链接 Qt 库
target_link_libraries(RealtimeCustomPlotDemo PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Widgets
    Qt6::PrintSupport
)