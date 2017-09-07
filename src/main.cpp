#include <QPushButton>
#include <QApplication>
#include <QFrame>

#include "global_defs.h"
#include "sharedmemory.h"
#include "media_source.h"
#include "multicast_group.h"
#include "TMM_StreamCtrl.h"
#include "radio.h"
#include "shoutdown.h"
#include "domain.h"

#include <boost/program_options.hpp>
#include <iostream>




namespace po = boost::program_options;

int main(int argc, char **argv)
{




    QApplication app (argc, argv);

    std::string supported_names;
    supported_names = "Supported identities:\n";

    for (uint8_t k=0; k< MediaSource::size(); k++)
    {
        supported_names = supported_names +  std::to_string(uint32_t(k)) + ") " + MediaSource(k).getName() + "\n";
    }




      std::string ctrl_file_name(RootSwapName);
      uint16_t identity(0);
      bool initialise_swap(false);

  po::options_description description ("MyTool Usage");
  description.add_options ()
        ("ctrl_file", po::value<std::string > (&ctrl_file_name), (std::string("file used as persistent swap \n default=")+ctrl_file_name).c_str())
        ("re-init", po::value<bool> (&initialise_swap), "reset persistent swap back to default")
        ("identity,i", po::value<uint16_t > (&identity), supported_names.c_str())
        ("help,h", "Display this help message")
        ("version,v", "Display the version number");
  po::variables_map vm;
  po::store (po::command_line_parser (argc, argv).options (description).run (), vm);
  po::notify (vm);



  if (vm.count ("help"))
    {
      std::cout << description;
      return -1;

    }

  if (vm.count ("version"))
    {
      std::cout << "MyTool Version 1.0" << std::endl;
      return -1;
    }

  if (identity>=MediaSource::size()) return -1;

    MediaSource me(identity);
    SharedMemory<TMM_CntlBuffer> ctrl_buffer(ctrl_file_name.c_str(),initialise_swap); //publish the buffer out to the world


    QFrame frame;
    frame.setFrameShadow(QFrame::Raised);
    frame.setFrameShape(QFrame::Box);

    Radio radio1(&frame,me,MediaSource("Radio 1"),MulticastGroup("Radio 1"),ctrl_buffer);
    Radio radio2(&frame,me,MediaSource("Radio 2"),MulticastGroup("Radio 2"),ctrl_buffer);
    Domain domain(&frame,ctrl_buffer);
    Shoutdown shoutdown(&frame,me,MulticastGroup("Shoutdown"),ctrl_buffer);

    //grid is domain/2 .... domain/2+domain .... domain/2+domain + radio1

    int collumn1 = domain.width()/2;
    int collumn2 = collumn1 + domain.width();
    int collumn3 = collumn2 + radio1.width();
    int collumn4 = collumn3 + radio2.width() + collumn1;
    int row_1 = shoutdown.height();
    int row_3 = row_1 + radio1.height();
    int row_4 = row_1 + domain.height() + shoutdown.height();

    domain.move(collumn1,row_1);
    radio1.move(collumn2,row_1);
    radio2.move(collumn3,row_1);

    shoutdown.move(collumn2,row_3);
    frame.setFixedSize(collumn4,row_4);



    frame.show();

    return app.exec();
}
