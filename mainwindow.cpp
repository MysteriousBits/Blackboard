#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Blackboard - Untitled");
    scribblewindow = new scribblearea;
    setCentralWidget(scribblewindow);
    scribblewindow->SetWindowBackground(Qt::black);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionBackground_triggered()
{
    QColor newColor =  QColorDialog::getColor(Qt::white, this);
    if (newColor.isValid()) scribblewindow->SetWindowBackground(newColor);
}


void MainWindow::on_actionBrush_color_triggered()
{
    QColor newColor = QColorDialog::getColor(scribblewindow->getPenColor());
    if (newColor.isValid()) scribblewindow->setPenColor(newColor);
}


void MainWindow::on_actionEraser_toggled(bool arg1)
{
    scribblewindow->toggleEraser(arg1);
}


void MainWindow::on_actionPen_width_triggered()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Blackboard"), tr("Select pen size:"), scribblewindow->getPenWidth(), 3, 180, 1, &ok);
    if (ok) scribblewindow->setPenWidth(newWidth);
}


void MainWindow::on_actionClear_Board_triggered()
{
    scribblewindow->ClearScreen();
}


void MainWindow::on_actionIncrease_triggered()
{
    scribblewindow->setPenWidth(scribblewindow->getPenWidth() + 2);
}


void MainWindow::on_actionDecrease_triggered()
{
    scribblewindow->setPenWidth(scribblewindow->getPenWidth() - 2);
}

void MainWindow::on_actionUndo_triggered()
{
    scribblewindow->back();
}

bool MainWindow::saveWarn()
{
    if (scribblewindow->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Blackoard"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }

    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    if (saveWarn())
    {
        QString filename = QFileDialog::getOpenFileName(this, "Open Image");

        if (!scribblewindow->openImage(filename))
        {
            QMessageBox::warning(this, "Blackboard Warning", "Can't read file");
        }
    }
}
void MainWindow::on_actionSave_2_triggered()
{
    save();
}

bool MainWindow::save()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "untitled.jpg");

    if (filename.isEmpty()) return false;
    if (!scribblewindow->saveImage(format(filename)))
    {
        QMessageBox::warning(this, "Blackboard warning", "Can't save file");
        return false;
    }
    return true;
}

QString MainWindow::format(const QString &filename)
{
    if (filename.contains(".")) return filename;
    return  filename + ".jpg";
}

void MainWindow::on_actionGit_page_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/UnknownComplexity/Blackboard"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (saveWarn()) event->accept();
    else event->ignore();
}
