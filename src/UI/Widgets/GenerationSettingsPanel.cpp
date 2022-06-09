#include "GenerationSettingsPanel.h"
#include "ui_GenerationSettingsPanel.h"

GenerationSettingsPanel::GenerationSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GenerationSettingsPanel),
    mIsUpdatingGui(false)
{
    ui->setupUi(this);

    // Size Combo Boxes
    QStringList sizeOptions = QStringList() << "Auto" << "128" << "256" << "512" << "1024" << "2048";
    ui->genComboBoxWidth->addItems(sizeOptions);
    ui->genComboBoxHeight->addItems(sizeOptions);

    QObject::connect(ui->genComboBoxWidth, SIGNAL(currentTextChanged(QString)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->genComboBoxHeight, SIGNAL(currentTextChanged(QString)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->genSpinBoxPadding, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->genColorSwatch, SIGNAL(colorValueChanged(QColor)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->genSpinBoxSpacing, SIGNAL(valueChanged(int)), this, SLOT(subWidgetValueChanged()));
}


GenerationSettingsPanel::~GenerationSettingsPanel()
{
    delete ui;
}


void GenerationSettingsPanel::setValue(const SFBGenerationSettings & value)
{
    mValue = value;
    mIsUpdatingGui = true;

    ui->genComboBoxWidth->setCurrentText((mValue.width == 0) ? "Auto" : QString::number(mValue.width));
    ui->genComboBoxHeight->setCurrentText((mValue.height == 0) ? "Auto" : QString::number(mValue.height));
    ui->genSpinBoxPadding->setValue(mValue.padding);
    ui->genSpinBoxSpacing->setValue(mValue.spacing);
    ui->genColorSwatch->setColorValue(mValue.color);

    mIsUpdatingGui = false;
}


SFBGenerationSettings GenerationSettingsPanel::getValue() const
{
    return mValue;
}


void GenerationSettingsPanel::subWidgetValueChanged()
{
    if ( mIsUpdatingGui ) {
        return;
    }

    mValue.width = ui->genComboBoxWidth->currentText().toInt();
    mValue.height = ui->genComboBoxHeight->currentText().toInt();
    mValue.padding = ui->genSpinBoxPadding->value();
    mValue.spacing = ui->genSpinBoxSpacing->value();
    mValue.color = ui->genColorSwatch->colorValue();

    emit valueChanged(mValue);
}
