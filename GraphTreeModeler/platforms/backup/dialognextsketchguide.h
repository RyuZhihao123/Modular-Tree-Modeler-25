#ifndef DIALOGNEXTSKETCHGUIDE_H
#define DIALOGNEXTSKETCHGUIDE_H

#include <QDialog>

namespace Ui {
class DialogNextSketchGuide;
}

class DialogNextSketchGuide : public QDialog
{
    Q_OBJECT

public:
    bool is_ok = false;
    explicit DialogNextSketchGuide(QWidget *parent = 0);
    ~DialogNextSketchGuide();
public slots:
    void slot_ComboBox();
    void slot_ButtonOk();
    void slot_ButtonCancel();

private:
    Ui::DialogNextSketchGuide *ui;
};

#endif // DIALOGNEXTSKETCHGUIDE_H
