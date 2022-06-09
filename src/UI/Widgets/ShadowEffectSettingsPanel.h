#ifndef SHADOWEFFECTSETTINGSPANEL_H
#define SHADOWEFFECTSETTINGSPANEL_H

#include <QWidget>

#include "../../Model/Effects/SFBShadowEffect.h"

namespace Ui {
class ShadowEffectSettingsPanel;
}

class ShadowEffectSettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit ShadowEffectSettingsPanel(QWidget *parent = 0);
    ~ShadowEffectSettingsPanel();

    void setValue(const SFBShadowEffectSettings & value);
    SFBShadowEffectSettings getValue() const;

signals:
    void valueChanged(SFBShadowEffectSettings value);

private slots:
    void subWidgetValueChanged();

private:
    Ui::ShadowEffectSettingsPanel *ui;
    bool mIsUpdatingGui;
    SFBShadowEffectSettings mValue;

};

#endif // SHADOWEFFECTSETTINGSPANEL_H
