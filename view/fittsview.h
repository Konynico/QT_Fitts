#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicwidget.h"

#include <QMainWindow>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStackedLayout>
#include <QGraphicsView>
#include <QLabel>
#include <QLineSeries>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

class FittsModel;
class FittsController;

class FittsView : public QMainWindow
{
    Q_OBJECT
public:
    FittsView(FittsModel *fittsModel);
    ~FittsView();

    void initWindows();
    void displayOptionDialog();
    void displayAideDialog();
private:
    void updateTestMsg();
    void displayResults();


    FittsModel *fittsModel;
    FittsController *fittsController;

    QPushButton *homeLeaveBtn;
    QPushButton *homeStartBtn;
    QPushButton *testHomeBtn;
    QPushButton *testRestartBtn;
    QPushButton *resultHomeBtn;
    QPushButton *resultRestartBtn;

    QDoubleSpinBox *aValue;
    QDoubleSpinBox *bValue;
    QSpinBox *nbCible;
    QSpinBox *minSize;
    QSpinBox *maxSize;

    QStackedLayout *mainStack;

    GraphicWidget *graphicView;
    QGraphicsScene *scene;

    QLabel *testLabel;
    QLabel *ecartType;
    QLabel *erreurType;
    QLabel *diffMoy;
    QLabel *itc95;

    QChartView *plot;
    QChartView *plot2;

    QMenu *menu_fichier;

    QAction *actionStart;
    QAction *actionQuitter;
    QAction *actionParametres;
    QAction *actionAide;
    //édité
    QAction *actionTheme;

    QDialog *optionDialog;



    friend FittsController;




};

#endif // MAINWINDOW_H
