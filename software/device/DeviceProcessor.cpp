#include <QtMath>
#include <QJSEngine>
#include "Sample.h"
#include "DeviceProcessor.h"

DeviceProcessor::DeviceProcessor(QObject *parent)
    : QObject(parent),
      m_size(100),
      m_sliding(true)
{
}

void DeviceProcessor::clear()
{
    m_samples.clear();
    updateCache(qQNaN());
}

void DeviceProcessor::take(qreal value)
{
    updateCache(value);
}

void DeviceProcessor::saveToSettings(QSettings &settings) const
{
}

void DeviceProcessor::restoreFromSettings(QSettings &settings)
{
}

void DeviceProcessor::updateCache(qreal absolute)
{
    const Sample::Type t = Sample::FrequencyType;
    const QString empty = tr("---");

    if (qIsFinite(absolute))
    {
        m_samples.append(absolute);

        const qreal relative = absolute - m_samples.first();
        emit absoluteChanged(Sample::toString(absolute, t));
        emit relativeChanged(Sample::toString(relative, t));
    }
    else
    {
        emit absoluteChanged(empty);
        emit relativeChanged(empty);
    }

    if (m_samples.isEmpty())
    {
        emit minChanged(empty);
        emit maxChanged(empty);
        emit bandChanged(empty);
        emit meanChanged(empty);
        emit standartDeviationChanged(empty);
        emit countChanged(empty);
        emit trendChanged(QVector<qreal>());
        emit barsChanged(QVector<qreal>());
    }
    else
    {
        qreal min = m_samples.value(0);
        qreal max = m_samples.value(0);
        qreal sum = 0.0;
        foreach (qreal sample, m_samples)
        {
            min = qMin(min, sample);
            max = qMax(max, sample);
            sum += sample;
        }

        emit minChanged(Sample::toString(min, t));
        emit maxChanged(Sample::toString(max, t));

        const qreal band = max - min;
        emit bandChanged(Sample::toString(band, t));

        const qreal mean = sum / m_samples.count();
        emit meanChanged(Sample::toString(mean, t));

        if (m_samples.count() > 1)
        {
            qreal sum2 = 0.0;
            foreach (qreal sample, m_samples)
                sum2 += qPow(sample - mean, 2);

            const qreal deviation = qSqrt(sum2 / (m_samples.count() - 1));
            emit standartDeviationChanged(Sample::toString(deviation, t));
        }
        else
        {
            emit standartDeviationChanged(empty);
        }

        const int k = 1 + qLn(m_samples.count()) / qLn(2);
        QVector<qreal> histogram(k);
        foreach (qreal sample, m_samples)
        {
            const int i = qBound(0, int(k * (sample - min) / (max - min)), k - 1);
            histogram[i] += 1;
        }

        emit countChanged(Sample::toString(m_samples.count(), Sample::NoType));
        emit trendChanged(m_samples);
        emit barsChanged(histogram);
    }
}

