#ifndef DOMAIN_H
#define DOMAIN_H

#include <QFrame>
#include <QPushButton>
#include <QTimer>
#include <QGridLayout>
#include "global_defs.h"
#include "media_source.h"
#include "multicast_group.h"
#include "TMM_StreamCtrl.h"
#include "sharedmemory.h"

class Domain : public QFrame
{
    Q_OBJECT

public:

    QGridLayout Layout;
    QPushButton Domain1;
    QPushButton Domain2;
    QPushButton Domain3;
    SharedMemory<TMM_CntlBuffer>& ctrl_buffer;

    Domain(SharedMemory<TMM_CntlBuffer>& ctrl_buffer_);

    virtual ~Domain();


public slots:

    void Domain_changed();

};

#endif // DOMAIN_H
