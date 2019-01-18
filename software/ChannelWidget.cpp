#include <QDial>
#include <QSettings>
#include <QGridLayout>
#include "PopupButton.h"
#include "ChannelOptions.h"
#include "ChannelWidget.h"

ChannelWidget::ChannelWidget(const QString &title, QWidget *parent)
    : QGroupBox(parent),
      m_title(title),
      m_couplingButton(new PopupButton),
      m_probeButton(new PopupButton),
      m_thresholdDial(new QDial)
{
    m_thresholdDial->setRange(ChannelOptions::min(), ChannelOptions::max());
    connect(m_thresholdDial, &QDial::valueChanged, this, &ChannelWidget::updateWidget);
    connect(m_thresholdDial, &QDial::valueChanged, this, &ChannelWidget::optionsChanged);

    m_couplingButton->appendData(tr("DC"), ChannelOptions::DcCoupling);
    m_couplingButton->appendData(tr("AC"), ChannelOptions::AcCoupling);
    connect(m_couplingButton, &PopupButton::currentDataChanged, this, &ChannelWidget::updateWidget);
    connect(m_couplingButton, &PopupButton::currentDataChanged, this, &ChannelWidget::optionsChanged);

    m_probeButton->appendData("1:1", x1Probe);
    m_probeButton->appendData("1:10", x10Probe);
    m_probeButton->appendData("1:100", x100Probe);
    connect(m_probeButton, &PopupButton::currentDataChanged, this, &ChannelWidget::updateWidget);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_thresholdDial, 0, 1, 3, 3);
    layout->addWidget(m_couplingButton, 0, 0);
    layout->addWidget(m_probeButton, 2, 0);

    updateWidget();
}

ChannelWidget::Probe ChannelWidget::probe() const
{
    return ChannelWidget::Probe(m_probeButton->currentData().toInt());
}

ChannelOptions ChannelWidget::options() const
{
    return ChannelOptions(ChannelOptions::Coupling(m_couplingButton->currentData().toInt()), m_thresholdDial->value());
}

void ChannelWidget::saveToSettings(QSettings &settings)
{
    settings.setValue("Threshold", m_thresholdDial->value());
    settings.setValue("Coupling", m_couplingButton->currentData());
    settings.setValue("Probe", m_probeButton->currentData());
}

void ChannelWidget::restoreFromSettings(const QSettings &settings)
{
    m_thresholdDial->setValue(settings.value("Threshold", (ChannelOptions::min() + ChannelOptions::max()) / 2).toInt());
    m_couplingButton->setCurrentData(settings.value("Coupling", ChannelOptions::DcCoupling));
    m_probeButton->setCurrentData(settings.value("Probe", x1Probe));
}

void ChannelWidget::updateWidget()
{
    const ChannelOptions opt = options();
    switch (probe())
    {
    case ChannelWidget::x1Probe:
        setTitle(tr("%1: %2V").arg(m_title).arg(opt.voltage(), 0, 'f', 2));
        break;

    case ChannelWidget::x10Probe:
        setTitle(tr("%1: %2V").arg(m_title).arg(10.0 * opt.voltage(), 0, 'f', 1));
        break;

    case ChannelWidget::x100Probe:
        setTitle(tr("%1: %2V").arg(m_title).arg(100.0 * opt.voltage(), 0, 'f', 0));
        break;
    }
}
