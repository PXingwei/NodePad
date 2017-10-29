#include "MainWindow.h"

QString MainWindow::showFileDialog(QFileDialog::AcceptMode mode, QString title)
{
    QString ret = "";
    QFileDialog fd(this);
    QStringList filters;

    QMap<QString, QString> map;
    const char* filterArray[][2] = {
        {"Text File(*.txt)", ".txt"},
        {"All File(*)", "*"},
        {NULL, NULL}
    };

    for(int i=0; filterArray[i][0] != NULL; i++)
    {
        filters.append(filterArray[i][0]);
        map.insert(filterArray[i][0], filterArray[i][1]);
    }

    fd.setWindowTitle(title);
    fd.setAcceptMode(mode);

    if(mode == QFileDialog::AcceptOpen)
    {
        fd.setFileMode(QFileDialog::ExistingFile);
    }

    fd.setFilters(filters);

    if(fd.exec() == QFileDialog::Accepted)
    {
        QStringList fs = fd.selectedFiles();
        ret = fs[0];

        if(mode == QFileDialog::AcceptSave)
        {
            QString postFix = map[fd.selectedFilter()];

            if((postFix != "*") && (!ret.endsWith(postFix)))
            {
                ret = ret + postFix;
            }
        }
    }

    return ret;
}

void MainWindow::showErrorMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Error");
    msg.setText(QString(message));
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Ok);

    msg.exec();

}

void MainWindow::openFileToEditor(QString path)
{
    if(path != "")
    {
        QFile file(path);

        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QByteArray ba = file.readAll();
            mainEditor.setPlainText(QString(ba));

            file.close();

            m_filePath = path;
            setWindowTitle("NodePad-[" + m_filePath + "]");

            m_isTextChanged = false;
        }
        else
        {
            showErrorMessage(QString("Open File Error!\n\n") + "\"" + path + "\"");
        }
    }
}

void MainWindow::onFileOpen()
{
    preEditorChange();

    if(!m_isTextChanged)
    {
        QString path = showFileDialog(QFileDialog::AcceptOpen, "Open");

        openFileToEditor(path);
    }
}

void MainWindow::onFileSave()
{
    QString path = saveCurrentData(m_filePath);

    if(path != "")
    {
        m_filePath = path;
    }
}

void MainWindow::onFileSaveAs()
{
    QString path = saveCurrentData();

    if(path != "")
    {
        m_filePath = path;
    }
}

void MainWindow::OnTextChanged()
{
    if(!m_isTextChanged)
    {
        setWindowTitle("*" + windowTitle());
    }

    m_isTextChanged = true;
}

void MainWindow::onFileNew()
{
    preEditorChange();

    if(!m_isTextChanged)
    {
        mainEditor.clear();
        setWindowTitle("Note-[new]");
        m_filePath = "";
        m_isTextChanged = false;
    }
}

int MainWindow::showQuerryMessage(QString message)
{
    QMessageBox msg(this);

    msg.setWindowTitle("Querry");
    msg.setText(message);
    msg.setIcon(QMessageBox::Question);
    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);
    return msg.exec();
}

QString MainWindow::saveCurrentData(QString path)
{
    QString ret = path;

    if(ret == "")
    {
        ret = showFileDialog(QFileDialog::AcceptSave, "Save");
    }

    if(ret != "")
    {
        QFile file(ret);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream in(&file);

            in<<mainEditor.toPlainText();

            file.close();

            setWindowTitle("NodePad-[" + ret + "]");
            m_isTextChanged = false;
        }
        else
        {
            showErrorMessage(QString("Save File Error!\n\n") + "\"" + m_filePath + "\"");
            ret = "";
        }
    }

    return ret;
}

void MainWindow::preEditorChange()
{
    if(m_isTextChanged)
    {
        int ret = showQuerryMessage("Do you want to save the changes to file?");

        switch(ret)
        {
        case QMessageBox::Yes:
            saveCurrentData(m_filePath);
            break;
        case QMessageBox::No:
            m_isTextChanged = false;
            break;
        case QMessageBox::Cancel:
            break;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    preEditorChange();

    if(!m_isTextChanged)
    {
        QMainWindow::closeEvent(e);
    }
    else
    {
        e->ignore();
    }
}

QAction* MainWindow::findMenuBarAction(QString text)
{

    QAction* ret = NULL;
    const QObjectList& list = menuBar()->children();

    for(int i=0; i<list.count(); i++)
    {
        QMenu* menu = dynamic_cast<QMenu*>(list[i]);

        if(menu != NULL)
        {
            QList<QAction*> actions = menu->actions();

            for(int j=0; j<actions.count(); j++)
            {
                if(actions[j]->text().startsWith(text))
                {
                    ret = actions[j];
                    break;
                }
            }
        }
    }

    return ret;
}

QAction* MainWindow::findToolBarAction(QString text)
{

    QAction* ret = NULL;

    const QObjectList& list = children();

    for(int i=0; i<list.count(); i++)
    {
        QToolBar* toolbar = dynamic_cast<QToolBar*>(list[i]);

        if(toolbar != NULL)
        {
            QList<QAction*> actions = toolbar->actions();

            for(int j=0; j<actions.count(); j++)
            {
                if(actions[j]->toolTip().startsWith(text))
                {
                    ret = actions[j];
                    return ret;
                }
            }
        }
    }

    return ret;
}

void MainWindow::onCopyAvailable(bool available)
{
    findMenuBarAction("Copy")->setEnabled(available);
    findMenuBarAction("Cut")->setEnabled(available);
    findToolBarAction("Copy")->setEnabled(available);
    findToolBarAction("Cut")->setEnabled(available);
}
void MainWindow::onRedoAvailable(bool available)
{
    findMenuBarAction("Redo")->setEnabled(available);
    findToolBarAction("Redo")->setEnabled(available);
}
void MainWindow::onUndoAvailable(bool available)
{
    findMenuBarAction("Undo")->setEnabled(available);
    findToolBarAction("Undo")->setEnabled(available);
}

void MainWindow::onFilePrint()
{
    QPrintDialog dlg(this);

    dlg.setWindowTitle("Print");

    if(dlg.exec() == QPrintDialog::Accepted)
    {
        QPrinter* p = dlg.printer();

        mainEditor.document()->print(p);
    }
}

void MainWindow::onCursorPositionChanged()
{
    int pos = mainEditor.textCursor().position();
    QString text = mainEditor.toPlainText();

    int col = 0;
    int ln = 0;
    int flag = -1;

    for(int i=0; i<pos; i++)
    {
        if(text[i] == '\n')
        {
            ln++;
            flag = i;
        }
    }

    flag++;
    col = pos - flag;

    StatusLbl.setText("Ln:"+QString::number(ln+1)+"  "+"Col:"+QString::number(col+1));
}

//void MainWindow::dropEvent(QDropEvent *e)
//{
//    if(e->mimeData()->hasUrls())
//    {
//        QList<QUrl> list = e->mimeData()->urls();
//        QString path = list[0].toLocalFile();
//        QFileInfo fi(path);

//        if(fi.isFile())
//        {
//            preEditorChange();
//            if(!m_isTextChanged)
//            {
//                openFileToEditor(path);
//            }
//        }
//        else
//        {
//            showErrorMessage("cannot open a folder");
//        }
//    }
//    else
//    {
//        e->ignore();
//    }
//}

