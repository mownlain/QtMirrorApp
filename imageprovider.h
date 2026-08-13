#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class LiveImageProvider : public QQuickImageProvider {
public:
    LiveImageProvider() : QQuickImageProvider(QQuickImageProvider::Image) {}

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override {
        Q_UNUSED(id);
        QMutexLocker locker(&m_mutex);

        // Frame မရှိသေးပါက Error မတက်စေရန် Transparent Image အလွတ် ထုတ်ပေးမည်
        if (m_image.isNull()) {
            QImage emptyImg(1, 1, QImage::Format_ARGB32);
            emptyImg.fill(Qt::transparent);
            if (size) *size = emptyImg.size();
            return emptyImg;
        }

        if (size) *size = m_image.size();

        if (requestedSize.width() > 0 && requestedSize.height() > 0) {
            return m_image.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }

        return m_image;
    }

    void updateImage(const QImage &image) {
        QMutexLocker locker(&m_mutex);
        m_image = image;
    }

private:
    QImage m_image;
    QMutex m_mutex;
};

#endif // IMAGEPROVIDER_H