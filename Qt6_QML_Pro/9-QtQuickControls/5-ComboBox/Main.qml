import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("ComboBox")


    ColumnLayout{
        width: parent.width
        height: parent.height

        //Non Editable ComboBox

        Label{
            text: "Non Editable Combo"
            wrapMode: Label.Wrap
            Layout.fillWidth: true
        }

        ComboBox{
            id: nonEditableComboId
            model: ["One", "Two", "Three", "Four"]
            Layout.fillWidth: true
            onActivated: {
                console.log("[" + currentIndex + "] " + currentText + " is activated")
            }
        }

        //Editable ComboBox
        Label {
            text: "Editable Combo"
            wrapMode: Label.Wrap //自动换行
            Layout.fillWidth: true
        }

        ComboBox{
            id: editableComboId
            editable: true
            textRole: "text"
            Layout.fillWidth: true
            model: ListModel{
                id: model
                ListElement{
                    text: "Dog"
                    location: "Kigali"
                    favorite_food: "Meat"
                }
                ListElement{
                    text: "Chicken"
                    location: "Nairobi"
                    favorite_food: "Rice"
                }
                ListElement{
                    text: "Cat"
                    location: "Mumbai"
                    favorite_food: "Fish"
                }
            }

            onActivated: function(){ //确认编辑 -- 当用户从下拉列表中选择一个选项（激活条目）时触发
                console.log("[" + currentIndex + "] " + currentText + " is activated")
            }


            onAccepted: function(){ //激活输入选项 -- ComboBox处于editable:true时，当用户在编辑框里输入内容并按下Enter/Return（或等效的确认动作）时触发
                if(find(editText) === -1){
                    model.append({ text: editText, location: "US", favorite_food: "Apple"})
                }
            }
        }

        Button{
            text: "Captrue current element"
            Layout.fillWidth: true
            onClicked: function(){
                console.log( model.get(editableComboId.currentIndex).text + ", " + model.get(editableComboId.currentIndex).location)
            }
        }

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

    }


}
