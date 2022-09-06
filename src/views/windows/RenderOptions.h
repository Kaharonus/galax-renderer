
#pragma once

#include "../../renderer/Scene.h"
#include <QCodeEditor>
#include <QMainWindow>
#include <QOpenGLContext>
#include <memory>
#include <QGridLayout>
#include <QTreeWidget>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
    class RenderOptions;
}
QT_END_NAMESPACE

class RenderOptions : public QMainWindow {
    Q_OBJECT

public:
    explicit RenderOptions(QWidget* parent = nullptr);

    void setScene(std::shared_ptr<Galax::Renderer::Scene> scene);

    ~RenderOptions() override;

protected slots:

    void onItemClicked();
    void codeChanged();

private:
    Ui::RenderOptions* ui;

    std::shared_ptr<Galax::Renderer::Scene> scene;
    QCodeEditor* codeEditor;
    QLabel* errorLabel;
    QTimer* timer;
    // Renderer* renderer;
    QLayout* baseGridLayout;
    QTreeWidget* nodeView;
    QVBoxLayout* dataWidget;
    std::shared_ptr<Shader> currentShader;

    void editShader(std::shared_ptr<Shader> shader);
    void editCamera(std::shared_ptr<Camera> camera);

    void prepareCodeEditor();
    void addNode(std::shared_ptr<Node> node, QTreeWidgetItem* parent);

    std::string styleString = R"(<?xml version="1.0" encoding="UTF-8"?>
<style-scheme version="1.0" name="Dracula">
    <style name="Text" foreground="#f8f8f2" background="#282a36"/>
    <style name="Link" foreground="#8be9fd" underlineStyle="SingleUnderline"/>
    <style name="Selection" background="#44475a"/>
    <style name="LineNumber" foreground="#6272a4"/>
    <style name="SearchResult" foreground="#d57544" bold="true"/>
    <style name="SearchScope" foreground="#000000" background="#f8f8f2"/>
    <style name="Parentheses" foreground="#f8f8f2" bold="true"/>
    <style name="ParenthesesMismatch" foreground="#f8f8f2"/>
    <style name="AutoComplete" foreground="#f8f8f2"/>
    <style name="CurrentLine" foreground="#000000" background="#383b4c"/>
    <style name="CurrentLineNumber" foreground="#f8f8f2"/>
    <style name="Occurrences" foreground="#000000" background="#f8f8f2"/>
    <style name="Occurrences.Unused" foreground="#f8f8f2"/>
    <style name="Occurrences.Rename" foreground="#000000" background="#f8f8f2"/>
    <style name="Number" foreground="#bd93f9"/>
    <style name="String" foreground="#f1fa8c"/>
    <style name="Type" foreground="#7ce4fb"/>
    <style name="Local" foreground="#ffffff"/>
    <style name="Global" foreground="#ffb86c"/>
    <style name="Field" foreground="#ffffff"/>
    <style name="Static" foreground="#ffb86c"/>
    <style name="VirtualMethod" foreground="#50fa7b" italic="true"/>
    <style name="Function" foreground="#50fa7b"/>
    <style name="Keyword" foreground="#ff79c6" bold="true"/>
    <style name="PrimitiveType" foreground="#7ce4fb" italic="true"/>
    <style name="Operator" foreground="#ffffff"/>
    <style name="Overloaded Operator"/>
    <style name="Preprocessor" foreground="#ff79c6"/>
    <style name="Label" foreground="#ffb86c"/>
    <style name="Comment" foreground="#6272a4" bold="true"/>
    <style name="Doxygen.Comment" foreground="#6272a4"/>
    <style name="Doxygen.Tag" foreground="#6272a4" bold="true"/>
    <style name="VisualWhitespace" foreground="#6272a4"/>
    <style name="QmlLocalId" foreground="#50fa7b" italic="true"/>
    <style name="QmlExternalId" foreground="#ffb86c" italic="true"/>
    <style name="QmlTypeId" foreground="#8be9fd"/>
    <style name="QmlRootObjectProperty" foreground="#50fa7b" italic="true"/>
    <style name="QmlScopeObjectProperty" foreground="#50fa7b" italic="true"/>
    <style name="QmlExternalObjectProperty" foreground="#ffb86c" italic="true"/>
    <style name="JsScopeVar" foreground="#bd93f9" italic="true"/>
    <style name="JsImportVar" foreground="#8be9fd" italic="true"/>
    <style name="JsGlobalVar" foreground="#8be9fd" italic="true"/>
    <style name="QmlStateName" foreground="#50fa7b" italic="true"/>
    <style name="Binding" foreground="#ff79c6"/>
    <style name="DisabledCode" foreground="#6272a4"/>
    <style name="AddedLine" foreground="#50fa7b"/>
    <style name="RemovedLine" foreground="#ff5555"/>
    <style name="DiffFile" foreground="#8be9fd"/>
    <style name="DiffLocation" foreground="#ffb86c"/>
    <style name="DiffFileLine" foreground="#282a36" background="#f1fa8c"/>
    <style name="DiffContextLine" foreground="#282a36" background="#8be9fd"/>
    <style name="DiffSourceLine" background="#ff5555"/>
    <style name="DiffSourceChar" background="#cc2222"/>
    <style name="DiffDestLine" foreground="#282a36" background="#50fa7b"/>
    <style name="DiffDestChar" foreground="#282a36" background="#1dc748"/>
    <style name="LogChangeLine" foreground="#ff5555"/>
    <style name="Warning" underlineColor="#ffb86c" underlineStyle="SingleUnderline"/>
    <style name="WarningContext" underlineColor="#ffb86c" underlineStyle="DotLine"/>
    <style name="Error" underlineColor="#ff5555" underlineStyle="SingleUnderline"/>
    <style name="ErrorContext" underlineColor="#ff5555" underlineStyle="DotLine"/>
    <style name="Declaration" bold="true"/>
    <style name="FunctionDefinition"/>
    <style name="OutputArgument" italic="true"/>
</style-scheme>)";

    void clearLayout(QLayout *layout);

    void editNode(std::shared_ptr<Node> node);
};
