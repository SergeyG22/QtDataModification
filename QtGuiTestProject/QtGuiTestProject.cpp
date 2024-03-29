#include "QtGuiTestProject.h"
#include <qdir.h>
#include <qdebug.h>
#include <qfiledialog.h>
#include <iostream>
#include <qstringlistmodel.h>
#include <qtimer.h>
#include <qmessagebox.h>



QtGuiTestProject::QtGuiTestProject(QWidget *parent)
	: QMainWindow(parent), get_text_for_path("C:/"), get_text_for_mask("*.txt")
{
	ui.setupUi(this);
	save_f.setDirectory("C:/");
	QTimer* timer1 = new QTimer();
	timer2 = new QTimer();


	this->setWindowTitle("file modification 1.0.1");
	QRegExp rx("[a-z]?[A-Z]");
	
	QValidator* validator = new QRegExpValidator(rx, this);
	ui.lineEdit_timer->setValidator(new QIntValidator(0, 60));
	ui.lineEdit_modify->setValidator(validator);



	connect(ui.pushButton_open_file, SIGNAL(clicked()), this, SLOT(open_file()));
	connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(save_file()));
	connect(ui.pushButton_read, SIGNAL(clicked()), this, SLOT(read()));
	connect(ui.pushButton_modify, SIGNAL(clicked()), this, SLOT(modify()));
	connect(timer1, SIGNAL(timeout()), this, SLOT(observer()));
	connect(timer2, SIGNAL(timeout()), this, SLOT(make()));
	connect(ui.pushButton_timer, SIGNAL(clicked()), this, SLOT(set_timer()) );
	connect(ui.pushButton_delete, SIGNAL(clicked()), this, SLOT(clear_list()));
	connect(ui.pushButton_find,SIGNAL(clicked()), this, SLOT(clear_enter_file()));
	timer1->start(5000);
	
}

void QtGuiTestProject::observer() // ��������� ������� �������� ����� ������ 5 ������
{	
	if (!ui.checkBox_2->checkState()) 
	{
		if (ui.listWidget->count())
		{
			ui.label->setText(QString::fromLocal8Bit("������� ������� ����"));
		}
		else
			ui.label->setText(QString::fromLocal8Bit("������� ���� �����c�����"));
	}
}

  void QtGuiTestProject::get_dir() // ������� �������� ���� � ���������� �����
{

	ui.listWidget->clear();
	QDir dir(get_text_for_path);
	QFileInfoList dirContent = dir.entryInfoList(QStringList()<< get_text_for_mask);
	dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	for (int i =0;i < dirContent.size();++i)
	{
		QFileInfo fileInfo = dirContent.at(i);
		QString a = fileInfo.absoluteFilePath();
		get_text_for_path  = fileInfo.absoluteFilePath();
		ui.listWidget->addItem(a);
		
	}
	
}



void QtGuiTestProject::open_file() // ������� ��������� ����
{
	allLines = new QStringList();
	allLines->clear();

	QFileDialog open_file;
	QString name_file = open_file.getOpenFileName(0, "Open", "", "All files (*.*) ;; Document files (*.txt *.rtf);; PNG files (*.png)");

	ui.listWidget->clear();
	ui.listWidget->addItem(name_file);

	QFile file(name_file);
	
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "open file failed";
		ui.listWidget->clear();
		observer();
	}
	else observer();

	data_l = file.readAll();
	const char* line_c = data_l.data();
	line_str = QString(QString::fromLocal8Bit(line_c));
	ui.textBrowser_2->clear();
	ui.textBrowser_2->append(line_str);               
	
	allLines->append(line_str);
}



void QtGuiTestProject::save_file() // ������� ��������� � ����
{	
	if (ui.checkBox->checkState() && ui.listWidget->count() > 0)
	{
		QMessageBox::information(0, "Information", QString::fromLocal8Bit("������ ������� ����!"));
	}
	else 
	{
		QString dir = save_f.getSaveFileName(0, "Open", "", "All files (*.*) ;; Document files (*.txt);");
		QFile file(dir);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream stream(&file);
			stream << data_l;
		}
		file.close();
	}
}

void QtGuiTestProject::read() // ��������� ������ � �������� � ������
{   	

	if (ui.checkBox->checkState() && ui.listWidget->count() > 0)
	{
		QMessageBox::information(0, "Information", QString::fromLocal8Bit("������ ������� ����!"));
	}
	else
	{
		if (ui.listWidget->count() > 0)
		{
			linesModel = new QStringListModel(*allLines, NULL);
			linesModel->setStringList(*allLines);
		}
		else
		{
			QMessageBox::information(0, "Information", QString::fromLocal8Bit("������� ���� �����������"));
		}
	}
}

void QtGuiTestProject::modify() // �������� �������� ������ ��������� �� � �����
{

	if (ui.checkBox->checkState() && ui.listWidget->count() > 0)
	{
		QMessageBox::information(0, "Information", QString::fromLocal8Bit("������ ������� ����!"));
	}
	else
	{
		if (ui.listWidget->count() > 0)
		{
			if (ui.lineEdit_modify->text().size() > 0) {
				QString get_text = ui.lineEdit_modify->text();
				for (int i = 0; i < line_str.size(); ++i)
				{
					if (get_text == line_str[i])
					{
						line_str.remove(get_text);
					}
				}
				read();
				allLines->append(line_str);
				ui.textBrowser_2->clear();                          
				ui.textBrowser_2->append(line_str);                   	
				data_l.clear();          // ������� ������� �����
				data_l.append(line_str); // ���������� � ����� ����� ������
			}
			else
			{
				QMessageBox::information(0, "Information", QString::fromLocal8Bit("����� ����������� �����"));
			}

		}
		else {
			QMessageBox::information(0, "Information", QString::fromLocal8Bit("������� ���� �����������"));
		}

	}



}

void QtGuiTestProject::set_timer() // ������� ��������� �������
{	

	if (ui.checkBox->checkState() && ui.listWidget->count() > 0)
	{
		QMessageBox::information(0, "Information", QString::fromLocal8Bit("������ ������� ����!"));
	}
	else
	{
		if (ui.lineEdit_modify->text().size() > 0)
		{
			if (ui.lineEdit_timer->text().size() > 0)
			{
				if (ui.listWidget->count() > 0)
				{
					timer2->start((ui.lineEdit_timer->text().toInt()) * 1000);
				}
				else {
					QMessageBox::information(0, "Information", QString::fromLocal8Bit("������� ���� �����������"));
				}

			}
			else {
				QMessageBox::information(0, "Information", QString::fromLocal8Bit("�������� ������� �� �������"));
			}
		}
		else
		{
			QMessageBox::information(0, "Information", QString::fromLocal8Bit("����� ����������� �����"));
		}
	}
}

void QtGuiTestProject::make() // ��������� ����������� � ������������� ������
{
	modify();
	QMessageBox::information(0, "Information", QString::fromLocal8Bit("�������������� ������ ���������!"));
	timer2->stop();
}

void QtGuiTestProject::clear_list() // ������� ������ ����������� ����������� �����
{
	if (ui.checkBox->checkState() && ui.listWidget->count() > 0)
	{
		QMessageBox::information(0, "Information", QString::fromLocal8Bit("������ ������� ����!"));
	}
	else
	{
		ui.textBrowser_2->clear();
	}
	
}

void QtGuiTestProject::clear_enter_file() // ������� ������ ������� ������
{
	
	if (ui.checkBox->checkState()&& ui.listWidget->count() > 0) 
	{		
			QMessageBox::information(0, "Information", QString::fromLocal8Bit("������ ������� ����!"));		
	}
	else
	{
		ui.listWidget->clear();
	}
	
}

