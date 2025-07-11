#ifndef GameEnd_H
#define GameEnd_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class GameEnd : public QWidget
{
    Q_OBJECT

public:
    explicit GameEnd(QWidget *parent = nullptr);
    ~GameEnd();

    void setWinner(QVector<int> winners);  // Метод для установки победителя

signals:
    void playAgainClicked();    // Сигнал для кнопки "Играть снова"
    void exitClicked();         // Сигнал для кнопки "Выход"

private:
    void setupUI();
    void updateDisplay();

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QLabel *titleLabel;
    QLabel *winnerLabel;
    QPushButton *playAgainButton;
    QPushButton *exitButton;

    QString currentWinner;
};

#endif // GameEnd_H
