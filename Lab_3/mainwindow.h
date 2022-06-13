#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QTableView>


class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:

    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void redraw();
    void on_checkBoxClicked();
    void on_comboBoxChanged();
    void on_paintClicked();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QFileSystemModel *fileModel;
    QFileSystemModel *dirModel;
    QTreeView *treeView;
    QTableView *tableView;
    QtCharts::QChartView *chartView;
    QtCharts::QChart *chart;
    QComboBox *qbox;
    QCheckBox *checkBox;
    QtCharts::QValueAxis *axisY;
};

#endif // MAINWINDOW_H
