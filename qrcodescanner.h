#ifndef QRCODESCANNER_H
#define QRCODESCANNER_H

#include <QImage>
#include <QTimer>
#include <QObject>
#include <QVideoSink>
#include <QQmlEngine>
#include <QFutureWatcher>
#include "lib/zxing-cpp/core/src/ReadBarcode.h"

class QRCodeScanner : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int scanInterval READ scanInterval WRITE setScanInterval NOTIFY scanIntervalChanged FINAL)
    Q_PROPERTY(QVideoSink* videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged FINAL)
    Q_PROPERTY(QRect scanArea READ scanArea WRITE setScanArea NOTIFY scanAreaChanged FINAL)

public:
    explicit QRCodeScanner(QObject *parent = nullptr);

    QVideoSink* videoSink() const;
    void setVideoSink(QVideoSink* newVideoSink);

    int scanInterval() const;
    void setScanInterval(int newScanInterval);

    QRect scanArea() const;
    void setScanArea(const QRect& newScanArea);

signals:
    void videoSinkChanged();
    void scanIntervalChanged();
    void scanAreaChanged();
    void qrDecoded(const QString&);

public slots:
    void start();
    void stop();

private slots:
    void scanFrame();            // Called by the Timer, will request QtConcurrent to performScan()
    ZXing::Result performScan(); // Called by QtConcurrent, will get the scan Result
    void scanFinished();         // Called by the Future Watcher, emitting resultReady with the Result

private:
    QTimer m_timer;
    QRect m_scanArea;
    QImage m_lastFrame;
    int m_scanInterval;
    QPointer<QVideoSink> m_videoSink;
    QFutureWatcher<ZXing::Result> m_watcher;
    QString m_lastCode;
};

#endif // QRCODESCANNER_H
