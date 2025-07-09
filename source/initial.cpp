#include "initial.h"

#include <QFileInfo>
#include <QGridLayout>
#include <QDebug>


Initial::Initial(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupLoadOptions();
}

Initial::~Initial()
{
}

void Initial::setupUI()
{
    mainLayout = new QVBoxLayout(this);


    setStyleSheet(
        "Initial {"
        "    background-color: #1a1a2e;"
        "    background-image: url('');"
        "}"
        );


    titleLabel = new QLabel("ЭКОНОМИЧЕСКАЯ СТРАТЕГИЯ", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 48px;"
        "    font-weight: bold;"
        "    color: #111111;"
        "    background-color: transparent;"
        "    padding: 40px 20px 20px 20px;"
        "    text-shadow: 2px 2px 0px #003d14;"
        "    letter-spacing: 3px;"
        "}"
        );



    // Подзаголовок
    subtitleLabel = new QLabel("Выберите действие для начала", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 24px;"
        "    color: #7f8c8d;"
        "    margin-bottom: 30px;"
        "}"
        );



    // Создаем кнопки
    startButton = new QPushButton("НАЧАТЬ", this);
    loadButton = new QPushButton("ЗАГРУЗИТЬ", this);



    // Стилизация кнопок
    QString buttonStyle =
        "QPushButton {"
        "    font-family: 'Courier New', monospace;"
        "    font-size: 30px;"
        "    font-weight: bold;"
        "    color: #000000;"
        "    background-color: transparent;"
        "    border: none;"
        "    padding: 15px 20px;"
        "    text-align: left;"
        "    min-height: 30px;"
        "}"
        "QPushButton:hover {"
        "    color: #00ff41;"
        "    background-color: #2a3f5f;"
        "}"
        "QPushButton:pressed {"
        "    color: #ffffff;"
        "    background-color: #00ff41;"
        "}";




    startButton->setStyleSheet(buttonStyle);
    loadButton->setStyleSheet(buttonStyle);



    loadComboBox = new QComboBox(this);
    loadComboBox->setStyleSheet(
        "QComboBox {"
        "    background-color: white;"
        "    border: 2px solid #3498db;"
        "    border-radius: 5px;"
        "    padding: 8px;"
        "    font-size: 14px;"
        "    min-width: 200px;"
        "    min-height: 35px;"
        "}"
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #3498db;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url(down_arrow.png);"
        "    width: 12px;"
        "    height: 12px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 1px solid #3498db;"
        "    background-color: white;"
        "    selection-background-color: #3498db;"
        "    selection-color: white;"
        "}"
        );
    loadComboBox->hide(); // Скрыто по умолчанию

    // Создаем layout для кнопок
    buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(startButton);
    buttonLayout->addSpacing(30);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addStretch();

    // Добавляем элементы в главный layout
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(loadComboBox);
    mainLayout->addStretch();

    // Устанавливаем фоновый цвет для всей страницы
    setStyleSheet("Initial { background-color: #f8f9fa; }");

    setLayout(mainLayout);

    // Подключаем сигналы кнопок
    connect(startButton, &QPushButton::clicked, this, &Initial::handleStartButton);
    connect(loadButton, &QPushButton::clicked, this, &Initial::handleLoadButton);
}


void Initial::setupLoadOptions()
{
    // Добавляем опции для загрузки
    loadOptions << "Сохранение 1" << "Сохранение 2" << "Сохранение 3"
                << "Последняя игра" << "Загрузить из файла...";

    loadComboBox->addItems(loadOptions);
    loadComboBox->setCurrentIndex(0); // Устанавливаем первую опцию по умолчанию
}

void Initial::handleStartButton()
{
    // Просто отправляем сигнал для перехода на page1
    emit startClicked();
}

void Initial::handleLoadButton()
{
    // Показываем комбобокс для выбора опции загрузки
    loadComboBox->show();

    // Создаем модальный диалог для выбора
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Выбор опции загрузки");
    msgBox.setText("Выберите что загрузить:");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);

    // Добавляем комбобокс в диалог
    QComboBox *dialogCombo = new QComboBox(&msgBox);
    dialogCombo->addItems(loadOptions);
    dialogCombo->setCurrentIndex(loadComboBox->currentIndex());

    // Получаем layout диалога и добавляем комбобокс
    QGridLayout *layout = qobject_cast<QGridLayout*>(msgBox.layout());
    if (layout) {
        layout->addWidget(dialogCombo, 1, 1);
    }

    int result = msgBox.exec();

    if (result == QMessageBox::Ok) {
        QString selectedOption = dialogCombo->currentText();

        // Если выбрана опция "Загрузить из файла..."
        if (selectedOption == "Загрузить из файла...") {
            QString fileName = QFileDialog::getOpenFileName(
                this,
                "Выберите файл для загрузки",
                "",
                "Файлы сохранения (*.sav);;Все файлы (*.*)"
                );

            if (!fileName.isEmpty()) {
                selectedOption = QString("Файл: %1").arg(QFileInfo(fileName).fileName());
            } else {
                // Пользователь отменил выбор файла
                loadComboBox->hide();
                return;
            }
        }

        // Отправляем сигнал с выбранной опцией
        emit loadClicked(selectedOption);
    }

    // Скрываем комбобокс после выбора
    loadComboBox->hide();
}
