#ifndef SHOUTDOWN_H
#define SHOUTDOWN_H

#include <QFrame>
#include <QPushButton>
#include <QTimer>
#include <QGridLayout>
#include "global_defs.h"
#include "media_source.h"
#include "multicast_group.h"
#include "TMM_StreamCtrl.h"
#include "sharedmemory.h"

class Shoutdown : public QFrame
{
    Q_OBJECT

public:

    QTimer      StatusTimer;
    QGridLayout                     Layout;

    QPushButton Button;

    MediaSource                 theOperator;
    MulticastGroup              theShoutdownGroup;
    SharedMemory<TMM_CntlBuffer>& ctrl_buffer;

    Shoutdown(MediaSource theOperator_, MulticastGroup   theShoutdownGroup_, SharedMemory<TMM_CntlBuffer>& ctrl_buffer_);

    virtual ~Shoutdown();


public slots:

    void timer_update();
    void pressed(void);
    void released(void);


};


#endif // SHOUTDOWN_H
