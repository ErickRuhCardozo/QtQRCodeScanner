import QtCore
import QtQuick
import QtMultimedia
import QtQuick.Layouts
import QtQuick.Controls
import QRCodeScanner

ApplicationWindow {
    visible: true
    title: 'QR Code Scanner'

    CameraPermission { id: cameraPermission }

    MediaDevices { id: mediaDevices }

    CaptureSession {
        videoOutput: videoOutput
        camera: Camera {
            id: camera
            cameraDevice: mediaDevices.defaultVideoInput
            focusMode: Camera.FocusModeAuto
            onActiveChanged: {
                if (active)
                    scanner.start()
                else
                    scanner.stop()
            }
        }
    }

    QRCodeScanner {
        id: scanner
        scanInterval: 500
        videoSink: videoOutput.videoSink
        onQrDecoded: (qr) => resultText.text = qr
    }

    ColumnLayout {
        anchors { fill: parent; margins: 10 }

        Text {
            id: errorText
            Layout.fillWidth: true
            Layout.fillHeight: true
            font { bold: true; pointSize: 22 }
            color: 'red'
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            visible: false
        }

        VideoOutput {
            id: videoOutput
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Text {
            id: resultText
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            horizontalAlignment: Qt.AlignLeft
            wrapMode: Text.WrapAnywhere
            font { pointSize: 16 }
        }

        Button {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignBottom
            text: 'Scan'
            onClicked: {
                if (camera.active) {
                    camera.stop()
                } else {
                    cameraPermission.request()
                    camera.start()
                }
            }
        }
    }
}
