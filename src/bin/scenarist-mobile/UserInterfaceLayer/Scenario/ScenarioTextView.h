#ifndef SCENARIOTEXTVIEW_H
#define SCENARIOTEXTVIEW_H

#include <QWidget>

namespace Ui {
	class ScenarioTextView;
}


namespace UserInterface
{
	class ScenarioTextView : public QWidget
	{
		Q_OBJECT

	public:
		explicit ScenarioTextView(QWidget *parent = 0);
		~ScenarioTextView();

		/**
		 * @brief Панель инструментов представления
		 */
		QWidget* toolbar() const;

    private:
        /**
         * @brief Настроить представление
         */
        void initView();

        /**
         * @brief Настроить соединения для формы
         */
        void initConnections();

        /**
         * @brief Настроить внешний вид
         */
        void initStyleSheet();

	private:
		/**
		 * @brief Интерфейс представления
		 */
		Ui::ScenarioTextView *m_ui;
	};
}

#endif // SCENARIOTEXTVIEW_H
