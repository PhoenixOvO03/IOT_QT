#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QPushButton;
class CircleProgress;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QPushButton* m_add10btn;
    QPushButton* m_add50btn;
    QPushButton* m_sub10btn;
    QPushButton* m_sub50btn;

    CircleProgress* m_dash;
};
#endif // MAINWINDOW_H
