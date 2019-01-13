#include <QLabel>
#include <QFontDatabase>
#include <QBoxLayout>
#include "DeviceDisplay.h"
#include "DeviceDisplayWidget.h"

DeviceDisplayWidget::DeviceDisplayWidget(DeviceDisplay *display, QWidget *parent)
    : QWidget(parent),
      m_display(display),
      m_firstLabel(new QLabel),
      m_secondLabel(new QLabel)

{
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    const int s = font.pointSize();

    font.setPointSize(3 * s);
    m_firstLabel->setFont(font);
    m_firstLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_firstLabel->setText("000.000Hz");

    font.setPointSize(2 * s);
    m_secondLabel->setFont(font);
    m_secondLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    m_secondLabel->setText("000.000S");

    QBoxLayout * const labelLayout = new QVBoxLayout;
    labelLayout->addWidget(m_firstLabel);
    labelLayout->addWidget(m_secondLabel);

    QBoxLayout * const layout = new QHBoxLayout(this);
    layout->addLayout(labelLayout);

    updateDeviceDisplayWidget();
}

void DeviceDisplayWidget::updateDeviceDisplayWidget()
{

}

