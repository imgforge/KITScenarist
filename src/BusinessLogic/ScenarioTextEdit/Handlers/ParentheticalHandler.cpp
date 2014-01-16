#include "ParentheticalHandler.h"

#include <BusinessLogic/ScenarioTextEdit/ScenarioTextEdit.h>
#include <BusinessLogic/ScenarioTextEdit/ScenarioTextBlock/ScenarioTextBlockStyle.h>

#include <QKeyEvent>
#include <QTextBlock>

using namespace KeyProcessingLayer;


ParentheticalHandler::ParentheticalHandler(ScenarioTextEdit* _editor) :
	StandardKeyHandler(_editor)
{
	ScenarioTextBlockStyle style(ScenarioTextBlockStyle::Parenthetical);
	m_stylePrefix = style.prefix();
	m_stylePostfix = style.postfix();
}

void ParentheticalHandler::handleEnter(QKeyEvent*)
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст до курсора
	QString cursorBackwardText = currentBlock.text().left(cursor.positionInBlock());
	// ... текст после курсора
	QString cursorForwardText = currentBlock.text().mid(cursor.positionInBlock());


	//
	// Обработка
	//
	if (editor()->isCompleterVisible()) {
		//! Если открыт подстановщик

		//
		// Ни чего не делаем
		//
	} else {
		//! Подстановщик закрыт

		if (cursor.hasSelection()) {
			//! Есть выделение

			//
			// Ни чего не делаем
			//
		} else {
			//! Нет выделения

			if ((cursorBackwardText.isEmpty() && cursorForwardText.isEmpty())
				|| (cursorBackwardText + cursorForwardText == m_stylePrefix + m_stylePostfix)) {
				//! Текст пуст

				//
				// Ни чего не делаем
				//
			} else {
				//! Текст не пуст

				if (cursorBackwardText.isEmpty()
					|| cursorBackwardText == m_stylePrefix) {
					//! В начале блока

					//
					// Ни чего не делаем
					//
				} else if (cursorForwardText.isEmpty()
						   || cursorForwardText == m_stylePostfix) {
					//! В конце блока

					//
					// Перейдём к блоку реплики
					//
					cursor.movePosition(QTextCursor::EndOfBlock);
					editor()->setTextCursor(cursor);
					editor()->addScenarioBlock(ScenarioTextBlockStyle::Dialog);
				} else {
					//! Внутри блока

					//
					// Переместим обрамление в правильное место
					//
					cursor.movePosition(QTextCursor::EndOfBlock);
					for (int deleteReplays = m_stylePostfix.length(); deleteReplays > 0; --deleteReplays) {
						cursor.deletePreviousChar();
					}
					cursor = editor()->textCursor();
					cursor.insertText(m_stylePostfix);

					//
					// Перейдём к блоку реплики
					//
					editor()->setTextCursor(cursor);
					editor()->addScenarioBlock(ScenarioTextBlockStyle::Dialog);
				}
			}
		}
	}
}

void ParentheticalHandler::handleTab(QKeyEvent*)
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст до курсора
	QString cursorBackwardText = currentBlock.text().left(cursor.positionInBlock());
	// ... текст после курсора
	QString cursorForwardText = currentBlock.text().mid(cursor.positionInBlock());


	//
	// Обработка
	//
	if (editor()->isCompleterVisible()) {
		//! Если открыт подстановщик

		//
		// Ни чего не делаем
		//
	} else {
		//! Подстановщик закрыт

		if (cursor.hasSelection()) {
			//! Есть выделение

			//
			// Ни чего не делаем
			//
		} else {
			//! Нет выделения

			if ((cursorBackwardText.isEmpty() && cursorForwardText.isEmpty())
				|| (cursorBackwardText + cursorForwardText == m_stylePrefix + m_stylePostfix)) {
				//! Текст пуст

				//
				// Меняем стиль на реплику
				//
				editor()->changeScenarioBlockType(ScenarioTextBlockStyle::Dialog);
			} else {
				//! Текст не пуст

				if (cursorBackwardText.isEmpty()
					|| cursorBackwardText == m_stylePrefix) {
					//! В начале блока

					//
					// Ни чего не делаем
					//
				} else if (cursorForwardText.isEmpty()
						   || cursorForwardText == m_stylePostfix) {
					//! В конце блока

					//
					// Вставляем блок реплики
					//
					cursor.movePosition(QTextCursor::EndOfBlock);
					editor()->setTextCursor(cursor);
					editor()->addScenarioBlock(ScenarioTextBlockStyle::Dialog);
				} else {
					//! Внутри блока

					//
					// Ни чего не делаем
					//
				}
			}
		}
	}
}

void ParentheticalHandler::handleOther(QKeyEvent* _event)
{
	//
	// Получим необходимые значения
	//
	// ... курсор в текущем положении
	QTextCursor cursor = editor()->textCursor();
	// ... блок текста в котором находится курсор
	QTextBlock currentBlock = cursor.block();
	// ... текст до курсора
	QString cursorBackwardText = currentBlock.text().left(cursor.positionInBlock());
	// ... текст после курсора
	QString cursorForwardText = currentBlock.text().mid(cursor.positionInBlock());

	//
	// Была нажата открывающая скобка
	//
	if (_event != 0
		&& _event->text() == "(") {
		//
		// Если в начале строки и скобки не было
		//
		if (cursorBackwardText == "("
			&& !cursorForwardText.startsWith("(")
			&& !cursorForwardText.contains("(")) {
			//
			// Ни чего не делаем
			//
		}
		//
		// Во всех остальных случаях удаляем введённую скобку
		//
		else {
			cursor.deletePreviousChar();

			//
			// BUG: Если курсор в начале документа, то не прорисовывается текст
			//
		}
	}
	//
	// Была нажата закрывающая скобка
	//
	else if (_event != 0
			 && _event->text() == ")") {
		//
		// Если в конце строки или перед закрывающей скобкой
		//
		if (cursorForwardText.isEmpty()
			|| cursorForwardText == ")") {
			//
			// Если необходимо удаляем лишнюю скобку
			//
			if (cursorBackwardText.endsWith("))")
				|| cursorForwardText == ")") {
				cursor.deletePreviousChar();
				cursor.movePosition(QTextCursor::EndOfBlock);
				editor()->setTextCursor(cursor);
			}

			//
			// Переходим к блоку реплики
			//
			editor()->addScenarioBlock(ScenarioTextBlockStyle::Dialog);
		}
		//
		// Во всех остальных случаях удаляем введённую скобку
		//
		else {
			cursor.deletePreviousChar();
		}
	}
	//
	// В противном случае, обрабатываем в базовом классе
	//
	else {
		StandardKeyHandler::handleOther(_event);
	}
}
