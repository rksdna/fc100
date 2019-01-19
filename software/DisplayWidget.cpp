#include <QLabel>
#include <QGridLayout>
#include <QFontDatabase>
#include "DisplayWidget.h"

DisplayWidget::DisplayWidget(const QString &title, QWidget *parent)
    : QGroupBox(title, parent),
      m_locale(QLocale::system()),
      m_firstLabel(new QLabel),
      m_secondLabel(new QLabel),
      m_firstUnitLabel(new QLabel),
      m_secondUnitLabel(new QLabel)
{
    m_locale.setNumberOptions(QLocale::NumberOptions());

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    const int fontSize = font.pointSize();

    font.setPointSize(6 * fontSize);
    m_firstLabel->setFont(font);
    m_firstLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    font.setPointSize(3 * fontSize);
    m_secondLabel->setFont(font);
    m_secondLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    font.setPointSize(2 * fontSize);
    m_firstUnitLabel->setFont(font);
    m_firstUnitLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_firstUnitLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    m_secondUnitLabel->setFont(font);
    m_secondUnitLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_secondUnitLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_firstLabel, 0, 0);
    layout->addWidget(m_firstUnitLabel, 0, 1);
    layout->addWidget(m_secondLabel, 1, 0);
    layout->addWidget(m_secondUnitLabel, 1, 1);
}

void DisplayWidget::display(Sample::Type type, qreal first, qreal second)
{
    switch (type)
    {
    case Sample::FrequencyType:
        setFirstLabel(first, 3, tr("Hz"));
        setSecondLabel(second, 3, tr("S"));
        break;

    case Sample::RpmType:
        setFirstLabel(first, 0, tr("RPM"));
        setSecondLabel(second, 3, tr("S"));
        break;

    case Sample::PeriodType:
        setFirstLabel(1E+9 * first, 3, tr("ns"));
        setSecondLabel(second, 3, tr("S"));
        break;

    case Sample::CountType:
        setFirstLabel(first, 0, tr(""));
        setSecondLabel(second, 3, tr("S"));
        break;

    case Sample::TimeType:
        setFirstLabel(1E+9 * first, 6, tr("ns"));
        clearSecondLabel(tr("S"));
        break;
    }
}

void DisplayWidget::setFirstLabel(qreal value, int precision, const QString &unit)
{
    m_firstLabel->setText(format(value, precision));
    m_firstUnitLabel->setText(unit);
}

void DisplayWidget::setSecondLabel(qreal value, int precision, const QString &unit)
{
    m_secondLabel->setText(format(value, precision));
    m_secondUnitLabel->setText(unit);
    m_secondLabel->setEnabled(true);
    m_secondUnitLabel->setEnabled(true);
}

void DisplayWidget::clearSecondLabel(const QString &unit)
{
    m_secondLabel->setText(tr("---"));
    m_secondUnitLabel->setText(unit);
    m_secondLabel->setEnabled(false);
    m_secondUnitLabel->setEnabled(false);
}

QString DisplayWidget::format(qreal value, int precision) const
{
    QString text = qIsFinite(value) ? m_locale.toString(value, 'f', precision) : tr("---");
    text.replace(m_locale.groupSeparator(), tr(" "));
    text.replace(m_locale.decimalPoint(), tr("."));

    return text;
}

