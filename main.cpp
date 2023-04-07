#include "fittsmodel.h"
#include "fittscontroller.h"
#include <QApplication>
#include <QStandardPaths>

int main(int argc, char *argv[])
{

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);
    a.setOrganizationDomain("fr.titi.fitts");
    a.setApplicationName("FittsInterface");

    new FittsController;

    return a.exec();
}

void writeHisto(){
}

void loadHisto(){

}
