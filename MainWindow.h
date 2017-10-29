#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QToolBar>
#include <QIcon>
#include <QStatusBar>
#include <QLabel>
#include <QPlainTextEdit>
#include <QDebug>
#include <QFileDialog>
#include <QStringList>
#include <QFile>
#include <QIODevice>
#include <QByteArray>
#include <QMessageBox>
#include <QTextStream>
#include <QMap>
#include <QCloseEvent>
#include <QDragEnterEvent>
#include <QList>
#include <QUrl>
#include <QObjectList>
#include <QMenu>
#include <QPrintDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
private:
    QPlainTextEdit mainEditor;
    QLabel StatusLbl;
    QString m_filePath;
    bool m_isTextChanged;

    MainWindow();
    /**For ui*/
    bool initMenuBar();
    bool initToolBar();
    bool initStatusBar();
    bool initMainEditor();

    bool initFileMenu(QMenuBar* mb);
    bool initEditMenu(QMenuBar* mb);
    bool initFormatMenu(QMenuBar* mb);
    bool initViewMenu(QMenuBar* mb);
    bool initHelpMenu(QMenuBar* mb);

    bool initFileToolItem(QToolBar* tb);
    bool initEditToolItem(QToolBar* tb);
    bool initFormatToolItem(QToolBar* tb);
    bool initViewToolItem(QToolBar* tb);

    bool makeAction(QAction*& action, QWidget* parent, QString text, int key);
    bool makeAction(QAction*& action, QWidget* parent, QString tip, QString icon);
    /**For Slot Function*/
    QString showFileDialog(QFileDialog::AcceptMode mode, QString title);
    void showErrorMessage(QString message);
    int showQuerryMessage(QString message);
    QString saveCurrentData(QString path = "");
    void preEditorChange();
    void openFileToEditor(QString path);
    QAction* findMenuBarAction(QString text);
    QAction* findToolBarAction(QString text);

    bool construct();

private slots:
    void onFileOpen();
    void onFileSave();
    void onFileSaveAs();
    void OnTextChanged();
    void onFileNew();
    void onCopyAvailable(bool available);
    void onRedoAvailable(bool available);
    void onUndoAvailable(bool available);
    void onFilePrint();
    void onCursorPositionChanged();

protected:
    void closeEvent(QCloseEvent *e);
    /**unfinished function*/
//    void dragEnterEvent(QDragEnterEvent *e);
//    void dropEvent(QDropEvent *e);

public:
    static MainWindow* NewInstance();
    ~MainWindow();
};

#endif // MAINWINDOW_H
