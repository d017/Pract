#include "bidTable.h"
#include <QDebug>

BidTable::BidTable(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

BidTable::~BidTable()
{
}

void BidTable::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Заголовок страницы
    titleLabel = new QLabel("Информация о ставках", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #333; margin-bottom: 20px; }");

    // Создаем таблицу с 6 столбцами
    bidTablewidget = new QTableWidget(0, 6, this);

    // Устанавливаем заголовки столбцов
    QStringList headers;
    headers << "Месяц" << "Колич. прод. ЕС" << "Цена прод. ЕС" << "Колич. пок. ГП" << "Цена пок. ГП" << "Счастливый случай";
    bidTablewidget->setHorizontalHeaderLabels(headers);

    // Настраиваем размеры столбцов
    bidTablewidget->horizontalHeader()->setStretchLastSection(true);
    bidTablewidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Запрещаем редактирование
    bidTablewidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Настраиваем выделение
    bidTablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    bidTablewidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Стилизация таблицы
    bidTablewidget->setStyleSheet(
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
    bidTablewidget->setAlternatingRowColors(true);

    // Создаем кнопки навигации
    // backButton = new QPushButton("BACK", this);
    // nextButton = new QPushButton("NEXT", this);

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

    // backButton->setStyleSheet(buttonStyle);
    // nextButton->setStyleSheet(buttonStyle);

    // Создаем layout для кнопок
    buttonLayout = new QHBoxLayout();
    // buttonLayout->addWidget(backButton);
    // buttonLayout->addStretch(); // Добавляем растягивающий элемент между кнопками
    // buttonLayout->addWidget(nextButton);

    // Добавляем элементы в главный layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(bidTablewidget);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Подключаем сигналы кнопок
    // connect(backButton, &QPushButton::clicked, this, &BidTable::backClicked);
    // connect(nextButton, &QPushButton::clicked, this, &BidTable::nextClicked);
}

void BidTable::resizeEvent(QResizeEvent* ev) {
    QWidget::resizeEvent(ev);
    updateDisplay();
}

void BidTable::setBidData(const QVector<bid>& bidDataArray)
{
    currentBidArray = bidDataArray;
    updateDisplay();
}

void BidTable::updateDisplay()
{
    // Очищаем таблицу
    bidTablewidget->setRowCount(0);

    // Если массив пуст, отображаем пустую таблицу
    if (currentBidArray.empty()) {
        titleLabel->setText("Информация о ставках");
        return;
    }

    // Обновляем заголовок с количеством элементов
    titleLabel->setText(QString("Информация о ставках (%1 м.)").arg(currentBidArray.size()));

    // Устанавливаем количество строк равное размеру массива
    bidTablewidget->setRowCount(currentBidArray.size());

    // Заполняем таблицу данными
    for (int i = 0; i < currentBidArray.size(); ++i) {
        const bid currentBid = currentBidArray[i];

        // Столбец 0: ID (номер строки)
        bidTablewidget->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));

        // Столбец 1: Raw Sell Count
        bidTablewidget->setItem(i, 1, new QTableWidgetItem(QString::number(currentBid.rawSellBid.count)));

        // Столбец 2: Raw Sell Cost
        bidTablewidget->setItem(i, 2, new QTableWidgetItem(QString::number(currentBid.rawSellBid.cost)));

        // Столбец 3: Product Buy Count
        bidTablewidget->setItem(i, 3, new QTableWidgetItem(QString::number(currentBid.prodBuyBid.count)));

        // Столбец 4: Product Buy Cost
        bidTablewidget->setItem(i, 4, new QTableWidgetItem(QString::number(currentBid.prodBuyBid.cost)));

        // Столбец 5: Occasion
        bidTablewidget->setItem(i, 5, new QTableWidgetItem(occasionToString(currentBid.happyCase)));

        // Выравниваем текст в ячейках
        for (int col = 0; col < 6; ++col) {
            QTableWidgetItem *item = bidTablewidget->item(i, col);
            if (item) {
                if (col == 0) {
                    item->setTextAlignment(Qt::AlignCenter);
                    item->setFont(QFont("Arial", 10, QFont::Bold));
                } else if (col == 5) {
                    item->setTextAlignment(Qt::AlignCenter);
                    item->setFont(QFont("Arial", 10, QFont::Normal));
                } else {
                    item->setTextAlignment(Qt::AlignCenter);
                }
            }
        }
    }
    int w = this->width();
    int h = w / 8;

    for (int col = 0; col < 5; ++col) {
        bidTablewidget->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Fixed);
        bidTablewidget->setColumnWidth(col, h);
    }
    bidTablewidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
}

QString BidTable::occasionToString(happyCaseOccasion occasion)
{
    switch (occasion.index) {
    case 0:
        return "-";
    case 1:
        return "Взорвалась фабрика игрока " + QString::number(occasion.target + 1);
    case 2:
        return "Игрок " + QString::number(occasion.target + 1) + " получил обычную фабрику";
    case 3:
        return "Игрок " + QString::number(occasion.target + 1) + " выиграл в лотерею";
    case 4:
        return "День рождения игрока " + QString::number(occasion.target + 1);
    default:
        qDebug() << occasion.index;
        return "Unknown";
    }
}
