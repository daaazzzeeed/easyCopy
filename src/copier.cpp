#include "copier.h"
#include "ui_copier.h"
#include <qdebug.h>
#include <QTime>
#include <QSqlQuery>
#include <QTableWidget>
#include <QLatin1String>

Copier::Copier(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Copier)
{
    ui->setupUi(this);
   //set all QLineEdits to READONLY mode
   ui->pathFromLE->setReadOnly(true);
   ui->pathToLE->setReadOnly(true);
   ui->Log->setReadOnly(true);
   //set placeholder text for log QLineEdit
   ui->Log->setPlaceholderText("Operation history will be displayed here...");

   //connecting SIGNALs to SLOTs
   connect(ui->choosePathFromPb, SIGNAL(clicked()), this, SLOT(onLoadDbFromClicked()));
   connect(ui->choosePathToPb, SIGNAL(clicked()), this, SLOT(onLoadDbToClicked()));

   connect(ui->tablesFromLV,QTableWidget::doubleClicked,this, &Copier::onLoadTableFromClicked);
   connect(ui->tablesToLV,QTableWidget::doubleClicked,this, &Copier::onLoadTableToClicked);

   connect(ui->moveFromPb,SIGNAL(clicked()),this, SLOT(onLoadColumnFromClicked()));
   connect(ui->moveToPb,SIGNAL(clicked()),this, SLOT(onLoadColumnToClicked()));

   connect(ui->copyPb,SIGNAL(clicked()),this,SLOT(onAcceptClicked()));
   connect(ui->clearPb,SIGNAL(clicked()),this,SLOT(onClearClicked()));



}//Copier::Copier


Copier::~Copier()
{   dbFrom.close();
    dbTo.close();

    delete ui;
}//Copier::~Copier()

QString Copier::loadDatabaseFile(){
    QString filename = QFileDialog::getOpenFileName(this,"Open DataBase", QDir::currentPath(),"DataBase files (*.db)");
    return filename;
}

void Copier::onLoadDbFromClicked(){
    QString filename = loadDatabaseFile();
    ui->pathFromLE->setText(filename);
    QLatin1String dbFromConnectionName("connection_from");
    //adding and setting database to copy FROM
    dbFrom = QSqlDatabase::addDatabase("QSQLITE", dbFromConnectionName);
    dbFrom.setHostName("localhost");
    dbFrom.setDatabaseName(filename);
    if(dbFrom.open()){
        addLog("database to copy from added");
    }else{
        addLog("database failed to open");
    }

   // modelFrom = new QSqlRelationalTableModel(Q_NULLPTR, dbFrom);

    getAllTables(dbFrom, ui->tablesFromLV);
}

void Copier::onLoadDbToClicked(){
    QString filename = loadDatabaseFile();
    addLog("database to copy to added");
    ui->pathToLE->setText(filename);
    //adding and setting database to copy TO

    dbTo = QSqlDatabase::addDatabase("QSQLITE");
    dbTo.setHostName("localhost");
    dbTo.setDatabaseName(filename);
    bool ok = dbTo.open();

    if(ok){
        addLog("database to copy to added");
    }else{
        addLog("database failed to open");
    }

    getAllTables(dbTo, ui->tablesToLV);
}

void Copier::onLoadTableFromClicked(){
     modelFrom = new QSqlRelationalTableModel(Q_NULLPTR, dbFrom);
     modelFrom->setTable(ui->tablesFromLV->currentItem()->text());
     modelFrom->setEditStrategy(QSqlTableModel::OnFieldChange);
     modelFrom->select();
     for(int i =0; i<modelFrom->columnCount();i++){
          modelFrom->setHeaderData(i, Qt::Horizontal, ui->tablesFromLV->currentItem()->text());
     }
     ui->columnsFromLV->setModel(modelFrom);
     modelFrom->select();
     qDebug() << "rows: " << modelFrom->rowCount();
     qDebug() << "columns: " <<modelFrom->columnCount();
}

void Copier::onLoadTableToClicked(){
    modelTo = new QSqlRelationalTableModel(Q_NULLPTR, dbTo);
    modelTo->setTable(ui->tablesToLV->currentItem()->text());
    modelTo->setEditStrategy(QSqlTableModel::OnFieldChange);
    modelTo->select();
    for(int i =0; i<modelTo->columnCount();i++){
         modelTo->setHeaderData(i, Qt::Horizontal, ui->tablesToLV->currentItem()->text());
    }
    ui->columnsToLV->setModel(modelTo);
    modelTo->select();
}

void Copier::onLoadColumnFromClicked(){
    QItemSelectionModel* sel_mod = ui->columnsFromLV->selectionModel();
    ui->correlationLvFrom->setColumnCount(sel_mod->selectedIndexes().length());
    int r = ui->correlationLvFrom->rowCount();
    ui->correlationLvFrom->insertRow(r);

    for(int i=0; i<sel_mod->selectedIndexes().length(); i++){
        QString selected_item = sel_mod->selectedIndexes().at(i).data().toString();
        QTableWidgetItem *item = new QTableWidgetItem();
                     item->setText(selected_item);
                     ui->correlationLvFrom->setSortingEnabled(false);
                     ui->correlationLvFrom->setItem(r, i, item );
    }
}

void Copier::onLoadColumnToClicked(){
    QItemSelectionModel* sel_mod = ui->columnsToLV->selectionModel();
    ui->correlationLvTo->setColumnCount(sel_mod->selectedIndexes().length());
    int r = ui->correlationLvTo->rowCount();
    ui->correlationLvTo->insertRow(r);

    for(int i=0; i<sel_mod->selectedIndexes().length(); i++){
        QString selected_item = sel_mod->selectedIndexes().at(i).data().toString();
        QTableWidgetItem *item = new QTableWidgetItem();
                     item->setText(selected_item);
                     ui->correlationLvTo->setSortingEnabled(false);
                     ui->correlationLvTo->setItem(r, i, item );
    }
}

void Copier::onAcceptClicked(){
    qDebug() << "it's ok";
}

void Copier::onClearClicked(){
    qDebug() << "it's ok";
}

void Copier::addLog(const QString string){
    ui->Log->append(QTime::currentTime().toString() + ": " + string);
}

void Copier::getAllTables(QSqlDatabase db, QListWidget *listWidget){
    QString query = "SELECT name FROM sqlite_master WHERE type='table'";
    QSqlQuery sqlQuery(db);

    addLog("QUERY: " + query);

    if(sqlQuery.exec(query)){
        while(sqlQuery.next()){
            QString value = sqlQuery.value(0).toString();
            if(value != "sqlite_sequence"){
                listWidget->addItem(value);
            }
        }
        addLog("Tables to Copy From/To Loaded");
    }
    else{
          addLog("query failed...");
      }
}
