#include "ShadowEffectSettingsPanel.h"
#include "ui_ShadowEffectSettingsPanel.h"

ShadowEffectSettingsPanel::ShadowEffectSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShadowEffectSettingsPanel)
{
    ui->setupUi(this);

    // enabled, type, color, gradient
    ui->comboBoxBlend->addItems(SFBEffectTypes::BlendModeOptions());
    ui->comboBoxShadowType->addItems(SFBEffectTypes::ShadowTypeOptions());

    QObject::connect(ui->comboBoxBlend, SIGNAL(currentIndexChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxOpacity, SIGNAL(valueChanged(double)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->comboBoxShadowType, SIGNAL(currentIndexChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->colorSwatch, SIGNAL(colorValueChanged(QColor)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxAngle, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxDistance, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxBlur, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->spinBoxSize, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
}

ShadowEffectSettingsPanel::~ShadowEffectSettingsPanel()
{
    delete ui;
}


void ShadowEffectSettingsPanel::setValue(const SFBShadowEffectSettings & value)
{
    mValue = value;
    mIsUpdatingGui = true;

    ui->comboBoxBlend->setCurrentText(SFBEffectTypes::BlendModeToString(mValue.blendMode));
    ui->spinBoxOpacity->setValue(mValue.opacity);
    ui->comboBoxShadowType->setCurrentText(SFBEffectTypes::ShadowTypeToString(mValue.shadowType));
    ui->colorSwatch->setColorValue(mValue.color);
    ui->spinBoxAngle->setValue(mValue.angle);
    ui->spinBoxDistance->setValue(mValue.distance);
    ui->spinBoxBlur->setValue(mValue.blur);
    ui->spinBoxSize->setValue(mValue.size);

    mIsUpdatingGui = false;
}


SFBShadowEffectSettings ShadowEffectSettingsPanel::getValue() const
{
    return mValue;
}


void ShadowEffectSettingsPanel::subWidgetValueChanged()
{
    if ( mIsUpdatingGui ) {
        return;
    }

    mValue.blendMode = SFBEffectTypes::BlendModeFromString(ui->comboBoxBlend->currentText());
    mValue.opacity = ui->spinBoxOpacity->value();
    mValue.shadowType = SFBEffectTypes::ShadowTypeFromString(ui->comboBoxShadowType->currentText());
    mValue.color = ui->colorSwatch->colorValue();
    mValue.angle = ui->spinBoxAngle->value();
    mValue.distance = ui->spinBoxDistance->value();
    mValue.blur = ui->spinBoxBlur->value();
    mValue.size = ui->spinBoxSize->value();

    emit valueChanged(mValue);
}
