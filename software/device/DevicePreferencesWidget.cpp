#include <QBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include "Device.h"
#include "DeviceFormatBox.h"
#include "DeviceController.h"
#include "CustomColorButton.h"
#include "DevicePreferencesWidget.h"

DevicePreferencesWidget::DevicePreferencesWidget(Device *device, QWidget *parent)
    : QWidget(parent),
      m_device(device),
      m_processorButton(new CustomColorButton),
      m_controllerButton(new CustomColorButton)
{
    DeviceProcessor * const processor = device->processor();
    DeviceController * const controller = device->controller();

    m_widgets.append(new DeviceFormatBox(tr("Number format"), processor, DeviceProcessor::NoType));
    m_widgets.append(new DeviceFormatBox(tr("Duty cycle format" ), processor, DeviceProcessor::DutyType));
    m_widgets.append(new DeviceFormatBox(tr("Time format"), processor, DeviceProcessor::TimeType));
    m_widgets.append(new DeviceFormatBox(tr("Frequency format"), processor, DeviceProcessor::FrequencyType));
    m_widgets.append(new DeviceFormatBox(tr("Custom format"), processor, DeviceProcessor::CustomType));

    m_processorButton->setColor(processor->color());
    m_controllerButton->setColor(controller->color());

    QGroupBox * const colorBox = new QGroupBox(tr("Colors"));
    QFormLayout * const colorLayout = new QFormLayout(colorBox);
    colorLayout->addRow(tr("Display"), m_processorButton);
    colorLayout->addRow(tr("Control"), m_controllerButton);

    QGridLayout * const groupsLayout = new QGridLayout;

    int index = 0;
    foreach (DeviceFormatBox *widget, m_widgets)
    {
        groupsLayout->addWidget(widget, index / 2, index % 2);
        index++;
    }

    groupsLayout->addWidget(colorBox, index / 2, index % 2);

    QBoxLayout * const layout = new QVBoxLayout(this);
    layout->addLayout(groupsLayout);
    layout->addStretch();
}

void DevicePreferencesWidget::accept()
{
    DeviceProcessor * const processor = m_device->processor();
    DeviceController * const controller = m_device->controller();

    foreach (DeviceFormatBox *widget, m_widgets)
        widget->accept();

    processor->setColor(m_processorButton->color());
    controller->setColor(m_controllerButton->color());
}
