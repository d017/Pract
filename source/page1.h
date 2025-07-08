#ifndef PAGE1_H
#define PAGE1_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QFrame>


class Page1 : public QWidget
{
    Q_OBJECT

public:
    explicit Page1(QWidget *parent = nullptr);
    ~Page1();

signals:
    void nextClicked(int value);
private slots:
    void updatePlayerValueLabel(int value);
private:
    void setupUI();

    QVBoxLayout *layout;
    QLabel *label;
    QLabel *PlayerValueLabel;
    QPushButton *nextButton;
    QSlider *slider;
    QFrame *contentFrame;
    QVBoxLayout *contentLayout;
};

#endif // PAGE1_H
