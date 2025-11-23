import QtQuick
import QtQuick.Controls

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("TextArea and ScrollView")

    Column{
        spacing: 40
        width: parent.width

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "TextArea is a multi-line text editor."
        }

        ScrollView{ //提供滚动功能的容器，当内容超出显示区域时会自动显示滚动条
            id: scrollViewId
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: 150
            TextArea{
                id: textAreaId
                font.pointSize: 15
                wrapMode: TextArea.WordWrap
                placeholderText: "Type in some text" //当输入框为空时显示的占位符文本
            }
        }

        Button{
            text: "Submit"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: function(){
                //console.log("The text is: " + textAreaId.text)
                textAreaId.text = "The sky is blue my friend!  The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! The sky is blue my friend! vvThe sky is blue my friend! "
            }
        }


    }

}
