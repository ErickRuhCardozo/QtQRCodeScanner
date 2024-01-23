#include <QVideoFrame>
#include <QtConcurrent>
#include "qrcodescanner.h"

QRCodeScanner::QRCodeScanner(QObject *parent)
    : QObject{parent}
{
    m_timer.setInterval(2500);
    connect(&m_timer, &QTimer::timeout, this, &QRCodeScanner::scanFrame);
    connect(&m_watcher, &QFutureWatcher<ZXing::Result>::finished, this, &QRCodeScanner::scanFinished);
}

QVideoSink* QRCodeScanner::videoSink() const
{
    return m_videoSink;
}

void QRCodeScanner::setVideoSink(QVideoSink* newVideoSink)
{
    if (m_videoSink == newVideoSink)
        return;

    m_videoSink = newVideoSink;
    emit videoSinkChanged();
}

void QRCodeScanner::start()
{
    if (m_timer.isActive())
        return;

    m_timer.start();
}

void QRCodeScanner::stop()
{
    if (!m_timer.isActive())
        return;

    m_timer.stop();
}

int QRCodeScanner::scanInterval() const
{
    return m_scanInterval;
}

void QRCodeScanner::setScanInterval(int newScanInterval)
{
    if (m_scanInterval == newScanInterval)
        return;

    m_scanInterval = newScanInterval;
    m_timer.setInterval(newScanInterval);
    emit scanIntervalChanged();
}

void QRCodeScanner::scanFrame()
{
    if (m_videoSink.isNull() || m_watcher.isRunning())
        return;

    QVideoFrame frame(m_videoSink.data()->videoFrame());
    QImage image = frame.toImage();
    m_lastFrame = image.copy(m_scanArea);
    m_watcher.setFuture(QtConcurrent::run(&QRCodeScanner::performScan, this));
}

ZXing::Result QRCodeScanner::performScan()
{
    const uchar* data = m_lastFrame.bits();
    auto image = ZXing::ImageView(data, m_lastFrame.width(), m_lastFrame.height(), ZXing::ImageFormat::XRGB);
    auto options = ZXing::ReaderOptions().setFormats(ZXing::BarcodeFormat::QRCode);
    auto result = ZXing::ReadBarcode(image, options);
    return result;
}

void QRCodeScanner::scanFinished()
{
    ZXing::Result result = m_watcher.result();

    if (!result.isValid())
        return;

    QString text = QString::fromStdString(result.text());

    if (text == m_lastCode)
        return;

    m_lastCode = text;
    emit qrDecoded(text);
}

QRect QRCodeScanner::scanArea() const
{
    return m_scanArea;
}

void QRCodeScanner::setScanArea(const QRect& newScanArea)
{
    if (m_scanArea == newScanArea)
        return;

    m_scanArea = newScanArea;
    emit scanAreaChanged();
}
