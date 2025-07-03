#ifndef STARTGAMESCREEN_H
#define STARTGAMESCREEN_H

#include <QWidget>

class StartGameScreen : public QWidget
{
    Q_OBJECT

public:
    StartGameScreen(QWidget *parent = nullptr);
    ~StartGameScreen();

signals:
    void GameLoaded(QString filename);
    void GameInitialized();
private slots:
    void onStartButtonPressed();
    void onLoadButtonPressed();

};


#endif // STARTGAMESCREEN_H
