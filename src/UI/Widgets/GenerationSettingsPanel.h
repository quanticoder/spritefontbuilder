#ifndef GENERATIONSETTINGSPANEL_H
#define GENERATIONSETTINGSPANEL_H

#include <QWidget>

#include "../../Model/SFBTypes.h"


namespace Ui {
class GenerationSettingsPanel;
}

class GenerationSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit GenerationSettingsPanel(QWidget *parent = 0);
    ~GenerationSettingsPanel();

    void setValue(const SFBGenerationSettings & value);
    SFBGenerationSettings getValue() const;

signals:
    void valueChanged(SFBGenerationSettings value);

private slots:
    void subWidgetValueChanged();

private:
    Ui::GenerationSettingsPanel *ui;

    bool mIsUpdatingGui;
    SFBGenerationSettings mValue;
};

#endif // GENERATIONSETTINGSPANEL_H
