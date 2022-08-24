#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QClipboard>

#include <algorithm>
#include <random>
#include <chrono>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadDirectory();
    void nextImage();
    void prevImage();
    void reloadImage();
    void randomizeOrder();
    void resetTimer();
    void toggleTimer();
    void copyImageToClipboard();


protected:
    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void timerEvent(QTimerEvent *event);
private:
    Ui::MainWindow *ui;
    int imageIdx;
    std::vector<QString> imageList;
    std::vector<QString> extensions;
    int timerID;
    QTimer* timer;
    int currSecond;
    int timerInterval;
    bool isTimerActive;
    int timerCount;
    QLabel* label_timerCount;
    bool isTimerPaused;

public slots:
    void updateTimer();
};
#endif // MAINWINDOW_H
