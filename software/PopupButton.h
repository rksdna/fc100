#ifndef POPUPBUTTON_H
#define POPUPBUTTON_H

#include <QVariant>
#include <QToolButton>

class PopupButton : public QToolButton
{
    Q_OBJECT

public:
    explicit PopupButton(QWidget *parent = 0);

    void appendItem(const QString &text, const QVariant &data);

    QVariant data() const;
    void setData(const QVariant &value);

signals:
    void dataChanged();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    void onTriggered();

private:
    QVariant m_data;
};

#endif // POPUPBUTTON_H
