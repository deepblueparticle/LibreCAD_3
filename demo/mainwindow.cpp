
#include <memory>

#include <QTime>

#include "mainwindow.h"
#include "listener.h"
#include "ui_mainwindow.h"

#include "scenemanager.h"

#include "cad/primitive/circle.h"
#include "cad/primitive/arc.h"
#include "cad/primitive/line.h"
#include "cad/meta/color.h"
#include "cad/meta/linewidth.h"
#include "cad/interface/metatype.h"
#include "cad/dochelpers/documentlayerimpl.h"
#include "cad/document/layermanager.h"
#include "cad/document/selectionmanager.h"
#include "cad/dochelpers/layermanagerimpl.h"
#include "cad/dochelpers/selectionmanagerimpl.h"
#include "cad/dochelpers/entitymanagerimpl.h"
#include "cad/dochelpers/undomanagerimpl.h"
#include "cad/dochelpers/documentimpl.h"
#include "cad/operations/createentities.h"

#include <QtGui>

#include <math.h>

int MainWindow::randInt(int low, int high) {
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);



    // Create new LibreCAD AbstractDocument
    lc::LayerManager* newLayerManager = new lc::LayerManagerImpl();
    lc::SelectionManager* newSelectionManager = new lc::SelectionManagerImpl(newLayerManager, 5.0);

    lc::EntityManager* entityManager = new lc::EntityManagerImpl();

    // Create a new document
    _document = new lc::DocumentImpl(newLayerManager, entityManager);

    SceneManager* sceneManager = new SceneManager(ui->lCADViewer, _document);
    _undoManager = new lc::UndoManagerImpl(10);
    _undoManager->setDocument(_document);

    // Listener that listens for document events
    Listener* l = new Listener(_document);

    ui->lCADViewer->setAbstractDocument(_document);

    lc::CreateEntities* foo = new  lc::CreateEntities(_document, "0");
    foo->add(lc::CADEntityPtr(new lc::Line(lc::geo::Coordinate(-100, 100), lc::geo::Coordinate(100, -100))));
    foo->add(lc::CADEntityPtr(new lc::Line(lc::geo::Coordinate(-100, -100), lc::geo::Coordinate(100, 100))));
    _document->operateOn(shared_ptr<lc::Operation>(foo));


}

MainWindow::~MainWindow() {
    delete ui;
    delete _document;
}


void MainWindow::wheelEvent(QWheelEvent* event) {
    //   ui->lCADViewer->scaleView(pow((double)2, -event->delta() / 240.0));
}

void MainWindow::on_undoButton_clicked() {
    _undoManager->undo();
}

void MainWindow::on_redoButtom_clicked() {
    _undoManager->redo();
}

void MainWindow::on_addEntities_clicked() {
    lc::CreateEntities* foo = new  lc::CreateEntities(_document, "0");

    QTime myTimer;
    myTimer.start();

    for (int i = 0; i < 1000; i++) {
        double x1 = randInt(-4000, 4000);
        double y1 = randInt(-4000, 4000);

        double x2 = x1 + randInt(-50, 50);
        double y2 = y1 + randInt(-50, 50);
        foo->add(lc::CADEntityPtr(new lc::Line(lc::geo::Coordinate(x1, y1), lc::geo::Coordinate(x2, y2))));
    }

    qDebug() << "Create : " << myTimer.elapsed();
    myTimer.start();
    _document->operateOn(shared_ptr<lc::Operation>(foo));
    qDebug() << "Process : " << myTimer.elapsed();
}

void MainWindow::on_addCircles_clicked() {
    lc::CreateEntities* foo = new  lc::CreateEntities(_document, "0");

    for (int i = 0; i < 1000; i++) {
        double x1 = randInt(-4000, 4000);
        double y1 = randInt(-4000, 4000);

        double r = randInt(0, 150);
        foo->add(lc::CADEntityPtr(new lc::Circle(lc::geo::Coordinate(x1, y1), r)));
    }

    _document->operateOn(shared_ptr<lc::Operation>(foo));
}


void MainWindow::on_clearUndoables_clicked() {
    _undoManager->removeUndoables();
}

void MainWindow::on_addArcs_clicked() {
    lc::CreateEntities* foo = new  lc::CreateEntities(_document, "0");

    for (int i = 0; i < 1000; i++) {
        double x1 = randInt(-4000, 4000);
        double y1 = randInt(-4000, 4000);

        double r = randInt(0, 150);
        double s = randInt(0, 6283) / 1000.0;
        double e = randInt(0, 6283) / 1000.0;

        if (s < e) {
            double t = e;
            e = s;
            s = t;
        }

        s = (0 + 45) / (360.0 / PI / 2);
        e = (180 + 45) / (360.0 / PI / 2);
        foo->add(lc::CADEntityPtr(new lc::Arc(lc::geo::Coordinate(x1, y1), r, s, e)));
    }

    _document->operateOn(shared_ptr<lc::Operation>(foo));
}
