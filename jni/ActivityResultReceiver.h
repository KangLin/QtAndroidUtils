/**
 * @note   THIS IS PRIVATE CLASS， YOU CAN NOT USE IT.
 * @brief  Android Directory
 * @author Kang Lin (kl222@!26.com) 
 */

#ifndef ACTIVITYRESULTRECEIVER_H
#define ACTIVITYRESULTRECEIVER_H

#pragma once
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    #include <QtCore/private/qandroidextras_p.h>
#else
    #include <QAndroidActivityResultReceiver>
#endif
#include <AndroidUtils.h>

// @note THIS IS PRIVATE CLASS， YOU CAN NOT USE IT.
class CActivityResultReceiver : public QAndroidActivityResultReceiver
{    
public:
    CActivityResultReceiver(CAndroidUtils* pUtils);
    virtual void handleActivityResult(int receiverRequestCode,
                                      int resultCode,
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                      const QJniObject &data
#else
                                      const QAndroidJniObject &data
#endif
                                      );

    enum RESULT_CODE
    {
        RESULT_CODE_PHOTO,
        RESULT_CODE_CAMERA,
        RESULT_CODE_PHONE,
        RESULT_CODE_INSTALL,
        RESULT_CODE_UNINSTALL
    };
    
private:
    CAndroidUtils* m_pUtils;
};

#endif // ACTIVITYRESULTRECEIVER_H
