#include "mainwindow.h"
#include <QSplitter>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QVBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QStatusBar>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : //QWidget(parent)
      QMainWindow(parent)
{
    //Устанавливаем размер главного окна
    this->setGeometry(100, 100, 1000, 500);
    this->setStatusBar(new QStatusBar(this));
    this->statusBar()->showMessage("Выберите файл БД");
     QString homePath = QDir::currentPath();
    // Определим  файловой системы:
    dirModel =  new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setRootPath(homePath);

    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(homePath);
    //Показать как дерево, пользуясь готовым видом:

    treeView = new QTreeView(); //дерево файлов
    treeView->setModel(dirModel);

    treeView->expandAll();
    QSplitter *splitter = new QSplitter(parent);//создаем сплиттер
    tableView = new QTableView;//создаем таблицу
    tableView->setModel(fileModel);//добавляем наше дерево файлов в таблицу
    splitter->addWidget(tableView);//добавляем таблицу к сплиттеру
    setCentralWidget(splitter);

    QSplitter *splitterRight = new QSplitter(parent);//создаем правый сплиттер
    splitterRight->setOrientation(Qt::Vertical);//размещение элементов в нем вертикальное
    splitter->addWidget(splitterRight);//доблавяем сплиттер к нашему основному сплиттеру, правая часть программы

    QSplitter *splitterEmptyVertical = new QSplitter(parent);//пустой сплиттер, чтобы сдвинуть элементы вверху
    splitterEmptyVertical->setMaximumHeight(10);//задаем максимальную высоту
    splitterRight->addWidget(splitterEmptyVertical);//добавляем сплиттер

    int rightopSplitterHeight = 30;//задает высоту панели
    QSplitter *splitterRightFirst = new QSplitter(parent);//создаем сплиттер с нашими элементами управления
    splitterRightFirst->setOrientation(Qt::Horizontal);//ориентация размещения на нем горизонтальная
    splitterRightFirst->setMaximumHeight(rightopSplitterHeight);//максимальная высота
    splitterRight->addWidget(splitterRightFirst);//добавляем к нашему правому сплиттеру

    QLabel *caption = new QLabel("Выберите тип диаграммы: "); //текст
    caption->setMaximumWidth(200); //максимальная ширина
    caption->setAlignment(Qt::AlignRight|Qt::AlignCenter); //ориентация право по горизонтали и по вертикали по середине
    caption->setMaximumHeight(rightopSplitterHeight); //максимальная высота
    splitterRightFirst->addWidget(caption); //добавляем к сплиттеру

    qbox = new QComboBox(); //раскрывающийся список
    qbox->setMaximumWidth(100); //максимальная ширина
    qbox->setMaximumHeight(rightopSplitterHeight); //максимальная высота
    qbox->addItem("BarChart"); //добавляем элемент для выбора
    qbox->addItem("PieChart"); //добавляем элемент для выбора
    //делаем вызов функции on_comboBoxChanged при изменении выбранного элемента в списке
    connect(qbox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_comboBoxChanged()));
    splitterRightFirst->addWidget(qbox); //доблавяем к сплиттеру



    QItemSelectionModel *selectionModel = treeView->selectionModel();
    QModelIndex rootIx = dirModel->index(0, 0, QModelIndex());//корневой элемент

    QModelIndex indexHomePath = dirModel->index(homePath);
    QFileInfo fileInfo = dirModel->fileInfo(indexHomePath);

    /* Рассмотрим способы обхода содержимого папок на диске.
     * Предлагается вариант решения, которы может быть применен для более сложных задач.
     * Итак, если требуется выполнить анализ содержимого папки, то необходимо организовать обход содержимого. Обход выполняем относительно модельного индекса.
     * Например:*/
    if (fileInfo.isDir()) {
        /*
         * Если fileInfo папка то заходим в нее, что бы просмотреть находящиеся в ней файлы.
         * Если нужно просмотреть все файлы, включая все вложенные папки, то нужно организовать рекурсивный обход.
        */
        QDir dir  = fileInfo.dir();

        if (dir.cd(fileInfo.fileName())) {
            /**
             * Если зашли в папку, то пройдемся по контейнеру QFileInfoList ,полученного методом entryInfoList,
             * */

            foreach (QFileInfo inf, dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot, QDir::Type)) {
                qDebug() << inf.fileName() << "---" << inf.size();
            }

            dir.cdUp();//выходим из папки
        }
    }

    QDir dir = fileInfo.dir();

    foreach (QFileInfo inf, dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDir::Type)) {

        qDebug() << inf.fileName() << "---" << inf.size();
    }


    treeView->header()->resizeSection(0, 200);
    //Выполняем соединения слота и сигнала который вызывается когда осуществляется выбор элемента в TreeView
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(on_selectionChangedSlot(const QItemSelection &, const QItemSelection &)));
    //Пример организации установки курсора в TreeView относит ельно модельного индекса
    QItemSelection toggleSelection;
    QModelIndex topLeft;
    topLeft = dirModel->index(homePath);
    dirModel->setRootPath(homePath);

    toggleSelection.select(topLeft, topLeft);
    selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

//Слот для обработки выбора элемента в TreeView
//выбор осуществляется с помощью курсора

void MainWindow::on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    redraw(); //вызываем функции перерисовки графика
}

void MainWindow::on_checkBoxClicked()
{
    redraw(); //вызываем функции перерисовки графика
}

void MainWindow::on_comboBoxChanged()
{
   redraw(); //вызываем функции перерисовки графика
}
