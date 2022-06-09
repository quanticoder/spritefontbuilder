#include "GradientEditorDialog.h"
#include "ui_GradientEditorDialog.h"


//
// Static Methods
//

SFBGradient GradientEditorDialog::getGradient(const SFBGradient &initial, QWidget *parent, const QString &title)
{
    GradientEditorDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.setGradient(initial);
    dialog.exec();

    return dialog.getGradient();
}


//
// Object Lifecycle
//

GradientEditorDialog::GradientEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradientEditorDialog)
{
    ui->setupUi(this);

    QObject::connect(ui->gradientStopEditor, SIGNAL(gradientChanged(SFBGradient)), this, SLOT(updateGuiFromModel()));
    QObject::connect(ui->gradientStopEditor, SIGNAL(selectedStopChanged(int)), this, SLOT(updateGuiFromModel()));

    QObject::connect(ui->stopColorSwatch, SIGNAL(colorValueChanged(QColor)), this, SLOT(guiValueChanged()));
    QObject::connect(ui->stopSpinBoxPosition, SIGNAL(valueChanged(double)), this, SLOT(guiValueChanged()));

    QObject::connect(ui->comboBoxStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(guiValueChanged()));
    QObject::connect(ui->spinBoxAngle, SIGNAL(valueChanged(int)), this, SLOT(guiValueChanged()));
    QObject::connect(ui->spinBoxScale, SIGNAL(valueChanged(int)), this, SLOT(guiValueChanged()));


    QStringList styleOptions = QStringList() << "Linear" << "Radial" << "Conical";
    ui->comboBoxStyle->addItems(styleOptions);
}


GradientEditorDialog::~GradientEditorDialog()
{
    delete ui;
}



//
// Get / Set Gradient
//


SFBGradient GradientEditorDialog::getGradient()
{
    return ui->gradientStopEditor->gradient();
}


void GradientEditorDialog::setGradient(const SFBGradient &gradient)
{
    ui->gradientStopEditor->setGradient(gradient);
    updatePreview();
    updateGuiFromModel();
}


void GradientEditorDialog::updatePreview()
{
    ui->gradientSwatchPreview->setGradient(ui->gradientStopEditor->gradient());
}


void GradientEditorDialog::updateGuiFromModel()
{
    mSuppressGuiUpdates = true;

    // Get data from stop editor
    SFBGradient gradient = ui->gradientStopEditor->gradient();
    int stopIndex = ui->gradientStopEditor->selectedGradientStop();

    // Update Gradient Fields
    ui->spinBoxAngle->setValue(gradient.angle);
    ui->spinBoxScale->setValue(gradient.scale * 100);

    switch( gradient.style ) {
        case SFBGradientStyle::Gradient_Linear: ui->comboBoxStyle->setCurrentIndex(0); break;
        case SFBGradientStyle::Gradient_Radial: ui->comboBoxStyle->setCurrentIndex(1); break;
        case SFBGradientStyle::Gradient_Conical: ui->comboBoxStyle->setCurrentIndex(2); break;
    }

    // Update Selected Stop Fields
    if ( stopIndex < 0 || stopIndex >= gradient.stops.size() )
    {
        ui->stopColorSwatch->setColorValue(QColor(0,0,0,255));
        //ui->stopColorSwatch->setEnabled(false);

        ui->stopSpinBoxPosition->setValue(0.0);
        ui->stopSpinBoxPosition->setEnabled(false);
    }
    else
    {
        SFBGradientStop stop = gradient.stops[stopIndex];

        //ui->stopColorSwatch->setEnabled(true);
        ui->stopColorSwatch->setColorValue(stop.color);

        ui->stopSpinBoxPosition->setEnabled(true);
        ui->stopSpinBoxPosition->setValue(stop.position);
    }

    updatePreview();
    mSuppressGuiUpdates = false;
}


void GradientEditorDialog::updateModelFromGui()
{
    int stopIndex = ui->gradientStopEditor->selectedGradientStop();
    SFBGradient gradient = ui->gradientStopEditor->gradient();

    gradient.angle = ui->spinBoxAngle->value();
    gradient.scale = (float)(ui->spinBoxScale->value()) * 0.01f;

    switch( ui->comboBoxStyle->currentIndex() ) {
        case 0: gradient.style = SFBGradientStyle::Gradient_Linear; break;
        case 1: gradient.style = SFBGradientStyle::Gradient_Radial; break;
        case 2: gradient.style = SFBGradientStyle::Gradient_Conical; break;
    }

    if ( stopIndex >= 0 && stopIndex < gradient.stops.size() )
    {
        SFBGradientStop& stop = gradient.stops[stopIndex];
        stop.color = ui->stopColorSwatch->colorValue();
        stop.position = ui->stopSpinBoxPosition->value();
    }

    ui->gradientStopEditor->updateGradient(gradient);
    updatePreview();
}


void GradientEditorDialog::guiValueChanged()
{
    if ( !mSuppressGuiUpdates )
    {
        updateModelFromGui();
    }
}
