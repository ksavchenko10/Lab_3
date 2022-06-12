#ifndef FILEEXPLORERMODEL_H
#define FILEEXPLORERMODEL_H

class FileExplorerModel: public QFileSystemModel
{
public:
    FileExplorerModel(QObject *parent = nullptr);
};

#endif // FILEEXPLORERMODEL_H
