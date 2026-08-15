import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: window
    width: 900
    height: 600
    visible: true
    title: qsTr("Qt Real-time Wi-Fi Screen Share")
    color: "#181825"

    property int currentTab: 0

    // ESC Key နှိပ်ပါက Receiver Screen မှ ပြန်ထွက်မည်
    Shortcut {
        sequence: "Esc"
        enabled: window.currentTab === 1
        onActivated: {
            window.visibility = Window.Windowed
            window.currentTab = 0
        }
    }

    // Main Control View
    RowLayout {
        anchors.fill: parent
        spacing: 0
        visible: window.currentTab === 0

        // Sidebar Navigation
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 220
            color: "#1E1E2E"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Text {
                    text: "QT MIRROR"
                    color: "#89B4FA"
                    font.bold: true
                    font.pixelSize: 20
                    Layout.alignment: Qt.AlignHCenter
                }

                // IP Address Badge
                Rectangle {
                    Layout.fillWidth: true
                    height: 42
                    color: "#313244"
                    radius: 8

                    ColumnLayout {
                        anchors.centerIn: parent
                        spacing: 2

                        Text {
                            text: "YOUR DEVICE IP"
                            color: "#A6ADC8"
                            font.pixelSize: 9
                            font.bold: true
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text {
                            text: (typeof screenStreamer !== "undefined" && screenStreamer !== null) ? screenStreamer.localIp : "Checking..."
                            color: "#A6E3A1"
                            font.bold: true
                            font.pixelSize: 13
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }

                Item { height: 10 }

                Button {
                    text: "💻 Cast Screen"
                    Layout.fillWidth: true
                    onClicked: window.currentTab = 0
                }

                Button {
                    text: "📺 Receive Screen"
                    Layout.fillWidth: true
                    onClicked: {
                        window.currentTab = 1
                        window.visibility = Window.FullScreen
                        if (typeof screenStreamer !== "undefined" && screenStreamer !== null) {
                            screenStreamer.startReceiving()
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // Sender Settings
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 20

                Text {
                    text: "Connect to Receiver PC/Mac"
                    color: "white"
                    font.pixelSize: 20
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                }

                TextField {
                    id: targetIpInput
                    placeholderText: "Enter Receiver IP (e.g. 192.168.1.50)"
                    Layout.preferredWidth: 320
                    horizontalAlignment: Text.AlignHCenter
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 10

                    Button {
                        text: "Start Mirroring"
                        onClicked: {
                            if (typeof screenStreamer !== "undefined" && screenStreamer !== null) {
                                screenStreamer.startStreaming(targetIpInput.text)
                            }
                        }
                    }

                    Button {
                        text: "Stop Mirroring"
                        onClicked: {
                            if (typeof screenStreamer !== "undefined" && screenStreamer !== null) {
                                screenStreamer.stopStreaming()
                            }
                        }
                    }
                }
            }
        }
    }

    // Fullscreen Receive View
    Rectangle {
        anchors.fill: parent
        color: "black"
        visible: window.currentTab === 1

        Rectangle {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 15
            width: 380
            height: 36
            color: "#CC313244"
            radius: 8
            z: 10

            Text {
                anchors.centerIn: parent
                text: "Your IP: " + ((typeof screenStreamer !== "undefined" && screenStreamer !== null) ? screenStreamer.localIp : "Checking...") + "  |  Press [ ESC ] to Exit"
                color: "#A6E3A1"
                font.bold: true
                font.pixelSize: 13
            }
        }

        Image {
            id: liveDisplay
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: ""

            Connections {
                target: (typeof screenStreamer !== "undefined" && screenStreamer !== null) ? screenStreamer : null

                function onFrameUpdated() {
                    liveDisplay.source = "image://live/screen?id=" + Math.random()
                }
            }
        }
    }
}