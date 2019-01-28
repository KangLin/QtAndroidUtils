/*
 * THIS IS PRIVATE CLASS， YOU CAN NOT USE IT.
 */

// Author: KangLin(kl222@!26.com) 

#ifndef ACTIVITYRESULTRECEIVER_H
#define ACTIVITYRESULTRECEIVER_H

#include <QAndroidActivityResultReceiver>
#include <AndroidUtils.h>

class CActivityResultReceiver : public QAndroidActivityResultReceiver
{    
public:
    CActivityResultReceiver(CAndroidUtils* pUtils);
    virtual void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject &data);
        
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
