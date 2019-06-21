#pragma once

#include <QWidget>

class QLineEdit;

class BrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserWidget(QWidget *parent = nullptr);

private:
    QLineEdit *m_lineEdit;
};
