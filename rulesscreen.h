#ifndef RULESSCREEN_H
#define RULESSCREEN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class RulesScreen : public QWidget
{
    Q_OBJECT
public:
    explicit RulesScreen(QWidget *parent = nullptr);
private:
    QVBoxLayout* mainLayout;
    QPushButton* backButton;
signals:
    void back();
private slots:
    void backButtonPressed();
};

#endif // RULESSCREEN_H
