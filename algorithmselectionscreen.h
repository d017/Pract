#ifndef ALGORITHMSELECTIONSCREEN_H
#define ALGORITHMSELECTIONSCREEN_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QLineEdit>

class AlgorithmSelectionScreen : public QWidget
{
    Q_OBJECT
public:
    explicit AlgorithmSelectionScreen(int, QVector<QString>, QWidget *parent = nullptr);
    ~AlgorithmSelectionScreen();

signals:
    void CodeSelected(int index, QString code);

    void BotSelected(int index);

    void AlgorithmDenied(int index);
private:
    int index;

    QVector<QString> codes;

    QLineEdit* inputEdit;

private slots:
    void onBotButtonPressed();

    void onAlgorithmSubmitted();

    void onAlgorithmDenied();
};

#endif // ALGORITHMSELECTIONSCREEN_H
