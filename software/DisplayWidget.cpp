#include <QLabel>
#include <QGridLayout>
#include <QFontDatabase>
#include "DisplayWidget.h"

DisplayWidget::DisplayWidget(const QString &title, QWidget *parent)
    : QGroupBox(title, parent),
      m_locale(QLocale::c()),
      m_firstLabel(new QLabel),
      m_secondLabel(new QLabel)
{
    m_locale.setNumberOptions(QLocale::NumberOptions());

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    const int fontSize = font.pointSize();

    font.setPointSize(3 * fontSize);
    m_firstLabel->setFont(font);
    m_firstLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_firstLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    font.setPointSize(2 * fontSize);
    m_secondLabel->setFont(font);
    m_secondLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_secondLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    QGridLayout * const layout = new QGridLayout(this);
    layout->addWidget(m_firstLabel, 0, 0);
    layout->addWidget(m_secondLabel, 1, 0);
}

void DisplayWidget::display(Sample::Type type, qreal first, qreal second)
{
    switch (type)
    {
    case Sample::FrequencyType:
        setFirstLabel(first, 3, tr("Hz"));
        clearSecondLabel();
        break;

    case Sample::RpmType:
        setFirstLabel(first, 0, tr("RPM"));
        clearSecondLabel();
        break;

    case Sample::PeriodType:
        setFirstLabel(1E+9 * first, 3, tr("ns"));
        clearSecondLabel();
        break;

    case Sample::TimeType:
        setFirstLabel(1E+9 * first, 3, tr("ns"));
        clearSecondLabel();
        break;

    default:
        clearFirstLabel();
        clearSecondLabel();
        break;
    }
}

void DisplayWidget::setFirstLabel(qreal value, int precision, const QString &unit)
{
    m_firstLabel->setText(tr("%1 %2").arg(qIsFinite(value) ? m_locale.toString(value, 'f', precision) : tr("---")).arg(unit));
    m_firstLabel->show();
}

void DisplayWidget::clearFirstLabel()
{
    m_firstLabel->hide();
}

void DisplayWidget::setSecondLabel(qreal value, int precision, const QString &unit)
{
    m_secondLabel->setText(tr("%1 %2").arg(qIsFinite(value) ? m_locale.toString(value, 'f', precision) : tr("---")).arg(unit));
    m_secondLabel->show();
}

void DisplayWidget::clearSecondLabel()
{
    m_secondLabel->hide();
}

