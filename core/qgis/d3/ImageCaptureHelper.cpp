//
// Created by etl on 1/21/25.
//

#include "ImageCaptureHelper.h"


ImageCaptureHelper::ImageCaptureHelper(QgsOffscreen3DEngine* engine, Qgs3DMapScene* scene, const QString& savePath)
        : m_engine(engine), m_scene(scene), m_savePath(savePath)
{
    // 连接 imageCaptured 信号
    connect(m_engine, &QgsOffscreen3DEngine::imageCaptured, this, &ImageCaptureHelper::onImageCaptured);
}

void ImageCaptureHelper::captureImage()
{
    // 请求捕获图像
    m_engine->requestCaptureImage();
}