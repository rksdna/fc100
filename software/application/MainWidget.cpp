#include "Device.h"
#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QMainWindow(parent),
      m_device(new Device(this))
{


}
