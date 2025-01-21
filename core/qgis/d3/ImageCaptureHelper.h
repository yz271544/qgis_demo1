//
// Created by etl on 1/21/25.
//

#ifndef IMAGECAPTUREHELPER_H
#define IMAGECAPTUREHELPER_H



class ImageCaptureHelper : public QObject
{
    Q_OBJECT

public:
    ImageCaptureHelper(QgsOffscreen3DEngine* engine, Qgs3DMapScene* scene, const QString& savePath)
        : m_engine(engine), m_scene(scene), m_savePath(savePath)
    {
        // 连接 imageCaptured 信号
        connect(m_engine, &QgsOffscreen3DEngine::imageCaptured, this, &ImageCaptureHelper::onImageCaptured);
    }

    void captureImage()
    {
        // 请求捕获图像
        m_engine->requestCaptureImage();
    }

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
