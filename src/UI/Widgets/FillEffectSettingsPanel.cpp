#include "FillEffectSettingsPanel.h"
#include "ui_FillEffectSettingsPanel.h"

FillEffectSettingsPanel::FillEffectSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FillEffectSettingsPanel),
    mValue()
{
    ui->setupUi(this);

    // enabled, type, color, gradient
    ui->comboBoxBlend->addItems(SFBEffectTypes::BlendModeOptions());
    ui->comboBoxType->addItems(SFBEffectTypes::FillTypeOptions());

    QObject::connect(ui->comboBoxBlend, SIGNAL(currentIndexChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxOpacity, SIGNAL(valueChanged(double)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->colorSwatch, SIGNAL(colorValueChanged(QColor)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->gradientSwatch, SIGNAL(gradientChanged(SFBGradient)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->patternSwatch, SIGNAL(patternValueChanged(SFBPattern)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxInset, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
}

FillEffectSettingsPanel::~FillEffectSettingsPanel()
{
    delete ui;
}


void FillEffectSettingsPanel::setValue(const SFBFillEffectSettings & value)
{
    mValue = value;
    mIsUpdatingGui = true;

    ui->comboBoxBlend->setCurrentText(SFBEffectTypes::BlendModeToString(mValue.blendMode));
    ui->spinBoxOpacity->setValue(mValue.opacity);
    ui->comboBoxType->setCurrentText(SFBEffectTypes::FillTypeToString(mValue.fillType));
    ui->colorSwatch->setColorValue(mValue.color);
    ui->gradientSwatch->setGradient(mValue.gradient);
    ui->patternSwatch->setPatternValue(mValue.pattern);

    mIsUpdatingGui = false;
    refreshVisibleElements();
}


SFBFillEffectSettings FillEffectSettingsPanel::getValue() const
{
    return mValue;
}


void FillEffectSettingsPanel::subWidgetValueChanged()
{
    if ( mIsUpdatingGui ) {
        return;
    }

    mValue.blendMode = SFBEffectTypes::BlendModeFromString(ui->comboBoxBlend->currentText());
    mValue.opacity = ui->spinBoxOpacity->value();
    mValue.fillType = SFBEffectTypes::FillTypeFromString(ui->comboBoxType->currentText());
    mValue.color = ui->colorSwatch->colorValue();
    mValue.gradient = ui->gradientSwatch->gradient();
    mValue.pattern = ui->patternSwatch->patternValue();
    mValue.inset = ui->spinBoxInset->value();

    emit valueChanged(mValue);

    refreshVisibleElements();
}


void FillEffectSettingsPanel::refreshVisibleElements()
{
    if ( mValue.fillType == SFBFillType::Fill_Color )
    {
        ui->labelColor->show();
        ui->colorSwatch->show();
        ui->labelGradient->hide();
        ui->gradientSwatch->hide();
        ui->labelPattern->hide();
        ui->patternSwatch->hide();
    }
    else if ( mValue.fillType == SFBFillType::Fill_Gradient )
    {
        ui->labelColor->hide();
        ui->colorSwatch->hide();
        ui->labelGradient->show();
        ui->gradientSwatch->show();
        ui->labelPattern->hide();
        ui->patternSwatch->hide();
    }
    else if ( mValue.fillType == SFBFillType::Fill_Pattern )
    {
        ui->labelColor->hide();
        ui->colorSwatch->hide();
        ui->labelGradient->hide();
        ui->gradientSwatch->hide();
        ui->labelPattern->show();
        ui->patternSwatch->show();
    }
}
