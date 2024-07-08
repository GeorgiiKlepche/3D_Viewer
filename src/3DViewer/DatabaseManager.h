#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql>
#include <QLabel>
#include <QFileInfo>

class DatabaseManager {
public:
    DatabaseManager(const QString& databaseName) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(databaseName);
    }

    bool openDatabase() {
        return db.open();
    }

    void closeDatabase() {
        db.close();
    }

    bool createTable() {
        QSqlQuery query;
        if (!query.exec("CREATE TABLE IF NOT EXISTS FileInformation ("
                        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "FileName TEXT, "  // Add a new column for FileName
                        "VertexCount INTEGER, "
                        "EdgeCount INTEGER)")) {
            qDebug() << "Ошибка при создании таблицы:" << query.lastError().text();
                return false;
        }
        return true;
    }

    bool insertFileInfo(const QString& fileName, int vertexCount, int edgeCount) {
        QSqlQuery query;
        QFileInfo fileInfo(fileName);
        QString justFileName = fileInfo.fileName();

        qDebug() << "FileName:" << fileName;
        qDebug() << "JustFileName:" << justFileName;  // Это название файла
        qDebug() << "FileName:" << fileName;
        qDebug() << "VertexCount:" << vertexCount;
        qDebug() << "EdgeCount:" << edgeCount;

        query.prepare("INSERT INTO FileInformation (FileName, VertexCount, EdgeCount) VALUES (:fileName, :vertexCount, :edgeCount)");
        query.bindValue(":fileName", justFileName);
        query.bindValue(":vertexCount", vertexCount);
        query.bindValue(":edgeCount", edgeCount);

        if (!query.exec()) {
            qDebug() << "Ошибка при вставке данных:" << query.lastError().text();
                return false;
        }

        return true;
    }

    void displayDatabaseContent(QLabel* label) {
        if (openDatabase()) {
            QSqlQueryModel* model = new QSqlQueryModel();

            model->setQuery("SELECT FileName, VertexCount, EdgeCount FROM FileInformation");

            QString result;
            int rows = model->rowCount();

            for (int row = 0; row < rows; ++row) {
                QString name = model->data(model->index(row, 0)).toString();
                int vertexCount = model->data(model->index(row, 1)).toInt();
                int edgeCount = model->data(model->index(row, 2)).toInt();

                result += " name: " + name + ", vertex: " + QString::number(vertexCount) + ", edges: " + QString::number(edgeCount) + "\n";
            }

            label->setText(result);

            closeDatabase();
        } else {
            qDebug() << "Не удалось открыть базу данных.";
        }
    }



private:
    QSqlDatabase db;
};
#endif // DATABASEMANAGER_H
