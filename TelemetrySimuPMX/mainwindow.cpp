#include "mainwindow.h"

#include <QtWidgets>
#include <QBoxLayout>
#include <QSvgWidget>
#include <QSvgRenderer>
#include <QDebug>
#include <QDateTime>

#include "svgview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_view(new SvgView)
{
    setWindowTitle(tr("PMX Log & Simu"));

    createActions();
    createStatusBar();
    createInsideMainWindows();
    createDockWindows();



    QTimer *refreshTimer = new QTimer(this);
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshForm()));
    refreshTimer->start(100);


    m_robot_x = 1500;
    m_robot_y = 1000;
    m_robot_a = 0;
    m_robot_time = 0;

    sock = new QUdpSocket(this);
    if (!sock->bind(10000, QUdpSocket::ShareAddress))
        qDebug() << "Unable to bind socket";
    connect(sock, SIGNAL(readyRead()), this, SLOT(readPacket()));

    qDebug() << "Config OK";
}

MainWindow::~MainWindow()
{

}

void MainWindow::menu_about()
{
    QMessageBox::about(this, tr("About PMX Logs & Simu"),
                       tr("This an application created by PMX, "
                          "it use Qt's dock widgets. "
                          ));
}

void MainWindow::menu_save()
{
    /*
    QMimeDatabase mimeDatabase;
    QString fileName = QFileDialog::getSaveFileName(this,
                        tr("Choose a file name"), ".",
                        mimeDatabase.mimeTypeForName("text/html").filterString());
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Dock Widgets"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toHtml();
    QApplication::restoreOverrideCursor();

    statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);*/
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save..."), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::menu_save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    fileMenu->addSeparator();

    QAction *quitAct = fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(tr("Quit the application"));
    /*
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    QToolBar *editToolBar = addToolBar(tr("&Edit"));
    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(":/images/undo.png"));
    QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, &QAction::triggered, this, &MainWindow::undo);
    editMenu->addAction(undoAct);
    editToolBar->addAction(undoAct);
*/
    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::menu_about);
    aboutAct->setStatusTip(tr("Show the application's About box"));

    QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


void MainWindow::createInsideMainWindows()
{
    loadFile(":/images/table2020.svg");
    m_view->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);
//m_view->scale(0.1,0.1);
m_view->fitInView(0, 0, 4000, 1000, Qt::KeepAspectRatioByExpanding);
    setCentralWidget(m_view);

    /*
    QWidget * m_mainWidget = new QWidget(this);
    QBoxLayout * mylayout = new QBoxLayout(QBoxLayout::TopToBottom);
    QSvgWidget * svgWidget = new QSvgWidget(":/images/table2020.svg", this);

    //mylayout->setGeometry(QRect(50,50,100,100) );
    svgWidget->show();

qDebug() << svgWidget->renderer()->isValid();
    //m_mainWidget->setAttribute(Qt::WA_NoSystemBackground, true);

    mylayout->addWidget(svgWidget);
    m_mainWidget->setLayout(mylayout);
    m_mainWidget->heightForWidth(2);

    setCentralWidget(m_mainWidget);
*/
    /*
    QWidget * m_mainWidget = new QWidget(this);
    //m_vLayout = new QVBoxLayout(this);
    m_hLayout = new QHBoxLayout();

    QPushButton* test1 = new QPushButton("button1", m_mainWidget);
    QPushButton* test2 = new QPushButton("button2", m_mainWidget);
    m_hLayout->addWidget(test1);
    m_hLayout->addWidget(test2);


    m_mainWidget->setLayout(m_hLayout);

    setCentralWidget(m_mainWidget);*/
}

QGroupBox *MainWindow::createLogsButtonGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("&Logs"));
    groupBox->setCheckable(true);
    groupBox->setChecked(true);


    //QPushButton *pushButton = new QPushButton(tr("&Normal Button"));
    QPushButton *toggleButtonR1 = new QPushButton(tr("&Bot n°1"));
    toggleButtonR1->setCheckable(true);
    toggleButtonR1->setChecked(false);

    QPushButton *toggleButtonRS1 = new QPushButton(tr("&Sensors of Bot n°1"));
    toggleButtonRS1->setCheckable(true);
    toggleButtonRS1->setChecked(false);

    QPushButton *toggleButtonR1Asserv = new QPushButton(tr("&Motion Bot n°1"));
    toggleButtonR1Asserv->setCheckable(true);
    toggleButtonR1Asserv->setChecked(false);



    QPushButton *toggleButtonR2 = new QPushButton(tr("&Bot n°2"));
    toggleButtonR2->setCheckable(true);
    toggleButtonR2->setChecked(false);

    QPushButton *toggleButtonRS2 = new QPushButton(tr("&Sensors of Bot n°2"));
    toggleButtonRS2->setCheckable(true);
    toggleButtonRS2->setChecked(false);

    QPushButton *toggleButtonR2Asserv = new QPushButton(tr("&Motion Bot n°2"));
    toggleButtonR2Asserv->setCheckable(true);
    toggleButtonR2Asserv->setChecked(false);




    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(toggleButtonR1);
    vbox->addWidget(toggleButtonRS1);
    vbox->addWidget(toggleButtonR1Asserv);
    vbox->addStretch(1);
    vbox->addWidget(toggleButtonR2);
    vbox->addWidget(toggleButtonRS2);
    vbox->addWidget(toggleButtonR2Asserv);

    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

QGroupBox *MainWindow::createSimuButtonGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("&Simulation"));
    groupBox->setCheckable(true);
    groupBox->setChecked(false);


    //QPushButton *pushButton = new QPushButton(tr("&Normal Button"));
    QPushButton *toggleButtonR1 = new QPushButton(tr("&Bot n°1"));
    toggleButtonR1->setCheckable(true);
    toggleButtonR1->setChecked(true);

    QPushButton *toggleButtonR2 = new QPushButton(tr("&Bot n°2"));
    toggleButtonR2->setCheckable(true);
    toggleButtonR2->setChecked(true);


    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(toggleButtonR1);
    vbox->addWidget(toggleButtonR2);

    vbox->addStretch(1);
    groupBox->setLayout(vbox);

    return groupBox;
}

void MainWindow::createDockWindows()
{
    QDockWidget *dock1 = new QDockWidget(tr("LOG"), this);
    /*
    QWidget* m_d1Widget = new QWidget(dock1);
    m_vLayout = new QVBoxLayout(m_d1Widget);
    QPushButton* test3 = new QPushButton("button3", m_d1Widget);
    QPushButton* test4 = new QPushButton("b4", m_d1Widget);
    m_vLayout->addWidget(test3);
    m_vLayout->addWidget(test4);
    //m_d1Widget->setLayout(m_vLayout); //non necessaire car c'est dans le constructeur
    dock1->setWidget(m_d1Widget);
*/
    dock1->setWidget(createLogsButtonGroup());
    //dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea |Qt::TopDockWidgetArea |Qt::BottomDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, dock1);
    viewMenu->addAction(dock1->toggleViewAction());


    QDockWidget *dock2 = new QDockWidget(tr("SIMU"), this);
    dock2->setWidget(createSimuButtonGroup());
    addDockWidget(Qt::LeftDockWidgetArea, dock2);
    viewMenu->addAction(dock2->toggleViewAction());



    QDockWidget *dockLogR1 = new QDockWidget(tr("BIG Bot log"), this);
    /*
    m_logList1 = new QListWidget(dockLogR1);
    m_logList1->addItems(QStringList()
            << "Log of Robot n°1");
    dockLogR1->setWidget(m_logList1);
    addDockWidget(Qt::BottomDockWidgetArea, dockLogR1);
    */
    m_textEditLogR1 = new QTextEdit(dockLogR1);
    m_textEditLogR1->setReadOnly(true);
    m_textEditLogR1->setWordWrapMode(QTextOption::WordWrap);
    //m_textEditLogR1->setPlainText(tr("Log of Bot n°1\n"));
    m_textEditLogR1->moveCursor (QTextCursor::End);
    //m_textEditLogR1->insertPlainText (tr("new log line...\n"));
    //m_textEditLogR1->appendPlainText (tr("This is a new log line..."));
    //m_textEditLogR1->moveCursor (QTextCursor::End);
    m_textEditLogR1->setStyleSheet("color: white; background-color: black;");

    dockLogR1->setWidget(m_textEditLogR1);
    addDockWidget(Qt::TopDockWidgetArea, dockLogR1);
    viewMenu->addAction(dockLogR1->toggleViewAction());



    QDockWidget *dockLogR2 = new QDockWidget(tr("SML Bot log"), this);
    m_textEditLogR2 = new QTextEdit(dockLogR2);
    m_textEditLogR2->setReadOnly(true);
    m_textEditLogR2->setWordWrapMode(QTextOption::WordWrap);
    //m_textEditLogR2->setPlainText(tr("Log of Bot n°2\n"));
    m_textEditLogR2->moveCursor (QTextCursor::End);
    m_textEditLogR2->setStyleSheet("color: white; background-color: black;");

    //    m_textEditLogR2->insertPlainText (tr("new log line...\n"));
    //    m_textEditLogR2->appendPlainText (tr("This is a new log line..."));
    //    m_textEditLogR2->moveCursor (QTextCursor::End);
    dockLogR2->setWidget(m_textEditLogR2);
    addDockWidget(Qt::TopDockWidgetArea, dockLogR2);
    viewMenu->addAction(dockLogR2->toggleViewAction());




    QDockWidget *dockADCR1 = new QDockWidget(tr("ADC R1"), this);
    QWidget* ADCWidgetR1 = new QWidget(dockADCR1);
    m_ADCLayout1 = new QVBoxLayout(ADCWidgetR1);
    for (int i = 0; i < 4; ++i)
      {
        m_ADCPlot1[i] = new QwtPlot(ADCWidgetR1);
        m_ADCPlot1[i]->enableAxis(QwtPlot::yLeft, false);
        m_ADCPlot1[i]->enableAxis(QwtPlot::xBottom, false);
        m_ADCPlot1[i]->setMinimumWidth(500);
        m_ADCPlot1[i]->setCanvasBackground(QBrush(QColor(38, 64, 115)));
        m_ADCPlot1[i]->setAxisScale(QwtPlot::yLeft, 0, 4096);
        m_ADCCurve1[i] = new QwtPlotCurve("ADC" + QString::number(i));
        m_ADCCurve1[i]->attach(m_ADCPlot1[i]);
        m_ADCCurve1[i]->setPen(QColor(255, 255, 0), 3);
        m_ADCLayout1->addWidget(m_ADCPlot1[i]);
      }
    dockADCR1->setWidget(ADCWidgetR1);
    addDockWidget(Qt::RightDockWidgetArea, dockADCR1);
    viewMenu->addAction(dockADCR1->toggleViewAction());


    QDockWidget *dockADCR2 = new QDockWidget(tr("ADC R2"), this);
    QWidget* ADCWidgetR2 = new QWidget(dockADCR2);
    m_ADCLayout2 = new QVBoxLayout(ADCWidgetR2);
    for (int i = 0; i < 4; ++i)
      {
        m_ADCPlot2[i] = new QwtPlot(ADCWidgetR2);
        m_ADCPlot2[i]->enableAxis(QwtPlot::yLeft, false);
        m_ADCPlot2[i]->enableAxis(QwtPlot::xBottom, false);
        m_ADCPlot2[i]->setMinimumWidth(500);
        m_ADCPlot2[i]->setCanvasBackground(QBrush(QColor(38, 64, 115)));
        m_ADCPlot2[i]->setAxisScale(QwtPlot::yLeft, 0, 4096);
        m_ADCCurve2[i] = new QwtPlotCurve("ADC" + QString::number(i));
        m_ADCCurve2[i]->attach(m_ADCPlot2[i]);
        m_ADCCurve2[i]->setPen(QColor(255, 255, 0), 3);
        m_ADCLayout2->addWidget(m_ADCPlot2[i]);
      }
    dockADCR2->setWidget(ADCWidgetR2);
    addDockWidget(Qt::RightDockWidgetArea, dockADCR2);
    viewMenu->addAction(dockADCR2->toggleViewAction());



    QDockWidget *dockPWMR1 = new QDockWidget(tr("PWM R1"), this);
    QWidget* PMWWidgetR1 = new QWidget(dockPWMR1);
    m_PMWLayout1 = new QVBoxLayout(PMWWidgetR1);

    for (int i = 0; i < 4; ++i)
      {
        MotorLayout1[i] = new QHBoxLayout();
        PWMPlot1[i] = new QwtPlot(PMWWidgetR1);
        PWMPlot1[i]->enableAxis(QwtPlot::yLeft, false);
        PWMPlot1[i]->enableAxis(QwtPlot::xBottom, false);
        PWMPlot1[i]->setMinimumWidth(500);
        PWMPlot1[i]->setCanvasBackground(QBrush(QColor(38, 64, 115)));
        PWMPlot1[i]->setAxisScale(QwtPlot::yLeft, -256, 256);
        PWMCurve1[i] = new QwtPlotCurve("PWM" + QString::number(i));
        PWMCurve1[i]->attach(PWMPlot1[i]);
        PWMCurve1[i]->setPen(QColor(255, 255, 0), 3);
        MotorLayout1[i]->addWidget(PWMPlot1[i]);
        PWMSlider1[i] = new QwtSlider(Qt::Vertical, PMWWidgetR1);
        PWMSlider1[i]->setScale(-255, 255);
        //PWMSlider2[i]->setScaleStepSize(50);
        //connect(PWMSlider2[i], SIGNAL(sliderMoved(double)), this, SLOT(updatePWM(double)));
        MotorLayout1[i]->addWidget(PWMSlider1[i]);
        m_PMWLayout1->addLayout(MotorLayout1[i]);
      }
    dockPWMR1->setWidget(PMWWidgetR1);
    addDockWidget(Qt::BottomDockWidgetArea, dockPWMR1);
    viewMenu->addAction(dockPWMR1->toggleViewAction());


    QDockWidget *dockPWMR2 = new QDockWidget(tr("PWM R2"), this);
    QWidget* PMWWidgetR2 = new QWidget(dockPWMR2);
    m_PMWLayout2 = new QVBoxLayout(PMWWidgetR2);

    for (int i = 0; i < 4; ++i)
      {
        MotorLayout2[i] = new QHBoxLayout();
        PWMPlot2[i] = new QwtPlot(PMWWidgetR2);
        PWMPlot2[i]->enableAxis(QwtPlot::yLeft, false);
        PWMPlot2[i]->enableAxis(QwtPlot::xBottom, false);
        PWMPlot2[i]->setMinimumWidth(500);
        PWMPlot2[i]->setCanvasBackground(QBrush(QColor(38, 64, 115)));
        PWMPlot2[i]->setAxisScale(QwtPlot::yLeft, -256, 256);
        PWMCurve2[i] = new QwtPlotCurve("PWM" + QString::number(i));
        PWMCurve2[i]->attach(PWMPlot2[i]);
        PWMCurve2[i]->setPen(QColor(255, 255, 0), 3);
        MotorLayout2[i]->addWidget(PWMPlot2[i]);
        PWMSlider2[i] = new QwtSlider(Qt::Vertical, PMWWidgetR2);
        PWMSlider2[i]->setScale(-255, 255);
        //PWMSlider2[i]->setScaleStepSize(50);
        //connect(PWMSlider2[i], SIGNAL(sliderMoved(double)), this, SLOT(updatePWM(double)));
        MotorLayout2[i]->addWidget(PWMSlider2[i]);
        m_PMWLayout2->addLayout(MotorLayout2[i]);
      }
    dockPWMR2->setWidget(PMWWidgetR2);
    addDockWidget(Qt::BottomDockWidgetArea, dockPWMR2);
    viewMenu->addAction(dockPWMR2->toggleViewAction());

}


static inline QString picturesLocation()
{
    return QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).value(0, QDir::currentPath());
}

void MainWindow::menu_openFile()
{/*
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setMimeTypeFilters(QStringList() << "image/svg+xml" << "image/svg+xml-compressed");
    fileDialog.setWindowTitle(tr("Open SVG File"));
    if (m_currentPath.isEmpty())
        fileDialog.setDirectory(picturesLocation());

    while (fileDialog.exec() == QDialog::Accepted && !loadFile(fileDialog.selectedFiles().constFirst()));*/
}

bool MainWindow::loadFile(const QString &fileName)
{
    if (!QFileInfo::exists(fileName) || !m_view->openFile(fileName)) {
        QMessageBox::critical(this, tr("Open SVG File"),
                              tr("Could not open file '%1'.").arg(QDir::toNativeSeparators(fileName)));
        return false;
    }

    if (!fileName.startsWith(":/")) {
        m_currentPath = fileName;
        setWindowFilePath(fileName);



        const QSize size = m_view->svgSize();

        const QString message =
                tr("Opened %1, %2x%3").arg(QFileInfo(fileName).fileName()).arg(size.width()).arg(size.width());
        statusBar()->showMessage(message);

        qDebug() << message;
    }

    //const QSize availableSize = QApplication::desktop()->availableGeometry(this).size();
    //resize(m_view->sizeHint().expandedTo(availableSize / 4) + QSize(380, 180 + menuBar()->height()));


    return true;
}

void MainWindow::setRenderer(int renderMode)
{

    //m_highQualityAntialiasingAction->setEnabled(renderMode == SvgView::OpenGL);
    m_view->setRenderer(static_cast<SvgView::RendererType>(renderMode));
}


void MainWindow::readPacket()
{
    char buf[1024];
    //qDebug() << "Packet received";
    sock->readDatagram(buf, 1024);
    QString pkt(buf);
    // qDebug() << pkt;"
    QString label = pkt.split("|")[0];
    //qDebug() << "Packet type is " << label;
    if (label == "POS")
    {
        QString pos = pkt.split(":")[1];
        QString x = pos.split(" ", QString::SkipEmptyParts)[0];
        m_robot_x = x.toFloat();
        QString y = pos.split(" ", QString::SkipEmptyParts)[1];
        m_robot_y = y.toFloat();
        QString a = pos.split(" ", QString::SkipEmptyParts)[2];
        m_robot_a = a.toFloat();
        statusBar()->showMessage("X:" + QString::number(m_robot_x) + " Y:" + QString::number(m_robot_y) + " A:" + QString::number(m_robot_a));
    }
    /*
  if (label == "ADC")
    {
      qint64 now = QDateTime::currentMSecsSinceEpoch();
      QString pos = pkt.split(":")[1];
      ADC_ts.append(now);
      for (int i = 0; i < 8; ++i)
    {
      QString x = pos.split(" ", QString::SkipEmptyParts)[i];
      ADCData[i].append(x.toUShort());
    }
    }
  if (label == "PWM")
    {
      qint64 now = QDateTime::currentMSecsSinceEpoch();
      QString pos = pkt.split(":")[1];
      PWM_ts.append(now);
      for (int i = 0; i < 4; ++i)
    {
      QString x = pos.split(" ", QString::SkipEmptyParts)[i];
      PWMData[i].append(x.toShort());
    }
  }
  */
    if (label == "MSG")
    {
        QString id = pkt.split("|")[1];
        QString timeus = pkt.split("|")[2];
        QString msg = pkt.split("|")[3];
        QString level = msg.split(" ")[1];
        qDebug() << id << "|" << timeus << "|" << msg;


        if (id == "BIG")
        {
            if (level=="ERROR")
            {
                m_textEditLogR1->setFontWeight(QFont::Bold );
                m_textEditLogR1->setTextColor(QColor( "darkred" ));
            }else if (level=="INFO")
            {
                m_textEditLogR1->setFontWeight(QFont::Normal );
                m_textEditLogR1->setTextColor(QColor(108,160,220)); //Blue
            }else if (level=="DEBUG")
            {
                m_textEditLogR1->setFontWeight(QFont::Normal );
                m_textEditLogR1->setTextColor(QColor( "lightgray" ));
            }


            m_textEditLogR1->append(timeus + "|" +msg);
        }
        if (id == "SML")
        {
            if (level=="ERROR")
            {
                m_textEditLogR2->setFontWeight(QFont::Bold );
                m_textEditLogR2->setTextColor(QColor( "darkred" ));
            }else if (level=="INFO")
            {
                m_textEditLogR2->setFontWeight(QFont::Normal );
                m_textEditLogR2->setTextColor(QColor(108,160,220));//Blue
            }else if (level=="DEBUG")
            {
                m_textEditLogR2->setFontWeight(QFont::Normal );
                m_textEditLogR2->setTextColor(QColor( "lightgray" ));
            }

            m_textEditLogR2->append(timeus + "|" +msg);
        }

    }
}

void MainWindow::refreshForm()
{

}
