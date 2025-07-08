#ifndef PAGE3_H
#define PAGE3_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class Page3 : public QWidget
{
    Q_OBJECT

public:
    explicit Page3(QWidget *parent = nullptr);
    ~Page3();

    void setWinner(const QString& winner);  // Метод для установки победителя

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

#endif // PAGE3_H
