//
// Created by tomas on 16.11.22.
//

// You may need to build the project (run Qt uic code generator) to get "ui_CodeEditor.h" resolved


#include "ui_CodeEditor.h"
#include "CodeEditor.h"
#include "QGLSLCompleter"
#include <QLayout>
#include "QGLSLHighlighter"
#include "QSyntaxStyle"
#include <QLayout>
#include <QWidget>
#include <sstream>


CodeEditor::CodeEditor(std::shared_ptr<IShader> shader, QWidget *parent) : QWidget(parent), ui(new Ui::CodeEditor) {
	ui->setupUi(this);

	this->shader = shader;
	codeEditor = new QCodeEditor(this);
	codeEditor->setAutoIndentation(true);
	codeEditor->setAutoParentheses(true);
	codeEditor->setCompleter(new QGLSLCompleter(this));
	codeEditor->setHighlighter(new QGLSLHighlighter());
	codeEditor->setTabReplaceSize(4);

	codeEditor->setText(shader->getSource().c_str());
	this->ui->layout->addWidget(codeEditor);

	auto qStyleString = QString(styleString.data());

	auto style = new QSyntaxStyle(this);
	codeEditor->setSyntaxStyle(style->load(qStyleString) ? style : QSyntaxStyle::defaultStyle());

	errorLabel = new QLabel(this);
	errorLabel->setStyleSheet("QLabel { color : red; }");
	this->ui->layout->addWidget(errorLabel);

	timer = new QTimer(this);
	timer->setInterval(33);
	connect(codeEditor, &QCodeEditor::textChanged, this, &CodeEditor::codeChanged);

	connect(timer, &QTimer::timeout, this, [this]() {
		if (errorLabel && this->shader) {
			std::stringstream ss(this->shader->getInfoLog());
			std::string to;
			std::string result;
			int count = 0;
			int max = 5;
			while (std::getline(ss, to, '\n') && count < max) {
				result += to + "\n";
				count++;
			}
			errorLabel->setText(result.c_str());
		}
	});
	timer->start();
}


void CodeEditor::codeChanged() {
	auto code = codeEditor->toPlainText().toStdString();
	if (shader == nullptr) {
		return;
	}
	shader->setSource(code);
	this->errorLabel->setText(QString(shader->getInfoLog().c_str()));
}


CodeEditor::~CodeEditor() {
	delete ui;
}
