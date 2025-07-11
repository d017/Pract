#include "infotable.h"
#include <QMessageBox>

InfoTable::InfoTable(QWidget *parent, QVector<infoTable> it)
    : QWidget(parent)
{
    this->currentInfoTables = it;
    setupUI();
    setPlayerData(currentInfoTables.size()-1);
}

InfoTable::~InfoTable()
{
}

void InfoTable::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Заголовок страницы
    titleLabel = new QLabel("Information Table", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #333; margin-bottom: 20px; }");




    playerTable = new QTableWidget(0, 6, this);


    QStringList headers;
    headers << "Игрок" << "Об. фабрики" << "Авт. фабрики" << "Деньги" << "ЕС" << "ГП";
    playerTable->setHorizontalHeaderLabels(headers);

    // Настраиваем размеры столбцов
    playerTable->horizontalHeader()->setStretchLastSection(true);
    playerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Запрещаем редактирование
    playerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Настраиваем выделение
    playerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    playerTable->setSelectionMode(QAbstractItemView::SingleSelection);



    // Стилизация таблицы
    playerTable->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: #d0d0d0;"
        "    background-color: #f9f9f9;"
        "    alternate-background-color: #f0f0f0;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "    border-bottom: 1px solid #d0d0d0;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: #3daee9;"
        "    color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: #e0e0e0;"
        "    padding: 8px;"
        "    border: 1px solid #d0d0d0;"
        "    font-weight: bold;"
        "}"
        );



    // Включаем чередование цветов строк
    playerTable->setAlternatingRowColors(true);

    // Создаем кнопки навигации
    backButton = new QPushButton("НАЗАД", this);
    nextButton = new QPushButton("ДАЛЬШЕ", this);

    // Стилизация кнопок
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    padding: 10px 20px;"
        "    border: none;"
        "    border-radius: 5px;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3e8e41;"
        "}";

    backButton->setStyleSheet(buttonStyle);
    nextButton->setStyleSheet(buttonStyle);

    // Создаем layout для кнопок
    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch(); // Добавляем растягивающий элемент между кнопками
    buttonLayout->addWidget(nextButton);

    // Добавляем элементы в главный layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(playerTable);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Подключаем сигналы кнопок
    connect(backButton, &QPushButton::clicked, this, &InfoTable::backClicked);
    connect(nextButton, &QPushButton::clicked, this, &InfoTable::nextClicked);
}

void InfoTable::setPlayerData(int month)
{
    currentInfoTable = currentInfoTables[month];
    currentMonth = month;
    updateDisplay();
}

void InfoTable::updateDisplay()
{
    // Очищаем таблицу
    playerTable->setRowCount(0);
    // Если массив пуст, отображаем пустую таблицу
    if (currentInfoTable.playersTable.empty()) {
        titleLabel->setText("Нет информации");
        return;
    }
    QString info = "Информация за " + QString::number(currentMonth + 1) + "-ый месяц";
    titleLabel->setText(info);
    // Обновляем заголовок с количеством элементов
    // Устанавливаем количество строк равное размеру массива
    playerTable->setRowCount(currentInfoTable.playersTable.size());

    // Заполняем таблицу данными
    for (size_t i = 0; i < currentInfoTable.playersTable.size(); ++i) {
        const playerInfo& player = currentInfoTable[i];

        // Столбец 0: Player ID (номер игрока)
        playerTable->setItem(i, 0, new QTableWidgetItem(QString("Игрок %1").arg(i + 1)));

        // Столбец 1: Common Factories
        playerTable->setItem(i, 1, new QTableWidgetItem(QString::number(player.commonFactories)));

        // Столбец 2: Auto Factories
        playerTable->setItem(i, 2, new QTableWidgetItem(QString::number(player.autoFactories)));

        // Столбец 3: Balance
        playerTable->setItem(i, 3, new QTableWidgetItem(QString::number(player.balance)));

        // Столбец 4: Raw
        playerTable->setItem(i, 4, new QTableWidgetItem(QString::number(player.raw)));

        // Столбец 5: Production
        playerTable->setItem(i, 5, new QTableWidgetItem(QString::number(player.prod)));

        // Выравниваем текст в ячейках
        for (int col = 0; col < 6; ++col) {
            QTableWidgetItem *item = playerTable->item(i, col);
            if (item) {
                if (col == 0) {
                    // Player ID - центрировано и жирным шрифтом
                    item->setTextAlignment(Qt::AlignCenter);
                    item->setFont(QFont("Arial", 10, QFont::Bold));
                } else {
                    // Числовые значения - центрировано
                    item->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }
}

void InfoTable::nextClicked() {
    if (currentMonth + 1 == currentInfoTables.size())
        QMessageBox::warning(this, "Ошибка", "Это последняя таблица");
    else
        setPlayerData(currentMonth + 1);
}

void InfoTable::backClicked() {
    if (currentMonth == 0)
        QMessageBox::warning(this, "Ошибка", "Это первая таблица");
    else
        setPlayerData(currentMonth - 1);
}
