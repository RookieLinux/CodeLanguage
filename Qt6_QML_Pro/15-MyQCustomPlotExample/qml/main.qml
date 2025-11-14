import QtQuick
import QtQuick.Controls
import RealtimePlot 1.0

ApplicationWindow {
    id: win
    width: 1000
    height: 600
    visible: true
    title: "Realtime QCustomPlot Stream Demo"

    RealtimePlot {
        id: realtimePlot
        objectName: "realtimePlot"
        anchors.fill: parent
        xMin: 0
        xMax: 5        // 显示窗口宽度（秒）
        yMin: -3
        yMax: 3
        xAxis.ticker.tickCount: 5
        yAxis.ticker.tickCount: 10
        yAxis.ticker.ticks: true
        yAxis.ticker.subTicks: false
        xAxis.ticker.ticks: true
        xAxis.ticker.subTicks: false
        width:200
        height:300
        maxBufferPoints: 12000
        refreshMs: 33  // 30 FPS
        backgroundColor: true ? Qt.rgba(39/255, 39/255, 39/255, 1) : "white"

    }

    // 控件：暂停/继续/清空/缩放
    Column {
        anchors.right: parent.right
        anchors.top: parent.top
        spacing: 8
        padding: 8
        Rectangle { width: 220; height: 8; color: "transparent" } // spacer
        Button { text: realtimePlot.paused ? "Resume" : "Pause"; onClicked: realtimePlot.paused = !realtimePlot.paused }
        Button { text: "Clear"; onClicked: realtimePlot.clearBuffer() }
        Row {
            spacing: 4
            Label { text: "xWindow(s)" }
            Button { text: "Zoom In"; onClicked: { realtimePlot.xMax = realtimePlot.xMin + (realtimePlot.xMax - realtimePlot.xMin)/2 } }
            Button { text: "Zoom Out"; onClicked: { realtimePlot.xMax = realtimePlot.xMin + (realtimePlot.xMax - realtimePlot.xMin)*2 } }
        }
    }
}
