#pragma once
#include "qcustomplot.h"

class QCPAxisRangeLimiter : public QObject
{
    Q_OBJECT

public:
    static QCPAxisRangeLimiter *instance() {
        static QCPAxisRangeLimiter ins;
        return &ins;
    }

    ~QCPAxisRangeLimiter()
    {
    }

    void bounded(QCPAxis *axis, QCPRange range)
    {
        if (axis == nullptr)
        {
            qDebug() << Q_FUNC_INFO << "QCPAxis pointer is zero";
            return;
        }
        if (m_map.find(axis) == m_map.end())
        {
            connect(axis, SIGNAL(destroyed(QObject*)), this, SLOT(axisDestroed(QObject*)));
            connect(axis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(rangeBound(QCPRange,QCPRange)));
        }
        axis->setRange(axis->range().bounded(range.lower, range.upper));
        m_map.insert(axis, range);
    }

    void unbounded(QCPAxis *axis)
    {
        if (axis == nullptr)
            return;
        if (m_map.find(axis) == m_map.end())
        {
            qDebug() << Q_FUNC_INFO << "Can't find" << axis << "in QCPAxisRangeLimiter";
            return;
        }
        disconnect(axis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(rangeBound(QCPRange,QCPRange)));
        m_map.remove(axis);
    }

    QCPRange boundedRange(QCPAxis *axis)
    {
        Q_ASSERT(m_map.find(axis) != m_map.end());
        return m_map[axis];
    }

private slots:
    void rangeBound(const QCPRange &newRange, const QCPRange &oldRange)
    {
        Q_UNUSED(oldRange)
        QCPAxis *axis = qobject_cast<QCPAxis *>(sender());
        QCPRange range = m_map[axis];
        axis->setRange(newRange.bounded(range.lower, range.upper));
    }

    void axisDestroed(QObject *obj)
    {
        m_map.remove(static_cast<QCPAxis *>(obj));
    }

private:
    QCPAxisRangeLimiter() = default;
    QMap<QCPAxis *, QCPRange> m_map;

private:
    Q_DISABLE_COPY(QCPAxisRangeLimiter)
};
