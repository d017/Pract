#ifndef PlayerCount_H
#define PlayerCount_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QFrame>


class PlayerCount : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerCount(QWidget *parent = nullptr);
    ~PlayerCount();

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

#endif // PlayerCount_H
