#ifndef COPIER_H
#define COPIER_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QSqlRelationalTableModel>
#include <QListWidget>

namespace Ui {
class Copier;
}

class Copier : public QMainWindow
{
    Q_OBJECT

public:
    explicit Copier(QWidget *parent = 0);
    ~Copier();

    QString loadDatabaseFile();

    QSqlDatabase dbFrom;
    QSqlDatabase dbTo;
    QSqlRelationalTableModel *modelFrom;
    QSqlRelationalTableModel *modelTo;
    QString path;

    void addLog(const QString string);
    void getAllTables(QSqlDatabase db, QListWidget* listWidget);

private:
    Ui::Copier *ui;

private slots:
    void onLoadDbFromClicked();
    void onLoadDbToClicked();
    void onLoadTableFromClicked();
    void onLoadTableToClicked();
    void onLoadColumnFromClicked();
    void onLoadColumnToClicked();
    void onAcceptClicked();
    void onClearClicked();
};

#endif // COPIER_H
