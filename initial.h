#ifndef INITIAL_H
#define INITIAL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QMessageBox>
#include <QFileDialog>

class Initial : public QWidget
{
    Q_OBJECT

public:
    explicit Initial(QWidget *parent = nullptr);
    ~Initial();

signals:
    void startClicked();                    // Сигнал для кнопки "Начать"
    void loadClicked(QString filename); // Сигнал для кнопки "Загрузить" с выбранной опцией
    void rulesCalled();

private slots:
    void handleStartButton();
    void handleLoadButton();
    void handleRulesButton();

private:
    void setupUI();
    //void setupLoadOptions();

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QPushButton *startButton;
    QPushButton *loadButton;
    //QComboBox *loadComboBox;
    QPushButton* rulesButton;

    //QStringList loadOptions;


};

#endif // INITIAL_H
