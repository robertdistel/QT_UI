#ifndef RADIO_H
#define RADIO_H

#include <QFrame>
#include <QPushButton>
#include <QTimer>
#include "media_source.h"
#include "multicast_group.h"
#include "sharedmemory.h"
#include "TMM_StreamCtrl.h"


class Radio : public QFrame
{

Q_OBJECT

public:

    QPushButton                     RX_Button;
    QPushButton                     TX_Button;
    QTimer                          StatusTimer;
    MediaSource                     theOperator;
    MediaSource                     theRadio;
    MulticastGroup                  theRadioGroup;
    SharedMemory<TMM_CntlBuffer>&   ctrl_buffer;

    Radio(QWidget* parent, MediaSource theOperator_, MediaSource theRadio_,MulticastGroup theRadioGroup_, SharedMemory<TMM_CntlBuffer>& ctrl_buffer_);

    virtual ~Radio();


public slots:

    void timer_update();
    void RX_toggled(bool state);
    void TX_pressed(void);
    void TX_released(void);

};


#endif // RADIO_H
