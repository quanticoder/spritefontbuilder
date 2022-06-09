#ifndef FILLEFFECTSETTINGSPANEL_H
#define FILLEFFECTSETTINGSPANEL_H

#include <QWidget>

#include "../../Model/Effects/SFBFillEffect.h"

namespace Ui {
class FillEffectSettingsPanel;
}

class FillEffectSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit FillEffectSettingsPanel(QWidget *parent = 0);
    ~FillEffectSettingsPanel();

    void setValue(const SFBFillEffectSettings & value);
    SFBFillEffectSettings getValue() const;

signals:
    void valueChanged(SFBFillEffectSettings value);

private slots:
    void subWidgetValueChanged();

private:
    void refreshVisibleElements();

private:
    Ui::FillEffectSettingsPanel *ui;

    bool mIsUpdatingGui;
    SFBFillEffectSettings mValue;
};

#endif // FILLEFFECTSETTINGSPANEL_H
