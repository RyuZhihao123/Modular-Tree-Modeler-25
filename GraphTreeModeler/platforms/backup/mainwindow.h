#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "parameterpanel.h"
#include "paintwidget.h"
#include "showcasewidget.h"
#include "tcphelper.h"
#include "rangeslider.h"
#include "linechartview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    PaintWidget* m_paintWidget;
    ShowcaseWidget* m_showcaseWidget;
    TcpHelper* m_tcpHelper;

    void initializeData();

protected:
    void resizeEvent(QResizeEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;

protected slots:
    void slot_btnGenerateNew();
    void slot_btnRandSeed();
    void slot_btnMakeScene();
    void slot_setAddLevelType();
    void slot_SpineChartPanel();
    void slot_updateSelectedUI();
    void slot_updateUI_by_CurNode();
    void slot_updateCurNode_by_UI();

    void slot_loadGraph();
    void slot_saveGraph();

    void slot_sendBotanicalParameters();   // 更新一下植物学参数，并重新生成网格。




private:
    Ui::MainWindow *ui;
    ParameterPanel* m_parameterPanel;  // Twig的parameterPanel
    ParameterPanel* m_parameterPanelLeaf;  // Leaf的parameterPanel

    LineChartView* m_lineChartView;

};

#endif // MAINWINDOW_H
