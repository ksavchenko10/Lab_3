#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:

    void on_selectionChangedSlot(const QItemSelection &selected, const QItemSelection &deselected);
    void redraw();
    void on_checkBoxClicked();
    void on_comboBoxChanged();
    void on_paintClicked();

#endif // MAINWINDOW_H
