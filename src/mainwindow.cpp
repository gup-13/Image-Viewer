#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "FileIO.h"

void MainWindow::loadDirectory() {
    QString fileDir = FileIO::getFileDirectoryWithDialog();
    if(fileDir != "") {

        std::vector<QString> output;
        FileIO::iterateDirectory(fileDir, output);

        // filter out non-image files
        imageList.clear();
        for(int i = 0; i < output.size(); i++){
            if(FileIO::isFileExtensionPresent(output[i], extensions)) {
                imageList.push_back(output[i]);
            }
        }
    }
}

void MainWindow::nextImage() {
    imageIdx++;
    if(imageIdx >= imageList.size()) {
        imageIdx = 0;
    }
    reloadImage();
}

void MainWindow::prevImage() {
    imageIdx--;
    if(imageIdx < 0) {
        imageIdx = imageList.size() - 1;
    }
    reloadImage();
}

void MainWindow::reloadImage() {
    QLabel* imageLabel = ui->label_ImageViewer;
    int w = imageLabel->width();
    int h = imageLabel->height();

    QPixmap image(imageList[imageIdx]);

    // set a scaled pixmap to a window keeping its aspect ratio
    imageLabel->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation));
}

void MainWindow::randomizeOrder() {
    imageIdx = 0;

    auto rng = std::default_random_engine {};
    int currTime = std::chrono::system_clock::now().time_since_epoch().count();
    rng.seed(currTime);
    std::shuffle(std::begin(imageList), std::end(imageList), rng);
}

void MainWindow::updateTimer() {
    if(isTimerPaused){
        statusBar()->showMessage(QString("Time:" + QString::number(currSecond) + "--PAUSED--"), 5000);
        return;
    }
    currSecond--;   //Update time remaining

    // Move to next image
    if(currSecond == 0) {
        currSecond = timerInterval;
        timerCount++;
        nextImage();        
    }
    timer->start(1000);

    statusBar()->showMessage(QString("Time:" + QString::number(currSecond)), 5000);
    label_timerCount->setText(QString("Count: " + QString::number(timerCount)));
}

void MainWindow::resetTimer() {
    timer->stop();
    isTimerActive = false;
    currSecond = timerInterval;
    statusBar()->clearMessage();
    timerCount = 0;
    label_timerCount->setVisible(false);
    isTimerPaused = false;
}

void MainWindow::toggleTimer() {
    isTimerActive = !isTimerActive;
    if(isTimerActive) {
        label_timerCount->setVisible(true);
        currSecond = timerInterval;
        updateTimer();
    }
    else {
        resetTimer();
    }
}

void MainWindow::copyImageToClipboard() {
    QClipboard* cb = QApplication::clipboard();
    QPixmap image(imageList[imageIdx]);
    cb->setPixmap(image);
}

// Use QLabel to display images
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    extensions = {"png","jpg","jpeg"};
    imageIdx = 0;
    label_timerCount = new QLabel("M");
    statusBar()->addPermanentWidget(label_timerCount);

    //load directory
    loadDirectory();
    reloadImage();

    //init timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
    timerInterval = 60;
    resetTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    //Description: start timer event after n milliseconds
    if(timerID > 0) {   //timer exists
        //delete/reset timer
        killTimer(timerID);
        timerID = 0;
    }
    timerID = startTimer(50); //millisecond
}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Space) {
        // Toggle timer
        isTimerPaused = !isTimerPaused;
        statusBar()->showMessage(QString("Time:" + QString::number(currSecond) + "--WAIT--"), 5000);
    }

    if(event->key() == Qt::Key_F4) {
        // randomize order of images
        randomizeOrder();
        reloadImage();
    }

    // == Timers ==
    if(event->key() == Qt::Key_F5) {
        timerInterval = 30;
        toggleTimer();
    }
    if(event->key() == Qt::Key_F6) {
        timerInterval = 60;
        toggleTimer();
    }
    if(event->key() == Qt::Key_F7) {
        timerInterval = 300;
        toggleTimer();
    }
    if(event->key() == Qt::Key_F8) {
        timerInterval = 600;
        toggleTimer();
    }

    // == View other images ==
    if(event->key() == Qt::Key_Left) {
        prevImage();
        currSecond = timerInterval;
    }

    if(event->key() == Qt::Key_Right) {
        nextImage();
        currSecond = timerInterval;
    }

    if(event->key() == Qt::Key_C && Qt::ControlModifier) {
        // Copy image to clipboard
        copyImageToClipboard();
        statusBar()->showMessage(QString("IMAGE COPIED"), 5000);
    }
}

void MainWindow::timerEvent(QTimerEvent *event) {
    //resize image
    reloadImage();

    //delete existing timer
    killTimer(timerID);
    timerID = 0;
}
