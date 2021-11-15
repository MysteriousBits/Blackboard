#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class scribblearea;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionBackground_triggered();

    void on_actionBrush_color_triggered();

    void on_actionEraser_toggled(bool arg1);

    void on_actionPen_width_triggered();

    void on_actionClear_Board_triggered();

    void on_actionIncrease_triggered();

    void on_actionDecrease_triggered();

    void on_actionUndo_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_2_triggered();

    void on_actionGit_page_triggered();

private:
    Ui::MainWindow *ui;

    scribblearea *scribblewindow;

    bool saveWarn();
    bool save();
    QString format(const QString &filename);

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
