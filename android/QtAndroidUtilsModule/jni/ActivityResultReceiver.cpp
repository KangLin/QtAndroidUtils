// Author: KangLin(kl222@!26.com) 

#include "ActivityResultReceiver.h"
#include <QtDebug>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

CActivityResultReceiver::CActivityResultReceiver(CAndroidUtils* pUtils)
{
    Q_ASSERT(pUtils);
    m_pUtils = pUtils;
}

void CActivityResultReceiver::handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data)
{
    qDebug() << "handleActivityResult, requestCode - " << receiverRequestCode
          << " resultCode - " << resultCode
          << " data - " << data.toString();
    
    switch (receiverRequestCode) {
    case RESULT_CODE_PHOTO:
    {
        if(!data.isValid())
            break;
        jint nResultCode =
                QAndroidJniObject::getStaticField<jint>
                ("com.dmcbig.mediapicker.PickerConfig", "RESULT_CODE");
        if(((int) nResultCode) != resultCode)
            break;
        QAndroidJniObject obResultExtra = 
           QAndroidJniObject::getStaticObjectField<jstring>
           ("com.dmcbig.mediapicker.PickerConfig", "EXTRA_RESULT");
        QAndroidJniObject objSel = data.callObjectMethod("getParcelableArrayListExtra",
                              "(Ljava/lang/String;)Ljava/util/ArrayList;",
                              obResultExtra.object<jstring>());
        int selectNumber = objSel.callMethod<jint>("size");
        //qDebug() << "seelct: " << selectNumber << " files";
        QStringList lstPath;
        for(int i = 0; i < selectNumber; i++)
        {
            QAndroidJniObject media = objSel.callObjectMethod("get",
                                            "(I)Ljava/lang/Object;",
                                            i);
            QAndroidJniObject objPath = media.getObjectField<jstring>("path");
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
