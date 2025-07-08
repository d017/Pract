#include "infotable.h"

InfoTable::InfoTable(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

InfoTable::~InfoTable()
{
}

void InfoTable::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Заголовок страницы
    titleLabel = new QLabel("Player Information Table", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #333; margin-bottom: 20px; }");

    // Создаем таблицу с 6 столбцами
    playerTable = new QTableWidget(0, 6, this);  // 0 строк, 6 столбцов

    // Устанавливаем заголовки столбцов
    QStringList headers;
    headers << "Player ID" << "Common Factories" << "Auto Factories" << "Balance" << "Raw" << "Production";
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
    backButton = new QPushButton("BACK", this);
    nextButton = new QPushButton("NEXT", this);

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

void InfoTable::setPlayerData(infoTable& playerData)
{
    currentPlayerData = playerData;
    updateDisplay();
}

void InfoTable::updateDisplay()
{
    // Очищаем таблицу
    playerTable->setRowCount(0);

    // Если массив пуст, отображаем пустую таблицу
    if (currentPlayerData.empty()) {
        titleLabel->setText("Player Information Table (Empty)");
        return;
    }

    // Обновляем заголовок с количеством элементов
    titleLabel->setText(QString("Player Information Table (%1 players)").arg(currentPlayerData.size()));

    // Устанавливаем количество строк равное размеру массива
    playerTable->setRowCount(currentPlayerData.size());

    // Заполняем таблицу данными
    for (size_t i = 0; i < currentPlayerData.size(); ++i) {
        const playerInfo& player = currentPlayerData[i];

        // Столбец 0: Player ID (номер игрока)
        playerTable->setItem(i, 0, new QTableWidgetItem(QString("Player %1").arg(i + 1)));

        // Столбец 1: Common Factories
        playerTable->setItem(i, 1, new QTableWidgetItem(QString::number(player.commonFactories)));

        // Столбец 2: Auto Factories
        playerTable->setItem(i, 2, new QTableWidgetItem(QString::number(player.autoFactories)));

        // Столбец 3: Balance
        playerTable->setItem(i, 3, new QTableWidgetItem(QString::number(player.balance)));

        // Столбец 4: Raw
        playerTable->setItem(i, 4, new QTableWidgetItem(QString::number(player.raw)));

        // Столбец 5: Production
        playerTable->setItem(i, 5, new QTableWidgetItem(QString::number(player.production)));

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
