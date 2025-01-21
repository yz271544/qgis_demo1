//
// Created by etl on 1/21/25.
//

#ifndef IMAGECAPTUREHELPER_H
#define IMAGECAPTUREHELPER_H
#include <qgs3dmapscene.h>
#include <qgsoffscreen3dengine.h>
#include <QObject>


class ImageCaptureHelper : public QObject
{
    Q_OBJECT

public:
    ImageCaptureHelper(QgsOffscreen3DEngine* engine, Qgs3DMapScene* scene, const QString& savePath);

    void captureImage();

signals:
    void imageSaved();

private slots:
    void onImageCaptured(const QImage& image)
    {
        qDebug() << "Image captured, size:" << image.size();
        bool saveResult = image.save(m_savePath);
        qDebug() << "Image saved:" << saveResult;
        deleteLater(); // 删除辅助对象
    }

private:
    QgsOffscreen3DEngine* m_engine;
    Qgs3DMapScene* m_scene;
    QString m_savePath;
};



#endif //IMAGECAPTUREHELPER_H
