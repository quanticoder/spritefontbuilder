#include "ExportSettingsPanel.h"
#include "ui_ExportSettingsPanel.h"

#include <QFileDialog>

ExportSettingsPanel::ExportSettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExportSettingsPanel),
    mIsUpdatingGui(false)
{
    ui->setupUi(this);


    QVector<SFBExportFileType> comboBoxContents = SFBFontExporter::ExportFileTypeList();

    for( auto it = comboBoxContents.begin(); it != comboBoxContents.end(); ++it )
    {
        QString typeCode = SFBFontExporter::ExportFileTypeToString((*it));
        QString label = SFBFontExporter::ExportFileTypeToUITitle((*it));

        ui->comboBoxFormat->addItem(label, typeCode);
    }

    QObject::connect(ui->comboBoxFormat, SIGNAL(currentTextChanged(QString)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->lineEditPath, SIGNAL(textChanged(QString)), this, SLOT(subWidgetValueChanged()));
    QObject::connect(ui->checkBoxRetina, SIGNAL(stateChanged(int)), this, SLOT(subWidgetValueChanged()));

    QObject::connect(ui->buttonSelectPath, SIGNAL(clicked()), this, SLOT(filePathButtonClicked()));

}


ExportSettingsPanel::~ExportSettingsPanel()
{
    delete ui;
}


void ExportSettingsPanel::setValue(const SFBExportSettings & value)
{
    mValue = value;
    mIsUpdatingGui = true;

    QString formatString = SFBFontExporter::ExportFileTypeToString(mValue.format);

    for( int i = 0; i < ui->comboBoxFormat->count(); ++i )
    {
        if ( ui->comboBoxFormat->itemData(i).toString().compare(formatString, Qt::CaseInsensitive) == 0 )
        {
            ui->comboBoxFormat->setCurrentIndex(i);
            break;
        }
    }

    ui->lineEditPath->setText(mValue.path);
    ui->checkBoxRetina->setChecked(mValue.retinaCopy);

    mIsUpdatingGui = false;
}


SFBExportSettings ExportSettingsPanel::getValue() const
{
    return mValue;
}


void ExportSettingsPanel::subWidgetValueChanged()
{
    if ( mIsUpdatingGui ) {
        return;
    }

    int selectedFormat = ui->comboBoxFormat->currentIndex();
    QString formatCode = ui->comboBoxFormat->itemData(selectedFormat).toString();
    mValue.format = SFBFontExporter::ExportFileTypeFromString(formatCode);

    mValue.path = ui->lineEditPath->text();
    mValue.retinaCopy = ui->checkBoxRetina->isChecked();

    emit valueChanged(mValue);
}


void ExportSettingsPanel::filePathButtonClicked()
{
    QString path = QFileDialog::getSaveFileName(this, "Select File Export Path");

    if ( !path.isNull() )
    {
        ui->lineEditPath->setText(path);        // Triggers update/did change methods
    }
}


