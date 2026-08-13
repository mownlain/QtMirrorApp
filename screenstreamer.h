#ifndef SCREENSTREAMER_H
#define SCREENSTREAMER_H

#include <QObject>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include "imageprovider.h"

class ScreenStreamer : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString localIp READ getLocalIp CONSTANT)

public:
    explicit ScreenStreamer(LiveImageProvider *provider, QObject *parent = nullptr);
    ~ScreenStreamer() override = default;

    Q_INVOKABLE void startStreaming(const QString &targetIp);
    Q_INVOKABLE void stopStreaming();
    Q_INVOKABLE void startReceiving();
    QString getLocalIp() const;

signals:
    void frameUpdated();

private slots:
    void captureAndSendFrame();
    void processIncomingDatagrams();

private:
    QTimer *m_timer = nullptr;
    QUdpSocket *m_udpSocket = nullptr;
    LiveImageProvider *m_imageProvider = nullptr;
    QString m_targetIp;
    quint16 m_port = 12345;
};

#endif // SCREENSTREAMER_H