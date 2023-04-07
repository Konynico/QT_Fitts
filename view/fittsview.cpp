#include "fittsview.h"
#include "graphicwidget.h"
#include "fittscontroller.h"
#include "fittsmodel.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>

#include <QStackedLayout>
#include <QGraphicsView>
#include <QChart>
#include <QPolarChart>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

FittsView::FittsView(FittsModel *fittsModel) : QMainWindow() {

    QFile styleFile( ":/styles/style.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style(styleFile.readAll());
    qApp->setStyleSheet(style);


    this->fittsModel = fittsModel;

    this->initWindows();
    this->fittsController = new FittsController(this, this->fittsModel);

    connect(actionStart, SIGNAL(triggered()), fittsController, SLOT(startSimulation()));

    //edité

    connect(actionTheme, SIGNAL(triggered()), fittsController, SLOT(actionTheme()));

    connect(actionParametres, SIGNAL(triggered()), fittsController, SLOT(parametresClicked()));
    connect(actionAide, SIGNAL(triggered()), fittsController, SLOT(aideClicked()));


    // Btn clicked
    connect(homeLeaveBtn,SIGNAL(clicked()),fittsController,SLOT(quit()));
    connect(homeStartBtn,SIGNAL(clicked()),fittsController,SLOT(startSimulation()));

    connect(testHomeBtn,SIGNAL(clicked()),fittsController,SLOT(backToSettings()));
    connect(testRestartBtn,SIGNAL(clicked()),fittsController,SLOT(startSimulation()));

    connect(resultHomeBtn,SIGNAL(clicked()),fittsController,SLOT(backToSettings()));
    connect(resultRestartBtn,SIGNAL(clicked()),fittsController,SLOT(startSimulation()));

    connect(graphicView, SIGNAL(mouseClicked(int,int)), fittsController, SLOT(cibleClicked(int,int)));

    // SpinBox values update
    connect(aValue,SIGNAL(valueChanged(double)),fittsController,SLOT(aValueChanged(double)));
    connect(bValue,SIGNAL(valueChanged(double)),fittsController,SLOT(bValueChanged(double)));

    connect(nbCible,SIGNAL(valueChanged(int)),fittsController,SLOT(nbCibleChanged(int)));
    connect(minSize,SIGNAL(valueChanged(int)),fittsController,SLOT(minSizeChanged(int)));
    connect(maxSize,SIGNAL(valueChanged(int)),fittsController,SLOT(maxSizeChanged(int)));
}

FittsView::~FittsView() {}

void FittsView::initWindows() {

    menu_fichier = this->menuBar()->addMenu(tr("Fichier"));

    actionStart = new QAction(tr("Lancer le test"),this);
    actionStart->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    actionStart->setShortcutVisibleInContextMenu(true);
    menu_fichier->addAction(actionStart);



    //edité
    actionTheme = new QAction(tr("Changer de thème"),this);
    actionTheme->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_W));
    actionTheme->setShortcutVisibleInContextMenu(true);
    menu_fichier->addAction(actionTheme);

    actionParametres = new QAction(tr("Paramètres"), this);
    actionParametres->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    actionParametres->setShortcutVisibleInContextMenu(true);
    this->menuBar()->addAction(actionParametres);

    actionAide = new QAction(tr("Aide"), this);
    actionAide->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    actionAide->setShortcutVisibleInContextMenu(true);
    this->menuBar()->addAction(actionAide);

    optionDialog = new QDialog();
    optionDialog->setWindowTitle("Paramètres");
    QVBoxLayout *rappelRightLayout = new QVBoxLayout();
    aValue = new QDoubleSpinBox;
    aValue->setValue(this->fittsModel->a);
    bValue = new QDoubleSpinBox;
    bValue->setValue(this->fittsModel->b);
    rappelRightLayout->addWidget(new QLabel("Variable a "));
    rappelRightLayout->addWidget(aValue);
    rappelRightLayout->addWidget(new QLabel("Variable b "));
    rappelRightLayout->addWidget(bValue);


    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    buttonBox->button(QDialogButtonBox::Close)->setText("Fermer");
    buttonBox->button(QDialogButtonBox::Close)->setCursor(Qt::PointingHandCursor);
    buttonBox->button(QDialogButtonBox::Close)->setProperty("class", "btn-green");
    buttonBox->setCenterButtons(true);
    buttonBox->setProperty("class", "btn-box");
    rappelRightLayout->addWidget(buttonBox);
    optionDialog->setLayout(rappelRightLayout);
    QObject::connect(buttonBox, SIGNAL(accepted()), optionDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), optionDialog, SLOT(reject()));


    QWidget *mainWidget = new QWidget;
    mainWidget->setProperty("id", "mainWidget");
    this->setCentralWidget(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    mainStack = new QStackedLayout;
    mainLayout->addLayout(mainStack);

    QWidget *settingsWidget = new QWidget;
    mainStack->addWidget(settingsWidget);

    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsWidget);
    settingsLayout->setContentsMargins(QMargins(40,0,40,10));
    QLabel *label;

    label = new QLabel("Loi de Fitts");
    label->setProperty("class", "title");
    settingsLayout->addWidget(label);

    QLabel *fitts = new QLabel("fitts");
    fitts->setProperty("class","fitts");
    fitts->setPixmap(QPixmap(":/images/fitts.png"));
    settingsLayout->addWidget(fitts);


    QGroupBox *configBox = new QGroupBox();
    configBox->setProperty("class","config");
    configBox->setContentsMargins(QMargins(10,5,10,10));

    settingsLayout->addWidget(configBox);
    QGridLayout *configLayout = new QGridLayout(configBox);
    configLayout->setProperty("class","configLayout");
    configLayout->setRowMinimumHeight(1,30);
    QLabel *configLabel = new QLabel("Configuration du test");
    configLabel->setProperty("class","configLabel");
    QLabel *targetNbr = new QLabel("Nombre de cibles:");
    QLabel *minSizeTarget = new QLabel("Taille minimum cible:");
    QLabel *maxSizeTarget = new QLabel("Taille maximal cible:");

    configLayout->addWidget(configLabel,1,0);
    configLayout->addWidget(targetNbr,2,0);
    configLayout->addWidget(minSizeTarget,3,0);
    configLayout->addWidget(maxSizeTarget,4,0);

    nbCible = new QSpinBox;
    nbCible->setValue(this->fittsModel->nbCible);
    nbCible->setMaximum(100);
    nbCible->setMinimum(5);
    configLayout->addWidget(nbCible,2,1);

    minSize = new QSpinBox;
    minSize->setMaximum(1000);
    minSize->setValue(this->fittsModel->minSize);
    configLayout->addWidget(minSize,3,1);

    maxSize = new QSpinBox;
    maxSize->setMaximum(1000);
    maxSize->setValue(this->fittsModel->maxSize);
    configLayout->addWidget(maxSize,4,1);


    QHBoxLayout *btnLayout = new QHBoxLayout;
    settingsLayout->addLayout(btnLayout);
    btnLayout->setContentsMargins(QMargins(0,40,0,0));

    homeLeaveBtn = new QPushButton("Quitter");
    homeLeaveBtn->setProperty("class", "btn-red");
    homeLeaveBtn->setCursor(Qt::PointingHandCursor);
    btnLayout->addWidget(homeLeaveBtn);


    homeStartBtn = new QPushButton("Lancer le test");
    homeStartBtn->setCursor(Qt::PointingHandCursor);
    homeStartBtn->setProperty("class", "btn-green");
    btnLayout->addWidget(homeStartBtn);



    // Test part

    QWidget *testWidget = new QWidget;
    mainStack->addWidget(testWidget);

    QVBoxLayout *testLayout = new QVBoxLayout(testWidget);

    testLabel = new QLabel;
    testLayout->addWidget(testLabel);

    graphicView = new GraphicWidget;
    testLayout->addWidget(graphicView);
    graphicView->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    graphicView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene;
    graphicView->fitInView(scene->sceneRect(),Qt::KeepAspectRatio);
    graphicView->setScene(scene);
    scene->setSceneRect(0,0,graphicView->width(),300);
    graphicView->setProperty("class", "testScene");

    label = new QLabel("Cliquez sur les cibles qui apparaissent en <font color='#20B773'><strong>vert</strong></font>");
    testLayout->addWidget(label);
    label->setProperty("class","testText");

    btnLayout = new QHBoxLayout;
    testLayout->addLayout(btnLayout);

    testHomeBtn = new QPushButton("Menu principal");
    testHomeBtn->setCursor(Qt::PointingHandCursor);
    testHomeBtn->setProperty("class", "btn-red");
    btnLayout->addWidget(testHomeBtn);

    testRestartBtn = new QPushButton("Recommencer");
    testRestartBtn->setCursor(Qt::PointingHandCursor);
    testRestartBtn->setProperty("class", "btn-green");
    btnLayout->addWidget(testRestartBtn);


    //result part

    QWidget *resultWidget = new QWidget;
    mainStack->addWidget(resultWidget);

    QVBoxLayout *resultLayout = new QVBoxLayout(resultWidget);

    QWidget *chartWidget = new QWidget;
    resultLayout->addWidget(chartWidget);

    QHBoxLayout *chartLayout = new QHBoxLayout(chartWidget);

    plot = new QChartView;
    chartLayout->addWidget(plot);

    plot2 = new QChartView;
    chartLayout->addWidget(plot2);

    QGroupBox *resultBox =  new QGroupBox("Stats");
    resultLayout->addWidget(resultBox);
    QGridLayout *resultBoxLayout = new QGridLayout(resultBox);

    label = new QLabel("Ecart-Type : ");
    resultBoxLayout->addWidget(label,0,0);
    ecartType = new QLabel;
    resultBoxLayout->addWidget(ecartType,0,1);
    label->setProperty("class", "resultLabel");

    label = new QLabel("Erreur-Type : ");
    resultBoxLayout->addWidget(label,1,0);
    erreurType = new QLabel;
    resultBoxLayout->addWidget(erreurType,1,1);
    label->setProperty("class", "resultLabel");

    label = new QLabel("Différence moyenne : ");
    resultBoxLayout->addWidget(label,0,2);
    diffMoy = new QLabel;
    resultBoxLayout->addWidget(diffMoy,0,3);
    label->setProperty("class", "resultLabel");

    label = new QLabel("Intervalle de confiance à 95% : ");
    resultBoxLayout->addWidget(label,1,2);
    itc95 = new QLabel;
    resultBoxLayout->addWidget(itc95,1,3);
    label->setProperty("class", "resultLabel");

    resultBoxLayout->setColumnStretch(1,10);
    resultBoxLayout->setColumnStretch(3,10);

    btnLayout = new QHBoxLayout;
    resultLayout->addLayout(btnLayout);

    resultHomeBtn = new QPushButton("Menu principal");
    resultHomeBtn->setCursor(Qt::PointingHandCursor);
    resultHomeBtn->setProperty("class", "btn-red");
    btnLayout->addWidget(resultHomeBtn);


    resultRestartBtn = new QPushButton("Recommencer");
    resultRestartBtn->setCursor(Qt::PointingHandCursor);
    resultRestartBtn->setProperty("class", "btn-green");
    btnLayout->addWidget(resultRestartBtn);
}

void FittsView::updateTestMsg() {
    this->testLabel->setText("<strong>Le test commencera après avoir appuyé sur la cible grise.</strong> Nombre de cibles restantes : " + QString::number(this->fittsModel->cibleLeft));
}

void FittsView::displayOptionDialog() {
    optionDialog->exec();
}

void FittsView::displayAideDialog() {
    QMessageBox::information(this, "Aide", "<h2>Qu'est-ce que la loi de Fitts ?</h2>"
"Cette application permet d'experimenter la loi de Fitts. "
"Basée sur une équation mathématique, "
"la loi de Fitts est utilisée afin de mettre en évidence le temps nécessaire pour atteindre un objet cible. "
"Quand on se met dans la cadre de l’IHM, un objet cible est n’importe quel élément interactif, comme un lien hypertexte, "
"un bouton d’envoi ou un champ de saisie dans un formulaire sur internet."
" Dans notre test les cibles seront des <span style='color:#20B773'>ronds verts</span>."
"<h2>Formule de la loi de Fitts</h2><br>"
"<img src=':/images/fitts.png'>"
"<br>T représente le temps pour accomplir l'action, a et b sont des constantes empiriques, D est la distance de l'objet cible et L est la largeur de l'objet cible."
"<h2>Déroulement du test</h2>"
"Une fois sur la page de test il vous faudra appuyer sur la <span style='color:grey'>cible gris</span>. Ensuite, le test se lance ensuite avec les paramètres que vous avez saisis dans la page principale et dans la fenetre de dialogue des paramètres. "
"Des <span style='color:#20B773'>cibles vertes</span>, sur lesquelles il faut cliquer le plus rapidement, apparaitront succesivement sur votre écran."
"<h2>Résultats du test</h2>"
"A la fin du test deux graphiques seront affichés. Le premier affiche le calcul de la loi de fitts pour chaque cible du test et  le second le temps d'exécution en fonction de la distance relative.");
}


void FittsView::displayResults() {
    this->diffMoy->setText(QString::number(this->fittsModel->diffMoy));
    this->ecartType->setText(QString::number(this->fittsModel->ecartType));
    this->erreurType->setText(QString::number(this->fittsModel->erreurType));
    this->itc95->setText(QString::number(this->fittsModel->itc95));
}
