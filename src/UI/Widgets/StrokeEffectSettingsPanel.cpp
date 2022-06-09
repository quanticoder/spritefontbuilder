#include "StrokeEffectSettingsPanel.h"
#include "ui_StrokeEffectSettingsPanel.h"

StrokeEffectSettingsPanel::StrokeEffectSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StrokeEffectSettingsPanel)
{
    ui->setupUi(this);

    // enabled, type, color, gradient
    ui->comboBoxBlend->addItems(SFBEffectTypes::BlendModeOptions());
    ui->comboBoxPosition->addItems(SFBEffectTypes::StrokePositionOptions());

    QObject::connect(ui->comboBoxBlend, SIGNAL(currentIndexChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxOpacity, SIGNAL(valueChanged(double)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->comboBoxPosition, SIGNAL(currentIndexChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->colorSwatch, SIGNAL(colorValueChanged(QColor)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxWidth, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
}

StrokeEffectSettingsPanel::~StrokeEffectSettingsPanel()
{
    delete ui;
}


void StrokeEffectSettingsPanel::setValue(const SFBStrokeEffectSettings & value)
{
    mValue = value;
    mIsUpdatingGui = true;

    ui->comboBoxBlend->setCurrentText(SFBEffectTypes::BlendModeToString(mValue.blendMode));
    ui->spinBoxOpacity->setValue(mValue.opacity);
    ui->comboBoxPosition->setCurrentText(SFBEffectTypes::StrokePositionToString(mValue.position));
    ui->colorSwatch->setColorValue(mValue.color);
    ui->spinBoxWidth->setValue(mValue.width);

    mIsUpdatingGui = false;
}


SFBStrokeEffectSettings StrokeEffectSettingsPanel::getValue() const
{
    return mValue;
}


void StrokeEffectSettingsPanel::subWidgetValueChanged()
{
    if ( mIsUpdatingGui ) {
        return;
    }

    mValue.blendMode = SFBEffectTypes::BlendModeFromString(ui->comboBoxBlend->currentText());
    mValue.opacity = ui->spinBoxOpacity->value();
    mValue.position = SFBEffectTypes::StrokePositionFromString(ui->comboBoxPosition->currentText());
    mValue.color = ui->colorSwatch->colorValue();
    mValue.width = ui->spinBoxWidth->value();

    emit valueChanged(mValue);
}
