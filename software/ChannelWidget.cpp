#include <QDial>
#include <QGridLayout>
#include "PopupButton.h"
#include "ChannelOptions.h"
#include "ChannelWidget.h"

ChannelWidget::ChannelWidget(const QString &title, QWidget *parent)
    : QGroupBox(parent),
      m_title(title),
      m_couplingButtons(new PopupButton),
      m_probeButtons(new PopupButton),
      m_thresholdDial(new QDial)
{
    m_thresholdDial->setRange(ChannelOptions::min(), ChannelOptions::max());
    connect(m_thresholdDial, &QDial::valueChanged, this, &ChannelWidget::updateWidget);
    connect(m_thresholdDial, &QDial::valueChanged, this, &ChannelWidget::optionsChanged);

    m_couplingButtons->addData(tr("DC"), ChannelOptions::DcCoupling);
    m_couplingButtons->addData(tr("AC"), ChannelOptions::AcCoupling);
    connect(m_couplingButtons, &PopupButton::currentDataChanged, this, &ChannelWidget::updateWidget);
    connect(m_couplingButtons, &PopupButton::currentDataChanged, this, &ChannelWidget::optionsChanged);

    m_probeButtons->addData("1:1", x1Probe);
    m_probeButtons->addData("1:10", x10Probe);
    m_probeButtons->addData("1:100", x100Probe);
    connect(m_probeButtons, &PopupButton::currentDataChanged, this, &ChannelWidget::updateWidget);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_thresholdDial, 0, 0, 2, 2);
    layout->addWidget(m_couplingButtons, 2, 0);
    layout->addWidget(m_probeButtons, 2, 1);

    updateWidget();
}

ChannelWidget::Probe ChannelWidget::probe() const
{
    return ChannelWidget::Probe(m_probeButtons->currentData().toInt());
}

void ChannelWidget::setProbe(ChannelWidget::Probe probe)
{
    m_probeButtons->setCurrentData(probe);
}

ChannelOptions ChannelWidget::options() const
{
    return ChannelOptions(ChannelOptions::Coupling(m_couplingButtons->currentData().toInt()), m_thresholdDial->value());
}

void ChannelWidget::setOptions(const ChannelOptions &options)
{
    m_couplingButtons->setCurrentData(options.coupling);
    m_thresholdDial->setValue(options.threshold);
    updateWidget();
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
