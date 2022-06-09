#include "InputSettingsPanel.h"
#include "ui_InputSettingsPanel.h"


InputSettingsPanel::InputSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputSettingsPanel),
    mEmitSignals(true)
{
    ui->setupUi(this);
    ui->inputTextEditCharacters->setWordWrapMode(QTextOption::WrapAnywhere);

    // Input Fields
    QObject::connect(ui->inputFontComboBoxFamily, SIGNAL(currentFontChanged(QFont)), this, SLOT(fontFamilyChanged()));
    QObject::connect(ui->inputFontComboBoxStyle, SIGNAL(currentTextChanged(QString)), this, SLOT(fontStyleChanged()));
    QObject::connect(ui->inputSpinBoxFontSize, SIGNAL(valueChanged(int)), this, SLOT(fontSizeChanged()));
    QObject::connect(ui->inputTextEditCharacters, SIGNAL(textChanged()), this, SLOT(inputCharactersChanged()));
}


InputSettingsPanel::~InputSettingsPanel()
{
    delete ui;
}


void InputSettingsPanel::setValue(const SFBInputSettings & value)
{
    mValue = value;

    bool prevEmitSignals = mEmitSignals;
    mEmitSignals = false;

    ui->inputFontComboBoxFamily->setCurrentText(mValue.fontFamily);
    fontFamilyChanged();

    for( int i = 0; i < ui->inputFontComboBoxStyle->count(); ++i ) {
        if ( ui->inputFontComboBoxStyle->itemText(i).compare(mValue.fontStyle) == 0 ) {
            ui->inputFontComboBoxStyle->setCurrentIndex(i);
        }
    }

    ui->inputSpinBoxFontSize->setValue(mValue.fontSize);
    ui->inputTextEditCharacters->setText(mValue.characters);

    mEmitSignals = prevEmitSignals;
}


SFBInputSettings InputSettingsPanel::getValue() const
{
    return mValue;
}


void InputSettingsPanel::fontFamilyChanged()
{
    mValue.fontFamily = ui->inputFontComboBoxFamily->currentText();
    QStringList fontStyles = mFontDatabase.styles(mValue.fontFamily);

    bool prevEmitSignals = mEmitSignals;
    mEmitSignals = false;

    ui->inputFontComboBoxStyle->clear();
    ui->inputFontComboBoxStyle->addItems(fontStyles);
    ui->inputFontComboBoxStyle->setCurrentIndex(0);

    int initialIndex = fontStyles.indexOf(QRegExp("Regular", Qt::CaseInsensitive));

    if ( initialIndex > -1 ) {
        ui->inputFontComboBoxStyle->setCurrentIndex(initialIndex);
    }

    mEmitSignals = prevEmitSignals;

    if ( mEmitSignals ) {
        emit valueChanged(mValue);
    }
}


void InputSettingsPanel::fontStyleChanged()
{
    mValue.fontStyle = ui->inputFontComboBoxStyle->currentText();

    if ( mEmitSignals ) {
        emit valueChanged(mValue);
    }
}


void InputSettingsPanel::fontSizeChanged()
{
    mValue.fontSize = ui->inputSpinBoxFontSize->value();

    if ( mEmitSignals ) {
        emit valueChanged(mValue);
    }
}


void InputSettingsPanel::inputCharactersChanged()
{
    mValue.characters = ui->inputTextEditCharacters->toPlainText();

    if ( mEmitSignals ) {
        emit valueChanged(mValue);
    }
}


void InputSettingsPanel::on_buttonAscii_clicked()
{
    ui->inputTextEditCharacters->setText(SFBInputSettings::kCharacterSetAscii);
}

void InputSettingsPanel::on_buttonNehe_clicked()
{
    ui->inputTextEditCharacters->setText(SFBInputSettings::kCharacterSetNehe);
}
