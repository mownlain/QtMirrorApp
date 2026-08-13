#include "screenstreamer.h"
#include <QPixmap>
#include <QBuffer>

ScreenStreamer::ScreenStreamer(LiveImageProvider *provider, QObject *parent)
    : QObject(parent), m_imageProvider(provider) {

    m_timer = new QTimer(this);
    m_udpSocket = new QUdpSocket(this);

    connect(m_timer, &QTimer::timeout, this, &ScreenStreamer::captureAndSendFrame);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &ScreenStreamer::processIncomingDatagrams);
}

void ScreenStreamer::startStreaming(const QString &targetIp) {
    m_targetIp = targetIp;
    m_timer->start(33); // ~30 FPS
}

void ScreenStreamer::stopStreaming() {
    m_timer->stop();
}

void ScreenStreamer::captureAndSendFrame() {
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen || m_targetIp.isEmpty()) return;

    QPixmap pixmap = screen->grabWindow(0);
    QImage img = pixmap.toImage().scaled(854, 480, Qt::KeepAspectRatio, Qt::FastTransformation);

    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "JPG", 30); // Quality 30%

    // UDP Safe Payload Limit Check (60 KB ထက် ငယ်မှ ပို့မည်)
    if (bytes.size() > 60000) {
        bytes.clear();
        buffer.seek(0);
        img.save(&buffer, "JPG", 20);
    }

    if (bytes.size() <= 65500) {
        m_udpSocket->writeDatagram(bytes, QHostAddress(m_targetIp), m_port);
    }
}

void ScreenStreamer::startReceiving() {
    if (m_udpSocket->state() == QAbstractSocket::BoundState) {
        m_udpSocket->close();
    }
    m_udpSocket->bind(QHostAddress::Any, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void ScreenStreamer::processIncomingDatagrams() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        QImage img;
        if (img.loadFromData(datagram, "JPG")) {
            if (m_imageProvider) {
                m_imageProvider->updateImage(img);
                emit frameUpdated();
            }
        }
    }
}

QString ScreenStreamer::getLocalIp() const {
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {

            for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    return entry.ip().toString();
                }
            }
        }
    }
    return "127.0.0.1";
}