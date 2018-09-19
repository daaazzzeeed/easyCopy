#include "copier.h"
#include "ui_copier.h"
#include <qdebug.h>
#include <QTime>
#include <QSqlQuery>
#include <QTableWidget>

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

   connect(ui->columnsFromLV,QTableWidget::doubleClicked,this, &Copier::onLoadColumnFromClicked);
   connect(ui->columnsToLV,QTableWidget::doubleClicked,this, &Copier::onLoadColumnToClicked);

   connect(ui->copyPb,SIGNAL(clicked()),this,SLOT(onAcceptClicked()));
   connect(ui->clearPb,SIGNAL(clicked()),this,SLOT(onClearClicked()));



}//Copier::Copier


Copier::~Copier()
{
    delete ui;
}//Copier::~Copier()

QString Copier::loadDatabaseFile(){
    QString filename = QFileDialog::getOpenFileName(this,"Open DataBase", QDir::currentPath(),"DataBase files (*.db)");
    return filename;
}

void Copier::onLoadDbFromClicked(){
    QString filename = loadDatabaseFile();
    ui->pathFromLE->setText(filename);

    //adding and setting database to copy FROM
    dbFrom = QSqlDatabase::addDatabase("QSQLITE");
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
    if(dbTo.open()){
        addLog("database to copy to added");
    }else{
        addLog("database failed to open");
    }
   // modelTo = new QSqlRelationalTableModel(Q_NULLPTR, dbTo);
    getAllTables(dbTo, ui->tablesToLV);
}

void Copier::onLoadTableFromClicked(){
     modelFrom->setTable(ui->tablesFromLV->currentItem()->text());
     qDebug() << ui->tablesFromLV->currentItem()->text();
     qDebug() << modelFrom->select();
     qDebug() << "rows: " << modelFrom->rowCount();
     qDebug() << "columns: " <<modelFrom->columnCount();
     for(int i = 0; i < modelFrom->rowCount(); i++){
         for(int j = 0; j < modelFrom->columnCount(); j++){
             QTableWidgetItem *item = new QTableWidgetItem();
             item->setText(modelFrom->index(i,j).data().toString());
             qDebug() << modelFrom->index(i,j).data().toString();
             ui->columnsFromLV->setItem(i, j, item);
         }
     }
qDebug() << "ok";
}

void Copier::onLoadTableToClicked(){
    qDebug() << "it's ok";
}

void Copier::onLoadColumnFromClicked(){
    qDebug() << "it's ok";
}

void Copier::onLoadColumnToClicked(){
    qDebug() << "it's ok";
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
