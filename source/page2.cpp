#include "page2.h"

Page2::Page2(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

Page2::~Page2()
{
}

void Page2::setupUI()
{
    mainLayout = new QVBoxLayout(this);

    // Заголовок страницы
    titleLabel = new QLabel("Page 2 - Bid Information Array", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #333; margin-bottom: 20px; }");

    // Создаем таблицу с 6 столбцами
    bidTable = new QTableWidget(0, 6, this);

    // Устанавливаем заголовки столбцов
    QStringList headers;
    headers << "ID" << "Raw Sell Count" << "Raw Sell Cost" << "Product Buy Count" << "Product Buy Cost" << "Occasion";
    bidTable->setHorizontalHeaderLabels(headers);

    // Настраиваем размеры столбцов
    bidTable->horizontalHeader()->setStretchLastSection(true);
    bidTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Запрещаем редактирование
    bidTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Настраиваем выделение
    bidTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bidTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Стилизация таблицы
    bidTable->setStyleSheet(
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
    bidTable->setAlternatingRowColors(true);

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
    mainLayout->addWidget(bidTable);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Подключаем сигналы кнопок
    connect(backButton, &QPushButton::clicked, this, &Page2::backClicked);
    connect(nextButton, &QPushButton::clicked, this, &Page2::nextClicked);
}

void Page2::setBidData(const std::vector<bid>& bidDataArray)
{
    currentBidArray = bidDataArray;
    updateDisplay();
}

void Page2::updateDisplay()
{
    // Очищаем таблицу
    bidTable->setRowCount(0);

    // Если массив пуст, отображаем пустую таблицу
    if (currentBidArray.empty()) {
        titleLabel->setText("Page 2 - Bid Information Array (Empty)");
        return;
    }

    // Обновляем заголовок с количеством элементов
    titleLabel->setText(QString("Page 2 - Bid Information Array (%1 items)").arg(currentBidArray.size()));

    // Устанавливаем количество строк равное размеру массива
    bidTable->setRowCount(currentBidArray.size());

    // Заполняем таблицу данными
    for (size_t i = 0; i < currentBidArray.size(); ++i) {
        const bid& currentBid = currentBidArray[i];

        // Столбец 0: ID (номер строки)
        bidTable->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));

        // Столбец 1: Raw Sell Count
        bidTable->setItem(i, 1, new QTableWidgetItem(QString::number(currentBid.rawSellBid.count)));

        // Столбец 2: Raw Sell Cost
        bidTable->setItem(i, 2, new QTableWidgetItem(QString::number(currentBid.rawSellBid.cost)));

        // Столбец 3: Product Buy Count
        bidTable->setItem(i, 3, new QTableWidgetItem(QString::number(currentBid.prodBuyBid.count)));

        // Столбец 4: Product Buy Cost
        bidTable->setItem(i, 4, new QTableWidgetItem(QString::number(currentBid.prodBuyBid.cost)));

        // Столбец 5: Occasion
        bidTable->setItem(i, 5, new QTableWidgetItem(occasionToString(currentBid.occasion)));

        // Выравниваем текст в ячейках
        for (int col = 0; col < 6; ++col) {
            QTableWidgetItem *item = bidTable->item(i, col);
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
}

QString Page2::occasionToString(happyCaseOccasion occasion)
{
    switch(occasion) {
    case happyCaseOccasion::BIRTHDAY:
        return "Birthday";
    case happyCaseOccasion::ANNIVERSARY:
        return "Anniversary";
    case happyCaseOccasion::HOLIDAY:
        return "Holiday";
    case happyCaseOccasion::OTHER:
        return "Other";
    default:
        return "Unknown";
    }
}
