import QtQuick
import QtQuick.Controls
import RealtimeCustomPlotDemo 1.0

ApplicationWindow {
    id: win
    width: 1000
    height: 600
    visible: true
    title: "Realtime QCustomPlot Stream Demo"
    RealTimePlot {
        id: realtimePlot
        objectName: "realtimePlot"
        // 移除固定宽高，改用 anchors 布局
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: controlsPanel.left // 锚定到右侧控制面板的左边
        maxBufferPoints: 12000
        refreshMs: 60  //FPS
        
        // 注意：initialXRange 赋值可能需要确认 C++ 接收的类型
        initialXRange : ({"lower":0, "upper":1.0})
        initialYRange : ({"lower":-2, "upper":2})
        backgroundColor: Qt.rgba(39/255, 39/255, 39/255, 1)
        baseColor:  "white"
        labelColor:  "white"
        Component.onCompleted: {
            addGraph("main");
            xAxis.visible = true
            yAxis.visible = true
            xAxis.ticker.tickCount =  2
            xAxis.ticker.ticks = true
            xAxis.ticker.subTicks = false
            yAxis.ticker.tickCount = 5
            yAxis.ticker.ticks = true
            yAxis.ticker.subTicks = false
        }
    }
    // 控件：暂停/继续/缩放
    Column {
        id: controlsPanel // 添加 ID 以供布局使用
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 8
        padding: 8
        Rectangle { width: 220; height: 8; color: "transparent" } // spacer
        Button { text: realtimePlot.paused ? "Resume" : "Pause"; onClicked: realtimePlot.paused = !realtimePlot.paused }
        Row {
            spacing: 4
            Label { text: "xWindow(s)" }
            Button {
                text: "Zoom In"
                onClicked: {
                    realtimePlot.xAxis.upper =
                        realtimePlot.xAxis.lower +
                        (realtimePlot.xAxis.upper - realtimePlot.xAxis.lower) / 2
                }
            }
            Button {
                text: "Zoom Out"
                onClicked: {
                    realtimePlot.xAxis.upper =
                        realtimePlot.xAxis.lower +
                        (realtimePlot.xAxis.upper - realtimePlot.xAxis.lower) * 2
                }
            }
        }
    }

}
