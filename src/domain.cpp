#include "domain.h"
#include "global_defs.h"
#include "sharedmemory.h"

#include <stdlib.h>
#include <QString>

const uint32_t Domain1_Mask = 1;
const uint32_t Domain2_Mask = 2;
const uint32_t Domain3_Mask = 4;

Domain::Domain(QWidget* parent, SharedMemory<TMM_CntlBuffer>& ctrl_buffer_):QFrame(parent), ctrl_buffer(ctrl_buffer_)
{
    Domain1.setCheckable(true);
    Domain2.setCheckable(true);
    Domain3.setCheckable(true);
//    Domain1.setChecked( !(ctrl_buffer->domain_mask & Domain1_Mask));
//    Domain2.setChecked( !(ctrl_buffer->domain_mask & Domain2_Mask));
//    Domain3.setChecked( !(ctrl_buffer->domain_mask & Domain3_Mask));

    Domain1.setText("plaintext");
    Domain2.setText("restricted");
    Domain3.setText("classified");
    Domain1.setParent(this);
    Domain2.setParent(this);
    Domain3.setParent(this);
    Domain1.setGeometry(::width/2,(::height*1)/2, ::width,::height);
    Domain2.setGeometry(::width/2,(::height*4)/2, ::width,::height);
    Domain3.setGeometry(::width/2,(::height*7)/2, ::width,::height);
    setFixedSize(2*::width, (10 * ::height)/2);
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);

    connect(&Domain1,SIGNAL(toggled(bool)),this,SLOT(Domain_changed()));
    connect(&Domain2,SIGNAL(toggled(bool)),this,SLOT(Domain_changed()));
    connect(&Domain3,SIGNAL(toggled(bool)),this,SLOT(Domain_changed()));
    Domain_changed();
}

Domain::~Domain()
{

}



void Domain::Domain_changed()
{
    QPalette  palette;

    uint32_t state=0;

    if (!Domain1.isChecked())
    {
        state = state | Domain1_Mask;
        palette.setColor(QPalette::ButtonText,Qt::black);    //i dont care about it
    }
    else
    {
        palette.setColor(QPalette::ButtonText,Qt::darkGray);    //i dont care about it
    }

    Domain1.setPalette(palette);


    if (!Domain2.isChecked())
    {
        state = state | Domain2_Mask;
        palette.setColor(QPalette::ButtonText,Qt::black);    //i dont care about it
    }
    else
    {
        palette.setColor(QPalette::ButtonText,Qt::darkGray);    //i dont care about it
    }

    Domain2.setPalette(palette);

    if (!Domain3.isChecked())
    {
        state = state | Domain3_Mask;
        palette.setColor(QPalette::ButtonText,Qt::black);    //i dont care about it
    }
    else
    {
        palette.setColor(QPalette::ButtonText,Qt::darkGray);    //i dont care about it
    }

    Domain3.setPalette(palette);

//    ctrl_buffer->domain_mask = state;

}
