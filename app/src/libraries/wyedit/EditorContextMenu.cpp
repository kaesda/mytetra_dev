#include <QAction>
#include <QDebug>
#include <QtGui>

#include "EditorContextMenu.h"
#include "Editor.h"
#include "EditorCursorPositionDetector.h"
#include "libraries/ShortcutManager.h"


extern ShortcutManager shortcutManager;


EditorContextMenu::EditorContextMenu(QWidget *parent) : QMenu(parent)
{
 setupActions();
 setupShortcuts();
 setupSignals();
 setupMenu();
}


EditorContextMenu::~EditorContextMenu(void)
{

}


void EditorContextMenu::setupActions(void)
{
 actionUndo=new QAction(this);
 actionRedo=new QAction(this);
 actionCut=new QAction(this);
 actionCopy=new QAction(this);
 actionPaste=new QAction(this);
 actionPasteAsPlainText=new QAction(this);
 actionSelectAll=new QAction(this);

 actionEditImageProperties=new QAction(this);
 actionEditMathExpression=new QAction(this);
 actionGotoReference=new QAction(this);
}


void EditorContextMenu::setupShortcuts(void)
{
    shortcutManager.initAction("editor-undo", actionUndo );
    shortcutManager.initAction("editor-redo", actionRedo );
    shortcutManager.initAction("editor-cut", actionCut );
    shortcutManager.initAction("editor-copy", actionCopy );
    shortcutManager.initAction("editor-paste", actionPaste );
    shortcutManager.initAction("editor-pasteAsPlainText", actionPasteAsPlainText );
    shortcutManager.initAction("editor-selectAll", actionSelectAll );

    // Инит только для того, чтобы сгенерировалась надпись с шорткатом, так как рабочие действия есть на панели инстументов
    shortcutManager.initAction("editor-insertImageFromFile", actionEditImageProperties );
    shortcutManager.initAction("editor-mathExpression", actionEditMathExpression );

    shortcutManager.initAction("editor-gotoReference", actionGotoReference );
}


void EditorContextMenu::update(void)
{
    // Сначала скрываются пункты редактирования формулы и картинки
    setEditMathExpression( false );
    setImageProperties( false );

    // Если выбрана формула или курсор находится на позиции формулы
    if(static_cast<Editor*>(this->parent())->cursorPositionDetector->isMathExpressionSelect() ||
       static_cast<Editor*>(this->parent())->cursorPositionDetector->isCursorOnMathExpression()) {
        setEditMathExpression( true );
    } else {

        // Если выбрана картинка или курсор находится на позиции картинки
        if(static_cast<Editor*>(this->parent())->cursorPositionDetector->isImageSelect() ||
           static_cast<Editor*>(this->parent())->cursorPositionDetector->isCursorOnImage()) {
            setImageProperties( true );
        }
    }


    // Если курсор находится на ссылке (URL)
    if(static_cast<Editor*>(this->parent())->cursorPositionDetector->isCursorOnReference()) {
        setGotoReference( true );
    } else {
        setGotoReference( false );
    }

    // Если в буфере обмена есть текст
    if(QGuiApplication::clipboard()->text().size()>0) {
        setPasteAsPlainText( true );
    } else {
        setPasteAsPlainText( false );
    }
}


QList<QAction *> EditorContextMenu::getActionsList()
{
    QList<QAction *> list;

    // Следующие действия уже есть на панели инструмнтов, они не должны добавляться на виджет редактора
    // << actionEditImageProperties
    // << actionEditMathExpression

    list << actionUndo
         << actionRedo
         << actionCut
         << actionCopy
         << actionPaste
         << actionPasteAsPlainText
         << actionSelectAll
         << actionGotoReference;

    return list;
}


// Показывать или нет пункт редактирования свойств изображения
void EditorContextMenu::setImageProperties(bool flag)
{
    qDebug() << "In EditorContextMenu::setImageProperties() " << flag;

    actionEditImageProperties->setVisible(flag);
    actionEditImageProperties->setEnabled(flag);
}


// Показывать или нет пункт редактирования математического выражения
void EditorContextMenu::setEditMathExpression(bool flag)
{
    qDebug() << "In EditorContextMenu::setEditMathExpression() " << flag;

    actionEditMathExpression->setVisible(flag);
    actionEditMathExpression->setEnabled(flag);

}


// Показывать или нет пункт перехода по ссылке
void EditorContextMenu::setGotoReference(bool flag)
{
  actionGotoReference->setVisible(flag);
  actionGotoReference->setEnabled(flag);
}


// Показывать или нет пункт "Вставить только текст"
void EditorContextMenu::setPasteAsPlainText(bool flag)
{
  actionPasteAsPlainText->setVisible(flag);
  actionPasteAsPlainText->setEnabled(flag);
}


void EditorContextMenu::setupSignals(void)
{
 connect(actionUndo,            &QAction::triggered, this, &EditorContextMenu::onActionUndo);
 connect(actionRedo,            &QAction::triggered, this, &EditorContextMenu::onActionRedo);
 connect(actionCut,             &QAction::triggered, this, &EditorContextMenu::onActionCut);
 connect(actionCopy,            &QAction::triggered, this, &EditorContextMenu::onActionCopy);
 connect(actionPaste,           &QAction::triggered, this, &EditorContextMenu::onActionPaste);
 connect(actionPasteAsPlainText,&QAction::triggered, this, &EditorContextMenu::onActionPasteAsPlainText);
 connect(actionSelectAll,       &QAction::triggered, this, &EditorContextMenu::onActionSelectAll);

 connect(actionEditImageProperties,&QAction::triggered, this, &EditorContextMenu::onActionContextMenuEditImageProperties);
 connect(actionEditMathExpression, &QAction::triggered, this, &EditorContextMenu::onActionContextMenuEditMathExpression);
 connect(actionGotoReference,      &QAction::triggered, this, &EditorContextMenu::onActionContextMenuGotoReference);
}


void EditorContextMenu::setupMenu(void)
{
 this->addAction(actionUndo);
 this->addAction(actionRedo);

 this->addSeparator();

 this->addAction(actionCut);
 this->addAction(actionCopy);
 this->addAction(actionPaste);
 this->addAction(actionPasteAsPlainText);

 this->addSeparator();

 this->addAction(actionSelectAll);
 this->addAction(actionEditImageProperties);
 this->addAction(actionEditMathExpression);
    this->addAction(actionGotoReference);
}


void EditorContextMenu::onActionUndo()
{
    update();
    if(actionUndo->isEnabled()) {
        emit undo();
    }
}

void EditorContextMenu::onActionRedo()
{
    update();
    if(actionRedo->isEnabled()) {
        emit redo();
    }
}

void EditorContextMenu::onActionCut()
{
    update();
    if(actionCut->isEnabled()) {
        emit cut();
    }
}

void EditorContextMenu::onActionCopy()
{
    update();
    if(actionCopy->isEnabled()) {
        emit copy();
    }
}

void EditorContextMenu::onActionPaste()
{
    update();
    if(actionPaste->isEnabled()) {
        emit paste();
    }
}

void EditorContextMenu::onActionPasteAsPlainText()
{
    update();
    if(actionPasteAsPlainText->isEnabled()) {
        emit pasteAsPlainText();
    }
}

void EditorContextMenu::onActionSelectAll()
{
    update();
    if(actionSelectAll->isEnabled()) {
        emit selectAll();
    }
}

void EditorContextMenu::onActionContextMenuEditImageProperties()
{
    update();
    if(actionEditImageProperties->isEnabled()) {
        emit contextMenuEditImageProperties();
    }
}

void EditorContextMenu::onActionContextMenuEditMathExpression()
{
    update();
    if(actionEditMathExpression->isEnabled()) {
        emit contextMenuEditMathExpression();
    }
}

void EditorContextMenu::onActionContextMenuGotoReference()
{
    update();
    if(actionGotoReference->isEnabled()) {
        emit contextMenuGotoReference();
    }
}

