#include "MainWindow.h"

MainWindow::MainWindow()
{
    m_filePath = "";
    m_isTextChanged = false;
    setWindowTitle("Note-[new]");
//    setAcceptDrops(true);
    setAcceptDrops(false);
}

MainWindow* MainWindow::NewInstance()
{
    MainWindow* ret = new MainWindow();

    if((ret == NULL) || !(ret->construct()))
    {
        delete ret;
        ret = NULL;
    }

    return ret;
}

bool MainWindow::construct()
{
    bool ret = true;

    ret = ret && initMenuBar();
    ret = ret && initToolBar();
    ret = ret && initStatusBar();
    ret = ret && initMainEditor();

    return ret;
}

bool MainWindow::initMainEditor()
{
    bool ret = true;

    mainEditor.setParent(this);
    setCentralWidget(&mainEditor);
    connect(&mainEditor, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
    connect(&mainEditor, SIGNAL(copyAvailable(bool)), this, SLOT(onCopyAvailable(bool)));
    connect(&mainEditor, SIGNAL(undoAvailable(bool)), this, SLOT(onUndoAvailable(bool)));
    connect(&mainEditor, SIGNAL(redoAvailable(bool)), this, SLOT(onRedoAvailable(bool)));
    connect(&mainEditor, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));

    return ret;
}

bool MainWindow::initStatusBar()
{
    bool ret = true;

    QStatusBar* sb = statusBar();

    QLabel* label = new QLabel("Northeastern University");

    if(label != NULL)
    {
        StatusLbl.setText("Ln:1  Col:1");
        StatusLbl.setMinimumWidth(200);
        StatusLbl.setAlignment(Qt::AlignCenter);

        label->setMinimumWidth(200);
        label->setAlignment(Qt::AlignCenter);

        sb->addPermanentWidget(new QLabel());
        sb->addPermanentWidget(&StatusLbl);
        sb->addPermanentWidget(label);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool MainWindow::initMenuBar()
{
    bool ret = true;

    QMenuBar* mb = menuBar();
    ret = ret && initFileMenu(mb);
    ret = ret && initEditMenu(mb);
    ret = ret && initFormatMenu(mb);
    ret = ret && initViewMenu(mb);
    ret = ret && initHelpMenu(mb);

    return ret;
}

bool MainWindow::initToolBar()
{
    bool ret = true;

    QToolBar* tb = addToolBar("Tool Bar");
    tb->setIconSize(QSize(16, 16));

    ret = ret && initFileToolItem(tb);

    tb->addSeparator();

    ret = ret && initEditToolItem(tb);

    tb->addSeparator();

    ret = ret && initFormatToolItem(tb);

    tb->addSeparator();

    ret = ret && initViewToolItem(tb);

    return ret;
}

bool MainWindow::initFileMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("File(&F)", mb);

    bool ret = (menu != NULL);

    if(ret)
    {
        QAction* action = NULL;

        ret = ret && makeAction(action, menu, "New(&N)", Qt::CTRL + Qt::Key_N);

        if(ret)
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(onFileNew()));
        }

        ret = ret && makeAction(action, menu, "Open(&O)", Qt::CTRL + Qt::Key_O);

        if(ret)
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
        }

        ret = ret && makeAction(action, menu, "Save(&S)", Qt::CTRL + Qt::Key_S);

        if(ret)
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(onFileSave()));
        }

        ret = ret && makeAction(action, menu, "Save As(&)", 0);

        if(ret)
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu, "Print(&P)", Qt::CTRL + Qt::Key_P);

        if(ret)
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
        }


        menu->addSeparator();

        ret = ret && makeAction(action, menu, "Exit(&X)", 0);

        if(ret)
        {
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;

}


bool MainWindow::initEditMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("Edit(&E)", mb);

    bool ret = (menu != NULL);

    if(menu != NULL)
    {
        QAction* action = NULL;

        ret = ret && makeAction(action, menu, "Undo", Qt::CTRL + Qt::Key_Z);

        if(ret)
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));
            action->setEnabled(false);
        }

        ret = ret && makeAction(action, menu, "Redo(&R)...", Qt::CTRL + Qt::Key_Y);

        if( ret )
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
            action->setEnabled(false);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu, "Cut(&T)", Qt::CTRL + Qt::Key_X);

        if( ret )
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(cut()));
            action->setEnabled(false);
        }

        ret = ret && makeAction(action, menu, "Copy(&C)...", Qt::CTRL + Qt::Key_C);

        if( ret )
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "Paste(&P)...", Qt::CTRL + Qt::Key_V);

        if( ret )
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
            action->setEnabled(false);
        }

        ret = ret && makeAction(action, menu, "Delete(&L)", Qt::Key_Delete);

        if( ret )
        {
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu, "Find(&F)...", Qt::CTRL + Qt::Key_F);

        if( ret )
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "Replace(&R)...", Qt::CTRL + Qt::Key_H);

        if( ret )
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "Goto(&G)...", Qt::CTRL + Qt::Key_G);

        if( ret )
        {
            menu->addAction(action);
        }

        menu->addSeparator();

        ret = ret && makeAction(action, menu, "Select All(&A)", Qt::CTRL + Qt::Key_A);

        if( ret )
        {
            menu->addAction(action);
            connect(action, SIGNAL(triggered()), &mainEditor, SLOT(selectAll()));
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initFormatMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("Format(&O)", mb);

    bool ret = (menu != NULL);

    if(menu != NULL)
    {
        QAction* action = NULL;

        ret = ret && makeAction(action, menu, "Auto Wrap(&W)", 0);

        if(ret)
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "Font(&F)...", 0);

        if( ret )
        {
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initViewMenu(QMenuBar* mb)
{
    bool ret = true;

    QMenu* menu = new QMenu("View(&V)", mb);

    if(menu != NULL)
    {
        QAction* action = NULL;

        ret = ret && makeAction(action, menu, "Tool Bar(&T)", 0);

        if(ret)
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "Status Bar(&S)", 0);

        if( ret )
        {
            menu->addAction(action);
        }
    }

    if(ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initHelpMenu(QMenuBar* mb)
{
    QMenu* menu = new QMenu("Help(&H)");

    bool ret = (menu != NULL);

    if( ret )
    {
        QAction* action = NULL;

        ret = ret && makeAction(action, menu, "User Manual", 0);

        if( ret )
        {
            menu->addAction(action);
        }

        ret = ret && makeAction(action, menu, "About NotePad...", 0);

        if( ret )
        {
            menu->addAction(action);
        }
    }

    if( ret )
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }

    return ret;
}

bool MainWindow::initFileToolItem(QToolBar* tb)
{
    bool ret = true;

    QAction* action = NULL;

    ret = ret && makeAction(action, tb, "New", ":/Res/new.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(onFileNew()));
    }

    ret = ret && makeAction(action, tb, "Open", ":/Res/open.png");

    if(ret)
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(onFileOpen()));
    }

    ret = ret && makeAction(action, tb, "Save", ":/Res/save.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(onFileSave()));
    }

    ret = ret && makeAction(action, tb, "Save As", ":/Res/saveas.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(onFileSaveAs()));
    }

    ret = ret && makeAction(action, tb, "Print", ":/Res/print.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), this, SLOT(onFilePrint()));
    }

    return ret;
}

bool MainWindow::initEditToolItem(QToolBar* tb)
{
    bool ret = true;

    QAction* action = NULL;

    ret = ret && makeAction(action, tb, "Undo", ":/Res/undo.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(undo()));
        action->setEnabled(false);
    }

    ret = ret && makeAction(action, tb, "Redo", ":/Res/redo.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(redo()));
        action->setEnabled(false);
    }

    ret = ret && makeAction(action, tb, "Cut", ":/Res/cut.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(cut()));
        action->setEnabled(false);
    }

    ret = ret && makeAction(action, tb, "Copy", ":/Res/copy.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(copy()));
        action->setEnabled(false);
    }

    ret = ret && makeAction(action, tb, "Paste", ":/Res/paste.png");

    if( ret )
    {
        tb->addAction(action);
        connect(action, SIGNAL(triggered()), &mainEditor, SLOT(paste()));
    }

    ret = ret && makeAction(action, tb, "Find", ":/Res/find.png");

    if( ret )
    {
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "Replace", ":/Res/replace.png");

    if( ret )
    {
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "Goto", ":/Res/goto.png");

    if( ret )
    {
        tb->addAction(action);
    }

    return ret;
}

bool MainWindow::initFormatToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;

    ret = ret && makeAction(action, tb, "Auto Wrap", ":/Res/wrap.png");

    if( ret )
    {
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "Font", ":/Res/font.png");

    if( ret )
    {
        tb->addAction(action);
    }

    return ret;
}

bool MainWindow::initViewToolItem(QToolBar* tb)
{
    bool ret = true;
    QAction* action = NULL;

    ret = ret && makeAction(action, tb, "Tool Bar", ":/Res/tool.png");

    if( ret )
    {
        tb->addAction(action);
    }

    ret = ret && makeAction(action, tb, "Status Bar", ":/Res/status.png");

    if( ret )
    {
        tb->addAction(action);
    }

    return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString tip, QString icon)
{
    bool ret = true;

    action = new QAction("", parent);

    if(action != NULL)
    {
        action->setToolTip(tip);
        action->setIcon(QIcon(icon));
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool MainWindow::makeAction(QAction*& action, QWidget* parent, QString text, int key)
{
    bool ret = true;

    action = new QAction(text, parent);

    if(action != NULL)
    {
        action->setShortcut(QKeySequence(key));
    }
    else
    {
        ret = false;
    }

    return ret;

}

MainWindow::~MainWindow()
{
    
}
