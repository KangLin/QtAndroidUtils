// Author: KangLin(kl222@!26.com)


#include <QLoggingCategory>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QJniObject>
#else
    #include <QAndroidJniObject>
    #include <QAndroidJniEnvironment>
#endif

#include "ActivityResultReceiver.h"

static Q_LOGGING_CATEGORY(log, "Receiver")

CActivityResultReceiver::CActivityResultReceiver(CAndroidUtils* pUtils)
{
    Q_ASSERT(pUtils);
    m_pUtils = pUtils;
}

void CActivityResultReceiver::handleActivityResult(
    int receiverRequestCode,
    int resultCode,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const QJniObject &data
#else
    const QAndroidJniObject &data
#endif
    )
{
    qDebug(log) << "handleActivityResult, requestCode - " << receiverRequestCode
          << " resultCode - " << resultCode
          << " data - " << data.toString();
    
    switch (receiverRequestCode) {
    case RESULT_CODE_PHOTO:
    case RESULT_CODE_CAMERA:
    {
        if(!data.isValid())
            break;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        jint nResultCode =
            QJniObject::getStaticField<jint>
            ("com.dmcbig.mediapicker.PickerConfig", "RESULT_CODE");
        if(((int) nResultCode) != resultCode)
            break;
        auto obResultExtra =
            QJniObject::getStaticObjectField<jstring>
            ("com.dmcbig.mediapicker.PickerConfig", "EXTRA_RESULT");
#else
        jint nResultCode =
                QAndroidJniObject::getStaticField<jint>
                ("com.dmcbig.mediapicker.PickerConfig", "RESULT_CODE");
        if(((int) nResultCode) != resultCode)
            break;
        QAndroidJniObject obResultExtra = 
           QAndroidJniObject::getStaticObjectField<jstring>
           ("com.dmcbig.mediapicker.PickerConfig", "EXTRA_RESULT");
#endif
        auto objSel = data.callObjectMethod("getParcelableArrayListExtra",
                              "(Ljava/lang/String;)Ljava/util/ArrayList;",
                              obResultExtra.object<jstring>());
        int selectNumber = objSel.callMethod<jint>("size");
        //qDebug() << "seelct: " << selectNumber << " files";
        QStringList lstPath;
        for(int i = 0; i < selectNumber; i++)
        {
            auto media = objSel.callObjectMethod("get",
                                            "(I)Ljava/lang/Object;",
                                            i);
            auto objPath = media.getObjectField<jstring>("path");
            //qDebug() << objPath.toString();
            lstPath << objPath.toString();
        }
        if(selectNumber > 0)
            m_pUtils->SelectPhotos(lstPath);
        break;
    }
    default:
        break;
    }
}
