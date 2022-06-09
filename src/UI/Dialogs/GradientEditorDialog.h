#ifndef GRADIENTEDITORDIALOG_H
#define GRADIENTEDITORDIALOG_H

#include <QDialog>
#include "../../Model/SFBTypes.h"

namespace Ui {
class GradientEditorDialog;
}

class GradientEditorDialog : public QDialog
{
    Q_OBJECT

    // Static Methods
public:
    /** Present a Gradient Editor Dialog box, starting with the given gradient, and return the created gradient. */
    static SFBGradient getGradient( const SFBGradient &initial, QWidget * parent, const QString & title );


    // Object Lifecycle
public:
    explicit GradientEditorDialog(QWidget *parent = 0);
    ~GradientEditorDialog();

    SFBGradient getGradient();
    void setGradient(const SFBGradient &gradient);

    void updatePreview();

    void updateModelFromGui();


public slots:
    void updateGuiFromModel();

    void guiValueChanged();

private:
    Ui::GradientEditorDialog *ui;
    bool mSuppressGuiUpdates;
};

#endif // GRADIENTEDITORDIALOG_H
