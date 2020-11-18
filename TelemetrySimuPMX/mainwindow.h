#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>
#include <QTimer>
#include <QUdpSocket>
#include <QVector>
#include <QWidget>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_slider.h>


QT_BEGIN_NAMESPACE
class SvgView;
class QAction;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsRectItem;
class QGroupBox;
class QListWidget;
class QTextEdit;
class QPlainTextEdit;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
     bool loadFile(const QString &path);
    ~MainWindow();

private slots:
    void menu_about();
    void menu_save();
    void menu_openFile();

    void setRenderer(int renderMode);


  void readPacket();
  void refreshForm();

private:
    void createActions();
    void createStatusBar();
    void createInsideMainWindows();
    void createDockWindows();
    QGroupBox *createLogsButtonGroup();
    QGroupBox *createSimuButtonGroup();

    QMenu* viewMenu;
    QWidget* m_mainWidget;
    QVBoxLayout* m_vLayout;
    QHBoxLayout* m_hLayout;

    QVBoxLayout* m_ADCLayout1;
    QwtPlot *m_ADCPlot1[8];
    QwtPlotCurve *m_ADCCurve1[8];
    QVector<double> m_ADC_ts1;
    QVector<double> m_ADCData1[8];

    QVBoxLayout* m_ADCLayout2;
    QwtPlot *m_ADCPlot2[8];
    QwtPlotCurve *m_ADCCurve2[8];
    QVector<double> m_ADC_ts2;
    QVector<double> m_ADCData2[8];

    QVBoxLayout* m_PMWLayout1;
    QHBoxLayout *MotorLayout1[4];
    QwtPlot *PWMPlot1[4];
    QwtPlotCurve *PWMCurve1[4];
    QVector<double> PWM_ts1;
    QVector<double> PWMData1[4];
    QwtSlider *PWMSlider1[4];

    QVBoxLayout* m_PMWLayout2;
    QHBoxLayout *MotorLayout2[4];
    QwtPlot *PWMPlot2[4];
    QwtPlotCurve *PWMCurve2[4];
    QVector<double> PWM_ts2;
    QVector<double> PWMData2[4];
    QwtSlider *PWMSlider2[4];

    QListWidget *m_logList1;
    QListWidget *m_logList2;


    SvgView *m_view;
    QString m_currentPath;

    QTextEdit *m_textEditLogR1;
    QTextEdit *m_textEditLogR2;

    float m_robot_x;
    float m_robot_y;
    float m_robot_a;
    float m_robot_time;

    QUdpSocket *sock;
    QTimer *refreshTimer;

};

#endif // MAINWINDOW_H
