#ifndef EXPORTSETTINGSPANEL_H
#define EXPORTSETTINGSPANEL_H

#include <QWidget>
#include "../../Model/Exporters/SFBFontExporter.h"
#include "../../Model/SFBExportSettings.h"

namespace Ui {
class ExportSettingsPanel;
}

class ExportSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ExportSettingsPanel(QWidget *parent = 0);
    ~ExportSettingsPanel();

    void setValue(const SFBExportSettings & value);
    SFBExportSettings getValue() const;

signals:
    void valueChanged(SFBExportSettings value);

private slots:
    void subWidgetValueChanged();

    void filePathButtonClicked();

private:
    Ui::ExportSettingsPanel *ui;

    bool mIsUpdatingGui;
    SFBExportSettings mValue;

};

#endif // EXPORTSETTINGSPANEL_H
